#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#define DHTPIN 5     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

const char* ssid = "ESP32";
const char* password =  "12345678";
 /* Put IP Address details */
IPAddress local_ip(192,168,1,8);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

String hum = "123";
String tmp = "12";
String alarmm = "a";

AsyncWebServer server(80);

float range = 40.0;
float tempre = 20.0;

void setup() {
  Serial.begin(9600);
  
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);
  Serial.println(WiFi.localIP());
 servers();
  server.begin();
  
//  delayMS = sensor.min_delay / 1000;
  pinMode(26,INPUT);
  pinMode(27,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(25,OUTPUT);
}

void servers()
{
  server.on("/setdata", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    Serial.println(paramsNr);
    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        range = p->value().toFloat();
        Serial.println("------");
    }
    String s = "";
    s.concat(String(tmp));
    s.concat("-");
    s.concat(String(hum));
      s.concat("-");
    s.concat(String(alarmm)); //
     AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", s);
     
//     response->addHeader("Access-Control-Allow-Origin", "*");
//    response->addHeader("Access-Control-Expose-Headers", "*");
//    response->addHeader("Access-Control-Allow-Credentials", "true");
//    response->addHeader("Server","ESP Async Web Server");

    request->send(response);

  });
  }
void loop() {
  // Delay between measurements.
  delay(500);
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    tmp = String(event.temperature);
    tempre = float( event.temperature);
    Serial.println(F("°C"));
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    hum = String(event.relative_humidity);
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  tempRange();
}

void tempRange(){
  if (!digitalRead(26) || tempre >= range ){
    digitalWrite(14,LOW);
    digitalWrite(27,HIGH);
    digitalWrite(12,LOW);
    alarmm = "tr";
    buzz();
    range= 40.0;
    //red + buzzer
    }
  else 
    Serial.println(tempre);
  if(tempre < 25){
      digitalWrite(27,LOW);
      digitalWrite(14,LOW);
      digitalWrite(12,HIGH);
      alarmm = "f";
      //blue
  }
  else if(tempre < 28){
    digitalWrite(14,HIGH);
    digitalWrite(27,LOW);
    digitalWrite(12,LOW);
    alarmm = "fse";
    //green
  } else
    {
      digitalWrite(27,HIGH);
      digitalWrite(14,LOW);
      digitalWrite(12,LOW);
      alarmm = "f";
    //red
    }
  }

  void buzz(){
  for(int i = 0 ; i< 5 ; i++){
    digitalWrite(25,HIGH);
    delay(500);
    digitalWrite(25,LOW);
    delay(500);
  }
}
