#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define FIREBASE_HOST " "   // Enter database HOST without "https:// "  and "/" at the end of URL             
#define FIREBASE_AUTH " "   // Real-time database secret key here
#define WIFI_SSID " "       // WIFI SSID here                                   
#define WIFI_PASSWORD ""    // WIFI password here

FirebaseData firebaseData;

const int trigPin = D5;  
const int echoPin = D6;
int trigPin2 = D1;
int echoPin2 = D2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);// Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin2 as an Output
  pinMode(echoPin2, INPUT);// Sets the echoPin2 as an Input
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() {
  // put your main code here, to run repeatedly:
sensorUpdate();
delayMicroseconds(3000);
}

void sensorUpdate(){
  
  long d,r,d2,r2;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  d = pulseIn(echoPin, HIGH) ;
  r = (d/2) / 29.1;

  delayMicroseconds(2000);
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  d2 = pulseIn(echoPin2, HIGH) ;
  r2 = (d2/2) / 29.1;
  
  Serial.print(r);   
  Serial.println(" cm");
  Serial.print(r2);   
  Serial.println(" cm");
  if (isnan(r)){
    Serial.print(F("Failed to read from Ultrasonic sensor Sensor"));
    return;
  }
   if (isnan(r2)){
    Serial.print(F("Failed to read from Ultrasonic sensor Sensor"));
    return;
  }

  if (Firebase.setFloat(firebaseData, "/distance/cm", r)){
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

   if (Firebase.setFloat(firebaseData, "/distance/cm2", r2)){
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  
}
