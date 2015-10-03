
#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include "ESP8266WiFi.h"
#include <functional>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

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

    uint32_t counter = 0;
    WiFiConnector(const char* ssid, const char* password);
    WiFiConnector();
    ~WiFiConnector();
    void init_config(const char*, const char*);
    void connect();
    void begin();
    void loop();

    void smartconfig_check(uint8_t pin) {
        pinMode(pin, INPUT_PULLUP);
        long memo = millis();
        if (digitalRead(pin) == LOW) {
          while (digitalRead(pin) == LOW) {
            if (millis() - memo > 2000) {
                enter_smartconfig_mode();
                break;
            }
            yield();
          }
        }
    }

    void enter_smartconfig_mode() {
        _on_smartconfig_waiting();

        WIFI_DEBUG_PRINTLN("BEGIN SMART CONFIG" );
        WiFi.beginSmartConfig();
        while (!WiFi.smartConfigDone()) {
            yield();
            _on_smartconfig_processing();
        }
        _on_smartconfig_done();
    }

    String get(String key) {
        if (key == "ssid") {
            return _ssid;

        }
        else if (key == "password") {
            return _password;
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

protected:

private:
    String _ssid = "";
    String _password = "";
    String _mac = "";
    int SMART_CONFIG_PIN = 0;

    wifi_callback_t _user_on_disconnected = NULL;
    wifi_callback_t _user_on_connected  = NULL;
    wifi_callback_t _user_on_connecting = NULL;
    wifi_callback_t _user_on_smartconfig_waiting = NULL;
    wifi_callback_t _user_on_smartconfig_processing = NULL;
    wifi_callback_t _user_on_smartconfig_done = NULL;    

    unsigned long _retries = 0;
    unsigned long prev_millis;

    void _on_smartconfig_done() {
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
    }


    void _on_smartconfig_waiting() {
        if (_user_on_smartconfig_waiting) {
            _user_on_smartconfig_waiting((void*) "enter");
        }
    }

    void _connect();
};



#endif//WIFI_CONNECTOR_H
