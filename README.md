ESP32-CAM with Web Server, MQTT Communication, and Animal Detection Control System

Project Overview

This project uses two ESP32 modules in a cooperative system to monitor for animals using image detection, control a water valve for refilling a bowl, and manage sensors for environmental monitoring:


ESP32-CAM:

-Hosts a web server to capture images every 5 seconds.

-A Python script running on a computer accesses the web server, captures images, and analyzes them using TensorFlow to detect animals.

-Based on detection, the Python script sends a control signal (1 or 0) to the ESP32-CAM via MQTT, indicating the presence or absence of animals.

-The ESP32-CAM forwards the detection result to another ESP32 (controller) via MQTT.


ESP32 (Controller):

-Monitors a motion sensor (PIR) and water level sensor.

-Controls a servo motor that regulates the water valve for filling the bowl.

-The valve is opened when animals are detected AND motion is detected, ensuring that water is only dispensed when necessary, based on the water level.


System Architecture

ESP32-CAM:

-Web server hosts live image captures accessible by the Python script.

-MQTT Publisher/Subscriber for communicating with the controller ESP32 and receiving detection results from the Python script.


ESP32 Controller:

-MQTT Subscriber to receive animal detection signals from the ESP32-CAM.

-PIR Motion Sensor to detect local movement.

-Water Level Sensor to check the current water level in the bowl.

-Servo Motor controls the water valve to refill the bowl based on the water level and detection criteria.


Features

-Image capture via ESP32-CAM web server every 5 seconds.

-Python-based image analysis for detecting animals using TensorFlow.

-MQTT Communication between the ESP32-CAM, Python script, and controller ESP32 for decision-making.

-Motion detection and water level monitoring on the controller ESP32 to manage the water valve.

-Automatic water dispensing based on both animal presence and local motion detection.


Possible Future Improvements

-Edge Processing: Implement TensorFlow Lite on the ESP32-CAM for local image processing.

-Enhanced Control: Introduce additional sensor feedback for more precise control of the water valve.

-Web Dashboard: Add a web-based interface to monitor sensor data, animal detection results, and manually control the water valve.

