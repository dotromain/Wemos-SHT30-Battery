////////////////////////////////////////////////////////
///Include Librarys
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WEMOS_SHT3X.h>

////////////////////////////////////////////////////////
///DeepSleep
boolean deepSleep      = true; //true or false. If true, you have to connect RST pin to D0 pin
const int sleepSeconds = 60;   //Enter sleep time in second

////////////////////////////////////////////////////////
///Locate SHT30 Pin
SHT3X sht30(0x45);

////////////////////////////////////////////////////////
///WIFI
#define ssid     "YOUR_SSID"
#define password "WIFI_PASSWORD"

////////////////////////////////////////////////////////
///MQTT
#define mqtt_server_ip          "YOUR_MQQT_BROKER_IP"
#define mqtt_server_port        "YOUR_MQQT_BROKER_PORT"       //If you don't know your MQTT broker port, fill this field with "1883"
#define mqtt_user               "YOUR_MQQT_BROKER_LOGIN"      //Optional
#define mqtt_password           "YOUR_MQQT_BROKER_PASSWORD"   //Optional
#define mqtt_humidity_topic     "YOUR_MQTT_HUMIDITY_TOPIC"    //E.G. sensor/ESP8266/SHT30/humidity
#define mqtt_temperature_topic  "YOUR_MQTT_TEMPERATURE_TOPIC" //E.G. sensor/ESP8266/SHT30/temperature
#define mqtt_voltage_topic      "YOUR_MQTT_VOLTAGE_TOPIC"     //E.G. sensor/ESP8266/SHT30/voltage
WiFiClient espClient;
PubSubClient mqqtclient(espClient);

////////////////////////////////////////////////////////
///FUNCTIONS
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);   //Init Wemos BUILTIN_LED
  digitalWrite(BUILTIN_LED, LOW); //Start LED
  Serial.begin(115200);           //Init Serial
  
  ////////////////////////////////////////////////
  //Connect to wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  
  mqqtclient.setServer(mqtt_server_ip, 1883); //Connect to MQTT
  digitalWrite(BUILTIN_LED, HIGH);            //Stop LED
}

void errorLedBlinking(char* error) {
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.print(error);
  Serial.println(" FAILURE");
  int value = 0;
  while (value < 3) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    value++;
  }
}

void checkMQTTStatus() {
  while (!mqqtclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqqtclient.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print(" failed, Error value=");
      Serial.print(mqqtclient.state());
      Serial.println(" try again in 5 seconds");
      errorLedBlinking("MQTT");
      delay(2000);      // Wait 2 seconds before retrying
    }
  }
}

void loopLed() {
  digitalWrite(BUILTIN_LED, LOW);  // turn on LED with voltage HIGH
  delay(300);                      // wait
  digitalWrite(BUILTIN_LED, HIGH); // turn off LED with voltage LOW
}

void loop() {
  ////////////////////////////////////////////////
  // Check WIFI connection
  while (WiFi.status() != WL_CONNECTED) {
    errorLedBlinking("WIFI");
    delay(2000);
  }

  ////////////////////////////////////////////////
  // Check or init MQQT connection
  while (!mqqtclient.connected()) {
    checkMQTTStatus();
  }
  
  ////////////////////////////////////////////////
  // Read humidity and temperatur from SHT30
  sht30.get();
  float humidity = sht30.humidity;
  float temperature = sht30.cTemp;
  
  ////////////////////////////////////////////////
  //Calculating battery voltage
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);

  ////////////////////////////////////////////////
  //Add in MQTT
  mqqtclient.publish(mqtt_temperature_topic, String(temperature).c_str(), true);
  mqqtclient.publish(mqtt_humidity_topic, String(humidity).c_str(), true);
  mqqtclient.publish(mqtt_voltage_topic, String(voltage).c_str(), true);

  loopLed();
  if (deepSleep) {
    Serial.printf("Deep sleep for %d seconds\n\n", sleepSeconds);
    ESP.deepSleep(sleepSeconds * 1000000);
  } else {
    Serial.printf("Sleep for %d seconds\n\n", sleepSeconds);
    delay(sleepSeconds * 1000);
  }
}