// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <ESP8266WebServer.h>
#include "FS.h"

#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""


WiFiConnector *wifi;
ESP8266WebServer server(80);


void fail(const char* msg) {
  Serial.println(msg);
  while (true) {
    yield();
  }
}

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");

  wifi = new WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);

  if (!SPIFFS.begin()) {
    fail("SPIFFS init failed");
  }


  Dir root = SPIFFS.openDir("/");
  Serial.println("READING ROOT");
  while (root.next()) {
    String fileName = root.fileName();
    File f = root.openFile("r");
    Serial.printf("%s: %d\r\n", fileName.c_str(), f.size());
  }

  // SETUP CALLBACKS
  wifi->on_connecting([&](const void* message)
  {
    Serial.println("Connecting -> ");
    Serial.println(wifi->counter);
    Serial.println((char*) message);
    delay(500);
  });


  wifi->on_connected([&](const void* message)
  {
    // Print the IP address
    Serial.print("WIFI CONNECTED => ");
    Serial.println(WiFi.localIP());

    server.on ( "/test", []() {
      char buff[100];
      String ms = String(millis());
      sprintf(buff, "{\"millis\": %s }", ms.c_str());
      server.send ( 200, "text/plain", buff );
    } );

    server.on ( "/hello", []() {
      server.send ( 200, "text/plain", String("Hi, there"));
    } );    
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
  });

}

void setup()
{
  init_hardware();

  Serial.print("CONNECTING TO ");
  Serial.println(wifi->SSID() + ", " + wifi->psk());

  wifi->connect();

}

void loop()
{
  wifi->loop();
  server.handleClient();
}
