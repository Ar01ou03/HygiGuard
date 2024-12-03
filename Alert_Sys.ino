void setup() {
  // put your setup code here, to run once:

}#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BUZZER_PIN 25 // Pin pour le buzzer

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    while (1);
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float humidite = bme.readHumidity();

  Serial.print("Température : ");
  Serial.println(temperature);
  Serial.print("Humidité : ");
  Serial.println(humidite);

  // Vérifier les seuils
  if (temperature < 15 || temperature > 25 || humidite < 30 || humidite > 50) {
    digitalWrite(BUZZER_PIN, HIGH); // Activer le buzzer
    Serial.println("Alerte : Valeurs hors des seuils !");
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Désactiver le buzzer
  }

  delay(2000);
}


void loop() {
  // put your main code here, to run repeatedly:

}
