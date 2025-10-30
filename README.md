# UDP Joystick

![UDP Joystick Interface](UDP-Joystick.png)

A wireless joystick controller that sends UDP packets over WiFi, designed for ESP32-based projects. This project allows you to create a simple yet effective remote control interface for any device that can receive UDP commands.

## Features

- **Wireless Control**: Uses WiFi for communication
- **UDP Protocol**: Lightweight and fast data transmission
- **Auto-calibration**: Automatically calibrates the joystick on startup
- **Simple Setup**: Easy to configure with minimal components
- **Open Source**: Fully customizable Arduino sketch

## Hardware Requirements

- ESP32 development board
- Analog joystick module (X, Y, and switch)
- Jumper wires
- USB cable for programming
- Power source (battery or USB power bank for mobile use)

## Pin Configuration

| ESP32 Pin | Joystick Pin |
|----------|-------------|
| 3.3V     | VCC         |
| GND      | GND         |
| GPIO34   | VRx (X-axis)|
| GPIO35   | VRy (Y-axis)|
| GPIO16   | SW (Button) |

## Setup Instructions

1. **Hardware Setup**:
   - Connect the joystick to the ESP32 as per the pin configuration above
   - Power the ESP32 via USB or an external power source

2. **WiFi Configuration**:
   - The ESP32 creates an access point named `AP-UDP-Receiver` with password `12345678`
   - Default UDP port: `12345`
   - Broadcast address: `192.168.4.255`

3. **Upload the Code**:
   - Open the `UDP-Joystick.ino` in Arduino IDE
   - Select your ESP32 board from Tools > Board
   - Choose the correct port from Tools > Port
   - Click the Upload button

## Usage

1. After uploading, open the Serial Monitor (115200 baud)
2. The joystick will automatically calibrate - move it in all directions
3. Connect to the `AP-UDP-Receiver` WiFi network from your device
4. The ESP32 will start sending UDP packets with joystick data in the format:
   ```
   X: [value], Y: [value], Button: [0/1]
   ```
5. The data is sent at 10Hz (100ms interval)

## Customization

- **WiFi Settings**: Modify `ssid`, `password`, and `targetIP` in the code
- **Sensitivity**: Adjust `calibrationSamples` and `sendInterval` as needed
- **Pin Configuration**: Update the pin definitions at the top of the sketch

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
