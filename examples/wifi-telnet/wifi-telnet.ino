// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>


//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1

#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""

WiFiConnector *wifi;

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];


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
  init_hardware();
  wifi = init_wifi(WIFI_SSID, WIFI_PASSPHARSE, 0);

  Serial.print("CONNECTING TO ");
  Serial.println(wifi->SSID() + ", " + wifi->psk());

  wifi->on_connecting([&](const void* message)
  {
    Serial.print("Connecting -> ");
    Serial.println(wifi->counter);
    Serial.println((char*) message);
    delay(500);
  });

  wifi->on_connected([&](const void* message)
  {
    // Print the IP address
    Serial.print("WIFI CONNECTED: ");
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