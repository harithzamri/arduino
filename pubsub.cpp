#include <Ethernet.h>
#include <SPI.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"
#include "Adafruit_CCS811.h"
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT22

const char *mqtt_user ="eeyukxqp";
const char *mqtt_pass ="Dy1aiZ3nfPHL";
const char *mqtt_server ="m14.cloudmqtt.com";


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
  client.setServer("m16.cloudmqtt.com", 11588);
  client.setCallback(callback);
  reconnect();

  
}


void loop()
{
    if(!client.connected()){
        reconnect();
    }

    client.loop();

     if(ccs.available()){
      float temp = ccs.calculateTemperature();
     }
     if(!ccs.readData()){
     float co2 = ccs.geteCO2();
     float voc = ccs.getTVOC();
     float t = dht.readTemperature();
     float h = dht.readHumidity();


      delay(1000);
      String tt = String(tt);

      int numt = t;
      char cstr[16];
      itoa(numt, cstr, 10);

      int numh = h;
      char cshr[16];
      itoa(numh, cshr, 10);

      int numvoc = voc;
      char csvoc[16];
      itoa(numvoc, csvoc, 10);

      int numco2 = co2;
      char csco2[16];
      itoa(numco2, csco2, 10);

    client.publish("temperature", cstr);
    delay(1000);
    client.publish("humidity", cshr );
    delay(1000);
    client.publish("voc", csvoc );
    delay(1000);
    client.publish("co2", csco2);
    }
   
}


void callback(char *topic, byte *payload, unsigned int length)
{
   Serial.print("Message Arrived [");
   Serial.print("topic");
   Serial.print("]");
   
   for(int i=0;i<length;i++)
   {
     Serial.print((char)payload[i]); 
   }
   
   Serial.println();
}

void reconnect()
{
    while(!client.connected())
    {
        Serial.print("Attempting MQTT Connection ..");

        String clientID = "ESP8266Client-";
        clientID += String(random(0xffff),HEX);
        if (client.connect(clientID.c_str(), "jhfsrkth", "LeA8c8Q7ZPoH"))
        {
          Serial.println("connected");
          client.publish("outTopic", "hello world");
          
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
