# ECOVAL: Human-Wildlife Conflict Evaluation System

## 🌿 Project Vision
ECOVAL is an IoT-driven Minimum Viable Product (MVP) designed to identify and evaluate conflict hotspots in forest buffer zones. By monitoring animal behavior and environmental factors in real-time, the system provides edge-based deterrents and cloud-based analytics to protect both local communities and wildlife.

## 🏗️ System Architecture & Workflow
The system follows a specific **Event-Driven Autonomous Cycle**:

1. **Active Scanning:** The system maintains a constant 180° sweep using a Servo motor to maximize the field of view.
2. **Interrupt Logic:** Upon the **PIR sensor** detecting motion, the servo rotation is immediately halted to stabilize the platform.
3. **Precision Sensing:** With the platform stationary, the **Ultrasonic sensor** calculates the exact distance to the wildlife, while the **Neo-6M GPS** module tags the geospatial coordinates.
4. **Cloud Transmission:** Data is published via **MQTT** to **AWS IoT Core** for real-time ingestion.

## ☁️ Cloud Orchestration & Backend Logic
The backend utilizes a serverless architecture to bridge the gap between edge data and the end-user application:
* **Ingestion & Storage:** An AWS Lambda function processes incoming MQTT messages and performs persistent storage in **Amazon DynamoDB**.
* **Data Retrieval:** A secondary Lambda function fetches time-period specific records from DynamoDB and serves them as JSON payloads via **Amazon API Gateway**.
* **End-User Access:** This allows a mobile application to display real-time conflict "hotspots" based on the frequency and distance of detected wildlife.

## 📂 Repository Structure
* **📂 hardware/**: Includes `ECOVAL_ESP32_Firmware.ino` (ESP32 control logic) and `secrets.h`.
* **📂 cloud_integration/**: `ecoval.py` — Python/Boto3 script for data synchronization.
* **📂 docs/**: Contains the [**ECOVAL Technical Summary (PDF)**](./docs/ECOVAL_Technical_Summary.pdf) featuring Circuit Diagrams, CAD models, and Logic Flowcharts.

## 🚀 Key Engineering Skills
* **Sensor Fusion & Coordination:** Synchronizing PIR triggers with Ultrasonic distance measurements for high-fidelity data capture.
* **PWM Actuation:** Implementing Pulse Width Modulation (PWM) to control servo sweeps and precision stops.
* **NoSQL Architecture:** Designing DynamoDB schemas for time-series geospatial data.

---
### 🎓 Relevance to Autonomous Systems
ECOVAL demonstrates my proficiency in designing **closed-loop control systems** and event-driven architectures. The ability to coordinate multiple sensors and actuators based on environmental triggers.
