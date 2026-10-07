# Praktikum Internet of Things

**Modul 4 – Komunikasi dan Pertukaran Data**

[![Status](https://img.shields.io/badge/Status-Complete-brightgreen)](https://github.com)
[![Protocol](https://img.shields.io/badge/Protocol-MQTT-blue)](https://mqtt.org/)
[![Board](https://img.shields.io/badge/Board-ESP8266-orange)](https://www.espressif.com/)
[![Contributor](https://img.shields.io/badge/Contributor-Farhan%20Nur%20Sahid-informational)](https://github.com)

---

## Identitas

| Informasi | Detail |
|-----------|--------|
| **Nama** | Farhan Nur Sahid |
| **NIM** | H1H024057 |
| **Mata Kuliah** | Praktikum Internet of Things (TK245002) |
| **Program Studi** | Teknik Komputer |
| **Universitas** | Universitas Jenderal Soedirman |
| **Tahun Ajaran** | 2026 / Semester 5 |
| **Modul** | Modul 4 – Komunikasi dan Pertukaran Data |

---

## Tentang Repository

Repository ini berisi dokumentasi dan source code **Modul 4 Praktikum Internet of Things** mengenai komunikasi dan pertukaran data menggunakan protokol **MQTT**.

Pada modul ini dipelajari mekanisme **publish dan subscribe**, penerimaan pesan menggunakan fungsi `callback()`, pengolahan data dalam format **JSON**, serta komunikasi dua arah antara perangkat IoT dengan MQTT broker.

Implementasi praktikum menggunakan **ESP8266 NodeMCU**, sensor **DHT11**, dan **LED** sebagai aktuator. MQTT Explorer digunakan untuk memantau data sekaligus mengirimkan perintah kepada ESP8266.

---

## Tujuan Praktikum

Tujuan dari Modul 4 adalah memahami komunikasi dan pertukaran data pada sistem IoT menggunakan MQTT, khususnya:

- Memahami mekanisme **publish dan subscribe** pada MQTT.
- Menggunakan ESP8266 sebagai MQTT client.
- Menerima pesan dari broker menggunakan `callback()`.
- Melakukan deserialisasi pesan **JSON**.
- Mengendalikan aktuator berdasarkan pesan MQTT.
- Mengirim data sensor melalui MQTT.
- Menerapkan komunikasi dua arah antara sensor dan aktuator.
- Memahami penggunaan `client.loop()` untuk menjaga komunikasi MQTT.
- Menggunakan `millis()` agar pengiriman data dapat dilakukan secara **non-blocking**.

---

## Konsep Komunikasi MQTT

MQTT menggunakan konsep **publish-subscribe**, sehingga perangkat tidak harus berkomunikasi secara langsung.

```text
                       MQTT Broker
                  broker.hivemq.com:1883
                         /       \
                        /         \
                 Publish           Subscribe
                    /                 \
                   /                   \
            ESP8266 NodeMCU        MQTT Explorer
             DHT11 + LED
```

Pada praktikum ini komunikasi dilakukan dua arah:

```text
DHT11
  |
  v
ESP8266
  |
  | Publish {"suhu": 24.5}
  v
MQTT Broker
  |
  v
MQTT Explorer


MQTT Explorer
  |
  | Publish {"perintah":"ON"}
  v
MQTT Broker
  |
  v
ESP8266
  |
  v
LED MENYALA
```

---

## Hardware yang Digunakan

| No. | Komponen | Fungsi |
|----:|-----------|--------|
| 1 | ESP8266 NodeMCU | Mikrokontroler dan MQTT client |
| 2 | DHT11 | Sensor untuk membaca suhu |
| 3 | LED | Aktuator yang dikendalikan melalui MQTT |
| 4 | Resistor 220 Ω | Pembatas arus LED |
| 5 | Breadboard | Media perakitan rangkaian |
| 6 | Kabel jumper | Menghubungkan komponen |
| 7 | Kabel USB | Menghubungkan ESP8266 ke komputer |

---

## Software dan Library

### Software

- **Arduino IDE**
- **MQTT Explorer**
- **Serial Monitor**
- MQTT Broker publik HiveMQ

### Library

Library yang digunakan:

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
```

Fungsi masing-masing library:

- `ESP8266WiFi.h` → menghubungkan ESP8266 dengan jaringan WiFi.
- `PubSubClient.h` → menangani komunikasi MQTT.
- `ArduinoJson.h` → melakukan serialisasi dan deserialisasi JSON.
- `DHT.h` → membaca data sensor DHT11.

---

## Konfigurasi MQTT

Broker MQTT yang digunakan:

```text
Broker : broker.hivemq.com
Port   : 1883
QoS    : 0
```

Topic yang digunakan pada praktikum:

### Topic Data Sensor

```text
unsoed/tk245004/farhan/data
```

Digunakan oleh ESP8266 untuk melakukan **publish data suhu**.

Contoh payload:

```json
{
  "suhu": 24.5
}
```

### Topic Perintah

```text
unsoed/tk245004/farhan/perintah
```

Digunakan untuk mengirim perintah dari MQTT Explorer menuju ESP8266.

Perintah menyalakan LED:

```json
{
  "perintah": "ON"
}
```

Perintah mematikan LED:

```json
{
  "perintah": "OFF"
}
```

---

# Percobaan 4A – Subscribe dan Kendali Aktuator

## Tujuan

Percobaan 4A dilakukan untuk memahami proses penerimaan pesan MQTT menggunakan mekanisme **subscribe**, kemudian memproses pesan JSON untuk mengendalikan aktuator.

ESP8266 melakukan subscribe pada topic:

```text
unsoed/tk245004/farhan/perintah
```

---

## Alur Percobaan 4A

```text
Mulai
  |
  v
Hubungkan ESP8266 ke WiFi
  |
  v
Hubungkan ke MQTT Broker
  |
  v
Subscribe Topic Perintah
  |
  v
client.loop()
  |
  v
Pesan MQTT diterima?
  |
  +---- Tidak ----> Kembali ke client.loop()
  |
 Ya
  |
  v
callback()
  |
  v
Ambil Payload
  |
  v
deserializeJson()
  |
  v
Ambil Nilai "perintah"
  |
  +------ ON ------> LED MENYALA
  |
  +------ OFF -----> LED MATI
```

---

## Hasil Percobaan 4A

Pengujian dilakukan dengan mengirim perintah melalui MQTT Explorer.

| No. | Perintah JSON | Pesan Diterima | Hasil Parsing | Status LED | Keterangan |
|---:|---|---|---|---|---|
| 1 | `{"perintah":"ON"}` | `{"perintah":"ON"}` | ON | Menyala | LED menyala setelah menerima perintah ON |
| 2 | `{"perintah":"OFF"}` | `{"perintah":"OFF"}` | OFF | Mati | LED mati setelah menerima perintah OFF |
| 3 | `{"perintah":"ON"}` | `{"perintah":"ON"}` | ON | Menyala | Perintah berhasil diproses |
| 4 | `{"perintah":"OFF"}` | `{"perintah":"OFF"}` | OFF | Mati | Perintah berhasil diproses |

Contoh hasil Serial Monitor:

```text
Pesan diterima [unsoed/tk245004/farhan/perintah]: {"perintah":"ON"}
Hasil parsing: ON
Aktuator: ON
LED MENYALA
```

Sedangkan ketika perintah OFF dikirim:

```text
Pesan diterima [unsoed/tk245004/farhan/perintah]: {"perintah":"OFF"}
Hasil parsing: OFF
Aktuator: OFF
LED MATI
```

Hasil tersebut menunjukkan bahwa ESP8266 berhasil menerima pesan dari broker, melakukan parsing JSON, kemudian mengubah kondisi LED sesuai perintah.

---

# Percobaan 4B – Pertukaran Data Dua Arah

## Tujuan

Percobaan 4B mengembangkan komunikasi menjadi **dua arah**.

ESP8266 melakukan dua proses secara bersamaan:

1. **Publish** data suhu dari sensor DHT11.
2. **Subscribe** perintah ON/OFF untuk mengendalikan LED.

Dengan demikian, perangkat tetap dapat mengirim data sensor sekaligus menerima perintah dari MQTT Explorer.

---

## Alur Percobaan 4B

```text
                +----------------+
                |     ESP8266    |
                +----------------+
                   |           ^
                   |           |
        Publish    |           | Subscribe
        data suhu  |           | perintah LED
                   v           |
                MQTT Broker
                   ^           |
                   |           |
                   |           v
                MQTT Explorer
```

Di dalam program:

```text
loop()
  |
  +--> Cek koneksi MQTT
  |
  +--> client.loop()
  |
  +--> Cek millis()
          |
          | interval >= 5000 ms
          v
       Baca DHT11
          |
          v
       Buat JSON
          |
          v
       Publish data suhu
```

Penggunaan `millis()` membuat proses publish tidak menghentikan `client.loop()`, sehingga ESP8266 tetap dapat menerima perintah MQTT.

---

## Hasil Publish Data Sensor

Sensor DHT11 berhasil membaca suhu dan ESP8266 berhasil mengirim data melalui topic:

```text
unsoed/tk245004/farhan/data
```

Beberapa nilai suhu yang diperoleh selama praktikum:

```text
25.3 °C
24.8 °C
24.5 °C
```

Contoh output Serial Monitor:

```text
Suhu terbaca: 25.30 °C
Data dipublish: {"suhu":25.3}
Topic: unsoed/tk245004/farhan/data
Status publish: BERHASIL
```

Data berikutnya:

```text
Suhu terbaca: 24.80 °C
Data dipublish: {"suhu":24.8}
Topic: unsoed/tk245004/farhan/data
Status publish: BERHASIL
```

Dan:

```text
Suhu terbaca: 24.50 °C
Data dipublish: {"suhu":24.5}
Topic: unsoed/tk245004/farhan/data
Status publish: BERHASIL
```

---

## Hasil Komunikasi Dua Arah

Ketika ESP8266 sedang melakukan publish data suhu, perintah LED tetap dapat diterima.

Contoh:

```text
Pesan diterima [unsoed/tk245004/farhan/perintah]: {"perintah":"ON"}
Hasil parsing: ON
Aktuator: ON
LED MENYALA

Suhu terbaca: 24.50 °C
Data dipublish: {"suhu":24.5}
Topic: unsoed/tk245004/farhan/data
Status publish: BERHASIL
```

Hal tersebut menunjukkan bahwa proses **publish dan subscribe dapat berjalan secara bersamaan**.

---

# Hasil Pengamatan

## Percobaan 4A

| No. | Perintah JSON | Pesan Diterima | Hasil Parsing | Status LED | Keterangan |
|---:|---|---|---|---|---|
| 1 | `{"perintah":"ON"}` | `{"perintah":"ON"}` | ON | Menyala | LED menyala setelah menerima perintah ON |
| 2 | `{"perintah":"OFF"}` | `{"perintah":"OFF"}` | OFF | Mati | LED mati setelah menerima perintah OFF |
| 3 | `{"perintah":"ON"}` | `{"perintah":"ON"}` | ON | Menyala | LED berhasil merespons perintah |
| 4 | `{"perintah":"OFF"}` | `{"perintah":"OFF"}` | OFF | Mati | LED berhasil merespons perintah |

---

## Percobaan 4B

Hasil utama pengujian komunikasi dua arah:

| Pengujian | Hasil |
|---|---|
| ESP8266 terhubung ke WiFi | Berhasil |
| ESP8266 terhubung ke broker MQTT | Berhasil |
| Publish data suhu | Berhasil |
| Data tampil di MQTT Explorer | Berhasil |
| Subscribe perintah ON | Berhasil |
| Subscribe perintah OFF | Berhasil |
| LED menyala melalui MQTT | Berhasil |
| LED mati melalui MQTT | Berhasil |
| Publish dan subscribe bersamaan | Berhasil |

---

# Pertanyaan dan Analisis Modul

## Percobaan 4A

### 1. Bagaimana alur proses penerimaan dan pemrosesan pesan pada `callback()`?

Alurnya adalah:

```text
Pesan MQTT diterima
        |
        v
callback() dipanggil
        |
        v
Payload dibaca
        |
        v
deserializeJson()
        |
        v
JSON valid?
   /          \
Tidak         Ya
 |             |
Error       Ambil "perintah"
               |
          +----+----+
          |         |
         ON        OFF
          |         |
     LED Nyala   LED Mati
```

Fungsi `callback()` akan dijalankan ketika terdapat pesan baru pada topic yang sudah di-subscribe. Payload kemudian diubah menjadi data yang dapat diproses dan dilakukan deserialisasi menggunakan `deserializeJson()`.

---

### 2. Apa yang terjadi jika pesan yang diterima bukan JSON yang valid?

Jika pesan bukan JSON yang valid, `deserializeJson()` akan menghasilkan error.

Program akan menampilkan pesan kegagalan parsing pada Serial Monitor dan perintah tidak diteruskan untuk mengubah kondisi LED.

Contohnya:

```text
Gagal parsing JSON
```

Dengan demikian, LED tidak berubah berdasarkan pesan yang gagal diproses.

---

### 3. Mengapa `client.subscribe()` dipanggil setelah koneksi MQTT berhasil?

Subscribe hanya dapat dilakukan setelah ESP8266 berhasil terhubung ke MQTT broker.

Selain itu, ketika koneksi MQTT terputus kemudian tersambung kembali, ESP8266 perlu melakukan subscribe kembali pada topic yang digunakan.

Karena itu, proses:

```cpp
client.subscribe(topicPerintah);
```

diletakkan pada bagian koneksi MQTT setelah `client.connect()` berhasil.

---

# Pertanyaan Percobaan 4B

### 1. Mengapa penggunaan `delay()` yang lama sebaiknya dihindari?

`delay()` yang terlalu lama dapat menghentikan sementara jalannya program.

Pada komunikasi MQTT, fungsi:

```cpp
client.loop();
```

harus dipanggil secara berkala agar ESP8266 tetap dapat menerima pesan dari broker.

Jika program terlalu lama berada di dalam `delay()`, pesan MQTT dapat terlambat diproses dan sistem menjadi kurang responsif.

---

### 2. Bagaimana mekanisme non-blocking menggunakan `millis()`?

`millis()` digunakan untuk mengetahui waktu yang telah berjalan sejak mikrokontroler aktif.

Program menyimpan waktu terakhir ketika data dikirim:

```cpp
unsigned long waktuTerakhirPublish = 0;
```

Kemudian waktu tersebut dibandingkan dengan waktu sekarang:

```cpp
if (waktuSekarang - waktuTerakhirPublish >= intervalPublish)
```

Jika selisih waktu telah mencapai interval yang ditentukan, data suhu dikirim.

Pada praktikum ini:

```cpp
const unsigned long intervalPublish = 5000;
```

artinya data dikirim setiap sekitar **5 detik**.

Keuntungan cara ini adalah program tidak berhenti menunggu seperti ketika menggunakan `delay()`, sehingga:

```cpp
client.loop();
```

tetap dapat dijalankan dan perintah ON/OFF tetap bisa diterima.

---

# Pengembangan Berdasarkan Tugas Modul

## 1. Kendali Intensitas LED Menggunakan PWM

Sistem dapat dikembangkan agar pesan MQTT tidak hanya berisi ON atau OFF, tetapi juga nilai intensitas LED.

Contoh JSON:

```json
{
  "intensitas": 128
}
```

Nilai tersebut kemudian dapat digunakan sebagai nilai PWM untuk mengatur tingkat kecerahan LED.

Alur:

```text
MQTT Explorer
      |
      | {"intensitas":128}
      v
MQTT Broker
      |
      v
ESP8266
      |
      v
Parsing JSON
      |
      v
PWM LED
```

Dengan cara ini, aktuator tidak hanya memiliki dua kondisi ON dan OFF, tetapi intensitasnya juga dapat dikendalikan.

---

## 2. Penambahan Aktuator Kedua

Sistem juga dapat dikembangkan dengan menambahkan aktuator kedua menggunakan topic MQTT yang berbeda.

Contoh:

```text
unsoed/tk245004/farhan/perintah
```

untuk LED pertama, sedangkan aktuator kedua dapat menggunakan topic lain.

ESP8266 kemudian melakukan subscribe pada kedua topic dan menentukan aktuator yang dikendalikan berdasarkan topic pesan yang diterima.

Konsepnya:

```text
                    MQTT Broker
                    /         \
                   /           \
          Topic Aktuator 1   Topic Aktuator 2
                 |                |
                 v                v
               LED 1            LED 2
```

---

# Kendala Praktikum

Pada saat pengujian Percobaan 4B, data suhu sempat tidak muncul pada panel topic MQTT Explorer walaupun Serial Monitor menampilkan:

```text
Status publish: BERHASIL
```

Sementara itu, pengiriman perintah dari MQTT Explorer ke ESP8266 sudah berhasil karena LED dapat menyala dan mati.

Permasalahan tersebut diselesaikan dengan mengatur subscription MQTT Explorer menjadi:

```text
unsoed/tk245004/farhan/#
```

Setelah melakukan reconnect, struktur topic berhasil muncul:

```text
unsoed
└── tk245004
    └── farhan
        ├── data
        └── perintah
```

Data suhu kemudian berhasil ditampilkan pada MQTT Explorer.

---

# Hasil Akhir

Praktikum Modul 4 berhasil menerapkan komunikasi MQTT dua arah.

ESP8266 berhasil:

- Terhubung dengan jaringan WiFi.
- Terhubung dengan broker `broker.hivemq.com`.
- Melakukan subscribe pada topic perintah.
- Menerima data JSON.
- Melakukan deserialisasi JSON.
- Mengendalikan LED dengan perintah ON dan OFF.
- Membaca suhu menggunakan DHT11.
- Mengubah data suhu menjadi format JSON.
- Melakukan publish data suhu setiap sekitar 5 detik.
- Menjalankan publish dan subscribe secara bersamaan.
- Mempertahankan komunikasi MQTT menggunakan `client.loop()`.
- Menggunakan `millis()` untuk proses pengiriman data secara non-blocking.

Hasil pengujian menunjukkan nilai suhu sekitar **24,5–25,3 °C**, sedangkan LED berhasil merespons perintah ON dan OFF yang dikirim melalui MQTT Explorer.

---

# Kesimpulan

Berdasarkan Praktikum Modul 4, protokol MQTT dapat digunakan untuk melakukan komunikasi dua arah pada sistem IoT menggunakan mekanisme publish dan subscribe.

Pada Percobaan 4A, ESP8266 berhasil menerima perintah dalam format JSON dari MQTT Explorer dan mengendalikan LED sesuai perintah `ON` atau `OFF`.

Pada Percobaan 4B, ESP8266 berhasil melakukan publish data suhu DHT11 secara berkala sekaligus tetap menerima perintah untuk mengendalikan LED. Penggunaan `millis()` membuat proses pengiriman data dapat dilakukan tanpa menghentikan proses komunikasi MQTT.

Dengan demikian, konsep MQTT, JSON, callback, publish, subscribe, `client.loop()`, dan komunikasi non-blocking berhasil diterapkan pada praktikum.

---

## Status Praktikum

- [x] Percobaan 4A — Subscribe dan Kendali Aktuator
- [x] Pengiriman perintah JSON
- [x] Deserialisasi JSON
- [x] Kendali LED ON/OFF
- [x] Percobaan 4B — Pertukaran Data Dua Arah
- [x] Pembacaan sensor DHT11
- [x] Publish data suhu
- [x] Subscribe perintah LED
- [x] Publish dan subscribe bersamaan
- [x] Pengujian MQTT Explorer
- [x] Modul 4 selesai

---

## Catatan Penting

Saat melakukan praktikum:

- Pastikan ESP8266 dan komputer memiliki koneksi internet.
- Pastikan broker MQTT dapat diakses.
- Gunakan topic yang sesuai agar tidak tercampur dengan topic pengguna lain.
- Pastikan format JSON yang dikirim benar.
- Jalankan `client.loop()` secara berkala.
- Hindari penggunaan `delay()` yang terlalu lama pada komunikasi dua arah.
- Gunakan resistor pada LED untuk membatasi arus.
- Periksa kembali pin sensor dan aktuator sebelum menyalakan rangkaian.

---

## Referensi & Resources

### Dokumentasi Library

- PubSubClient
- ArduinoJson
- DHT Sensor Library
- ESP8266 Arduino Core

### Teknologi yang Digunakan

- ESP8266 NodeMCU
- MQTT
- JSON
- HiveMQ Public Broker
- MQTT Explorer
- Arduino IDE

---

## Kontak & Informasi

**Praktikan:**

- Nama: Farhan Nur Sahid
- NIM: H1H024057
- Program Studi: Teknik Komputer
- Universitas: Universitas Jenderal Soedirman

---

**Terakhir diperbarui:** Oktober 2026

---

*Repository ini digunakan sebagai dokumentasi Praktikum Internet of Things Modul 4 – Komunikasi dan Pertukaran Data.*
