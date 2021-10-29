#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN 4 
#define DHTTYPE DHT11 
#include <LiquidCrystal_I2C.h>
const int ldrPin =0;
const int rainPin =A0;
DHT dht(DHTPIN, DHTTYPE);
// set the LCD address to 0x27 for a 16 chars and 2 line display
//connect lcd sda to D2 amd scl to D3
LiquidCrystal_I2C lcd(0x27,16,2); 

char auth[]= "_MQP3NfLCLkzOfUlN9jst5ioSwwycw31"; // Your Auth token
char ssid[]= "DEUS 03";
char pass[]= "88888888";
BlynkTimer timer;

void sendSensor()
{
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
    delay(2000);
  //code for ldr
int ldrStatus =digitalRead(ldrPin);
Serial.print("LI= ");
Serial.println(ldrStatus);
lcd.setCursor(7,0);
lcd.print("L=");
if(ldrStatus=1)
{Serial.println("lit");
lcd.setCursor(9,0);
lcd.print("light");
}
else{
Serial.println("dark");
lcd.setCursor(10,0);
lcd.print("dark");
}
//code for rainfall
int sensorValue = analogRead(rainPin);
sensorValue=map(sensorValue, 0, 1020, 100, 0);
Serial.println(sensorValue);
  lcd.setCursor(0,0);
  lcd.print("R=   %");
lcd.setCursor(2,0);
lcd.print(sensorValue);
if (sensorValue<10){
Serial.println("no rain");
  }
else{
Serial.println("raining");
  }
  //temp humidity code
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  int hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  int hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  lcd.setCursor(0,1);
  lcd.print("H=  %");
  lcd.setCursor(2,1);
  lcd.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  lcd.setCursor(6,1);
  lcd.print("T=  C ");  
  lcd.setCursor(8,1);
  lcd.print(t);  
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  /*lcd.setCursor(12,1);
  lcd.print("HI=  C ");
  lcd.setCursor(15,1);
  lcd.print(hic);*/
  Serial.print(hif);
  Serial.println(F("°F"));

  delay(1000);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, sensorValue);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Wire.begin(2,0);
    Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(ldrPin, INPUT);
lcd.init(); // initialize the lcd
 // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  // Sending authentication token to blynk. Getting access to your WiFi with SSID and password
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
