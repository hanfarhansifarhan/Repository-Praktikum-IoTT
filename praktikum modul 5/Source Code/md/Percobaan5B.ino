#include <ESP8266WiFi.h>  // Menghubungkan ESP8266 ke Wi-Fi
#include <PubSubClient.h> // Komunikasi MQTT
#include <ArduinoJson.h>  // Membuat payload JSON
#include <math.h>        // Fungsi sinus untuk suhu simulasi

// Modul 5 - Percobaan 5B - Kelompok 4
// Nama: Farhan Nur Sahid | NIM: H1H024057
// Board: ESP8266 NodeMCU 1.0 (ESP-12E Module) | Serial: 115200

// Hotspot yang dipakai saat praktikum Kelompok 4
const char* ssid = "nneptuneey";
const char* password = "rereaaa09";

// MQTT broker publik; topic harus sama persis dengan program Python
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topicData = "unsoed/tk245004/kelompok4/data";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long jumlahBaca = 0;

float bacaSuhuMock() {
  jumlahBaca++;
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);
  float noise = random(-5, 6) * 0.1;
  float suhu = suhuDasar + noise;
  if (jumlahBaca % 25 == 0) suhu += 8.0;
  return suhu;
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi berhasil terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266-Mock-Kelompok4-" + String(ESP.getChipId(), HEX);
    Serial.println("Mencoba koneksi MQTT...");

    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT berhasil terhubung!");
    } else {
      Serial.print("MQTT gagal, kode: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  randomSeed(micros());
  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  reconnectMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) setupWiFi();
  if (!client.connected()) reconnectMQTT();
  client.loop();

  float suhu = bacaSuhuMock();

  // Membentuk JSON: {"suhu":28.48}
  StaticJsonDocument<128> doc;
  doc["suhu"] = suhu;
  char payload[128];
  serializeJson(doc, payload, sizeof(payload));

  bool berhasil = client.publish(topicData, payload);

  Serial.print("Pembacaan ke-");
  Serial.print(jumlahBaca);
  Serial.print(" | Suhu mock: ");
  Serial.print(suhu, 2);
  Serial.print(" C | Topic: ");
  Serial.print(topicData);
  Serial.print(" | Publish: ");
  Serial.println(berhasil ? "Berhasil" : "Gagal");

  delay(5000); // Interval publish sekitar 5 detik
}
