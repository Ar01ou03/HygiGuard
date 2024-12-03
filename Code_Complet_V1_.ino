#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Configuration Wi-Fi
const char* ssid = "Votre_SSID";
const char* password = "Votre_MotDePasse";

// Configuration des seuils
const float TEMP_MIN = 15.0;
const float TEMP_MAX = 25.0;
const float HUM_MIN = 30.0;
const float HUM_MAX = 50.0;

// Configuration des broches
#define BUZZER_PIN 25 // Broche pour le buzzer ou une LED

// Objet BME280
Adafruit_BME280 bme;

// Serveur Web
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialisation du capteur BME280
  if (!bme.begin(0x76)) {
    Serial.println("Erreur : BME280 non détecté !");
    delay(1000);
    ESP.restart(); // Redémarrer si le capteur ne fonctionne pas
  }

  // Initialisation du Wi-Fi
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnecté au Wi-Fi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Initialisation du serveur web
  server.begin();

  // Configuration de la broche du buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Lecture des mesures
  float temperature = bme.readTemperature();
  float pression = bme.readPressure() / 100.0F; // Pression en hPa
  float humidite = bme.readHumidity();

  // Gestion des alertes
  if (temperature < TEMP_MIN || temperature > TEMP_MAX || 
      humidite < HUM_MIN || humidite > HUM_MAX) {
    digitalWrite(BUZZER_PIN, HIGH); // Activer le buzzer
    Serial.println("Alerte : Valeurs hors des seuils !");
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Désactiver le buzzer
  }

  // Gérer les requêtes du serveur web
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connecté !");
    String request = client.readStringUntil('\r');
    client.flush();

    // Générer la page HTML
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta http-equiv='refresh' content='5'>"; // Actualisation automatique
    html += "<title>Monitoring Salle d'Opérations</title></head><body>";
    html += "<h1>Surveillance des Conditions</h1>";
    html += "<p><b>Température :</b> " + String(temperature) + " °C</p>";
    html += "<p><b>Pression :</b> " + String(pression) + " hPa</p>";
    html += "<p><b>Humidité :</b> " + String(humidite) + " %</p>";
    html += "<p><b>État des Seuils :</b> " + 
            (digitalRead(BUZZER_PIN) ? "<span style='color:red;'>ALERTE</span>" : "<span style='color:green;'>NORMAL</span>") + "</p>";
    html += "<p><i>Page mise à jour automatiquement toutes les 5 secondes.</i></p>";
    html += "</body></html>";

    // Envoyer la réponse HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println(html);
    client.println();

    client.stop(); // Fermer la connexion
    Serial.println("Client déconnecté.");
  }

  delay(1000); // Rafraîchissement toutes les secondes
}
