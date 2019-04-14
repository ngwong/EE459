#include <Arduino.h>
#include <ESP8266WiFi.h>

String mySSID = "";
String yourSSID = "";
String gameID = "";

/*
// Return RSSI or 0 if target SSID not found
int32_t getRSSI(char* target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    if (strcmp(WiFi.SSID(network).c_str(), target_ssid) == 0) {
      return WiFi.RSSI(network);
    }
  }
  return 0;
}
*/

// Look for game of ESPTAG and return its network name
String searchGame(String theSSID) {
  String searchSSID = theSSID;
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    if (!(WiFi.SSID(network)).indexOf(searchSSID)) {
      return WiFi.SSID(network);
    }
  }
  return "";
}

void setup() {
  delay(100);
  Serial.begin(115200);
  String theHost = "";
  unsigned long start_time;
  unsigned long end_time;
  
  yourSSID = "ESPTAG_HOST_";
  Serial.println("Searching for host...");
  theHost = searchGame(yourSSID);
  if(theHost.equals(""))
  {
    Serial.println("Not found. Hosting new game...");
    mySSID = "ESPTAG_HOST_";
	if(gameID.equals(""))
	{
		for(int i=0; i<5; i++)
		{
			gameID += String(random(0,9));
		}
	}
	mySSID += gameID;
  }
  else
  {
    Serial.println("Found the host " + theHost+ ". Processing...");
	gameID = theHost.substring(12);
	mySSID = "ESPTAG_JOIN_" + gameID;
  }
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " started...");
  if(!mySSID.indexOf("ESPTAG_HOST"))
  {
	  start_time = millis();
	  do
	  {
	  	  delay(10000);
	  	  Serial.println("Waiting for players to join...");
	  	  end_time = millis();
	  }while((end_time-start_time)<60000);
  }
  
  
  
  start_time = millis();
  do
  {
	  end_time = millis();
  }while((end_time-start_time)<600000);
  

}

void loop() {
	Serial.println("" + mySSID);
	delay(5000);
  /*
  unsigned long before = millis();
  int32_t rssi = getRSSI(yourSSID);
  unsigned long after = millis();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dBm");

  Serial.print("Took ");
  Serial.print(after - before);
  Serial.println("ms");
  delay(1000);
  */
}
