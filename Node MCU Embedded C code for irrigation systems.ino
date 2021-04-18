#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "DHT.h"
#include "Adafruit_MQTT_Client.h"
#define DHTTYPE DHT11   // DHT 11

#define WLAN_SSID       "realme 3 Pro"
#define WLAN_PASS       "123456.06"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "Rishabhpandey"
#define AIO_KEY         "067af8f896e942bd971cf90120805d34"

uint8_t DHTPin = D4; 
DHT dht(DHTPin, DHTTYPE);
float Temp;
float Hum;

WiFiClient clien;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.

Adafruit_MQTT_Client mqtt(&clien, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

void setup() {
  Serial.begin(230400);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();
 // Connect to WiFi access point.

  Serial.println(); Serial.println();

  Serial.print("Connecting to ");

  Serial.println(WLAN_SSID);

 

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();

 Serial.println("WiFi connected");

  Serial.println("IP address: "); Serial.println(WiFi.localIP());

}


void loop(){
   MQTT_connect();
   delay(50);//waits for the DHT sensor to respond
    Temp = dht.readTemperature(); // Gets the values of the temperature
  Hum = dht.readHumidity(); // Gets the values of the humidity 
 // Sending data to the feeds
  Serial.print(F("\nSending Temperature value "));
  Serial.print("...");
  
  /*
   * Sending temperature data to the feed. The publish function will 
   * return a false if not succesful. The if statement gives you
   * the chance to trap an error and respond to it
   */
  if (! Temperature.publish(Temp)) { 
    Serial.println(F("Temperature Failed"));
  } else {
    Serial.println("Temperature: "+String(Temp)+"C");
  }
  Serial.print(F("\nSending Humidity val "));
 /*
   * Sending Humidity data to the feed. The publish function will 
   * return a false if not succesful. The if statement gives you
   * the chance to trap an error and respond to it
   */
  if (! Humidity.publish(Hum)) {
    Serial.println(F("Humidity Failed"));
  } else {
    Serial.println("Humidity: "+String(Hum)+"%");
  }

  
  delay(30000);
  
}







  // Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // reset me
        ESP.reset();
       }
  }
  Serial.println("MQTT Connected!");
}