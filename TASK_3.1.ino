#include <DHT.h>
#include <WiFiNINA.h>

char ssid[] = "Hakeem";
char pass[] = "Hussain@123";

char server[] = "api.thingspeak.com";
String apiKey = "PTAUIY0GYMNXQ4CC";

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to establish a connection to WiFi...");
    WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("Successfully connected to WiFi.");
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data from the DHT sensor!");
    delay(2000);
    return;
  }

  String data = "api_key=" + apiKey;
  data += "&field1=" + String(temperature);
  data += "&field2=" + String(humidity);

  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\r\n");
    client.print("Host: " + String(server) + "\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data successfully sent to ThingSpeak!");
  } else {
    Serial.println("Failed to establish a connection to ThingSpeak!");
  }

  delay(4000);
}
