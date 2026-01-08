# ECOVAL: Human-Wildlife Conflict Evaluation System

## 🌿 Project Vision
ECOVAL is a Minimum Viable Product (MVP) designed to identify and evaluate conflict hotspots in forest buffer zones. By monitoring animal behavior and human activity in real-time, the system provides edge-based deterrents and cloud-based analytics to protect both local communities and wildlife.

## 🏗️ System Architecture
The system follows a closed-loop autonomous cycle:
1. **Sensing:** ESP32-linked sensors (PIR, Ultrasonic, DHT11, Neo-6M GPS) capture movement, proximity, and geospatial coordinates.
2. **Edge Processing:** Local logic determines threat levels and triggers a servo-actuated deterrent mechanism with sub-second latency.
3. **Cloud Intelligence:** Data is published via **MQTT** to **AWS IoT Core** and stored in **Amazon DynamoDB** for long-term hotspot analysis.

## 📂 Repository Structure
* **📂 hardware/**: Includes `ECOVAL_Edge_Logic.ino` (ESP32 control logic) and `secrets.h`.
* **📂 cloud_integration/**: `ecoval.py` — Python/Boto3 script for data synchronization.
* **📂 docs/**: Contains the [**ECOVAL Technical Summary (PDF)**](./docs/ECOVAL_Technical_Summary.pdf) featuring Circuit Diagrams, CAD models, and Logic Flowcharts.

## 🚀 Key Engineering Skills
* **Autonomous Actuation:** Implementing PWM-based servo control based on real-time sensor thresholds.
* **Low-Power IoT:** Optimizing ESP32 performance for remote deployment.
* **Geospatial Data:** Mapping conflict zones using GPS coordinates for government/conservation agency research.

---
### 🎓 Relevance to Autonomous Systems
ECOVAL demonstrates my ability to design **closed-loop control systems** that operate in unpredictable environments

