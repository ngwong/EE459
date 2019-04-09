//Matt's version of Wemos tag game
//
//
//
//

#include <ESP8266WiFi.h>


//#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 
//#include <ESP8266WebServer.h>


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  //Serial.print("Configuring access point...");
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");


}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
      Serial.println("no networks found");
     } 
    else {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
         Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
        delay(10);
      }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);

  

}
