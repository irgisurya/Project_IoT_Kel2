#define mq2  32 // Pin analog untuk sensor MQ-2
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(mq2, INPUT);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  // dht.begin();
  lcd.print("m Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Reading...");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(mq2); // Membaca nilai sensor analog

  float voltage = sensorValue * (5.0 / 1023.0); // Mengonversi nilai sensor menjadi tegangan (5V adalah tegangan referensi Arduino)
  
  // Menghitung konsentrasi gas menggunakan rumus yang sesuai dengan sensor MQ-2
  float gasResistance = ((5.0 - voltage) / voltage) * 10.0; // Menggunakan faktor 10.0 untuk mengkoreksi nilai
  
  // Menampilkan hasil ke Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

    lcd.setCursor(0, 0);
  lcd.print(" Value: ");
  lcd.print(sensorValue);
  
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(voltage);
  lcd.print("V");

  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" KΩ");
  
  delay(2000);
}
