# Praktikum Internet of Things - Modul 2
## Konfigurasi Jaringan WiFi pada ESP8266

**Nama:** Farhan Nur Sahid  
**NIM:** H1H024057  
**Modul:** 2 - Konfigurasi Jaringan  
**Mikrokontroler:** ESP8266 NodeMCU  
**Mata Kuliah:** Praktikum Internet of Things  

---

## 1. Gambaran Umum Praktikum

Praktikum Modul 2 membahas konfigurasi jaringan WiFi pada ESP8266. Pada percobaan ini digunakan beberapa mode operasi jaringan, yaitu **Station (STA)**, **Access Point (AP)**, dan kombinasi **AP+STA**.

Mode Station digunakan ketika ESP8266 ingin bergabung ke jaringan WiFi yang sudah tersedia. Mode Access Point digunakan ketika ESP8266 menyediakan jaringan WiFi sendiri sehingga perangkat lain dapat terhubung langsung. Sementara itu, mode AP+STA memungkinkan kedua fungsi tersebut dijalankan secara bersamaan.

Praktikum juga mengamati beberapa parameter jaringan seperti **IP Address, MAC Address, status koneksi, jumlah client, dan RSSI (Received Signal Strength Indicator)**.

---

## 2. Library yang Digunakan

Karena perangkat yang digunakan pada praktikum ini adalah **ESP8266 NodeMCU**, library WiFi yang digunakan adalah:

```cpp
#include <ESP8266WiFi.h>
```

Library `ESP8266WiFi.h` menyediakan fungsi-fungsi yang diperlukan untuk mengatur koneksi jaringan pada ESP8266.

| Fungsi | Kegunaan |
|---|---|
| `WiFi.mode()` | Menentukan mode operasi WiFi |
| `WiFi.begin()` | Memulai koneksi ESP8266 ke jaringan WiFi |
| `WiFi.status()` | Memeriksa keadaan koneksi WiFi |
| `WiFi.localIP()` | Menampilkan IP Address pada mode Station |
| `WiFi.macAddress()` | Membaca MAC Address ESP8266 |
| `WiFi.RSSI()` | Membaca kekuatan sinyal WiFi dalam dBm |
| `WiFi.softAP()` | Mengaktifkan ESP8266 sebagai Access Point |
| `WiFi.softAPIP()` | Menampilkan IP Address Access Point |
| `WiFi.softAPgetStationNum()` | Menghitung jumlah client yang terhubung ke AP |

---

# 3. Percobaan 2A - Mode Station (STA)

## 3.1 Tujuan

Percobaan ini dilakukan untuk mengonfigurasi ESP8266 sebagai **Station (STA)** sehingga dapat bergabung ke jaringan WiFi yang tersedia serta mengetahui informasi jaringan setelah koneksi berhasil dilakukan.

Informasi yang diamati antara lain:

- Status koneksi
- IP Address
- MAC Address
- Nilai RSSI
- Kondisi LED indikator

---

## 3.2 Cara Kerja

Dalam mode Station, ESP8266 berfungsi sebagai **client**. ESP8266 tidak membuat jaringan WiFi sendiri, tetapi melakukan koneksi ke jaringan yang sudah tersedia menggunakan SSID dan password.

Alur koneksi secara sederhana:

```text
ESP8266
   |
   | WiFi.begin()
   v
Jaringan WiFi / Hotspot
   |
   v
Autentikasi SSID dan Password
   |
   v
Mendapatkan IP Address
   |
   v
ESP8266 Terhubung
```

---

## 3.3 Program Mode Station

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "realme C53";
const char* password = "PASSWORD_WIFI";

