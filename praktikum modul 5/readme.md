# Praktikum Internet of Things

**Modul 5 – Pengolahan dan Analisis Data IoT**

![Status](https://img.shields.io/badge/Status-Complete-brightgreen)
![Protocol](https://img.shields.io/badge/Protocol-MQTT-blue)
![Board](https://img.shields.io/badge/Board-ESP8266-orange)
![Language](https://img.shields.io/badge/Language-Python-yellow)
![Contributor](https://img.shields.io/badge/Contributor-Farhan%20Nur%20Sahid-informational)

---

## Identitas

| Informasi | Detail |
|---|---|
| **Nama** | Farhan Nur Sahid |
| **NIM** | H1H024057 |
| **Kelompok** | 4 |
| **Mata Kuliah** | Praktikum Internet of Things (TK245002) |
| **Program Studi** | Teknik Komputer |
| **Universitas** | Universitas Jenderal Soedirman |
| **Tahun / Semester** | 2026 / 5 |
| **Modul** | Modul 5 – Pengolahan dan Analisis Data IoT |

---

## Tentang Repository

Repository ini berisi source code dan dokumentasi praktikum Modul 5 mengenai pengolahan dan analisis data pada sistem Internet of Things.

Pada modul ini dipelajari proses pengolahan data pada mikrokontroler menggunakan metode **Moving Average**, pengiriman data menggunakan protokol **MQTT**, penyimpanan data menggunakan Python, serta analisis statistik dan deteksi anomali menggunakan metode **Z-score**.

Praktikum dilakukan menggunakan **ESP8266 NodeMCU** dengan fungsi simulasi suhu (*mock*) sebagai pengganti sensor DHT22 fisik.

---

## Tujuan Praktikum

1. Memahami tahapan pengolahan dan analisis data pada sistem IoT.
2. Memahami perbedaan pengolahan data di sisi perangkat dan komputer.
3. Mengimplementasikan filter Moving Average dan perhitungan statistik sederhana.
4. Mengimplementasikan pengambilan data melalui MQTT menggunakan Python.
5. Menyimpan data hasil pengamatan dalam format CSV.
6. Menganalisis data suhu menggunakan rata-rata, simpangan baku, minimum, dan maksimum.
7. Mendeteksi anomali menggunakan metode Z-score.
8. Memvisualisasikan data suhu dalam bentuk grafik.

---

## Hardware yang Digunakan

| No. | Komponen | Fungsi |
|---|---|---|
| 1 | ESP8266 NodeMCU 1.0 | Mikrokontroler utama |
| 2 | Kabel USB | Menghubungkan ESP8266 ke laptop |
| 3 | Laptop | Menjalankan Arduino IDE dan Python |
| 4 | Jaringan Wi-Fi | Menghubungkan ESP8266 ke internet |

Data suhu diperoleh menggunakan fungsi simulasi, sehingga pengujian tidak membutuhkan sensor DHT fisik.

---

## Software dan Library

**Software:**
- Arduino IDE 2.3.10
- Python 3
- MQTT Explorer
- Visual Studio Code
- HiveMQ Public MQTT Broker

**Library Arduino:**
- ESP8266WiFi
- PubSubClient
- ArduinoJson
- math.h

**Library Python:**
- paho-mqtt
- pandas
- matplotlib

Instalasi library Python:

`python -m pip install paho-mqtt pandas matplotlib`

---

## Konfigurasi MQTT

| Parameter | Konfigurasi |
|---|---|
| Broker | broker.hivemq.com |
| Port | 1883 |
| Protocol | MQTT |
| Topic | unsoed/tk245004/kelompok4/data |
| Format data | JSON |
| Interval pengiriman | 5 detik |

Contoh payload JSON:

`{"suhu":28.48}`

ESP8266 bertindak sebagai publisher, sedangkan program Python dan MQTT Explorer bertindak sebagai subscriber.

---

# Percobaan 5A – Pengolahan Data Sensor pada ESP8266

## Tujuan

Percobaan 5A bertujuan untuk memahami proses pengolahan data suhu pada sisi perangkat menggunakan metode Moving Average, serta pencatatan nilai suhu minimum dan maksimum.

## Penjelasan Program

Program menggunakan fungsi `bacaSuhuMock()` untuk menghasilkan data suhu simulasi.

Suhu dasar dibuat menggunakan fungsi sinus sehingga menghasilkan perubahan suhu secara bertahap. Kemudian ditambahkan noise acak untuk menyimulasikan variasi pembacaan sensor.

Setiap 25 pembacaan, program menambahkan lonjakan suhu sebesar 8 °C untuk menyimulasikan anomali.

Data suhu diproses menggunakan Moving Average dengan ukuran buffer 5 data.

Rumus Moving Average:

**Moving Average = Jumlah suhu dalam buffer / Jumlah data dalam buffer**

Program juga mencatat nilai suhu minimum dan maksimum dari seluruh pembacaan.

## Alur Program

1. Memulai program dan Serial Monitor.
2. Menghasilkan data suhu simulasi.
3. Memasukkan data baru ke buffer Moving Average.
4. Menghitung rata-rata suhu dari buffer.
5. Memperbarui suhu minimum dan maksimum.
6. Menampilkan hasil melalui Serial Monitor.
7. Mengulangi proses setiap 2 detik.

## Hasil Percobaan 5A

Hasil pengujian menunjukkan bahwa program berhasil menampilkan suhu mentah, Moving Average, suhu minimum, dan suhu maksimum.

Nilai Moving Average terlihat lebih stabil dibandingkan data mentah karena menggunakan rata-rata dari lima pembacaan terakhir.

Ketika terjadi lonjakan suhu simulasi, nilai suhu mentah berubah secara tiba-tiba. Hasil filter berubah lebih bertahap karena pengaruh data sebelumnya.

## Analisis Percobaan 5A

Penggunaan Moving Average membantu mengurangi fluktuasi acak pada data suhu.

Dengan ukuran buffer 5, hasil filter menjadi lebih halus dibandingkan suhu mentah. Namun, filter juga menyebabkan respons terhadap perubahan suhu mendadak menjadi lebih lambat.

Pengolahan langsung pada ESP8266 merupakan contoh *edge processing*, yaitu pemrosesan data sebelum data dikirimkan ke sistem lain.

---

# Percobaan 5B – Akuisisi, Analisis, dan Visualisasi Data

## Tujuan

Percobaan 5B bertujuan untuk menerima data suhu melalui MQTT menggunakan Python, menyimpan data dalam format CSV, melakukan analisis statistik, mendeteksi anomali menggunakan Z-score, dan memvisualisasikan data suhu.

## Penjelasan Program

ESP8266 menghasilkan data suhu simulasi menggunakan fungsi `bacaSuhuMock()`.

Data tersebut diubah menjadi format JSON kemudian dipublikasikan melalui MQTT ke topic:

`unsoed/tk245004/kelompok4/data`

Program Python terhubung ke broker yang sama, kemudian melakukan subscribe untuk menerima data suhu.

Setiap data yang diterima dicatat bersama waktu penerimaannya. Data kemudian digunakan untuk menghitung rata-rata, simpangan baku, nilai minimum, nilai maksimum, dan Z-score.

Program dirancang untuk mengumpulkan 30 sampel data.

## Alur Komunikasi

1. ESP8266 menghasilkan suhu simulasi.
2. ESP8266 terhubung ke Wi-Fi.
3. ESP8266 melakukan publish data JSON ke broker MQTT.
4. Python melakukan subscribe pada topic yang sama.
5. Python menerima dan membaca data JSON.
6. Data suhu dan waktu penerimaan dicatat.
7. Data diolah menggunakan pandas.
8. Program menghitung statistik dan Z-score.
9. Program menyimpan hasil analisis ke CSV.
10. Program menghasilkan grafik suhu dan penanda anomali.

## Hasil Pengamatan MQTT

Berdasarkan pengujian, ESP8266 berhasil mempublikasikan data ke broker MQTT.

Serial Monitor menunjukkan status **Publish: Berhasil** pada pembacaan suhu.

MQTT Explorer juga berhasil menampilkan topic `unsoed/tk245004/kelompok4/data` beserta nilai suhu yang dikirimkan.

Program Python berhasil terhubung ke broker dengan status **Success** dan menerima data dari topic yang sama.

## Hasil Pengamatan Python

Berdasarkan 26 data yang terlihat pada dokumentasi Python, diperoleh:

| Parameter | Hasil |
|---|---|
| Data terdokumentasi | 26 sampel |
| Rata-rata suhu | 29,29 °C |
| Simpangan baku | 1,90 °C |
| Suhu minimum | 27,47 °C |
| Suhu maksimum | 38,03 °C |
| Anomali terdeteksi | 1 data |

Pada pembacaan ke-18 terdapat lonjakan suhu sebesar **38,03 °C**.

Lonjakan tersebut berasal dari simulasi yang digunakan untuk menguji proses deteksi anomali.

## Deteksi Anomali Menggunakan Z-score

Z-score digunakan untuk mengukur seberapa jauh sebuah data menyimpang dari rata-rata berdasarkan simpangan baku.

Rumus:

**Z = (Suhu - Rata-rata) / Simpangan baku**

Pada praktikum ini, data dikategorikan sebagai anomali apabila nilai absolut Z-score lebih besar dari 2.

Berdasarkan data pengamatan, pembacaan suhu 38,03 °C menghasilkan Z-score sekitar 4,59 sehingga termasuk anomali.

## Moving Average dan Deteksi Anomali

Selain melakukan analisis data mentah, data dapat diolah menggunakan Moving Average dengan ukuran jendela 5 data.

Berdasarkan perhitungan pada 26 data yang terdokumentasi:

| Metode | Jumlah anomali |
|---|---:|
| Data suhu mentah | 1 |
| Data setelah Moving Average 5 | 0 |

Hal ini menunjukkan bahwa Moving Average dapat mengurangi pengaruh lonjakan suhu sesaat.

Namun, penggunaan filter juga berpotensi menyamarkan perubahan suhu mendadak yang sebenarnya perlu dideteksi.

## Visualisasi Data

Visualisasi data dilakukan menggunakan library matplotlib.

Grafik digunakan untuk memperlihatkan perubahan suhu berdasarkan urutan pembacaan, garis rata-rata, dan posisi data yang terdeteksi sebagai anomali.

Program Python dirancang untuk menghasilkan file `grafik_suhu.png`.

---

# Hasil Pengamatan

## Percobaan 5A

- Pembacaan suhu simulasi berhasil dilakukan.
- Moving Average berhasil menghaluskan perubahan suhu.
- Nilai minimum dan maksimum berhasil dicatat.
- Lonjakan suhu simulasi dapat diamati pada hasil pembacaan.

## Percobaan 5B

- ESP8266 berhasil terhubung ke jaringan Wi-Fi.
- ESP8266 berhasil terhubung ke broker MQTT.
- Proses publish data suhu berhasil dilakukan.
- MQTT Explorer berhasil menampilkan topic data.
- Python berhasil terhubung dan melakukan subscribe.
- Python berhasil menerima data suhu secara bertahap.
- Terlihat 26 dari 30 target data pada dokumentasi terminal.
- Ditemukan satu anomali pada data suhu yang terdokumentasi.

**Catatan:** Bukti screenshot yang tersedia memperlihatkan penerimaan hingga 26/30 data. Hasil ekspor otomatis setelah 30 sampel belum terverifikasi dari screenshot tersebut.

---

# Kesimpulan

Berdasarkan hasil praktikum Modul 5, pengolahan data suhu pada perangkat ESP8266 dapat dilakukan menggunakan metode Moving Average.

Metode tersebut membantu menghaluskan data suhu dan mengurangi pengaruh variasi acak.

Pada Percobaan 5B, komunikasi MQTT antara ESP8266 dan Python berhasil dilakukan menggunakan broker HiveMQ. Data suhu yang diterima dapat digunakan untuk analisis statistik dan deteksi anomali.

Penggunaan Z-score berhasil mengidentifikasi lonjakan suhu simulasi sebagai data anomali.

Dengan demikian, praktikum menunjukkan penerapan pengolahan data pada sisi perangkat dan analisis data pada sisi komputer dalam sistem IoT.

---

## Struktur Repository

- **Source Code**: program Arduino Percobaan 5A, 5B, dan program Python.
- **dokumentasi praktikum**: screenshot Serial Monitor, MQTT Explorer, Python, serta data hasil pengamatan.
- **readme.md**: penjelasan dan dokumentasi utama Modul 5.

---

## Referensi

Modul Praktikum 5 – Pengolahan dan Analisis Data IoT: Filtering, Analisis dan Visualisasi Data.

Program Studi Teknik Komputer, Universitas Jenderal Soedirman.
