#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// defines pins numbers
const int trigPin = 11;
const int echoPin = 12;

const int trigPin2 = 6;
const int echoPin2 = 5;

const int buzzer = 7;
const int buzzer2 = 9;
const int ledPin = 13;
const int ledPin2 = 8;

SoftwareSerial serial_gps(3, 2); 

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
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  lcd.begin();
   lcd.backlight();
   lcd.setCursor(0, 0);
   lcd.print("PERMISI");
   lcd.setCursor(0, 1);
   lcd.print("SAYA TUNANETRA");
}

void loop() {
  
  
  //GPS
  while(serial_gps.available()){
    gps.encode(serial_gps.read());
  }
    if(gps.location.isUpdated()) 
    //TEMPERATUR
   temperature = temperature1 / 2.0479; 
   //1'C = 10mV (sesuai datasheet)<br>// 5v /1023 = 4,883 mV (5v = tegangan refrensi, 1023 = resolusi 10 bit)
  // setiap kenaikan 1'C --> 10 / 4.883 = 2.0479

  long duration, distance, duration2, distance2;
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(1);
  
  digitalWrite(trigPin, HIGH);
  digitalWrite(ledPin, HIGH);
  digitalWrite(trigPin2, HIGH);
  digitalWrite(ledPin2, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;

  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2)/29.1;
  
  //JARAK
  if (distance <= 70) {
    tone(buzzer,50,100);

  }
  
  else if (distance2 <= 20) {
    tone(buzzer2,200,100);

  }
  else if (temperature > 80){
  }

  else {
    digitalWrite(buzzer, LOW);
    digitalWrite(buzzer2, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
  }
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    Serial.print("Google Maps : ");
    String link = "www.google.com/maps/place/" + String(latitude) + "," + String(longitude) ;
    Serial.println(link);
    Serial.print(distance);
    Serial.println(" cm Jarak Atas");
    Serial.print(distance2);
    Serial.println(" cm Jarak Bawah");
    Serial.print(temperature);
    Serial.println(" derajat Celcius");
 
    delay(500);
}
