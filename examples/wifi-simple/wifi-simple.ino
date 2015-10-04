// Copyright Nat Weerawan 2015-2016
// MIT License

#define WIFI_DEBUG_MODE

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector *wifi;

/* 
  WIFI INFO 
  DELETE ALL IF YOU WANT TO USE FULL FUNCTION OF SMARTCONFIG
*/

#define WIFI_SSID        "Nat"
#define WIFI_PASSPHARSE  "guestguest"

#include "init_wifi.h"

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");
}

void setup()
{
  uint8_t SMARTCONFIG_PIN = 0;
  init_hardware();
  init_wifi(SMARTCONFIG_PIN);
  
  Serial.print("CONNECTING TO ");
  Serial.println(wifi->SSID() + ", " + wifi->psk());
    
  wifi->on_connecting([&](const void* message)
  {
    char buffer[30];
    sprintf(buffer, "[%d] connecting -> %s ", wifi->counter, (char*) message);        
    Serial.println(buffer);
    delay(500);
  });

  wifi->on_connected([&](const void* message)
  {
    // Print the IP address    
    Serial.print("WIFI CONECTED: ");
    Serial.println(WiFi.localIP());
  });  
  
  wifi->connect();
  
}

void loop()
{
  wifi->loop();
}