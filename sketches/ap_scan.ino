#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
const char* SSID = "USC Secure Wireless";

// Return RSSI or 0 if target SSID not found
int32_t getRSSI(const char* target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    if (strcmp(WiFi.SSID(network).c_str(), target_ssid) == 0) {
      return WiFi.RSSI(network);
    }
  }
  return 0;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void loop() {
  unsigned long before = millis();
  int32_t rssi = getRSSI(SSID);
  unsigned long after = millis();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dBm");

  Serial.print("Took ");
  Serial.print(after - before);
  Serial.println("ms");
  delay(1000);
}