const int ledPin = D4;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Mengaktifkan ESP8266 sebagai Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi ke WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI (dBm): ");
  Serial.println(WiFi.RSSI());

  digitalWrite(ledPin, HIGH);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(ledPin, LOW);
  }

  delay(5000);
}
```

> Password hotspot tidak dicantumkan pada repository untuk menjaga keamanan jaringan.

---

## 3.4 Penjelasan Program

### `WiFi.mode(WIFI_STA)`

Perintah ini mengatur ESP8266 agar bekerja sebagai **Station**, yaitu perangkat yang bergabung dengan jaringan WiFi lain.

### `WiFi.begin(ssid, password)`

Digunakan untuk memulai proses koneksi berdasarkan SSID dan password yang telah dimasukkan.

### `WiFi.status()`

Digunakan untuk memeriksa kondisi koneksi. Apabila nilainya sama dengan `WL_CONNECTED`, ESP8266 telah berhasil tersambung ke jaringan.

### `WiFi.localIP()`

Menampilkan IP Address yang diberikan jaringan kepada ESP8266 setelah berhasil terkoneksi.

### `WiFi.macAddress()`

Digunakan untuk memperoleh MAC Address dari interface WiFi ESP8266.

### `WiFi.RSSI()`

Digunakan untuk membaca kuat sinyal WiFi yang diterima oleh ESP8266 dalam satuan dBm.

---

## 3.5 Penggunaan Conditional

Pemeriksaan status WiFi dilakukan menggunakan percabangan:

```cpp
if (WiFi.status() == WL_CONNECTED) {
  Serial.println("Status: Terhubung");
} else {
  Serial.println("Status: Terputus");
  digitalWrite(ledPin, LOW);
}
```

Apabila ESP8266 masih terhubung ke WiFi, Serial Monitor menampilkan status **Terhubung**. Apabila koneksi terputus, program menampilkan status **Terputus** dan mematikan LED indikator.

---

## 3.6 Hasil Percobaan STA

Pada pengujian yang dilakukan, ESP8266 berhasil menjalankan fungsi sebagai Station dan Serial Monitor dapat menampilkan status:

```text
Status: Terhubung
```

Hal tersebut menunjukkan bahwa ESP8266 telah berhasil melakukan koneksi sebagai client ke jaringan WiFi yang digunakan.

Data IP Address, MAC Address, dan RSSI pada tabel pengamatan dicatat berdasarkan keluaran Serial Monitor saat pengujian dilakukan sehingga tidak menggunakan data dari perangkat lain.

---

# 4. Modifikasi Percobaan 2A - Auto Reconnect

## 4.1 Tujuan Modifikasi

Pada program dasar, ketika jaringan terputus ESP8266 hanya mendeteksi perubahan status koneksi. Oleh karena itu, program dikembangkan agar ESP8266 dapat mencoba **menghubungkan kembali WiFi secara otomatis** ketika koneksi terputus.

Dengan mekanisme ini, perangkat tidak harus di-reset secara manual setiap kali terjadi gangguan jaringan.

---

## 4.2 Program Auto Reconnect

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "realme C53";
const char* password = "PASSWORD_WIFI";

const int ledPin = D4;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(ledPin, HIGH);
}

void loop() {

  // Mengecek apakah koneksi WiFi terputus
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Koneksi WiFi terputus!");
    digitalWrite(ledPin, LOW);

    Serial.println("Mencoba menghubungkan kembali...");

    // Memutus koneksi lama
    WiFi.disconnect();

    // Memulai koneksi kembali
    WiFi.begin(ssid, password);

    unsigned long waktuMulai = millis();

    // Memberikan waktu maksimal 10 detik untuk reconnect
    while (WiFi.status() != WL_CONNECTED &&
           millis() - waktuMulai < 10000) {

      delay(500);
      Serial.print(".");
    }

    Serial.println();

    // Mengecek hasil reconnect
    if (WiFi.status() == WL_CONNECTED) {

      Serial.println("Koneksi berhasil dipulihkan!");

      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      digitalWrite(ledPin, HIGH);

    } else {

      Serial.println("Reconnect belum berhasil.");
    }

  } else {

    Serial.println("Status: Terhubung");
    digitalWrite(ledPin, HIGH);
  }

  delay(1000);
}
```

---

## 4.3 Penjelasan Bagian yang Ditambahkan

### `WiFi.status() != WL_CONNECTED`

Digunakan untuk mengetahui apakah koneksi ESP8266 sedang terputus.

```cpp
if (WiFi.status() != WL_CONNECTED)
```

