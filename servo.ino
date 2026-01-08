#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "time.h"
#include <ESP32Servo.h>  // Include the Servo library

// Define ultrasonic sensor pins
#define TRIG_PIN 22  // Trig pin connected to GPIO 22
#define ECHO_PIN 23  // Echo pin connected to GPIO 23

// Define PIR sensor pin
#define PIR_PIN 19   // PIR sensor connected to GPIO 19

// Define Servo motor pin
#define SERVO_PIN 18 // Servo motor connected to GPIO 18

#define AWS_IOT_PUBLISH_TOPIC   "esp32_ultra/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32_ultra/sub"

// NTP server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 5.5 * 3600;  // New Delhi is UTC +5:30
const int   daylightOffset_sec = 0;      // No daylight savings in New Delhi

float distance;
bool motionDetected = false;
int servoAngle = 0;  // Variable to store the current angle of the servo motor
int servoDirection = 1;  // Variable to track the direction of the servo (1 for forward, -1 for reverse)
bool isServoRotating = true;  // Variable to track whether the servo should be rotating

// Wi-Fi and AWS IoT setup
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

// Servo object
Servo servoMotor;

// Function to connect to AWS IoT
void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

// Function to publish sensor data to AWS IoT
void publishMessage() {
  // Get current time
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

  // Create JSON document with distance and timestamp
  StaticJsonDocument<200> doc;
  doc["distance"] = distance;  // Sending distance
  doc["timestamp"] = timeString;  // Add real-time timestamp
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // Convert to JSON format

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

// Function to handle incoming messages from AWS IoT
void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

// Function to measure distance using ultrasonic sensor
float measureDistance() {
  // Send a 10us pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the duration of the echo pulse
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance (duration * speed of sound in air / 2)
  float distance = duration * 0.034 / 2;  // Speed of sound is 0.034 cm/us

  return distance;
}

// Function to setup NTP and get real-time clock
void setupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronized");
}

// Setup function
void setup() {
  Serial.begin(115200);
  connectAWS();
  
  // Set pin modes for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set pin mode for PIR sensor
  pinMode(PIR_PIN, INPUT);

  // Attach the servo motor to the appropriate pin
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(servoAngle);  // Initialize the servo motor

  // Set up time from NTP
  setupTime();
}

// Main loop
void loop() {
  motionDetected = digitalRead(PIR_PIN);  // Read PIR sensor value

  if (motionDetected) {
    if (isServoRotating) {
      // Stop the servo motor
      servoMotor.write(servoAngle);  // Stop at the current angle
      Serial.println("Motion detected! Stopping the servo motor.");
      isServoRotating = false;
    }

    distance = measureDistance();  // Measure distance

    if (isnan(distance)) {  // Check if distance measurement failed
      Serial.println(F("Failed to read from ultrasonic sensor!"));
      return;
    }

    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(F(" cm"));

    // Get current time to print
    time_t now;
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeString[20];
      strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
      Serial.print("Current Time (New Delhi): ");
      Serial.println(timeString);  // Print time based on New Delhi timezone
    }

    publishMessage();  // Publish the distance and timestamp data to AWS IoT
  } else {
    if (!isServoRotating) {
      isServoRotating = true;  // Resume rotating when no motion is detected
    }

    // Rotate the servo motor to a different position
    servoAngle += servoDirection * 5;  // Increment or decrement the servo angle
    if (servoAngle >= 180) {
      servoDirection = -1;  // Reverse direction when angle reaches 180
    } else if (servoAngle <= 0) {
      servoDirection = 1;  // Forward direction when angle reaches 0
    }
    servoMotor.write(servoAngle);  // Rotate to the new angle
    Serial.print("No motion detected. Rotating servo motor to ");
    Serial.print(servoAngle);
    Serial.println(" degrees.");
  }

  client.loop();  // Handle AWS IoT communication
  delay(100);  // Shorter delay to make the servo rotate smoothly
}
