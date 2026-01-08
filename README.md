# ECOVAL: Human-Wildlife Conflict Evaluation System

## 🌿 Project Vision
ECOVAL is a Minimum Viable Product (MVP) designed to identify conflict hotspots in forest buffer zones. By monitoring animal behavior and human activity in real-time, the system provides edge-based deterrents and cloud-based analytics to protect both local communities and wildlife.

## 🏗️ Technical Architecture
* **Edge Layer:** ESP32 microcontroller integrated with **PIR** (Motion), **Ultrasonic** (Distance), and **DHT11** (Environmental) sensors.
* **Actuation:** Servo-motor based deterrent mechanism triggered by edge-logic.
* **Cloud Layer:** Real-time data transmission to **AWS IoT Core** via MQTT.
* **Database:** NoSQL data storage using **Amazon DynamoDB** for historical hotspot analysis.

## 🚀 Key Features
* **Edge Computing:** Real-time motion detection and automated servo-response to deter wildlife without human intervention.
* **Geospatial Tracking:** Integration of **Neo-6M GPS** for precise hotspot mapping.
* **Data Pipeline:** Python-based ingestion script (`ecoval.py`) to migrate local sensor logs into the AWS cloud environment.

