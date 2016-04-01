// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector wifi("Nat", "123456789");

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
  WiFi.disconnect(true);
  init_hardware();

  wifi.init();

  wifi.on_connected([&](const void* message)
  {
    Serial.print("XXXXXXXX: WIFI CONNECTED WITH IP: ");
    Serial.println(WiFi.localIP());
  });

  wifi.on_connecting([&](const void* message)
  {
    Serial.println("WIFI CONNECTING..");
    delay(200);
  });

  Serial.println(wifi.get("ssid") + ", " + wifi.get("password"));

  wifi.connect();
}

void loop()
{
  wifi.loop();
}
