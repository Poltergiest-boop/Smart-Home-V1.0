# 🏠 Smart Home V1.0 FINAL

> **Embedded Systems & IoT Bootcamp Final Project**

**Author:** Harshal V\
**Board:** ESP32 DevKit V1\
**Framework:** Arduino (C++) using PlatformIO\
**Simulation:** Wokwi Simulator\
**Communication:** Wi-Fi + MQTT (HiveMQ Public Broker)

------------------------------------------------------------------------

# Project Overview

Smart Home V1.0 is a complete IoT-based smart home automation system
developed using the ESP32. The system continuously monitors
environmental conditions using multiple sensors, automatically controls
household appliances based on predefined rules, and supports remote
control through MQTT over Wi-Fi.

The project combines embedded programming, sensor interfacing, actuator
control, I²C communication, Wi-Fi networking, MQTT messaging and an OLED
dashboard into a single integrated system.

------------------------------------------------------------------------

# Development Note

The initial project framework and a few reference examples (Wi-Fi, MQTT
and OLED basics) were provided by the bootcamp mentors.

The final project was **not a direct copy** of those examples. The
following components were designed, implemented, integrated, debugged
and tested independently:

-   Complete automation logic
-   AUTO and MANUAL operating modes
-   MQTT topic hierarchy
-   MQTT publish/subscribe implementation
-   OLED multi-screen dashboard
-   Sensor integration
-   Actuator integration
-   GPIO mapping
-   Serial monitor dashboard
-   Network status monitoring
-   Debugging and optimization
-   Complete testing and validation

------------------------------------------------------------------------

# Features

## Sensors (7)

-   DHT22 Temperature Sensor
-   DHT22 Humidity Sensor
-   MQ2 Gas Sensor
-   PIR Motion Sensor
-   LDR Light Sensor
-   HC-SR04 Ultrasonic Distance Sensor
-   BMP180 Pressure Sensor

## Actuators (5)

-   🔴 Red LED → Room Light
-   🟣 Purple LED → Fan Indicator
-   🔊 Passive Buzzer → Gas Alarm
-   🚪 Servo Motor → Automatic Door
-   🪟 Servo Motor → Ventilation Window

## Connectivity

-   Wi-Fi
-   MQTT Publish
-   MQTT Subscribe
-   HiveMQ Public Broker

## User Interface

-   OLED Dashboard (4 Screens)
-   Boot Screen
-   Live Serial Monitor Dashboard
-   MQTT Remote Control

------------------------------------------------------------------------

# Six Week Learning Summary

## Session 1

-   Arduino Fundamentals
-   ESP32 Basics
-   Microcontroller Architecture
-   Arduino Programming Structure
-   setup() and loop()

## Session 2

Embedded C Programming

Topics: - Variables - Data Types - Functions - Loops - Conditions -
pinMode() - digitalWrite() - digitalRead() - analogRead() - tone() -
noTone() - delay() - pulseIn()

## Session 3

Communication Protocols

-   UART
-   SPI
-   I²C

Practical Components

-   LEDs
-   Buzzers
-   Basic Actuator Interfacing

## Session 4

Wireless Communication

-   Wi-Fi
-   Bluetooth
-   ZigBee
-   MQTT Protocol
-   HiveMQ Broker
-   IoT Communication

## Session 5

Sensor Interfacing in Wokwi

-   DHT22
-   MQ2
-   PIR
-   LDR
-   HC-SR04
-   BMP180
-   OLED Display

## Session 6

Final Project

-   System Integration
-   Project Planning
-   Documentation
-   GitHub Submission
-   Demonstration

------------------------------------------------------------------------

# GPIO Connections

  Component             GPIO
  ------------------- ------
  DHT22                    2
  MQ2                     34
  PIR                     13
  LDR                     35
  HC-SR04 Trigger         16
  HC-SR04 Echo            17
  BMP180 SDA              21
  BMP180 SCL              22
  OLED SDA                21
  OLED SCL                22
  Room Light              19
  Gas Alarm               18
  Fan Indicator            5
  Door Servo              27
  Ventilation Servo       25