Jika kondisi tersebut terpenuhi, program mulai menjalankan proses reconnect.

### `WiFi.disconnect()`

Perintah ini digunakan untuk mengakhiri koneksi sebelumnya sebelum ESP8266 melakukan percobaan koneksi ulang.

### `WiFi.begin(ssid, password)`

Setelah koneksi lama diputus, perintah ini digunakan kembali untuk mencoba tersambung ke jaringan yang sama.

### `millis()`

Fungsi `millis()` digunakan untuk menghitung lama proses reconnect.

```cpp
unsigned long waktuMulai = millis();
```

Kemudian proses koneksi diberikan batas waktu:

```cpp
while (WiFi.status() != WL_CONNECTED &&
       millis() - waktuMulai < 10000)
```

Artinya, ESP8266 mencoba menghubungkan kembali jaringan selama maksimal **10 detik** pada setiap percobaan reconnect.

---

## 4.4 Alur Auto Reconnect

```text
Periksa koneksi WiFi
        |
        v
Apakah masih terhubung?
     /       \
   Ya         Tidak
   |            |
   v            v
Status       LED mati
Terhubung       |
                v
        Putus koneksi lama
                |
                v
          WiFi.begin()
                |
                v
        Mencoba reconnect
                |
                v
        Berhasil terhubung?
           /        \
         Ya          Tidak
         |             |
         v             v
     LED menyala   Coba kembali
```

---

# 5. Percobaan 2B - Mode Access Point (AP)

## 5.1 Tujuan

Percobaan Access Point dilakukan untuk mengetahui kemampuan ESP8266 dalam **menyediakan jaringan WiFi sendiri** sehingga smartphone atau perangkat lain dapat terkoneksi langsung tanpa membutuhkan router sebagai pusat jaringan.

---

## 5.2 Cara Kerja

Pada mode AP, ESP8266 bertindak sebagai penyedia jaringan.

```text
             ESP8266
          Access Point
               |
        +------+------+
        |             |
        v             v
   Smartphone       Client
```

ESP8266 membuat SSID serta password yang kemudian dapat ditemukan melalui menu WiFi pada smartphone atau laptop.

---

## 5.3 Program Access Point

```cpp
#include <ESP8266WiFi.h>

const char* ap_ssid = "ESP32_AccessPoint";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Mengaktifkan mode Access Point
  WiFi.mode(WIFI_AP);

  // Membuat jaringan WiFi
  WiFi.softAP(ap_ssid, ap_password);

  // Mengambil IP Address AP
  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");

  Serial.print("SSID : ");
  Serial.println(ap_ssid);

  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {

  // Membaca jumlah client setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```

---

## 5.4 Fungsi Utama Mode AP

### `WiFi.mode(WIFI_AP)`

Mengatur ESP8266 agar hanya beroperasi sebagai Access Point.

### `WiFi.softAP(ap_ssid, ap_password)`

Digunakan untuk membuat jaringan WiFi menggunakan SSID dan password yang telah ditetapkan.

### `WiFi.softAPIP()`

Mengambil IP Address dari Access Point yang dibuat oleh ESP8266.

### `WiFi.softAPgetStationNum()`

Menghitung jumlah perangkat yang sedang terhubung ke jaringan ESP8266.

---

## 5.5 Hasil Pengamatan Access Point

Berdasarkan pengujian yang dilakukan, diperoleh konfigurasi berikut:

| Parameter | Hasil Pengamatan |
|---|---|
| SSID | `ESP32_AccessPoint` |
| Password AP | `12345678` |
| IP Address AP | `192.168.4.1` |
| Status Access Point | Aktif |
| SSID dapat ditemukan client | Ya |
| Client berhasil terhubung | Ya |
| Jumlah client yang teramati | Hingga 2 perangkat |

Serial Monitor menampilkan:

```text
Access Point aktif!
SSID : ESP32_AccessPoint
IP Address : 192.168.4.1
```

Sebelum terdapat perangkat yang masuk ke jaringan, jumlah client menunjukkan:

```text
Jumlah perangkat terhubung: 0
```

