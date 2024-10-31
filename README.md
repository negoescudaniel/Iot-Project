# ESP32-CAM with Web Server, MQTT Communication, and Animal Detection Control System

## Project Overview

This project utilizes two ESP32 modules in a cooperative system to detect animals, control a water valve for bowl refilling, and monitor environmental sensors. The system is structured as follows:

- **ESP32-CAM**: Hosts a web server to capture images and communicate animal detection results.
- **ESP32 Controller**: Manages motion detection, water level monitoring, and controls a servo motor to regulate a water valve based on detected conditions.

---

## System Architecture

### ESP32-CAM
- **Web Server**: Hosts live image captures, accessible by a Python script running on a computer.
- **Image Processing**: A Python script captures images every 5 seconds, analyzes them using TensorFlow to detect animals.
- **MQTT Communication**: The Python script sends a control signal (`1` or `0`) to the ESP32-CAM via MQTT, based on detection status, which is then forwarded to the controller ESP32.

### ESP32 Controller
- **MQTT Subscriber**: Receives animal detection signals from the ESP32-CAM.
- **PIR Motion Sensor**: Detects local movement near the water bowl.
- **Water Level Sensor**: Monitors the water level to ensure the bowl is adequately filled.
- **Servo Motor**: Controls the water valve to refill the bowl based on animal presence, motion detection, and water level.

---

## Features

- **Image Capture**: ESP32-CAM captures images every 5 seconds and hosts them on a web server.
- **Animal Detection**: Python-based image analysis using TensorFlow to detect animals.
- **MQTT Communication**: Enables efficient data exchange between the ESP32-CAM, the Python script, and the controller ESP32.
- **Conditional Water Dispensing**: The controller ESP32 dispenses water when both animal presence and motion are detected, while also monitoring water levels.

---

## Hardware Components

- **ESP32-CAM**: Hosts a web server for image capture and communicates detection results.
- **ESP32 Controller**: Controls the water valve, monitors sensors, and processes MQTT messages.
- **PIR Motion Sensor**: Detects nearby movement around the water bowl.
- **Water Level Sensor**: Tracks the water level in the bowl to prevent overflow.
- **Servo Motor**: Regulates the water valve, allowing refilling when conditions are met.
- **Power Supply**: Ensures continuous operation of both ESP32 modules and connected sensors.

---

## Code Functionality

- **ESP32-CAM Web Server**: Captures images accessible via a web browser or the Python script.
- **Python Script**:
  - Connects to the ESP32-CAM web server to retrieve images every 5 seconds.
  - Uses TensorFlow to analyze images and detect animals.
  - Sends an MQTT signal to the ESP32-CAM indicating the detection result (`1` for presence, `0` for absence).
- **ESP32 Controller Logic**:
  - Receives the animal detection signal from the ESP32-CAM.
  - Monitors the PIR motion sensor and water level sensor.
  - Opens the water valve if an animal is detected, motion is sensed, and the water level is low.

---

## Possible Future Improvements

- **Edge Processing**: Implement TensorFlow Lite on the ESP32-CAM for local, on-device image processing to reduce dependency on external Python processing.
- **Enhanced Control**: Integrate additional sensor feedback for finer control of the water valve and water dispensing system.
- **Web Dashboard**: Develop a web-based interface to monitor real-time sensor data, view detection results, and manually control the water valve.

---

