// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector *wifi;

#define WIFI_SSID        "Nat"
#define WIFI_PASSPHARSE  "devicenetwork"
uint8_t SMARTCONFIG_BUTTON_PIN = 2;

#include "init_wifi.h"

void init_hardware()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();    
  Serial.println("will be started in 500ms..");
  delay(500);  
}

void setup()
{
  init_hardware();
  wifi = init_wifi(WIFI_SSID, WIFI_PASSPHARSE, SMARTCONFIG_BUTTON_PIN);

  Serial.println("BEING CONNECTED TO: ");
  Serial.println(String(wifi->SSID() + ", " + wifi->psk()));
  Serial.print("SMARTCONFIG PIN: ");
  Serial.println(SMARTCONFIG_BUTTON_PIN);
  Serial.print("CORE FN()");
  Serial.println(String(WiFi.SSID() + ", " + WiFi.psk()));
  Serial.print("OK ?");

  wifi->on_connecting([&](const void* message)
  {
    Serial.print("STATE: CONNECTING -> ");
    Serial.println(wifi->counter);
    delay(500);
  });

  wifi->on_connected([&](const void* message)
  {
    // Print the IP address
    Serial.print("WIFI CONNECTED WITH IP: ");
    Serial.println(WiFi.localIP());
  });

  wifi->connect();

}

void loop()
{
  // wifi->loop();
}