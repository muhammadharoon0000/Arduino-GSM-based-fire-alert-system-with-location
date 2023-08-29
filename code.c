#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(9, 10);
SoftwareSerial sim800l(7, 8); 
TinyGPSPlus gps;

const int smokeSensorPin= 2;
const int fireSensorPin = 3;
const int sprinklerPin = 4;

void setup() {
  pinMode(smokePin, INPUT);
  pinMode(fireSensorPin, INPUT);
  pinMode(sprinklerPin, OUTPUT);
  digitalWrite(sprinklerPin, LOW);
  
  Serial.begin(9600);
  simModulePins.begin(9600);
  delay(1000);
}

void loop() {

  int smokeValue = digitalRead(smokePin);
  int fireValue = digitalRead(fireSensorPin);

  if (smokeValue == HIGH) {
    sendSMS("Alert! Smoke detected location is ", getCurrentLocation());
  }

  if (fireValue == HIGH) {
    sendSMS("Alert! Fire detected activating sprinkler location is ",  getCurrentLocation());
    digitalWrite(sprinklerPin, HIGH);
    delay(60000);
    digitalWrite(sprinklerPin, LOW);
  }
}

void sendSMS(String message, String location) {
  simModulePins.println("AT+CMGF=1");
  delay(100);
  simModulePins.println("AT+CMGS=\"+923009329075\"");
  delay(100);
  simModulePins.println(message + " Location is - " + location);
  delay(100);
  simModulePins.write(0x1A);  // End of message
  delay(1000);
}

String getCurrentLocation() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
  
  if (gps.location.isValid()) {
    return String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6);
  } else {
    return "Location not available";
  }
}