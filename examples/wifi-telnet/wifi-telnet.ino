// Copyright Nat Weerawan 2015-2016
// MIT License

#define WIFI_DEBUG_MODE

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

WiFiConnector *wifi;

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

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
    server.begin();
    server.setNoDelay(true);


    Serial.print("Ready! Use 'telnet ");
    Serial.print(WiFi.localIP());
    Serial.println(" 23' to connect");

  });

  wifi->connect();

}

void loop()
{
  wifi->loop();
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.println(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }
}