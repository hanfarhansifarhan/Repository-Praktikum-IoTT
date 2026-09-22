# Praktikum IoT Modul 3 — Protokol Komunikasi

Praktikum Modul 3 membahas proses komunikasi data pada sistem Internet of Things (IoT) menggunakan protokol HTTP dan MQTT. Data yang dikirim disusun menggunakan format JSON agar lebih terstruktur dan mudah diproses. Pada praktikum ini digunakan ESP8266 sebagai perangkat utama yang terhubung ke jaringan WiFi.

## Tujuan Praktikum

- Memahami konsep dasar protokol komunikasi pada sistem IoT.
- Memahami perbedaan cara kerja HTTP dan MQTT.
- Mengimplementasikan pengiriman data menggunakan metode HTTP POST.
- Mengimplementasikan komunikasi MQTT menggunakan konsep publish-subscribe.
- Memahami penggunaan JSON sebagai format pertukaran data.
- Mengamati hasil komunikasi melalui Serial Monitor dan MQTT Explorer.

---

## Perangkat dan Library

### Perangkat

- ESP8266
- Kabel Micro USB
- Breadboard
- Laptop/PC
- Jaringan WiFi
- Arduino IDE
- MQTT Explorer

### Library

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
```

Library `ESP8266WiFi.h` digunakan untuk menghubungkan ESP8266 dengan jaringan WiFi. `ESP8266HTTPClient.h` dan `WiFiClientSecure.h` digunakan dalam komunikasi HTTP, sedangkan `PubSubClient.h` digunakan untuk MQTT. Pengolahan data dalam format JSON dilakukan menggunakan `ArduinoJson.h`.

---

# Percobaan 3A — Komunikasi Data Menggunakan HTTP

## Deskripsi

Pada percobaan 3A, ESP8266 digunakan sebagai client untuk mengirimkan data menuju server melalui protokol HTTP menggunakan metode POST. Data yang dikirim berupa suhu dan kelembaban yang disusun dalam format JSON.

Endpoint yang digunakan pada percobaan adalah:

```text
https://httpbin.org/post
```

Hasil komunikasi diamati melalui Serial Monitor dengan melihat data yang dikirim, HTTP Response Code, dan Response Body dari server.

## Alur Komunikasi

```text
ESP8266
   ↓
Jaringan WiFi
   ↓
Membuat Data JSON
   ↓
HTTP POST
   ↓
httpbin.org/post
   ↓
Response Server
   ↓
Serial Monitor
```

## Data Sebelum Modifikasi

Data JSON yang dikirim:

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

Hasil yang diperoleh melalui Serial Monitor:

```text
Mengirim data: {"suhu":28.5,"kelembaban":65}
Kode Response HTTP: 200
Isi Response:
{
  "json": {
    "kelembaban": 65,
    "suhu": 28.5
  }
}
```

**Status pengiriman: Berhasil**

HTTP Response Code `200` menunjukkan bahwa request berhasil diterima dan diproses oleh server. Pada Response Body juga terlihat bahwa nilai suhu dan kelembaban yang dikirim oleh ESP8266 berhasil diterima oleh server.

## Fungsi Penting HTTP

| Fungsi | Kegunaan |
|---|---|
| `WiFi.begin()` | Memulai koneksi ESP8266 ke jaringan WiFi |
| `WiFi.status()` | Memeriksa status koneksi WiFi |
| `http.begin()` | Menentukan alamat server atau endpoint |
| `http.addHeader()` | Menentukan tipe data yang dikirim |
| `serializeJson()` | Mengubah objek JSON menjadi teks |
| `http.POST()` | Mengirim data menggunakan metode POST |
| `http.getString()` | Membaca Response Body dari server |
| `http.end()` | Mengakhiri komunikasi HTTP |

### Fungsi Header

```cpp
http.addHeader("Content-Type", "application/json");
```

Perintah tersebut digunakan untuk memberitahu server bahwa isi request yang dikirim memiliki format JSON. Dengan header tersebut, server dapat membaca dan memproses body request sebagai objek JSON, bukan sebagai teks biasa.

---

# Modifikasi Percobaan 3A — Menambahkan Data Waktu

Program kemudian dimodifikasi dengan menambahkan field baru bernama `waktu`. Nilainya diperoleh menggunakan fungsi `millis()` yang menunjukkan lama waktu perangkat telah berjalan dalam satuan milidetik.

## Sebelum Modifikasi

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65;
```

