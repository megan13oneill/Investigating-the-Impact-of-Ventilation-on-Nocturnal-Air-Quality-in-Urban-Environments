#include <ArduinoMqttClient.h>   // Include the Arduino MQTT client library
#include <WiFiNINA.h>            // Include the WiFi library for NINA-based WiFi modules
#include "Arduino_BHY2Host.h"    // Include the BHY2Host library for interacting with the BME680 sensor

// WiFi and MQTT configuration
char ssid[] = "VodafoneMobileWiFi-FF9758";  // Network SSID (WiFi name)
char pass[] = "1052868379";  // Network password

WiFiClient wifiClient;              // Create a WiFiClient object to handle the WiFi connection
MqttClient mqttClient(wifiClient);  // Create an MQTT client using the WiFiClient

const char broker[] = "pf-eveoxy0ua6xhtbdyohag.cedalo.cloud";  // MQTT broker address
int port = 1883;  // MQTT port (default is 1883)

const char topicTime[] = "28742time";         // MQTT topic for time
const char topicIAQ[] = "25817iaq";           // MQTT topic for IAQ (Indoor Air Quality)
const char topicVOC[] = "23094MS501voc";      // MQTT topic for VOC (Volatile Organic Compounds)
const char topicCO2[] = "27624MS501c02";      // MQTT topic for CO2 concentration
const char topicCompTemp[] = "24381comptemp";  // MQTT topic for compensated temperature
const char topicCompHum[] = "25907comphum";   // MQTT topic for compensated humidity
const char topicCompGas[] = "26574compgas";    // MQTT topic for compensated gas concentration

// Initialize sensor objects
Sensor temp(SENSOR_ID_TEMP);     // Temperature sensor object
Sensor gas(SENSOR_ID_GAS);       // Gas sensor object
SensorBSEC bsec(SENSOR_ID_BSEC); // BME680 sensor object (BSEC library)
Sensor humidity(SENSOR_ID_HUM);  // Humidity sensor object

// Interval for sending messages (milliseconds)
const long interval = 1000;       // 1 second between readings
unsigned long previousMillis = 0; // Variable to store the last time a message was sent

int totalReadings = 33000;  // Total number of readings to take before stopping
int readingsTaken = 0;      // Counter for the number of readings taken

void setup() {
  // Initialize serial communication and wait for port to open:
  Serial.begin(9600); 

  // Connect to WiFi
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {  // Attempt to connect to the WiFi network
    Serial.print(".");  // Print a dot for each attempt
    delay(5000);        // Wait 5 seconds before trying again
  }
  Serial.println("You're connected to the network");

  // Connect to MQTT broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {  // Attempt to connect to the MQTT broker
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());  // Print error if connection fails
    while (1);  // Stop execution if connection fails
  }
  Serial.println("You're connected to the MQTT broker!");

  // Initialize BHY2Host for BME680 sensor and other sensor objects
  BHY2Host.begin();  // Initialize the BME680 sensor host
  temp.begin();      // Initialize the temperature sensor
  gas.begin();       // Initialize the gas sensor
  bsec.begin();      // Initialize the BSEC (BME680) sensor
  humidity.begin();  // Initialize the humidity sensor
}

void loop() {
  mqttClient.poll();  // Keep the MQTT connection alive by calling this in the loop

  unsigned long currentMillis = millis();  // Get the current time in milliseconds

  // Take measurements at intervals (every 1 second)
  if (currentMillis - previousMillis >= interval) {  
    previousMillis = currentMillis;  // Update the last time a reading was taken

    if (readingsTaken < totalReadings) {  // If the total number of readings has not been reached
      send_data();        // Collect data from the sensors and send it via MQTT
      readingsTaken++;    // Increment the counter of readings taken
    } else {
      // After reaching the total readings, stop the program
      Serial.println("Completed all measurements.");
      while (1);  // Halt the program by entering an infinite loop
    }
  }
}

void send_data() {
  // Collect sensor data from the BME680 sensor
  BHY2Host.update();  // Update the sensor readings

  // Get sensor values (time, IAQ, VOC, CO2, temperature, humidity, gas)
  float dataT = millis();             // Time in milliseconds since the program started
  float databseciaq = bsec.iaq();     // IAQ (Indoor Air Quality) reading
  float databsecvoc = bsec.b_voc_eq();  // VOC (Volatile Organic Compounds) reading
  float databsecco2 = bsec.co2_eq();  // CO2 concentration reading
  float databsecT = bsec.comp_t();    // Compensated temperature reading from BME680
  float databsechum = bsec.comp_h();  // Compensated humidity reading from BME680
  float databsecgas = bsec.comp_g();  // Compensated gas concentration reading from BME680

  // Compensate temperature using a scaling factor (this is an example calculation)
  float caldatabsecT = (1.02 * (databsecT) - 0.47);

  // Print sensor data to the Serial Monitor for debugging
  Serial.print("Time: ");
  Serial.println(dataT);
  Serial.print("IAQ: ");
  Serial.println(databseciaq);
  Serial.print("VOC: ");
  Serial.println(databsecvoc);
  Serial.print("CO2: ");
  Serial.println(databsecco2);
  Serial.print("Comp Temp: ");
  Serial.println(caldatabsecT);
  Serial.print("Comp Humidity: ");
  Serial.println(databsechum);
  Serial.print("Comp Gas: ");
  Serial.println(databsecgas);

  // Send each sensor value to the corresponding MQTT topic
  mqttClient.beginMessage(topicTime);  // Begin sending a message on the "time" topic
  mqttClient.print(dataT);             // Send the time value
  mqttClient.endMessage();             // End the message

  mqttClient.beginMessage(topicIAQ);  // Begin sending a message on the "iaq" topic
  mqttClient.print(databseciaq);     // Send the IAQ value
  mqttClient.endMessage();           // End the message

  mqttClient.beginMessage(topicVOC);  // Begin sending a message on the "voc" topic
  mqttClient.print(databsecvoc);     // Send the VOC value
  mqttClient.endMessage();           // End the message

  mqttClient.beginMessage(topicCO2);  // Begin sending a message on the "co2" topic
  mqttClient.print(databsecco2);     // Send the CO2 value
  mqttClient.endMessage();           // End the message

  mqttClient.beginMessage(topicCompTemp);  // Begin sending a message on the "comptemp" topic
  mqttClient.print(caldatabsecT);          // Send the compensated temperature value
  mqttClient.endMessage();                // End the message

  mqttClient.beginMessage(topicCompHum);  // Begin sending a message on the "comphum" topic
  mqttClient.print(databsechum);          // Send the compensated humidity value
  mqttClient.endMessage();                // End the message

  mqttClient.beginMessage(topicCompGas);  // Begin sending a message on the "compgas" topic
  mqttClient.print(databsecgas);          // Send the compensated gas concentration value
  mqttClient.endMessage();                // End the message

  // Print data to the Serial Monitor in CSV format for logging
  Serial.print(dataT);
  Serial.print(',');
  Serial.print(databseciaq);
  Serial.print(',');
  Serial.print(databsecvoc);
  Serial.print(',');
  Serial.print(databsecco2);
  Serial.print(',');
  Serial.print(caldatabsecT);
  Serial.print(',');
  Serial.print(databsechum);
  Serial.print(',');
  Serial.print(databsecgas);
  Serial.print('\n');  // End the line after printing each data reading
}

