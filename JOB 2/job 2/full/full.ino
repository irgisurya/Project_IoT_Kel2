#include <DHT11.h>
#define mq2Pin 33 // Pin analog untuk sensor MQ-2


// Create an instance of the DHT11 class and set the digital I/O pin.
DHT11 dht11(32);
int SensorPin = 34;// deklarasi pin analog yg dipakai
int soilMoistureValue; // menyimpan nilai analog dari sensor ke esp32
int soilmoisturepercent; // 

void setup()
{
    // Initialize serial communication at 115200 baud.
    Serial.begin(115200);
}

void loop()
{
  {
    int sensorValue = analogRead(mq2Pin); // Membaca nilai sensor analog

  float voltage = sensorValue * (5.0 / 1023.0); // Mengonversi nilai sensor menjadi tegangan (5V adalah tegangan referensi Arduino)
  
  // Menghitung konsentrasi gas menggunakan rumus yang sesuai dengan sensor MQ-2
  float gasResistance = ((5.0 - voltage) / voltage) * 10.0; // Menggunakan faktor 10.0 untuk mengkoreksi nilai
  
  // Menampilkan hasil ke Serial Monitor
  Serial.print(" Sensor Value: ");
  Serial.println(sensorValue);

    Serial.print(" Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" KΩ");
delay(500);}
{
      
  //Sensorsoil
        soilMoistureValue = analogRead(SensorPin);
        Serial.print(" Nilai analog = ");
          Serial.print(soilMoistureValue);
           soilmoisturepercent = map(soilMoistureValue, 4095, 0, 0, 100);

        Serial.print(" Presentase kelembaban tanah= ");
       Serial.print(soilmoisturepercent);
      Serial.println("% ");
delay(500);}
{

    // Read the humidity from the sensor.
    float humidity = dht11.readHumidity();

    // Read the temperature from the sensor.
    float temperature = dht11.readTemperature();

    // If the temperature and humidity readings were successful, print them to the serial monitor.
    if (temperature != -1 && humidity != -1)
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" C");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
    else
    {
        // If the temperature or humidity reading failed, print an error message.
        Serial.println("Error reading data");
    }

    // Wait for 2 seconds before the next reading.
    delay(2000);
}
}