Data:

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

## Setelah Modifikasi

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65;
doc["waktu"] = millis();
```

Contoh hasil aktual yang diperoleh:

```text
Mengirim data: {"suhu":28.5,"kelembaban":65,"waktu":122878}
Kode Response HTTP: 200
Isi Response:
{
  "json": {
    "kelembaban": 65,
    "suhu": 28.5,
    "waktu": 122878
  }
}
```

**Status pengiriman: Berhasil**

Setelah program dimodifikasi, server tetap memberikan Response Code `200`. Hal tersebut menunjukkan bahwa penambahan field `waktu` tidak mengganggu proses komunikasi dan data tambahan berhasil diterima oleh server.

Nilai `122878` menunjukkan waktu berjalan ESP8266 dalam satuan milidetik sejak perangkat mulai menjalankan program.

---

# Percobaan 3B — Komunikasi Data Menggunakan MQTT

## Deskripsi

Pada percobaan 3B digunakan protokol MQTT untuk mengirimkan data menggunakan mekanisme publish-subscribe. ESP8266 bertindak sebagai publisher yang mengirimkan data dalam format JSON menuju broker MQTT.

Broker yang digunakan:

```text
broker.hivemq.com
```

Port:

```text
1883
```

Topic yang digunakan:

```text
farhan/esp8266/latihan
```

Data yang telah dipublish kemudian diamati melalui MQTT Explorer yang bertindak sebagai subscriber.

## Alur Komunikasi

```text
ESP8266 (Publisher)
        ↓
      WiFi
        ↓
Broker MQTT HiveMQ
        ↓
      Topic
        ↓
MQTT Explorer (Subscriber)
```

## Data Sebelum Modifikasi

Data JSON yang dipublish:

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

Contoh data yang diterima subscriber:

```text
{"suhu":28.5,"kelembaban":65}
```

Data yang muncul pada MQTT Explorer menunjukkan bahwa proses publish dari ESP8266 menuju broker berhasil dan subscriber dapat menerima pesan dari topic yang digunakan.

## Fungsi Penting MQTT

| Fungsi | Kegunaan |
|---|---|
| `WiFi.begin()` | Menghubungkan ESP8266 ke WiFi |
| `client.setServer()` | Menentukan alamat broker dan port |
| `client.connect()` | Menghubungkan ESP8266 dengan broker |
| `client.connected()` | Memeriksa status koneksi ke broker |
| `client.loop()` | Menjaga komunikasi MQTT tetap berjalan |
| `serializeJson()` | Mengubah objek JSON menjadi teks |
| `client.publish()` | Mengirim data ke topic MQTT |

---

# Modifikasi Percobaan 3B — Menambahkan Waktu `millis()`

Pada percobaan MQTT dilakukan modifikasi pada data JSON dengan menambahkan field `waktu`. Tujuannya agar data yang dikirim tidak hanya berisi suhu dan kelembaban, tetapi juga informasi mengenai waktu berjalan perangkat.

## Sebelum Modifikasi

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65;
```

Output:

```json
{
  "suhu": 28.5,
  "kelembaban": 65
}
```

## Setelah Modifikasi

```cpp
doc["suhu"] = 28.5;
doc["kelembaban"] = 65;
doc["waktu"] = millis();
```

Output menjadi:

```json
{
  "suhu": 28.5,
  "kelembaban": 65,
  "waktu": 122878
}
```

Field `waktu` diperoleh dari fungsi:

```cpp
millis()
```

Fungsi tersebut menghasilkan nilai waktu dalam milidetik sejak ESP8266 mulai menjalankan program.

---

# Source Code MQTT Setelah Modifikasi

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// =====================
// KONFIGURASI WIFI
// =====================

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// =====================
// KONFIGURASI MQTT
// =====================

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "farhan/esp8266/latihan";

WiFiClient espClient;
PubSubClient client(espClient);

// =====================
// KONEKSI WIFI
// =====================

void hubungkanWiFi() {

  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP ESP8266: ");
  Serial.println(WiFi.localIP());
}