------------------------------------------------------------------------

# Automation Logic

  Condition                                       Action
  ----------------------------------------------- -------------------
  Dark + Motion Detected                          Room Light ON
  Temperature = HOT                               Fan ON
  Gas = DANGER                                    Gas Alarm ON
  Distance = VERY NEAR                            Door Opens
  Temperature HOT + Humidity HUMID + Gas DANGER   Ventilation Opens

------------------------------------------------------------------------

# OLED Dashboard

### Screen 1

Environmental Data - Temperature - Humidity - Pressure

### Screen 2

Safety Dashboard - Gas Status - Motion Status - Light Status - Distance
Status

### Screen 3

Actuator Dashboard - Room Light - Fan - Door - Ventilation - Alarm

### Screen 4

Network Dashboard - Wi-Fi Status - MQTT Status - Current Mode

------------------------------------------------------------------------

# Operating Modes

## AUTO

All actuators are controlled automatically using predefined automation
logic.

## MANUAL

Automation is disabled. All actuators can be controlled remotely using
MQTT commands.

------------------------------------------------------------------------

# MQTT Topics

## Published

``` text
Harshal_SmartHome/sensor/temperature
Harshal_SmartHome/sensor/humidity
Harshal_SmartHome/sensor/pressure
Harshal_SmartHome/sensor/gas
Harshal_SmartHome/sensor/light
Harshal_SmartHome/sensor/motion
Harshal_SmartHome/sensor/distance

Harshal_SmartHome/actuator/light
Harshal_SmartHome/actuator/fan
Harshal_SmartHome/actuator/buzzer
Harshal_SmartHome/actuator/door
Harshal_SmartHome/actuator/ventilation

Harshal_SmartHome/network/wifi
Harshal_SmartHome/network/mqtt
Harshal_SmartHome/network/mode
```

## Subscribed

``` text
Harshal_SmartHome/control
```

------------------------------------------------------------------------

# Supported Commands

``` text
AUTO
MANUAL

LIGHT_ON
LIGHT_OFF

FAN_ON
FAN_OFF

DOOR_OPEN
DOOR_CLOSE

VENT_OPEN
VENT_CLOSE
```

------------------------------------------------------------------------

# User Manual

1.  Power ON the ESP32.
2.  Wait for the startup screen.
3.  Verify Wi-Fi and MQTT are connected.
4.  Observe the OLED dashboard rotating through four screens.
5.  In AUTO mode, the system performs all automation.
6.  Publish **MANUAL** to `Harshal_SmartHome/control` to enable remote
    control.
7.  Publish actuator commands to control the system.
8.  Publish **AUTO** to restore automation.

------------------------------------------------------------------------

# Repository Contents

``` text
Smart Home V1.0 FINAL
│
├── main.cpp
├── platformio.ini
├── diagram.json
├── README.md
├── Circuit Diagram Screenshot
├── OLED Dashboard Screenshots
├── Serial Monitor Screenshot
├── MQTT Dashboard Screenshot
```

------------------------------------------------------------------------

# Testing

The following tests were successfully completed:

-   Sensor validation
-   Actuator validation
-   OLED display validation
-   Wi-Fi connectivity
-   MQTT Publish
-   MQTT Subscribe
-   AUTO Mode
-   MANUAL Mode
-   Automation Logic
-   Long duration stability test

**Result: All test cases passed successfully.**

------------------------------------------------------------------------

# Future Scope

-   Mobile application
-   Cloud database
-   Historical data logging
-   Push notifications
-   Relay-based appliance control
-   Voice assistant integration
-   Home Assistant integration

------------------------------------------------------------------------

# Conclusion

Smart Home V1.0 demonstrates the practical application of embedded
systems and IoT concepts learned during the six-week bootcamp. The
project integrates seven sensors, five actuators, an OLED dashboard,
Wi-Fi networking and MQTT-based remote communication into a complete
smart home prototype. It successfully supports both autonomous operation
and manual remote control, providing a scalable foundation for future
real-world smart home implementations.
