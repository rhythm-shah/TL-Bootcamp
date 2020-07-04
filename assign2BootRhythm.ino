#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h> 

const char* ssid     = "";
const char* password = "";

int REDLED = 21;
int GREENLED = 32;
long ptime,currtime;
int acases,deaths,pacases,pdeaths;

void setup() 
{

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  Serial.begin(9600);

  WiFi.enableSTA(true);
  
  delay(2000);

  connectToWifi();
  currtime = 0;
  ptime = currtime;

  initializePcases();

}

void loop(){
  currtime = millis()/1000;
  if(WiFi.status() == WL_CONNECTED){
    if(currtime - ptime >= 86400){
      ptime = currtime;
      doTheTask();
    }
  }
  else{
     Serial.println("WiFi not connected. Please Turn it on.");
     delay(1000);
     connectToWifi();
  }
}

void connectToWifi()
{

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initializePcases()
{
      pacases = 235433;
      pdeaths = 18655; 
}

void doTheTask()
{
      HTTPClient httpac, httpdc;   
      httpac.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=RLOCJICJ6VECXJC6");  
      httpdc.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=BMRK3B3BAH61F2H6");
      if(httpac.GET()>0 && httpdc.GET()>0){ 
      String activeCases = httpac.getString();  
      String deathCount = httpdc.getString();
      acases = activeCases.toInt();
      deaths = deathCount.toInt();
      Serial.print("Active Cases  - ");
      Serial.println(acases); // printing on serial monitor
      Serial.print("Death Count  - ");
      Serial.println(deaths);
      float peracases = ((acases - pacases)/pacases)*100;
      float perdeaths = ((deaths - pdeaths)/pdeaths)*100;
      if(peracases>1 || perdeaths>1){
            digitalWrite(REDLED, HIGH);     
      }   
      else{
            digitalWrite(GREENLED, HIGH);                                                                    
      }
      pacases = acases;
      pdeaths = deaths;
}
}
