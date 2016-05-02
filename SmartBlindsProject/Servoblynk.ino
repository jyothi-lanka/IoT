#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "Jyothi_Pi_AP";
const char* password = "SaiJyothi";
IPAddress server(192, 168, 137, 245);
char auth[] = "52093849d2ba420c8de1a6504570aa64"; // You should get Auth Token in the Blynk App.
int prev=10;
Servo myservo;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

#define DHTPIN 11     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
 
  Blynk.begin(auth, "Jyothi_Pi_AP", "SaiJyothi");
  delay(1000);
  setup_wifi();
  client.setServer(server, 1883);

  dht.begin();
  myservo.attach(13);
}

// Write to menu item in Blynk App
BLYNK_WRITE(V0)
{
  switch (param.asInt())
  {
    
    case 0: 
    { // Item 1
      
      if(!prev==0)
      {
        myservo.attach(13);
      }
      Serial.println("Item 1 selected");
      myservo.write(80);                  // rotate counterclockwise full speed
      delay(2000);         
      myservo.detach(); 
      client.publish("blindsPosition", "open");  
      Blynk.virtualWrite(V5, "opened");
      prev=0;          
      break;
    }
    
    case 1: 
    { // Item 2
      Serial.println("Item 2 selected");
      if(!prev==1)
      {
        myservo.attach(13);
      }  
      myservo.write(135);                  // rotate counterclockwise full speed
      delay(2000);         
      myservo.detach(); 
      client.publish("blindsPosition", "close");
      Blynk.virtualWrite(V5, "closed");
      prev=1;          
      break;
    }    
  }
}  


// To Display on Blynk LCD at pin V2
BLYNK_WRITE(V2)
{
   
  float f = dht.readTemperature(true);  // Read temperature as Fahrenheit (isFahrenheit = true)
  float h = dht.readHumidity();
  
  float hif = dht.computeHeatIndex(f, h);  // Compute heat index in Fahrenheit (the default)
  Blynk.virtualWrite(V2, f);
}

// To Timer on Blynk at pin V3
BLYNK_WRITE(V3)
{
      myservo.attach(13);
      myservo.write(135);                  // rotate counterclockwise full speed close
      delay(2000);         
      myservo.detach(); 
}
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Blynk.run();
  
}
