#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Connect to LCD via I2C, default address 0x27

// defines pins numbers
const int trigPin = 11; //pin data output ultra atas ke 11
const int echoPin = 12; //pin data input ultra atas ke 12

const int trigPin2 = 6; //pin data output ultra bawah ke 6
const int echoPin2 = 5; //pin data input ultra bawah ke 5

const int buzzer = 7; //pin data ke 7
const int buzzer2 = 9; //pin data ke 9
const int ledPin = 13; //pin data ke 13
const int pinSensorSuhu = A0; //pin data ke A0

SoftwareSerial serial_gps(3, 2); //TX (TX GPS - RX 3 ARDUINO) DAN RX(RX GPS - TX 2 ARDUINO)

float temperature; //float: nilai sampai koma c: 29,44
int temperature1; //int: bil.bulat c: 1-10

TinyGPSPlus gps;
// Define variables latitude longitude
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
    
    //TEMPERATUR
   temperature1 = analogRead(pinSensorSuhu); //baca nilai analog pin A0 //konversi analog
   temperature = temperature1 / 2.0479; 
   //1'C = 10mV (sesuai datasheet)<br>// 5v /1023 = 4,883 mV (5v = tegangan refrensi, 1023 = resolusi 10 bit)
  // setiap kenaikan 1'C --> 10 / 4.883 = 2.0479

// Define variables durasi jarak
  long duration, distance, duration2, distance2;
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(1);
  
  digitalWrite(trigPin, HIGH);
  digitalWrite(ledPin, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;

  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2)/29.1;
  
  //JARAK
  if (distance <= 70) { //mengecek jarak minimal, jika jarak kurang 70 maka sensor memberikan respon ke buzzer 
    tone(buzzer,50,100);

  }
  
  else if (distance2 <= 20) { //mengecek jarak minimal, jika jarak kurang 20 maka sensor memberikan respon ke buzzer 
    tone(buzzer2,200,100);

  }
  else if (temperature > 80){ //mengecek sensor suhu, jika suhu diatas 80 derajat maka buzzer akan memberi
    tone(buzzer, HIGH);
  }
  else if(gps.location.isUpdated()) {
  }
  else { //jika tidak, output akan mati 
    digitalWrite(buzzer, LOW);
    digitalWrite(buzzer2, LOW);
    digitalWrite(ledPin, LOW);
  }
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    Serial.print("Google Maps : ");
    String link = "www.google.com/maps/place/" + String(latitude) + "," + String(longitude) ;
    Serial.println(link);  //Serial.println(link);
    Serial.print(distance); //serial print jarak sensor atas
    Serial.println(" cm Jarak Atas");
    Serial.print(distance2); //serial print jarak sensor bawah
    Serial.println(" cm Jarak Bawah");
    Serial.print(temperature); //serial print jarak sensor suhu
    Serial.println(" derajat Celcius");
 
    delay(500);
}
