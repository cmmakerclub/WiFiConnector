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

void WiFiConnector::init() {
  _wifi = WiFiConnector::instance();
  _wifi->setSsid(_ssid);
  _wifi->setPasspharse(_passphase);
  _initialised = true;
}

void WiFiConnector::loop() {
  if (_initialised == false) {
    if (millis() % 1000 == 0) {
      Serial.println("not initialized. return");
    }
    return;
  }
}
