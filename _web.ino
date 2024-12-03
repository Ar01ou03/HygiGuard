#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Définir les identifiants Wi-Fi
const char* ssid = "Votre_SSID";
const char* password = "Votre_MotDePasse";

// Objet BME280
Adafruit_BME280 bme;

// Adresse IP de l'ESP32
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialisation du capteur BME280
  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    while (1);
  }

  // Connexion au Wi-Fi
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnecté au Wi-Fi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Démarrer le serveur
  server.begin();
}

void loop() {
  // Attente de connexion d'un client
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Client connecté !");
  while (client.connected()) {
    if (client.available()) {
      client.read(); // Lecture des données entrantes

      // Lire les mesures
      float temperature = bme.readTemperature();
      float pression = bme.readPressure() / 100.0F;
      float humidite = bme.readHumidity();

      // Générer une page HTML
      String html = "<!DOCTYPE html><html><head>";
      html += "<meta http-equiv='refresh' content='5'>"; // Rafraîchit la page toutes les 5 secondes
      html += "<title>Salle d'Opérations - Monitoring</title></head><body>";
      html += "<h1>Surveillance des Conditions</h1>";
      html += "<p><b>Température :</b> " + String(temperature) + " °C</p>";
      html += "<p><b>Pression :</b> " + String(pression) + " hPa</p>";
      html += "<p><b>Humidité :</b> " + String(humidite) + " %</p>";
      html += "<p><i>Page mise à jour automatiquement toutes les 5 secondes.</i></p>";
      html += "</body></html>";

      // Envoyer la réponse HTTP
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();
      client.println(html);
      client.println();
      break;
    }
  }

  // Fermer la connexion avec le client
  client.stop();
  Serial.println("Client déconnecté.");
}