Setelah sebuah perangkat berhasil bergabung:

```text
Jumlah perangkat terhubung: 1
```

Pada pengujian berikutnya, jumlah perangkat yang terhubung berhasil meningkat menjadi:

```text
Jumlah perangkat terhubung: 2
```

Dari hasil tersebut dapat diketahui bahwa Access Point berhasil dibuat dan ESP8266 mampu mendeteksi perubahan jumlah client yang tersambung.

---

# 6. Modifikasi Percobaan 2B - Mode AP+STA

## 6.1 Tujuan Modifikasi

Mode **AP+STA** digunakan agar ESP8266 dapat menjalankan dua peran jaringan secara bersamaan.

ESP8266 dapat:

1. Bergabung ke jaringan WiFi sebagai **Station**.
2. Membuat jaringan sendiri sebagai **Access Point**.

---

## 6.2 Gambaran Kerja AP+STA

```text
          Jaringan WiFi
               |
               | STA
               v
          +----------+
          | ESP8266  |
          | AP + STA |
          +----------+
               |
               | AP
               v
       ESP32_AccessPoint
               |
        +------+------+
        |             |
        v             v
   Smartphone       Laptop
```

---

## 6.3 Program AP+STA

```cpp
#include <ESP8266WiFi.h>

// Jaringan yang digunakan pada mode Station
const char* ssidSTA = "realme C53";
const char* passwordSTA = "PASSWORD_WIFI";

// Jaringan yang dibuat ESP8266
const char* ssidAP = "ESP32_AccessPoint";
const char* passwordAP = "12345678";

void setup() {
  Serial.begin(115200);

  delay(100);

  Serial.println();
  Serial.println("Memulai Mode AP + STA");

  // Mengaktifkan Station dan Access Point
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan mode STA ke jaringan WiFi
  WiFi.begin(ssidSTA, passwordSTA);

  Serial.print("Menghubungkan STA ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Mode Station berhasil terhubung!");

  Serial.print("IP Address STA : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ssidAP, passwordAP);

  Serial.println("Access Point berhasil diaktifkan!");

  Serial.print("SSID AP : ");
  Serial.println(ssidAP);

  Serial.print("IP Address AP : ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

  // Memeriksa koneksi Station
  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Status STA: Terhubung");

  } else {

    Serial.println("Status STA: Terputus");
  }

  // Menampilkan jumlah client pada AP
  Serial.print("Jumlah client AP: ");
  Serial.println(WiFi.softAPgetStationNum());

  delay(5000);
}
```

---

## 6.4 Penjelasan Program AP+STA

### `WiFi.mode(WIFI_AP_STA)`

Digunakan untuk mengaktifkan mode Station dan Access Point secara bersamaan.

### `WiFi.begin(ssidSTA, passwordSTA)`

Menghubungkan ESP8266 dengan jaringan WiFi yang tersedia sebagai Station.

### `WiFi.softAP(ssidAP, passwordAP)`

Membuat jaringan WiFi yang dapat digunakan perangkat lain untuk terhubung langsung dengan ESP8266.

### `WiFi.localIP()`

Menampilkan alamat IP yang diperoleh ESP8266 ketika berperan sebagai Station.

### `WiFi.softAPIP()`

Menampilkan alamat IP pada sisi Access Point.

### `WiFi.softAPgetStationNum()`

Digunakan untuk mengetahui jumlah client yang masuk ke jaringan Access Point.

---

# 7. Pengamatan RSSI

## 7.1 Pengertian RSSI

**RSSI (Received Signal Strength Indicator)** merupakan nilai yang digunakan untuk menunjukkan tingkat kekuatan sinyal WiFi yang diterima ESP8266.

Nilai RSSI ditampilkan dalam satuan **dBm** dan umumnya memiliki nilai negatif.

Secara sederhana:

```text
Nilai lebih dekat ke 0 dBm  -> sinyal lebih kuat
Nilai semakin negatif       -> sinyal semakin lemah
```

Sebagai contoh:

