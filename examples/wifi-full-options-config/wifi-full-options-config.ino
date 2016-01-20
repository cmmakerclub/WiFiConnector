// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector *wifi;

#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""
uint8_t SMARTCONFIG_BUTTON_PIN = 0;

#include "init_wifi.h"

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
  WiFi.enableAP(false);
  WiFi.onEvent([](WiFiEvent_t event) {
    Serial.println(event);
    switch (event) {
      case WIFI_EVENT_STAMODE_CONNECTED:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_STAMODE_CONNECTED");
        break;
      case WIFI_EVENT_STAMODE_DISCONNECTED:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WiFi lost connection");
        break;
      case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_STAMODE_AUTHMODE_CHANGE");
        break;
      case WIFI_EVENT_STAMODE_GOT_IP:
        Serial.print(millis());
        Serial.print(" => ");
        Serial.println("WIFI_EVENT_STAMODE_GOT_IP");
        Serial.println(WiFi.localIP());
        break;
      case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_STAMODE_DHCP_TIMEOUT");
        break;
      case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_SOFTAPMODE_STACONNECTED");
        break;
      case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_SOFTAPMODE_STADISCONNECTED");
        break;
      case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
        // Serial.print(" => ");
        // Serial.println("WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED");
        break;
      case WIFI_EVENT_MAX:
        Serial.print(millis());
        Serial.print(" => ");

        Serial.println("WIFI_EVENT_MAX");
        break;
    }
  });
  init_hardware();
  delay(200);
  WiFi.printDiag(Serial);
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
  wifi->loop();
}