#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);

String begin_s = "ESPTAG_BEGIN_";
String host_s = "ESPTAG_HOST_";
String ready_s = "ESPTAG_READY_";
String wait_s = "ESPTAG_WAIT_";
String healthy_s = "ESPTAG_HEALTHY_";
String infected_s = "ESPTAG_INFECTED_";
String end_s = "ESPTAG_END_";

String mySSID = "";
String yourSSID = "";
String gameID = "";
String storeSSID = "";

// Return RSSI or 0 if target SSID not found
int32_t getRSSI(String target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
  if ((WiFi.SSID(network)).equals(target_ssid.c_str())) {
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
  delay(100);
  Serial.begin(115200);
  unsigned long start_time = 0;;
  unsigned long end_time = 0;;
  bool foundhost = false;
  bool mintwo = false;
  int rand_sleep = 0;
  int count = 1;
  String tempSSID = "";
  int32_t rssi = 0;
  
  pinMode(D5, OUTPUT);   
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  
  OLED.begin();
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(false);
  OLED.setTextSize(2);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println(" BOOTING");
 
  OLED.display(); //output 'display buffer' to screen  
  OLED.startscrollleft(0x00, 0x0F); //make display scroll
	
  for(int i=0; i<5; i++)
  {
  gameID += String(random(0,9));
  }
  
  mySSID = begin_s + gameID;  
  Serial.println("Configuring access point...");
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  
  while(!foundhost)
  {
  WiFi.softAP(mySSID);
    
  Serial.println("Searching for host...");
    yourSSID = firstNet(host_s);
  if(yourSSID.equals(""))
  {
    Serial.println("Existing host not found. Checking for possible hosts...");
    yourSSID = firstNet(begin_s);
    tempSSID = firstNet(host_s);
    if(yourSSID.equals("")&&tempSSID.equals(""))
    {
      Serial.println("No possible host not found. Hosting new game...");
      mySSID = host_s + gameID;
      foundhost = true;
    }
    else
    {
      rand_sleep = count*random(1,1000);
      count++;
      Serial.println("Possible host found. Sleeping for " + String(double(rand_sleep)/1000) + " seconds...");
      WiFi.softAPdisconnect(true);
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
  
  if(!mySSID.indexOf(host_s+gameID))
  {
    start_time = millis();
    Serial.print("Waiting for players to join... ");
    do
    {
	delay(100);
        end_time = millis();
    } while((end_time-start_time)<30000);
    Serial.println("Done waiting!");
    storeSSID = mySSID;
    mySSID = ready_s + gameID;
  }
  
  if(!mySSID.indexOf(ready_s+gameID))
  {
    if(!storeSSID.indexOf(host_s+gameID))
    {
       mySSID = wait_s + gameID;
    }
    else
    {
	  Serial.print("Waiting for host... ");
      do
      {
        yourSSID = firstNet(host_s+gameID);
      } while(!yourSSID.equals(host_s+gameID));
	  Serial.println("Done waiting!");
	    
      storeSSID = mySSID;
      mySSID = wait_s + gameID;
    }
  }
  
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  
  if(!mySSID.indexOf(wait_s+gameID))
  {
	Serial.print("Waiting for everyone... ");
    do
    {
      yourSSID = firstNet(wait_s+gameID);
      tempSSID = firstNet(host_s+gameID);
    } while(!yourSSID.equals(wait_s+gameID)&&!tempSSID.equals(""));
	Serial.println("Done waiting!");
    
    if(storeSSID.equals(host_s+gameID))
    {
      mySSID = infected_s + gameID;
    }
    else
    {
      mySSID = healthy_s + gameID;
    }
  }
  
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
  
  start_time = millis();
  Serial.println("15 second grace period has begun...");
  do
  {
      delay(100);
      end_time = millis();
  } while((end_time-start_time)<15000);
  Serial.println();
  Serial.println("Grace period is now over.");
  Serial.println();
  
  start_time = millis();
  do
  {
	  if(!mySSID.indexOf(infected_s+gameID))
	  {
		  rssi = -1*getRSSI(healthy_s+gameID);
		  Serial.print("Signal strength: ");
		  Serial.println(rssi);
		  digitalWrite(D5, HIGH);
		  if(rssi<40 && rssi>0)
		  {
			  digitalWrite(D6, HIGH);
			  delay(100);
			  digitalWrite(D5, LOW);
			  digitalWrite(D6, LOW);
		  }
		  else if(rssi<60 && rssi>0)
		  {
			  digitalWrite(D7, HIGH);
			  delay(100);
			  digitalWrite(D5, LOW);
			  digitalWrite(D7, LOW);
		  }
		  else if(rssi>60)
		  {
			  digitalWrite(D8, HIGH);
			  delay(100);
			  digitalWrite(D5, LOW);
			  digitalWrite(D8, LOW);
		  }
		  else
		  {
			  delay(100);
			  digitalWrite(D5, LOW);
		  }
	  }
	  else if(!mySSID.indexOf(healthy_s+gameID))
	  {
		  rssi = -1*getRSSI(infected_s+gameID);
		  Serial.print("Signal strength: ");
		  Serial.println(rssi);
		  if(rssi<40 && rssi>0)
		  {
			  digitalWrite(D6, HIGH);
			  digitalWrite(D5, HIGH);
			  delay(100);
			  digitalWrite(D6, LOW);
			  digitalWrite(D5, LOW);
			  
			  mySSID = infected_s + gameID;
			  Serial.println("You have become infected.");
			  Serial.println();
			  Serial.println("Configuring access point...");
			  WiFi.softAP(mySSID);
			  Serial.println("" + mySSID + " ap started...");
			  Serial.println();
		  }
		  else if(rssi<60 && rssi>0)
		  {
			  digitalWrite(D7, HIGH);
			  delay(100);
			  digitalWrite(D7, LOW);
		  }
		  else if(rssi>60)
		  {
			  digitalWrite(D8, HIGH);
			  delay(100);
			  digitalWrite(D8, LOW);
		  }
		  else
		  {
			  delay(100);
		  }
	  }
      end_time = millis();
  } while((end_time-start_time)<60000);
  Serial.println("The game is over!");
  
  if(!mySSID.indexOf(infected_s+gameID))
  {
    Serial.println("You lost.");
  }
  else
  {
    Serial.println("You won!");
  }
  
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(false);
  OLED.setTextSize(2);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println("GAME OVER");
 
  OLED.display(); //output 'display buffer' to screen  
  OLED.startscrollleft(0x00, 0x0F); //make display scroll
	
  mySSID = end_s + gameID;
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(mySSID);
  Serial.println("" + mySSID + " ap started...");
  Serial.println();
}

void loop() {
  Serial.println("" + mySSID);
      delay(5000);
}
