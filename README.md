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

	
	#include <ESP8266WiFi.h>
	#include <WiFiConnector.h>
	
	/*
	  WIFI INFO
	  DELETE ALL IF YOU WANT TO USE FULL FUNCTION OF SMARTCONFIG
	*/
	
	#define WIFI_SSID        "Nat"
	#define WIFI_PASSPHARSE  "guestguest"
	
	WiFiConnector wifi = WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);
	
	void init_hardware()
	{
	  Serial.begin(115200);
	  delay(10);
	  Serial.println();
	  Serial.println("BEGIN");
	  
	  // SETUP CALLBACKS
	  wifi.on_connecting([&](const void* message)
	  {
	    char buffer[30];
	    sprintf(buffer, "[%d] connecting -> %s ", wifi.counter, (char*) message);
	    Serial.println(buffer);
	    delay(500);
	  });
	
	  wifi.on_connected([&](const void* message)
	  {
	    // Print the IP address
	    Serial.print("WIFI CONNECTED => ");
	    Serial.println(WiFi.localIP());
	  });
	
	}
	
	void setup()
	{
	  init_hardware();
	
	  Serial.print("CONNECTING TO ");
	  Serial.println(wifi.SSID() + ", " + wifi.psk());
	
	  wifi.connect();
	
	}
	
	void loop()
	{
	  wifi.loop();
	}

## อ่านบันเรื่องราว อัพเดต และอ่านเพิ่มเติม

[![Join the chat at https://gitter.im/cmmakerclub/WiFiConnector](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/cmmakerclub/WiFiConnector?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
