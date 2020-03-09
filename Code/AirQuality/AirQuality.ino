/**
 * Copyright 2020 © John Melody Melissa
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @Author : John Melody Melissa
 * @Copyright: John Melody Melissa  © Copyright 2020
 * @INPIREDBYGF: Sin Dee <3
 * @Project: AIR QUALITY;
 */

#define BLYNK_PRINT Serial
#define BLYNK_DEFAULT_PORT 80
#define DHTPIN A2
#define DHTTYPE DHT11
// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600
//#include <ESP8266_Lib.h>
//#include <SerialESP8266wifi.h>
#include "BlynkSimpleShieldEsp8266.h"
#include "DHT.h"
#include <LiquidCrystal.h>
#include "MQ7.h"
#include <SoftwareSerial.h>
//#include <SimpleTimer.h>

//-----------(Variables)-----------------//
float gas_value;
float gaspercent;
const int rs = 10, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int buzzer = 8;
const int siren = A3;
//---------------------------------------//
//----------------------------------------//
MQ7 mq7(A1,5.0);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial EspSerial(11, 12); // RX, TX
ESP8266 wifi(&EspSerial);
BlynkTimer timer;
//SimpleTimer timer;
//---------------------------------------//
//-----------------(Characters)----------------------------//
byte termometru[8] = //icon for termometer
{
   B00100,
   B01010,
   B01010,
   B01110,
   B01110,
   B11111,
   B11111,
   B01110
};
byte picatura[8] = //icon for water droplet
{
   B00100,
   B00100,
   B01010,
   B01010,
   B10001,
   B10001,
   B10001,
   B01110,
};
byte blynk[] = {
  B11110,
 B11011,
 B11011,
 B11111,
 B11111,
 B11011,
 B11011,
 B11110
};
byte noblynk[] = {
 B01110,
 B11011,
 B11011,
 B00100,
 B00100,
 B01010,
 B11011,
 B01110
};
byte wificon[] = {
 B00100,
 B01010,
 B10101,
 B01010,
 B10101,
 B01010,
 B00000,
 B00100
};
//--------------------------------------------------------//
//-----------------(Blynk configuration variables)----------------------------//
char auth[] = "your Blynk token"; //Authentication code sent to your e-mail.
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your ssid";           // your network SSID (name)
char pass[] = "your ssid password";        // your network password (change it)
char server[] = "blynk-cloud.com";
int port = 80;
//---------------------------------------------------------------------------//
//-----------------(Functions)----------------------------//
void sendSensor()
{
 byte h = dht.readHumidity();  //Read the humidity
 byte t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 // You can send any value at any time.
 // Please don't send more that 10 values per second.
 Blynk.virtualWrite(V5, h);
 Blynk.virtualWrite(V6, t);
//  Blynk.virtualWrite(V7, gas_value);
// Blynk.virtualWrite(V8, co_value);
}
void sendSensor2()
{
 byte co_value = analogRead(A1);
 byte gas_value = analogRead(A0);
 Blynk.virtualWrite(V7, gas_value);
 Blynk.virtualWrite(V8, co_value);
}
void notification() {
//byte t = dht.readTemperature();
byte co_value = analogRead(A1);
byte gas_value = analogRead(A0);
 if (gas_value >150 || co_value >220){
   Blynk.notify("Gas!");
 }
}
//--------------------------------------------------------//
void setup()
{
 lcd.createChar(1,termometru);
 lcd.createChar(2,picatura);
 lcd.createChar(3,blynk);
 lcd.createChar(4,noblynk);
 lcd.createChar(5,wificon);
  pinMode(siren, OUTPUT);
 digitalWrite(siren, LOW);
pinMode(buzzer, OUTPUT);
pinMode(A0,INPUT);
Serial.begin(9600);
lcd.begin(16, 2);
lcd.clear();
 lcd.setCursor(2, 0);
 lcd.print("Air Quality");
 lcd.setCursor(5, 1);
 lcd.print("System");
Serial.println(F("DHTxx test!"));
dht.begin();
tone(buzzer, 1000); // Send 3KHz sound signal...
 delay(1000);        // ...for 1 sec
 noTone(buzzer);     // Stop sound...
delay(3000); //Wait before accessing Sensor
lcd.clear();
lcd.setCursor(2, 0);
lcd.print("Calibrating");
delay(13000);
lcd.clear();
lcd.setCursor(3, 0);
lcd.print("Connecting");
lcd.setCursor(3, 1);
lcd.print("to  Blynk!");
//blynk
EspSerial.begin(ESP8266_BAUD);
delay(10);
//
//Blynk.begin(auth, wifi, ssid, pass);
 //wifi.setStationIp("192.168.1.100","192.168.1.1","255.255.255.0");
Blynk.config(wifi, auth, server, port);
if (Blynk.connectWiFi(ssid, pass)) {
       lcd.setCursor(15,0);
       lcd.write(5);
  Blynk.connect();
}
timer.setInterval(10000L, sendSensor);
timer.setInterval(3000L, sendSensor2);
timer.setInterval(12500L, notification);
lcd.clear();
}
void loop()
{
timer.run();
gas_value =analogRead(A0);
delay (300);
gaspercent = (gas_value/1023)*100;
Serial.print(gaspercent);
Serial.println(" %");
Serial.println(mq7.getPPM());
int h = dht.readHumidity();
int t = dht.readTemperature();
Serial.print(F("Humidity: "));
 Serial.print(h);
 Serial.print(F("%  Temperature: "));
 Serial.print(t);
 Serial.println(F("°C "));
       if (Blynk.connected()){
          lcd.setCursor(15, 0);
          lcd.write(3);
          Blynk.run();
         }else{
         lcd.setCursor(15, 0);
          lcd.write(4);
          Blynk.connect();
       }
//Serial.println(Blynk.connected());
lcd.clear();
if (Blynk.connected()){
          lcd.setCursor(15, 0);
          lcd.write(3);
          lcd.setCursor(14, 0);
          lcd.write(5);
         }else{
         lcd.setCursor(15, 0);
          lcd.write(4);
         }
  //Temperature LCD
 lcd.setCursor(0, 0);
 //lcd.print("Tem:");
 lcd.write(1);
 lcd.setCursor(1, 0);
 lcd.print(t);
 lcd.setCursor(3, 0);
 lcd.print((char)223);
 lcd.setCursor(4, 0);
  lcd.print("C");
  //CO level
   lcd.setCursor(6, 0);
  lcd.print("CO:");
  lcd.setCursor(9, 0);
  lcd.print(mq7.getPPM());
  //Humiduty LCD
 lcd.setCursor(0, 1);
 //lcd.print("Hum:");
 lcd.write(2);
 lcd.setCursor(1, 1);
 lcd.print(h);
 lcd.setCursor(3, 1);
 lcd.print(" %");
 //NG level
 lcd.setCursor(6, 1);
 lcd.print("NG:");
 lcd.setCursor(9, 1);
 lcd.print(gas_value);
 if (gas_value > 130 || mq7.getPPM() > 15){
           digitalWrite(siren, HIGH);
           lcd.clear();
           lcd.setCursor (3, 0);
           lcd.print("Alarm High");
           lcd.setCursor (6, 1);
           lcd.print("Gas!");
           tone(buzzer, 1000); // Send 1KHz sound signal...
           delay(500);        // ...for 1 sec
           noTone(buzzer);     // Stop sound...
}else{
 digitalWrite(siren, LOW);
}delay (2500);
}