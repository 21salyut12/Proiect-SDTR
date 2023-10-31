# Proiect-SDTR
# Security System with Object Detection and Bluetooth Alerts

![Project Image](project_image.jpg)

## Project Overview

This project implements a security system using an ESP32 microcontroller that detects objects in its vicinity using ultrasonic sensors.
When an object is detected, the system triggers an alert by turning on an LED indicator and sending a notification with a timestamp and distance measurement to a user's smartphone via Bluetooth.
Users can remotely control the system, toggle detection on/off, and choose their preferred measurement system (metric/imperial).

## Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Usage](#usage)
- [Features](#features)

## Hardware Requirements

- ESP32 microcontroller (ESP32F030F4P6)
- Ultrasonic sensors (HC-SR05 or HC-SR04)
- LEDs and resistors
- HC-05 Bluetooth module
- Power supply
- Smartphone

## Software Requirements

- ESP32CubeIDE
- ESP32 HAL Library
- FreeRTOS (Real-Time Operating System)
- Mobile app development tools
- Bluetooth communication libraries
- Sensor libraries

## Usage

1. Power on the system.
2. Configure the smartphone app to connect to the Bluetooth module.
3. Use the app to:
   - Arm/disarm the detection system.
   - Select measurement units (metric/imperial).
4. When an object is detected, the system will:
   - Turn on an LED indicator.
   - Send an alert with timestamp and distance to the smartphone app.

## Features

- Real-time object detection.
- Bluetooth communication for alerts and remote control.
- User-friendly smartphone app.
- Configurable measurement units.
- LED indicators for detection status.
