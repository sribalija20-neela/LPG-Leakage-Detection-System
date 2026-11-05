# LPG Leakage Detection System

## Project Overview

An embedded systems project developed using **Embedded C** and the **ATmega328P microcontroller** to detect LPG leakage and provide an audible alert using a buzzer.

The system uses an MQ-6 gas sensor to monitor gas levels. When gas is detected above the configured threshold, the buzzer is activated to alert users.

## Key Features

- LPG detection using the MQ-6 gas sensor.
- Analog sensor data acquisition through the ATmega328P ADC.
- Threshold-based gas detection.
- Audible alert through a buzzer.
- Bare-metal Embedded C implementation.
- Hardware implementation and testing as a college project.

## Hardware Components

- ATmega328P Microcontroller
- MQ-6 LPG Gas Sensor
- Buzzer
- Connecting wires
- Power supply
- Supporting circuit components

## Software & Technologies

- Programming Language: Embedded C
- Microcontroller: ATmega328P
- Development Environment: AVR toolchain
- Architecture: Bare-metal AVR

## Working Principle

1. The MQ-6 sensor monitors the surrounding environment for LPG.
2. The sensor provides an analog output corresponding to the detected gas concentration.
3. The ATmega328P reads the sensor output through its ADC.
4. The measured value is compared against a predefined threshold.
5. When the threshold condition is met, the buzzer is activated to provide an audible warning.

## Hardware Implementation & Testing

This project was implemented and tested using physical hardware as part of a college project.

- Tested MQ-6 sensor response to gas.
- Verified ADC-based sensor readings.
- Tested buzzer activation during gas detection.
- Verified the basic detection and alert functionality.

## Project Structure

```text
LPG-Leakage-Detection/
│
├── main.c
├── README.md
└── .gitignore
```

## Applications

- LPG leakage warning systems
- Domestic gas safety monitoring
- Basic industrial gas detection applications
- Embedded safety monitoring systems

## Future Improvements

- Add an LCD display for sensor readings.
- Integrate GSM or Wi-Fi notifications.
- Improve calibration and threshold accuracy.
- Add data logging for monitoring gas-level changes.

## Author

**Neelasri Balija**

*Embedded Systems | Embedded C | Microcontrollers*

---

**Disclaimer:** This project is an educational prototype and should not be used as a replacement for certified commercial gas safety equipment.

