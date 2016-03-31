// #ifndef WIFI_CONNECTOR_H
// #define WIFI_CONNECTOR_H
/*

Copyright Nat Weerawan 2015-2016
MIT License

The MIT License (MIT)

Copyright (c) Nat Weerawan <nat.wrw@gmail.com> (cmmakerclub.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "WiFiConnector.h"

// static WiFiConnector* __wifi_connector = NULL;
WiFiConnector *_wifi = NULL;

WiFiConnector::WiFiConnector(String ssid, String password) {
  _ssid = ssid;
  _passphase = password;
};

void WiFiConnector::setSsid(String ssid){
  _ssid = ssid;
}

void WiFiConnector::setPasspharse(String pass){
  _passphase = pass;
}

String WiFiConnector::get(String key) {
  if (key == "ssid") {
    return WiFi.ssid();
  }
  else if (key == "psk") {
    return WiFi.psk();
  }
  else {
    return "";     
  }
}

void WiFiConnector::init() {
  _wifi = WiFiConnector::instance();
  _wifi->setSsid(_ssid);
  _wifi->setPasspharse(_passphase);
  _initialised = true;

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  static WiFiConnector *_this = this;
  WiFi.onEvent([](WiFiEvent_t event) {
      Serial.printf("[WiFi-event] event: %d\n", event);
      switch (event) {
          case WIFI_EVENT_STAMODE_CONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_CONNECTED\r\n", millis());
            if (_this->_user_on_connected) {
              _this->_user_on_connected((void*)"CONNECTED");
            }
            break;
          case WIFI_EVENT_STAMODE_DISCONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_DISCONNECTED\r\n", millis());
            if (_this->_user_on_disconnected) {
              _this->_user_on_disconnected((void*)"DISCONNECTED");
            }
            break;
          case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_AUTHMODE_CHANGE\r\n", millis());
            break;
          case WIFI_EVENT_STAMODE_GOT_IP:
            if (_this->_user_on_connected) {
              _this->_user_on_connected((void*)"CONNECTED");
            }
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_GOT_IP: ", millis());
            // Serial.println(WiFi.localIP());
            break;
          case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_DHCP_TIMEOUT: ", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_STACONNECTED: \r\n", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: \r\n", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED: \r\n", millis());
            break;
          case WIFI_EVENT_MAX:
            // Serial.printf("%lu => WIFI_EVENT_MAX: \r\n", millis());
            break;
          }
    });
}

void WiFiConnector::loop() {
  if (_initialised == false) {
    if (millis() % 1000 == 0) {
      Serial.println("not initialized.");
    }
    return;
  }
}

void WiFiConnector::on_disconnected(wifi_callback_t callback)
{
    _user_on_disconnected = callback;
}

void WiFiConnector::on_connected(wifi_callback_t callback)
{
  Serial.println("SET CALLBACK on_connected");
    _user_on_connected = callback;
}

void WiFiConnector::on_connecting(wifi_callback_t callback)
{
    _user_on_connecting = callback;
}

// #endif /* WIFI_CONNECTOR_H */
