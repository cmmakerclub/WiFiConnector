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

WiFiConnector::WiFiConnector(const char* ssid, const char* password, uint8_t smartconfig_pin)
{
    WIFI_DEBUG_PRINTLN("__CALLING WiFiConnector(ssid, pwd, smartconfig)");
    this->_smart_config_pin = smartconfig_pin;
    WIFI_DEBUG_PRINTLN("__CALLING WiFiConnector(ssid, password)");
    if (ssid == NULL || password == NULL) {
        use_smartconfig_wifi();
    }
    else if (strcmp(ssid, "") == 0 || strcmp(password, "") == 0) {
        use_smartconfig_wifi();
    }
    else {
        WIFI_DEBUG_PRINTLN("GOT SSID & PASSWORD");
        init_config(ssid, password);
    }
}



void WiFiConnector::use_smartconfig_wifi()
{
    WIFI_DEBUG_PRINTLN("__CALLING use_smartconfig_wifi()");
    const char* ssid = WiFi.SSID().c_str();

    WIFI_DEBUG_PRINT("SSID (");
    WIFI_DEBUG_PRINT(strlen(ssid));
    WIFI_DEBUG_PRINT("): ");
    WIFI_DEBUG_PRINTLN(ssid);

    const char* password = WiFi.psk().c_str();
    WIFI_DEBUG_PRINT("PASSWORD (");
    WIFI_DEBUG_PRINT(strlen(password));
    WIFI_DEBUG_PRINT("): ");
    WIFI_DEBUG_PRINTLN(password);

    init_config(WiFi.SSID().c_str(), WiFi.psk().c_str());
}


void WiFiConnector::init_config(const char* ssid, const char* password)
{
    pinMode(_smart_config_pin, INPUT_PULLUP);

    this->_ssid  = String(ssid);
    this->_password  = String(password);

    WIFI_DEBUG_PRINT("__CALLING init_config(");
    WIFI_DEBUG_PRINT(this->_ssid);
    WIFI_DEBUG_PRINT(", ");
    WIFI_DEBUG_PRINT(this->_password);
    WIFI_DEBUG_PRINTLN(")");
    prev_millis = millis();

    WIFI_DEBUG_PRINT("INIT CONFIG WITH SSID: ");
    WIFI_DEBUG_PRINTLN(this->_ssid);

    WIFI_DEBUG_PRINT("INIT CONFIG WITH password: ");
    WIFI_DEBUG_PRINTLN(this->_password);
}

void WiFiConnector::begin()
{
    WIFI_DEBUG_PRINTLN("__CALLING begin()");
    _connect();
}
void WiFiConnector::connect()
{
    WIFI_DEBUG_PRINTLN("__CALLING connect()");
    _connect();
}


void WiFiConnector::on_disconnected(wifi_callback_t callback)
{
    _user_on_disconnected = callback;
}

void WiFiConnector::on_connected(wifi_callback_t callback)
{
    _user_on_connected = callback;
}

void WiFiConnector::on_connecting(wifi_callback_t callback)
{
    _user_on_connecting = callback;
}

void WiFiConnector::on_smartconfig_waiting(wifi_callback_t callback)
{
    _user_on_smartconfig_waiting = callback;
}

void WiFiConnector::on_smartconfig_processing(wifi_callback_t callback)
{
    _user_on_smartconfig_processing = callback;
}

void WiFiConnector::on_smartconfig_done(wifi_callback_t callback)
{
    _user_on_smartconfig_done = callback;
}


void WiFiConnector::loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // return WiFi.status();
    }
    else
    {
        if (_user_on_disconnected != NULL)
        {
            _user_on_disconnected("wifi: WiFi disconnected.");
        }

        _connect();
    }
}

void WiFiConnector::_connect()
{

    WIFI_DEBUG_PRINT("__CALLING _connect() with: ");
    WIFI_DEBUG_PRINT(_ssid);
    WIFI_DEBUG_PRINT(", ");
    WIFI_DEBUG_PRINTLN(_password);

    _retries = 0;
    this->counter = 0;
    WiFi.begin(this->_ssid.c_str(), this->_password.c_str());

    while ((WiFi.status() != WL_CONNECTED))
    {
        if (_user_on_connecting != NULL)
        {
            static char buf[40];
            if (WiFi.status() == WL_CONNECT_FAILED) {
                strcpy(buf, "(4) WL_CONNECT_FAILED");
            }
            else if (WiFi.status() == WL_NO_SSID_AVAIL) {
                strcpy(buf, "(0) WL_NO_SSID_AVAIL");
            }
            else if (WiFi.status() == WL_IDLE_STATUS) {
                strcpy(buf, "(1) WL_IDLE_STATUS");
                WIFI_DEBUG_PRINTLN("FORCE ENTER SMART CONFIG");
                enter_smartconfig_mode();
            }
            else if (WiFi.status() == WL_DISCONNECTED) {
                strcpy(buf, "(6) WL_DISCONNECTED");
            }
            else {
                strcpy(buf, "CODE: !");
                itoa(WiFi.status(), buf+6, 10);
                buf[7] = '\0';
            }
            ++this->counter;
            _user_on_connecting((void*) buf);
        }

        _retries++;
        this->smartconfig_check(_smart_config_pin);
        yield();
    }


    if (_user_on_connected != NULL)
    {
        _user_on_connected("class: WIFI connected.");
    }

}


WiFiConnector::~WiFiConnector()
{

}
