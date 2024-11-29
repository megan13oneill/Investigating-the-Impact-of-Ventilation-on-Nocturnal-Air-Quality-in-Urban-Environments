#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "Arduino_BHY2Host.h"

// WiFi and MQTT configuration
char ssid[] = "VodafoneMobileWiFi-FF9758"; // network SSID
char pass[] = "1052868379"; // network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "pf-eveoxy0ua6xhtbdyohag.cedalo.cloud"; // MQTT server address
int port = 1883; // MQTT port

const char topicTime[] = "8742time"; 
const char topicIAQ[] = "5817iaq";
const char topicVOC[] = "3094MS501voc";
const char topicCO2[] = "7624MS501c02";
const char topicCompTemp[] = "4381comptemp";
const char topicCompHum[] = "5907comphum";
const char topicCompGas[] = "6574compgas";

// Initialize sensor objects
Sensor temp(SENSOR_ID_TEMP);
Sensor gas(SENSOR_ID_GAS);
SensorBSEC bsec(SENSOR_ID_BSEC);
Sensor humidity(SENSOR_ID_HUM);

// Interval for sending messages (milliseconds)
const long interval = 1000; // 1 second between readings
unsigned long previousMillis = 0;

int totalReadings = 33000; // Total number of readings to take
int readingsTaken = 0; // Track number of readings taken

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600); 

  // Connect to WiFi
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000); // Retry every 5 seconds
  }
  Serial.println("You're connected to the network");
  
  // Connect to MQTT broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1); // Infinite loop if MQTT connection fails
  }
  Serial.println("You're connected to the MQTT broker!");

  // Initialize BHY2Host for BME680 sensor
  BHY2Host.begin();
  temp.begin();
  gas.begin();
  bsec.begin();
  humidity.begin();
}

void loop() {
  mqttClient.poll(); // Keep the MQTT connection alive

  unsigned long currentMillis = millis();

  // Take measurements at intervals (every 1 second in this case)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (readingsTaken < totalReadings) {
      // Call send_data to send a reading
      send_data();
      readingsTaken++;
    } else {
      // After reaching the total readings, stop
      Serial.println("Completed all measurements.");
      while (1); // Halt the program here
    }
  }
}

void send_data() {
  // Collect sensor data
  BHY2Host.update();

  float dataT = millis(); // Time in milliseconds since start
  float databseciaq = bsec.iaq();
  float databsecvoc = bsec.b_voc_eq();
  float databsecco2 = bsec.co2_eq();
  float databsecT = bsec.comp_t();
  float databsechum = bsec.comp_h();
  float databsecgas = bsec.comp_g();

  // Debugging output
  Serial.print("Time: ");
  Serial.println(dataT);
  Serial.print("IAQ: ");
  Serial.println(databseciaq);
  Serial.print("VOC: ");
  Serial.println(databsecvoc);
  Serial.print("CO2: ");
  Serial.println(databsecco2);
  Serial.print("Comp Temp: ");
  Serial.println(databsecT);
  Serial.print("Comp Humidity: ");
  Serial.println(databsechum);
  Serial.print("Comp Gas: ");
  Serial.println(databsecgas);

  // Send data to MQTT
  mqttClient.beginMessage(topicTime);
  mqttClient.print(dataT);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicIAQ);
  mqttClient.print(databseciaq);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicVOC);
  mqttClient.print(databsecvoc);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicCO2);
  mqttClient.print(databsecco2);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicCompTemp);
  mqttClient.print(databsecT);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicCompHum);
  mqttClient.print(databsechum);
  mqttClient.endMessage();

  mqttClient.beginMessage(topicCompGas);
  mqttClient.print(databsecgas);
  mqttClient.endMessage();

  // Print data as CSV (for logging)
  Serial.print(dataT);
  Serial.print(',');
  Serial.print(databseciaq);
  Serial.print(',');
  Serial.print(databsecvoc);
  Serial.print(',');
  Serial.print(databsecco2);
  Serial.print(',');
  Serial.print(databsecT);
  Serial.print(',');
  Serial.print(databsechum);
  Serial.print(',');
  Serial.print(databsecgas);
  Serial.print('\n');
}

