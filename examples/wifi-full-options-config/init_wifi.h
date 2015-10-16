void init_wifi(uint8_t smartconfig_pin)
{
  wifi = new WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);
  
  wifi->on_disconnected([&](const void* message)
  {
    Serial.println("WIFI DISCONNECTED.");
    // Serial.println ((char*)message);
  });

  wifi->on_smartconfig_waiting([&](const void* message)
  {
    Serial.println("ENTER SMARTCONFIG.");
  });

  wifi->on_smartconfig_done([&](const void* message)
  {
    Serial.println("SMARTCONFIG DONE.");
  });

  wifi->on_smartconfig_processing([&](const void* message)
  {
    // Serial.println("CONFIGURING WIFI..");
    // delay(500);
  });
}