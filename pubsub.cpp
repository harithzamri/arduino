#include <Ethernet.h>
#include <SPI.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"
#include "Adafruit_CCS811.h"
#include "DHT.h"
#include <PubSubClient.h>
#define DHTPIN 7
#define DHTTYPE DHT22

const char *mqtt_user ='eeyukxqp';
const char *mqtt_pass ='Dy1aiZ3nfPHL';
const char *mqtt_server = 'm12.cloudmqtt.com'

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // RESERVED MAC ADDRESS
EthernetClient Eclient;

PubSubClient client(Eclient);

SoftwareSerial soft(2, 3); // RX, TX

unsigned long lastSend;

Adafruit_CCS811 ccs;
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(9600);
  dht.begin();
  

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP"); 
  }
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }
  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  lastSend = 0;
  client.setServer(mqtt_server, 17389);
  client.setCallback(callback);
  reconnect();

  
}


void loop()
{
    if(!client.connected(){
        reconnect();
    }

    client.loop();

     float co2 = ccs.geteCO2();
     float voc = ccs.getTVOC();
     float t = dht.readTemperature();
     float h = dht.readHumidity();

    client.publish('temperature', t);
    client.publish('humidity', h);
    client.publish('VOC', voc);
    client.publish('Carbon Dioxide', co2);
}


void callback(char *topic, byte *payload, unsigned int length)
{
   Serial.print("Message Arrived [");
   Serial.print("topic");
   Serial.print("]");
   for(int = 0 ; i <length ; i++)
   {
     Serial.print((char)payload[i])
   }
   Serial.println();
}

void reconnect()
{
    while(!client.connected())
    {
        Serial.print("Attempting MQTT Connection ..");

        String clientID = "ESP8266Client-";
        clientID += String(random(0xffff) HEX);
        if (client.connect(clientID.c_str(), mqtt_user, mqtt_pass))
        {
          Serial.println("connected");
          client.publish("outTopic", "hello world");
          client.subscribe("inTopic");
        }
        else 
        {
          Serial.print("failed , rc = ");
          Serial.print(client.state());
          Serial.println("try again 5 seconds later");
          delay(5000);
        }
    }
}

