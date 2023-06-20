#include <LiquidCrystal_I2C.h>

int SensorPin = 32;// deklarasi pin analog yg dipakai
int soilMoistureValue; // menyimpan nilai analog dari sensor ke esp32
int soilmoisturepercent; // nilai yg diperoleh dalam bentuk persen setelah dimaping

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200); // Baudrate komunikasi dengan serial monitor
  lcd.init();                      // initialize the lcd 
  lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Reading...");
  delay(2000);
  lcd.clear();
}

void loop() {
      soilMoistureValue = analogRead(SensorPin);
        Serial.print("Nilai analog = ");
          Serial.print(soilMoistureValue);
           soilmoisturepercent = map(soilMoistureValue, 4095, 0, 0, 100);

        Serial.print("Presentase kelembaban tanah= ");
       Serial.print(soilmoisturepercent);
      Serial.println("% ");
    lcd.setCursor(0, 0);
    lcd.print("Kelembaman: ");
  lcd.print(soilmoisturepercent);
  lcd.print("%");

   lcd.setCursor(0, 1);
  lcd.print("Nilai: ");
  lcd.print(soilMoistureValue);
  lcd.print("");


delay(500);
}