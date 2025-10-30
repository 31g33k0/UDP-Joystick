#include <WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>

// WiFi credentials
const char* ssid = "AP-UDP-Receiver";
const char* password = "12345678";

// UDP settings
WiFiUDP udp;
const int udpPort = 12345;
const char* targetIP = "192.168.4.255"; // Broadcast address

// Joystick pins
const int pinX = 34;  // X-axis pin (GPIO34)
const int pinY = 35;  // Y-axis pin (GPIO35)
const int pinSW = 16; // Switch/button pin (GPIO16)

// Calibration values
int xMin = 0, xMax = 4095;
int yMin = 0, yMax = 4095;
const int calibrationSamples = 50;

// Joystick values
int xValue = 0;
int yValue = 0;
int buttonState = 0;

// Timing
unsigned long lastSendTime = 0;
const long sendInterval = 100; // 100ms interval (10Hz)

void setup() {
  Serial.begin(115200);
  pinMode(pinSW, INPUT_PULLUP);
  
  // 1. Start
  Serial.println("Starting UDP Joystick...");
  
  // 2. Calibrate
  calibrateJoystick();
  
  // 3. Connect to WiFi
  connectToWiFi();
}

void loop() {
  // 4. Check connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectToWiFi();
    return;
  }
  
  // 5. Measure joystick values
  measureJoystick();
  
  // 6. Send data at fixed interval
  if (millis() - lastSendTime >= sendInterval) {
    sendJoystickData();
    lastSendTime = millis();
  }
  
  // 7. Small delay to prevent reading too fast
  delay(1);
}

void calibrateJoystick() {
  Serial.println("Calibrating joystick... Move it in all directions");
  
  // Initial values
  xMin = 4095; xMax = 0;
  yMin = 4095; yMax = 0;
  
  // Take multiple samples to find min/max values
  for (int i = 0; i < calibrationSamples; i++) {
    int x = analogRead(pinX);
    int y = analogRead(pinY);
    
    // Update min/max values
    xMin = min(xMin, x);
    xMax = max(xMax, x);
    yMin = min(yMin, y);
    yMax = max(yMax, y);
    
    delay(100);
  }
  
  // Add some margin to min/max values
  int margin = 50;
  xMin = max(0, xMin - margin);
  xMax = min(4095, xMax + margin);
  yMin = max(0, yMin - margin);
  yMax = min(4095, yMax + margin);
  
  Serial.print("Calibration complete. X:[");
  Serial.print(xMin);
  Serial.print(",");
  Serial.print(xMax);
  Serial.print("] Y:[");
  Serial.print(yMin);
  Serial.print(",");
  Serial.print(yMax);
  Serial.println("]");
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Start UDP
    udp.begin(udpPort);
    Serial.println("UDP started");
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void measureJoystick() {
  // Read raw values
  int rawX = analogRead(pinX);
  int rawY = analogRead(pinY);
  
  // Apply calibration
  xValue = constrain(map(rawX, xMin, xMax, 0, 4095), 0, 4095);
  yValue = constrain(map(rawY, yMin, yMax, 0, 4095), 0, 4095);
  

  // define a dead zone
  int deadZoneMargin=300;
  if ((xValue >= xMax/2 - deadZoneMargin) && (xValue <= xMax/2 + deadZoneMargin)) {
    xValue = xMax/2;
  }

  if ((yValue >= yMax/2 - deadZoneMargin) && (yValue <= yMax/2 + deadZoneMargin)) {
    yValue = yMax/2;
  }


  // Read button (inverted because of pullup)
  buttonState = !digitalRead(pinSW);
}

void sendJoystickData() {
  // Create a string with joystick data
  String data = String(xValue) + "," + String(yValue) + "," + String(buttonState);
  
  // Send UDP packet
  udp.beginPacket(targetIP, udpPort);
  udp.print(data);
  udp.endPacket();
  
  // For debugging
  Serial.print("Sent: ");
  Serial.println(data);
}
