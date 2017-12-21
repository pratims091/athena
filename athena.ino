/*
** Athena - Alexa voice controlled home automation
** Made with love with Pratim Sarangi <https://github.com/pratims091>
** First prototype
**
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
void LightOn();
void LightOff();
void NightLampOn();
void NightLampOff();
void FanOn();
void FanOff();
void WorkStationOn();
void WorkStationOff();

// Change this before you flash
const char* ssid = "Hogwarts";
const char* password = "alohomora";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *light = NULL;
Switch *nightLamp = NULL;
Switch *fan = NULL;
Switch *workStation = NULL;

void setup()
{
 pinMode (D1, OUTPUT);
 pinMode (D2, OUTPUT);
 pinMode (D3, OUTPUT);
 pinMode (D4, OUTPUT);
 digitalWrite (D1,HIGH);
 digitalWrite (D2,HIGH);
 digitalWrite (D3,HIGH);
 digitalWrite (D4,HIGH);
 Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    light = new Switch("Light", 80, LightOn, LightOff);
    nightLamp = new Switch("Night Lamp", 81, NightLampOn, NightLampOff);
    fan = new Switch("Fan", 82, FanOn, FanOff);
    workStation = new Switch("Work Station", 83, WorkStationOn, WorkStationOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*light);
    upnpBroadcastResponder.addDevice(*nightLamp);
    upnpBroadcastResponder.addDevice(*fan);
    upnpBroadcastResponder.addDevice(*workStation);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      light->serverLoop();
      nightLamp->serverLoop();
      fan->serverLoop();
      workStation->serverLoop();
	 }
}

void LightOn() {
    Serial.print("Light on turned on");
    digitalWrite (D1,LOW); 
}

void LightOff() {
    Serial.print("Light off turned off");
    digitalWrite (D1,HIGH); 
}

void NightLampOn() {
    Serial.print("Night lamp turned on");
    digitalWrite (D2,LOW); 
}

void NightLampOff() {
    Serial.print("Night lamp turned off");
    digitalWrite(D2,HIGH); 
}
void FanOn() {
    Serial.print("Fan turned on.");
    digitalWrite (D3,LOW); 
}

void FanOff() {
    Serial.print("Fan turned off.");
    digitalWrite (D3,HIGH); 
}

void WorkStationOn() {
    Serial.print("Work station turned on.");
    digitalWrite (D4,LOW); 
}

void WorkStationOff() {
  Serial.print("Work station turned off");
  digitalWrite(D4,HIGH); 
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 50){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
