// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector wifi("SSID", "PASSEORD");

#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""
uint8_t SMARTCONFIG_BUTTON_PIN = 0;

void init_hardware()
{
  Serial.begin(115200);
  delay(1000);
  Serial.flush();
  Serial.println();
  Serial.println();
  Serial.println("will be started in 500ms..");
}

void setup()
{
  init_hardware();
  // wifi.begin();
  // wifi.init();
}

void loop()
{
  // wifi.loop();
}
