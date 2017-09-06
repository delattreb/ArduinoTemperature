#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "FreeboxDELMaison"
#define WLAN_PASS       "!KillBill10!"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883  // use 8883 for SSL
#define AIO_USERNAME    "delattreb"
#define AIO_KEY         "66c9d289b70d417da29c7c1f516c0fe7"
#define nullvalue 9999

WiFiClient client;
//WiFiClientSecure client; // or use WiFiFlientSecure for SSL
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10); // TODO Wait for serial
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

float valf= nullvalue;
char command[10];

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) { // TODO val set 10 5000 before
    if (subscription == &onoffbutton) {
      //Serial.print(F("Got: "));
      //Serial.println((char *)onoffbutton.lastread);
    }
  }

  if (valf != nullvalue)
  {
    Serial.print(F("Sending photocell val: "));
    Serial.print(valf);
    Serial.print("...");
    if (! photocell.publish(valf)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    valf= nullvalue;
  }
    
  // ping the server to keep the mqtt connection alive NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
  if(Serial.available()) 
  {
    command[0] = '\n';
    int i=0;
    while(Serial.available())
    {
      command[i] = (char)Serial.read();
      i++;
    }
    command[i] = '\n';
    Serial.print("Read: ");
    Serial.println(command);
    valf = atof(command);
    }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) 
    return;
  
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
