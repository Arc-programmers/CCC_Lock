
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define relay D2
#define LED_G D1//define green LED pin
#define buzzer D7

#define ACCESS_DELAY 4000
#define DENIED_DELAY 1000

//paste the hidden code id: 1
//This part of code is removed for confidential information
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

//paste the hidden code id: 2
//This part of code is removed for confidential information
#define wifiName ""
#define password ""

// #define wifiName "ALIGATOR 7499"
// #define password "12341234"

bool flag=false;

WiFiClient wifiClient;
 
//access function
//1 Access granted
//2 Access denied
void access(int a){
  switch(a)
  {
    case 1: digitalWrite(relay,HIGH);
            for(int i=0;i<2;i++){
              digitalWrite(buzzer,HIGH);
              delay(250);
              digitalWrite(buzzer,LOW);
              delay(250);
            }
            delay(ACCESS_DELAY);
            digitalWrite(relay,LOW);
            break;
    case 2: 
            digitalWrite(buzzer,HIGH);
            delay(1000);
            digitalWrite(buzzer,LOW);
            delay(DENIED_DELAY);
  }
}
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  Serial.println();
  pinMode(LED_G, OUTPUT);digitalWrite(LED_G, LOW);
  pinMode(relay, OUTPUT);digitalWrite(relay, LOW);
  pinMode(buzzer, OUTPUT);digitalWrite(buzzer, LOW);
  WiFi.begin(wifiName, password);
 while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_G, HIGH);
    delay(500);
    Serial.println("Connecting to WiFi...");
    digitalWrite(LED_G, LOW);
    delay(500);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected to WiFi and Firebase"); 
      digitalWrite(LED_G, HIGH);
    }
}
void loop() 
{
  if(Serial.available()){
    char info = Serial.read();
    if(info == '!'){
      Serial.println("Someone opened door through bluetooth");
      access(1);
    }
    else{
      Serial.println("Wrong password");
      access(2);
    }
  }
  String state = Firebase.getString("lock");
  if(state == "open"){
    Serial.println("Someone opened door");
    access(1);
    Firebase.setString("lock","close");
  }
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi disconnected");
    flag=true;
    digitalWrite(LED_G, HIGH);
    delay(500);
    digitalWrite(LED_G, LOW);
    delay(500);
  }
  if(WiFi.status() == WL_CONNECTED&&flag){
    Serial.println("Wifi connected");
    flag=false;
    digitalWrite(LED_G, HIGH);
  }
}
