
#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <functional>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

// #define WIFI_DEBUG_MODE

#ifdef WIFI_DEBUG_MODE
    #define WIFI_DEBUG_PRINTER Serial
    #define WIFI_DEBUG_PRINT(...) { WIFI_DEBUG_PRINTER.print(__VA_ARGS__); }
    #define WIFI_DEBUG_PRINTLN(...) { WIFI_DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define WIFI_DEBUG_PRINT(...) { }
    #define WIFI_DEBUG_PRINTLN(...) { }
#endif

// TYPES
// typedef enum {
//     WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//     WL_IDLE_STATUS      = 0,
//     WL_NO_SSID_AVAIL    = 1,
//     WL_SCAN_COMPLETED   = 2,
//     WL_CONNECTED        = 3,
//     WL_CONNECT_FAILED   = 4,
//     WL_CONNECTION_LOST  = 5,
//     WL_DISCONNECTED     = 6
// } wl_status_t;


class WiFiConnector
{
public:
    typedef std::function<void(const void*)> wifi_callback_t;
    typedef std::function<void(void)> void_callback_t;
    uint32_t counter = 0;
    WiFiConnector(const char* ssid, const char* password, uint8_t smartconfig_pin = 0);
    ~WiFiConnector();
    void init_config(const char*, const char*);
    void connect();
    void begin();
    void loop();

    void long_press_check(uint8_t pin, void_callback_t callback) {
        pinMode(pin, INPUT_PULLUP);
        long memo = millis();
        if (digitalRead(pin) == LOW) {
          while (digitalRead(pin) == LOW) {
            if (millis() - memo > 2000) {
                Serial.println("LONG PRESSED > 2000");
                callback();
                break;
            }
            yield();
          }
        }
    }

    void enter_smartconfig_mode() {
        WiFi.mode(WIFI_STA);

        blink(20);

        WIFI_DEBUG_PRINTLN("BEGIN SMART CONFIG" );
        _on_smartconfig_waiting();
        delay(500);

        WiFi.beginSmartConfig();
        while (!WiFi.smartConfigDone()) {
            yield();
            _on_smartconfig_processing();
        }
        _on_smartconfig_done();
    }

    String get(String key) {
        WIFI_DEBUG_PRINT(String("GETTING ...") + key + " = ");
        if (key == "ssid") {
            WIFI_DEBUG_PRINTLN(this->_ssid);
            return _ssid;
        }
        else if (key == "password") {
            WIFI_DEBUG_PRINTLN(this->_password);
            return _password;
        }
        else if (key == "mac") {
            uint8_t mac[6];
            WiFi.macAddress(mac);
            String macAddr;
            for (int i = 0; i < 6; ++i)
            {
               macAddr += String(mac[i], 16);
            }
            // WiFi.macAddress();
            return macAddr;
        } 
    }


    
    String SSID() {
        return this->get("ssid");
    }
    
    String psk() {
        return this->get("password");
    }

    // CALLBACKS
    void on_disconnected(wifi_callback_t callback = NULL);
    void on_connected(wifi_callback_t callback = NULL);
    void on_connecting(wifi_callback_t callback = NULL);
    void on_smartconfig_processing(wifi_callback_t callback = NULL);
    void on_smartconfig_waiting(wifi_callback_t callback = NULL);
    void on_smartconfig_done(wifi_callback_t callback = NULL);

    void setLed(uint8_t);

protected:

private:
    int _ledPin = 255;
    ESP8266WebServer *_server;
    bool _server_started = false;
    Ticker _ticker;
    String _mac = "";
    String _ssid;
    String _password;    
    int _smart_config_pin = 0;

    wifi_callback_t _user_on_disconnected = NULL;
    wifi_callback_t _user_on_connected  = NULL;
    wifi_callback_t _user_on_connecting = NULL;
    wifi_callback_t _user_on_smartconfig_waiting = NULL;
    wifi_callback_t _user_on_smartconfig_processing = NULL;
    wifi_callback_t _user_on_smartconfig_done = NULL;    

    unsigned long _retries = 0;
    unsigned long prev_millis;
    void use_smartconfig_wifi();
    void blink(uint32_t);

    void _on_smartconfig_done() {
        // WiFi.disconnect();
        // WiFi.mode(WIFI_STA);
        if (_user_on_smartconfig_done) {
            _user_on_smartconfig_done((void*) "done");
        }
        WIFI_DEBUG_PRINTLN("CONFIG SUCCESS");
        static struct station_config conf;
        wifi_station_get_config(&conf);
        const char* ssid = reinterpret_cast<const char*>(conf.ssid);
        WIFI_DEBUG_PRINT("SSID (");
        WIFI_DEBUG_PRINT(strlen(ssid));
        WIFI_DEBUG_PRINT("): ");
        WIFI_DEBUG_PRINTLN(ssid);

        const char* passphrase = reinterpret_cast<const char*>(conf.password);
        WIFI_DEBUG_PRINT("Passphrase (");
        WIFI_DEBUG_PRINT(strlen(passphrase));
        WIFI_DEBUG_PRINT("): ");
        WIFI_DEBUG_PRINTLN(passphrase);

        init_config(ssid, passphrase);
    }

    void _on_smartconfig_processing() {
        if (_user_on_smartconfig_processing) {
            _user_on_smartconfig_processing((void*) "doing");
        }

        long_press_check(_smart_config_pin, [&]() {
            WiFi.persistent(false);
            WiFi.stopSmartConfig();
            // WiFi.disconnect();
            // WiFi.softAPdisconnect();
            blink(1000);

            // WiFi.mode(WIFI_AP);
            // delay(3000);
            const String ap = String(String("NAT_") + millis());
            Serial.println(ap.c_str());
            WiFi.softAP(ap.c_str());
            delay(3000);
            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);
            _server = new ESP8266WebServer(80);

            _server->on("/inline", [&](){
                _server->send(200, "text/plain", "this works as well");
            });

            _server->on("/", [&](){
                String SSID = _server->arg("ssid");
                String PASSPHARSE = _server->arg("password");
                SSID.replace("+", " ");
                SSID.replace("%40", "@");
                Serial.println(SSID);
                Serial.println(PASSPHARSE);
                _server->send(200, "text/html", "");

                if (SSID != NULL && PASSPHARSE != NULL) {
                    WiFi.persistent(true);
                    Serial.println("GOT SSID");
                    WiFi.begin(SSID.c_str(), PASSPHARSE.c_str());
                    _server->sendContent("OK");
                    _server->client().stop(); // Stop is needed because we sent no content length
                    ESP.reset();
                }
                else {
                  _server->sendContent(
                    "<html><head><title>Setting</title></head><body>"
                    "\r\n<br /><form method='POST' action='setting'><h4>Connect to network:</h4>"
                    "<input type='text' placeholder='network' name='ssid'/>"
                    "<br /><input type='password' placeholder='password' name='password'/>"
                    "<br /><input type='submit' value='Connect/Disconnect'/></form>"
                    "<p>You may want to <a href='/'>return to the home page</a>.</p>"
                    "</body></html>"
                  );
                  
                  _server->client().stop(); // Stop is needed because we sent no content length
                }

            });

            _server_started = true;
            _server->begin();
            Serial.println("SERVER STARTED");
        });
        if (_server_started) {
            _server->handleClient();
        }
    }


    void _on_smartconfig_waiting() {
        if (_user_on_smartconfig_waiting) {
            _user_on_smartconfig_waiting((void*) "enter");
        }
    }

    void _connect();

};



#endif //WIFI_CONNECTOR_H
