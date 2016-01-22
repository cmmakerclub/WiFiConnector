# WiFiConnector

WiFi Connector เป็น Arduino Library สำหรับ ESP8266 ที่ถูกสร้างขึ้นมาให้ช่วยให้การต่อ WiFi ง่ายขึ้น และลดความซับซ้อนของโปรแกรมลงไปเพราะว่า การทำงานเป็นแบบเรียก callback เมื่อเกิด Event ต่างๆ

## Events

 - on_connecting
 - on_connected			
 - on_disconnected						
 - on_smartconfig_done				
 - on_smartconfig_waiting		
 - on_smartconfig_processing

## ความสามารถ

 - auto connect เมื่อ Router หรือ wifi ดับไป โดยจะทำงานที่ on_disconnected  และ on_connecting ตามลำดับ
 - รองรับ smartconfig โดยจะเข้าสู่ `on_smartconfig_waiting`, `on_smartconfig_processing`, `on_smartconfig_done` ตามลำดับ

## การเข้าโหมด smart config

 - ในกดปุ่ม flash หรือ GPIO 0 ค้างไว้เป็นเวลา 2 วินาที ในจังหวะที่กำลังงตอนต่อ WiFi เท่านั้น หากกดหลังต่อ WiFi สำเร็จแล้วจะไม่เกิดผลใดๆ

## ตัวอย่างการใช้งาน

    	
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
      init_hardware();
      delay(200);
      wifi = init_wifi(WIFI_SSID, WIFI_PASSPHARSE, SMARTCONFIG_BUTTON_PIN);
      Serial.println("BEING CONNECTED TO: ");
      Serial.println(String(wifi->SSID() + ", " + wifi->psk()));

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

## อ่านบันเรื่องราว อัพเดต และอ่านเพิ่มเติม

[![Join the chat at https://gitter.im/cmmakerclub/WiFiConnector](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/cmmakerclub/WiFiConnector?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
