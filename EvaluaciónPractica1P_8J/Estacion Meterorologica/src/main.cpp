#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "POCO F8 Pro";
const char* password = "12345679";

WiFiClient client;

unsigned long channelID = 3292117;
const char* writeAPIKey = "OKX1XVY3J5DLUZHE";

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("WiFi conectado");

  ThingSpeak.begin(client);

  dht.begin();
}

void loop() {

  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if(isnan(temperatura) || isnan(humedad)){
    Serial.println("Error leyendo sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C  Humedad: ");
  Serial.println(humedad);

  // Enviar datos a ThingSpeak
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, humedad);

  int x = ThingSpeak.writeFields(channelID, writeAPIKey);

  if(x == 200){
    Serial.println("Datos enviados correctamente");
  } else {
    Serial.println("Error enviando datos");
  }

  delay(20000); // mínimo 15 seg
}