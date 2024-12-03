#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid = "Votre_SSID";
const char* password = "Votre_MotDePasse";
const char* serverURL = "http://votre-serveur.com/api"; // Remplacez par l'URL de votre serveur

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  
  // Initialisation Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connexion au Wi-Fi...");
  }
  Serial.println("Connecté au Wi-Fi");

  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    while (1);
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float pression = bme.readPressure() / 100.0F;
  float humidite = bme.readHumidity();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = String("{\"temperature\":") + temperature +
                      ",\"pression\":" + pression +
                      ",\"humidite\":" + humidite + "}";

    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      Serial.println("Données envoyées avec succès !");
    } else {
      Serial.println("Erreur d'envoi des données.");
    }
    http.end();
  }

  delay(5000); // Envoie toutes les 5 secondes
}
