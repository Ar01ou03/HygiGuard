#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <SD.h>

// Configuration Wi-Fi
const char* ssid = "Votre_SSID";
const char* password = "Votre_MotDePasse";

// Configuration des broches SD
#define CS_PIN 5 // Broche CS pour la carte SD

// Objets
Adafruit_BME280 bme;
WiFiServer server(80);

// Fichier CSV
File dataFile;

void setup() {
  Serial.begin(115200);

  // Initialisation du capteur BME280
  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    while (1);
  }

  // Initialisation de la carte SD
  Serial.println("Initialisation de la carte SD...");
  if (!SD.begin(CS_PIN)) {
    Serial.println("Erreur : Carte SD non détectée !");
    while (1);
  }
  Serial.println("Carte SD initialisée avec succès.");

  // Création ou ouverture du fichier CSV
  dataFile = SD.open("/data.csv", FILE_APPEND);
  if (!dataFile) {
    Serial.println("Erreur : Impossible de créer ou d'ouvrir le fichier CSV !");
    while (1);
  }
  // Ajouter les en-têtes si le fichier est vide
  if (dataFile.size() == 0) {
    dataFile.println("Heure,Température (°C),Pression (hPa),Humidité (%)");
  }
  dataFile.close();

  // Connexion Wi-Fi
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnecté au Wi-Fi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Démarrage du serveur web
  server.begin();
}

void loop() {
  // Lecture des mesures
  float temperature = bme.readTemperature();
  float pression = bme.readPressure() / 100.0F; // Pression en hPa
  float humidite = bme.readHumidity();

  // Enregistrer les données dans le fichier CSV
  dataFile = SD.open("/data.csv", FILE_APPEND);
  if (dataFile) {
    String dataString = String(millis() / 1000) + "," +
                        String(temperature) + "," +
                        String(pression) + "," +
                        String(humidite);
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Données enregistrées : " + dataString);
  } else {
    Serial.println("Erreur : Impossible d'ouvrir le fichier CSV !");
  }

  // Gestion des requêtes du serveur web
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connecté !");
    String request = client.readStringUntil('\r');
    client.flush();

    // Générer une page HTML
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>Monitoring et Stockage</title></head><body>";
    html += "<h1>Surveillance des Conditions</h1>";
    html += "<p><b>Température :</b> " + String(temperature) + " °C</p>";
    html += "<p><b>Pression :</b> " + String(pression) + " hPa</p>";
    html += "<p><b>Humidité :</b> " + String(humidite) + " %</p>";
    html += "<p>Les données sont enregistrées dans le fichier <b>data.csv</b>.</p>";
    html += "</body></html>";

    // Envoyer la réponse HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println(html);
    client.println();

    client.stop();
    Serial.println("Client déconnecté.");
  }

  delay(10000); // Lecture et enregistrement toutes les 10 secondes
}
