#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// defines pins numbers
const int trigPin = 11;
const int echoPin = 12;
const int buzzer = 9;
const int ledPin = 13;
int value = 0;
const int pirBuzzerPin = 7; // Pin Piezo Buzzer is connected to
const int pirSensorPin = 8; // PIN PIR Sensor is connected to
int motionDetected = LOW; // Start MotionDetected as low (No motion detected)

SoftwareSerial serial_gps(3, 2); 
int pinSensorSuhu = A0;

float temperature = 00;
int temperature1 = 00;

TinyGPSPlus gps;
double latitude, longitude;


void setup() {
  Serial.begin (9600);
  serial_gps.begin(9600);
  Serial.println("GPS Mulai");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirSensorPin, INPUT); // declare the PIR sensor as input
  pinMode(pirBuzzerPin, OUTPUT); //declare buzzer as output
}

void loop() {
  //PIR
  value = digitalRead(pirSensorPin);
  if (value == HIGH) {
    digitalWrite(pirBuzzerPin, HIGH);

    if (motionDetected == LOW) {
      Serial.println("Motion Detected!");
      motionDetected = HIGH;
    }
  }else{
    digitalWrite(pirBuzzerPin, LOW);

    if(motionDetected == HIGH){
      Serial.println("Motion Ended!");
      motionDetected = LOW;
      }
    }
  
  //GPS
  while(serial_gps.available()){
    gps.encode(serial_gps.read());
  }
    if(gps.location.isUpdated()) 
    //TEMPERATUR
   temperature1 = analogRead(pinSensorSuhu);
   temperature = temperature1 / 2.0479; 
   //1'C = 10mV (sesuai datasheet)<br>// 5v /1023 = 4,883 mV (5v = tegangan refrensi, 1023 = resolusi 10 bit)
  // setiap kenaikan 1'C --> 10 / 4.883 = 2.0479

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  
  digitalWrite(trigPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  //JARAK
  if (distance <= 30) {
    tone(buzzer,300,200);

  }
  else if (temperature > 80){
  }

  else {
    digitalWrite(buzzer, LOW);
    digitalWrite(ledPin, LOW);

  }
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    Serial.print("Google Maps : ");
    String link = "www.google.com/maps/place/" + String(latitude) + "," + String(longitude) ;
    Serial.println(link);
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print(temperature);
    Serial.println(" derajat Celcius");
 
    delay(500);
}