// =====================
// KONEKSI MQTT
// =====================

void hubungkanMQTT() {

  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    String clientId = "FarhanESP8266-";
    clientId += String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

    } else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" mencoba kembali dalam 2 detik");

      delay(2000);
    }
  }
}

// =====================
// SETUP
// =====================

void setup() {

  Serial.begin(115200);

  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);
}

// =====================
// LOOP
// =====================

void loop() {

  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();

  JsonDocument doc;

  doc["suhu"] = 28.5;
  doc["kelembaban"] = 65;
  doc["waktu"] = millis();

  char buffer[160];

  serializeJson(doc, buffer);

  bool berhasil = client.publish(mqttTopic, buffer);

  if (berhasil) {

    Serial.print("Data terkirim ke topic ");
    Serial.print(mqttTopic);
    Serial.print(": ");
    Serial.println(buffer);

  } else {

    Serial.println("Gagal mengirim data!");

  }

  delay(5000);
}
```

---

# Perbandingan HTTP dan MQTT

| Aspek | HTTP | MQTT |
|---|---|---|
| Pola komunikasi | Request-response | Publish-subscribe |
| Perantara | Server | Broker |
| Metode pengiriman | POST / GET | Publish / Subscribe |
| Format data praktikum | JSON | JSON |
| Overhead komunikasi | Relatif lebih besar | Relatif lebih kecil |
| Koneksi | Tidak harus selalu aktif | Dapat menggunakan persistent connection |
| Pengamatan hasil | Serial Monitor | MQTT Explorer |
| Cocok digunakan | Pengiriman periodik | Pengiriman kontinu / real-time |

HTTP bekerja dengan cara client mengirimkan request kepada server kemudian server memberikan response. Sementara itu, MQTT menggunakan broker sebagai penghubung antara publisher dan subscriber.

---

# Kesimpulan

1. ESP8266 berhasil digunakan untuk melakukan komunikasi data menggunakan protokol HTTP dan MQTT dengan format JSON.

2. Pada percobaan HTTP POST, data suhu dan kelembaban berhasil dikirim menuju `httpbin.org` dan server memberikan HTTP Response Code `200`, yang menunjukkan bahwa proses pengiriman berhasil.

3. MQTT dapat digunakan untuk mengirimkan data melalui broker menggunakan pola publish-subscribe. Data yang dipublish pada topic dapat diterima dan diamati melalui MQTT Explorer.

4. Penambahan field `waktu` menggunakan fungsi `millis()` berhasil menambahkan informasi waktu berjalan perangkat ke dalam data JSON tanpa mengganggu proses pengiriman.

5. HTTP lebih sesuai untuk komunikasi dengan pola request-response, sedangkan MQTT lebih sesuai untuk pertukaran data secara terus-menerus karena mekanisme komunikasinya lebih ringan.

---

# Dokumentasi Praktikum

## Percobaan 3A — HTTP POST

### Rangkaian

Tambahkan foto rangkaian ESP8266 pada bagian ini.

```markdown
![Rangkaian HTTP](NamaFileFoto.jpg)
```

### Output Sebelum Modifikasi

Tambahkan screenshot Serial Monitor sebelum modifikasi.

```markdown
![HTTP Sebelum Modifikasi](NamaFileFoto.jpg)
```

### Output Setelah Modifikasi

Tambahkan screenshot Serial Monitor setelah penambahan field `waktu`.

```markdown
![HTTP Setelah Modifikasi](NamaFileFoto.jpg)
```

---

## Percobaan 3B — MQTT

### Rangkaian

Tambahkan foto perangkat pada saat percobaan MQTT.

```markdown
![Rangkaian MQTT](NamaFileFoto.jpg)
```

### MQTT Explorer Sebelum Modifikasi

```markdown
![MQTT Sebelum Modifikasi](NamaFileFoto.jpg)
```

### MQTT Explorer Setelah Modifikasi

```markdown
![MQTT Setelah Modifikasi](NamaFileFoto.jpg)
```

---

# Identitas Praktikan

**Nama:** Farhan Nur Sahid  
**NIM:** H1H024057  
**Program Studi:** Teknik Komputer  
**Modul:** Modul 3 — Protokol Komunikasi IoT
