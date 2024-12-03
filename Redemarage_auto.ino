#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);

  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    delay(1000);
    ESP.restart(); // Redémarrage de l'ESP32
  }
}

void loop() {
  if (!bme.begin(0x76)) {
    Serial.println("Erreur : Tentative de recalibrage...");
    delay(1000);
    ESP.restart(); // Redémarrage en cas d'échec
  }

  // Lecture des mesures
  float temperature = bme.readTemperature();
  float pression = bme.readPressure() / 100.0F;
  float humidite = bme.readHumidity();

  // Afficher les données
  Serial.print("Température : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Pression : ");
  Serial.print(pression);
  Serial.println(" hPa");

  Serial.print("Humidité : ");
  Serial.print(humidite);
  Serial.println(" %");

  delay(5000);
}