```text
-30 dBm -> sinyal sangat kuat
-40 dBm -> sinyal kuat
-65 dBm -> sinyal lebih lemah dibanding -40 dBm
```

Nilai RSSI dapat diperoleh menggunakan:

```cpp
WiFi.RSSI();
```

Kekuatan sinyal dapat berubah akibat jarak ESP8266 dengan Access Point, penghalang fisik, interferensi, maupun kondisi jaringan di sekitar perangkat.

---

# 8. Pengujian Kredensial WiFi

Pengujian kredensial dilakukan untuk mengetahui respons ESP8266 ketika SSID dan password yang diberikan benar atau salah.

| Kondisi | Hasil yang Diamati |
|---|---|
| SSID dan password benar | ESP8266 dapat melakukan koneksi |
| Password salah | ESP8266 tidak berhasil terhubung |
| SSID salah | Jaringan tujuan tidak dapat dihubungkan |

Ketika kredensial benar, kondisi:

```cpp
WiFi.status() == WL_CONNECTED
```

akan terpenuhi setelah ESP8266 berhasil masuk ke jaringan.

Apabila SSID atau password salah, proses koneksi tidak mencapai kondisi `WL_CONNECTED` sehingga ESP8266 terus menunggu atau mencoba melakukan koneksi.

---

# 9. Jawaban Pertanyaan Percobaan 2A

## 9.1 Apa fungsi `WiFi.mode(WIFI_STA)`?

`WiFi.mode(WIFI_STA)` digunakan untuk menentukan bahwa ESP8266 bekerja sebagai **Station atau client**. Dengan mode ini ESP8266 dapat bergabung ke jaringan WiFi yang sudah tersedia.

---

## 9.2 Apa yang terjadi apabila SSID atau password salah?

Apabila SSID tidak sesuai, ESP8266 tidak dapat menemukan jaringan tujuan yang benar. Jika SSID benar tetapi password salah, proses autentikasi gagal.

Akibatnya ESP8266 tidak mencapai status `WL_CONNECTED` dan koneksi WiFi tidak berhasil dibentuk.

---

## 9.3 Flowchart Koneksi Mode Station

```text
        Mulai
          |
          v
 Inisialisasi Serial
          |
          v
 Aktifkan WIFI_STA
          |
          v
 WiFi.begin(SSID, Password)
          |
          v
  Periksa WiFi.status()
          |
       Terhubung?
       /       \
    Tidak       Ya
      |          |
      v          v
   Tunggu     Tampilkan
  500 ms      IP, MAC,
      |        dan RSSI
      |          |
      +----------+
                 |
                 v
             LED Menyala
                 |
                 v
        Periksa Koneksi
          Setiap 5 Detik
```

---

# 10. Jawaban Pertanyaan Percobaan 2B

## 10.1 Mengapa IP Address Access Point umumnya `192.168.4.1`?

Ketika ESP8266 membuat Access Point menggunakan konfigurasi standar library, interface AP memperoleh alamat IP lokal **192.168.4.1**. Alamat tersebut digunakan sebagai alamat ESP8266 pada jaringan lokal yang dibuatnya dan menjadi tujuan komunikasi bagi client yang terhubung.

Pada percobaan ini Serial Monitor juga menunjukkan:

```text
IP Address : 192.168.4.1
```

---

## 10.2 Apa perbedaan Mode Station dan Access Point?

Pada **Station (STA)**, ESP8266 bertindak sebagai client dan bergabung ke jaringan WiFi yang sudah tersedia.

Pada **Access Point (AP)**, ESP8266 justru menyediakan jaringan WiFi sendiri sehingga smartphone atau komputer dapat terhubung langsung ke ESP8266.

Perbedaannya dapat digambarkan sebagai berikut:

| Mode | Peran ESP8266 |
|---|---|
| STA | Client jaringan |
| AP | Penyedia jaringan |
| AP+STA | Client sekaligus penyedia jaringan |

---

## 10.3 Apa risiko jika Access Point tidak menggunakan password yang aman?

