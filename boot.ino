#include <Arduino.h>
#include <ESP8266WiFi.h>

String begin_s = "ESPTAG_BEGIN_";
String host_s = "ESPTAG_HOST_";
String ready_s = "ESPTAG_READY_";
String healthy_s = "ESPTAG_HEALTHY_";
String infected_s = "ESPTAG_INFECTED_";
String end_s = "ESPTAG_END_";

String mySSID = "";
String yourSSID = "";
String gameID = "";


// Return RSSI or 0 if target SSID not found
int32_t getRSSI(String target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
	if (strcmp(WiFi.SSID(network).c_str(), target_ssid.c_str()) == 0) {
	  return WiFi.RSSI(network);
	}
  }
  return 0;
}

// Look for the first host that contains the parameter in its network name
String firstNet(String host_param) {
	String hostid = host_param;
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
	if (!(WiFi.SSID(network)).indexOf(hostid)) {
	  return WiFi.SSID(network);
	}
  }
  return "";
}

void setup() {
  Serial.begin(115200);
  unsigned long start_time = 0;;
  unsigned long end_time = 0;;
  bool foundhost = false;
  bool mintwo = false;
  int rand_sleep = 0;
  
  for(int i=0; i<5; i++)
  {
	gameID += String(random(0,9));
  }

  /*
  mySSID = begin_s + gameID;  
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  */
  
  while(!foundhost)
  {
	mySSID = begin_s + gameID;  
    Serial.println("Configuring access point...");
    WiFi.softAP(mySSID);
    Serial.println("" + mySSID + " ap started...");
    Serial.println();
	  
	Serial.println("Searching for host...");
    yourSSID = firstNet(host_s);
	if(yourSSID.equals(""))
	{
		Serial.println("Existing host not found. Checking for possible hosts...");
		yourSSID = firstNet(begin_s);
		if(yourSSID.equals(""))
		{
			mySSID = host_s + gameID;
			foundhost = true;
		}
		else
		{
			WiFi.softAPdisconnect(true);
			rand_sleep = random(1,1000);
			Serial.println("Possible host found. Sleeping for " + String(double(rand_sleep)/1000) + " seconds...");
			delay(rand_sleep);
		}
	}
	else
	{
		Serial.println("Found the host " +yourSSID+ ". Processing...");
		gameID = yourSSID.substring(12);
		mySSID = ready_s + gameID;
		foundhost = true;
	}
  }
  
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  
  if(!mySSID.indexOf(host_s))
  {
	  start_time = millis();
	  Serial.print("Waiting for players to join");
	  do
	  {
	  	  delay(5000);
	  	  Serial.print(".");
	  	  end_time = millis();
	  }while((end_time-start_time)<60000);
	  mySSID = infected_s + gameID;
	  Serial.println();
  }
  
  if(!mySSID.indexOf(ready_s))
  {
	  do
	  {
		  delay(100);
		  yourSSID = firstNet(host_s+gameID);
	  }while(!yourSSID.equals(host_s+gameID));
	  mySSID = healthy_s + gameID;
  }
  
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  
  
  /*
  start_time = millis();
  do
  {
	  end_time = millis();
  }while((end_time-start_time)<600000);
  */
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