Access Point tanpa password atau menggunakan password yang terlalu sederhana lebih mudah dimasuki oleh perangkat yang tidak diinginkan. Kondisi tersebut dapat menyebabkan akses tanpa izin, penggunaan jaringan oleh pihak lain, dan meningkatkan risiko terhadap komunikasi perangkat IoT.

Oleh sebab itu, Access Point sebaiknya menggunakan password yang cukup kuat dan tidak menggunakan kredensial penting yang sama dengan akun lain.

---

# 11. Jawaban Pertanyaan Analisis Modul

## 11.1 Uraikan hasil praktikum pada setiap percobaan

### Mode Station (STA)

Pada percobaan Station, ESP8266 berhasil digunakan sebagai client yang melakukan koneksi ke jaringan WiFi. Setelah berhasil terkoneksi, status **Terhubung** dapat ditampilkan melalui Serial Monitor. Program juga dirancang untuk membaca IP Address, MAC Address, RSSI, dan mengendalikan LED berdasarkan kondisi koneksi.

### Mode Access Point (AP)

Pada mode Access Point, ESP8266 berhasil membuat jaringan dengan SSID:

```text
ESP32_AccessPoint
```

Access Point aktif menggunakan IP Address:

```text
192.168.4.1
```

Jaringan dapat ditemukan serta digunakan oleh perangkat lain. Hasil pengamatan pada Serial Monitor menunjukkan perubahan jumlah client dari **0 perangkat menjadi 1 perangkat**, dan pada pengujian selanjutnya berhasil mencapai **2 perangkat yang terhubung**.

### Mode AP+STA

Pada mode AP+STA, program dirancang agar ESP8266 mampu terhubung ke jaringan WiFi sebagai Station sambil tetap menyediakan Access Point bagi client. Dengan demikian, dua fungsi jaringan dapat digunakan secara bersamaan.

---

## 11.2 Bagaimana pengaruh RSSI terhadap kestabilan koneksi WiFi?

RSSI menunjukkan tingkat kekuatan sinyal yang diterima ESP8266. Nilai yang lebih mendekati 0 dBm menunjukkan bahwa sinyal yang diterima lebih kuat.

Sebaliknya, apabila nilai RSSI semakin negatif, kualitas sinyal semakin menurun. Sinyal yang lemah dapat meningkatkan kemungkinan terjadinya gangguan komunikasi, penurunan kualitas transfer data, atau koneksi menjadi kurang stabil.

Oleh karena itu, nilai RSSI dapat digunakan sebagai salah satu parameter untuk mengevaluasi kualitas koneksi perangkat IoT.

---

## 11.3 Bagaimana ESP8266 membedakan peran Station dan Access Point?

Peran ESP8266 ditentukan melalui konfigurasi mode WiFi pada program.

Untuk mode Station digunakan:

```cpp
WiFi.mode(WIFI_STA);
```

Pada mode tersebut ESP8266 bertindak sebagai client.

Untuk mode Access Point digunakan:

```cpp
WiFi.mode(WIFI_AP);
```

Pada mode tersebut ESP8266 menyediakan jaringan sendiri.

Sedangkan untuk menjalankan keduanya secara bersamaan digunakan:

```cpp
WiFi.mode(WIFI_AP_STA);
```

Dengan demikian, pemilihan mode pada program menentukan peran interface WiFi ESP8266.

---

## 11.4 Bagaimana AP+STA dimanfaatkan pada sistem IoT?

Mode AP+STA dapat diterapkan dalam proses **provisioning perangkat IoT**.

Sebagai contoh, ketika perangkat pertama kali digunakan, ESP8266 dapat membuat Access Point sementara. Pengguna kemudian menghubungkan smartphone ke jaringan tersebut untuk memberikan konfigurasi SSID dan password WiFi yang akan digunakan.

Setelah informasi jaringan diperoleh, ESP8266 dapat menggunakan fungsi Station untuk terhubung ke jaringan utama.

Gambaran prosesnya:

```text
Smartphone
    |
    | Terhubung ke AP
    v
+-----------+
|  ESP8266  |
|  AP + STA |
+-----------+
    |
    | Mode STA
    v
WiFi Utama
    |
    v
Jaringan / Internet
```

Penggunaan cara tersebut membuat konfigurasi jaringan perangkat IoT menjadi lebih fleksibel karena pengguna tidak harus mengubah source code setiap kali jaringan WiFi berganti.

---

# 12. Analisis Hasil Praktikum

Berdasarkan percobaan yang dilakukan, ESP8266 dapat menjalankan fungsi jaringan sesuai dengan mode yang dipilih pada program.

Pada mode **Station**, ESP8266 dapat bertindak sebagai client dan melakukan koneksi ke jaringan WiFi. Status koneksi dapat diperiksa menggunakan `WiFi.status()` sehingga program dapat mengetahui apakah perangkat masih terhubung atau mengalami gangguan jaringan.

Pada mode **Access Point**, ESP8266 berhasil menyediakan jaringan `ESP32_AccessPoint` dengan IP Address `192.168.4.1`. Jumlah client dapat dipantau menggunakan `WiFi.softAPgetStationNum()`. Hasil pengamatan menunjukkan jumlah perangkat dapat berubah dari 0 menjadi 1 dan mencapai 2 client sesuai jumlah perangkat yang bergabung ke Access Point.

Modifikasi **auto reconnect** memberikan kemampuan kepada ESP8266 untuk mencoba memulihkan koneksi secara otomatis ketika WiFi terputus. Sementara itu, penggunaan **AP+STA** memungkinkan ESP8266 berfungsi sebagai client dan penyedia jaringan pada waktu yang sama.

---

# 13. Kendala Selama Praktikum

Beberapa kendala dapat muncul ketika melakukan konfigurasi jaringan pada ESP8266, terutama ketika jaringan WiFi atau hotspot yang digunakan tidak dapat dideteksi atau ketika proses koneksi tidak berhasil.

Pada kondisi tersebut dilakukan pemeriksaan terhadap SSID, password, pengaturan hotspot, serta kemampuan ESP8266 dalam mendeteksi jaringan yang digunakan.

Selain itu, library pada program juga harus disesuaikan dengan board. Karena praktikum menggunakan **ESP8266**, library yang digunakan adalah:

```cpp
#include <ESP8266WiFi.h>
```

dan bukan:

```cpp
#include <WiFi.h>
```

yang digunakan pada program ESP32.

---

# 14. Kesimpulan

Berdasarkan Praktikum Modul 2, ESP8266 dapat dikonfigurasi menggunakan beberapa mode operasi WiFi sesuai kebutuhan sistem IoT.

Mode **Station (STA)** memungkinkan ESP8266 bergabung dengan jaringan WiFi sebagai client. Mode **Access Point (AP)** memungkinkan ESP8266 menyediakan jaringan WiFi sendiri bagi perangkat lain. Sedangkan mode **AP+STA** memungkinkan kedua fungsi tersebut dijalankan secara bersamaan.

Pada pengujian Access Point, jaringan `ESP32_AccessPoint` berhasil diaktifkan dengan IP Address `192.168.4.1`. Client dapat bergabung ke jaringan dan jumlah perangkat yang terhubung dapat dipantau melalui Serial Monitor hingga teramati sebanyak 2 perangkat.

Penggunaan RSSI membantu mengamati kekuatan sinyal WiFi, sedangkan fitur auto reconnect dapat meningkatkan keandalan koneksi karena ESP8266 dapat mencoba terhubung kembali apabila jaringan terputus.

Secara keseluruhan, percobaan menunjukkan bahwa konfigurasi mode WiFi dan pemantauan status jaringan merupakan bagian penting dalam membangun komunikasi yang lebih fleksibel pada perangkat IoT berbasis ESP8266.

---

## Dokumentasi

Dokumentasi berupa foto dan video pelaksanaan praktikum dapat ditambahkan pada folder:

```text
Dokumentasi Praktikum/
```

Source code setiap percobaan dapat disimpan pada folder:

```text
Source Code/
```

---

**Praktikan:** Farhan Nur Sahid  
**NIM:** H1H024057  
**Program Studi:** Teknik Komputer  
**Universitas Jenderal Soedirman**
