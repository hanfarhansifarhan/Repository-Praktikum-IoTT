# Pertemuan 1 - Sensor dan Aktuator

**Nama:** Farhan Nur Sahid  
**NIM:** H1H024057  
**Program Studi:** Teknik Komputer  

---

## Penjelasan Code

Pada praktikum ini digunakan ESP32 sebagai mikrokontroler utama, sensor DHT11 sebagai sensor suhu dan kelembaban, serta modul relay sebagai aktuator. Sensor DHT11 dihubungkan ke GPIO 4, sedangkan relay dikendalikan melalui GPIO 26.

Program bekerja dengan membaca nilai suhu dan kelembaban dari sensor DHT11 secara berkala. Nilai hasil pembacaan kemudian ditampilkan pada Serial Monitor. Selain melakukan pembacaan sensor, program juga membandingkan nilai suhu dengan nilai ambang atau `threshold` sebesar 30°C.

Jika suhu yang terbaca lebih dari 30°C, ESP32 akan memberikan sinyal kepada relay sehingga status aktuator menjadi ON. Sebaliknya, jika suhu masih berada di bawah atau sama dengan 30°C, status aktuator menjadi OFF.

Code final yang digunakan pada saat praktikum adalah sebagai berikut:

```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAYPIN 26

DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C | ");

    Serial.print("Kelembaban: ");
    Serial.print(kelembaban);
    Serial.print(" % | ");

    if (suhu > suhuThreshold) {
      digitalWrite(RELAYPIN, HIGH);
      Serial.println("Aktuator: ON");
    } else {
      digitalWrite(RELAYPIN, LOW);
      Serial.println("Aktuator: OFF");
    }
  }

  delay(2000);
}
```

---

## Penjelasan Setiap Fungsi

Pada bagian awal program terdapat:

```cpp
#include <DHT.h>
```

Baris tersebut digunakan untuk memanggil library DHT yang diperlukan agar ESP32 dapat membaca data dari sensor DHT11.

Kemudian terdapat konfigurasi pin:

```cpp
#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAYPIN 26
```

`DHTPIN 4` menunjukkan bahwa pin data sensor DHT11 terhubung ke GPIO 4 pada ESP32. `DHTTYPE DHT11` digunakan untuk menentukan jenis sensor yang dipakai, sedangkan `RELAYPIN 26` menunjukkan bahwa pin kendali relay terhubung ke GPIO 26.

Selanjutnya objek sensor dibuat menggunakan:

```cpp
DHT dht(DHTPIN, DHTTYPE);
```

Objek tersebut digunakan untuk menjalankan fungsi-fungsi pembacaan sensor DHT11.

Nilai ambang suhu ditentukan menggunakan:

```cpp
const float suhuThreshold = 30.0;
```

Nilai `30.0` berarti batas suhu yang digunakan untuk menentukan kondisi aktuator adalah 30°C.

Fungsi:

```cpp
void setup()
```

dijalankan satu kali ketika ESP32 pertama kali dinyalakan atau di-reset.

Pada fungsi tersebut terdapat:

```cpp
Serial.begin(115200);
```

yang digunakan untuk memulai komunikasi serial dengan baud rate 115200 sehingga hasil pembacaan sensor dapat dilihat melalui Serial Monitor.

Kemudian:

```cpp
dht.begin();
```

digunakan untuk melakukan inisialisasi sensor DHT11.

Perintah:

```cpp
pinMode(RELAYPIN, OUTPUT);
```

digunakan untuk menetapkan GPIO 26 sebagai pin output yang mengendalikan relay.

Sedangkan:

```cpp
digitalWrite(RELAYPIN, LOW);
```

digunakan untuk menentukan kondisi awal aktuator dalam keadaan OFF.

Fungsi:

```cpp
void loop()
```

dijalankan secara terus-menerus selama ESP32 aktif.

Nilai suhu dibaca menggunakan:

```cpp
float suhu = dht.readTemperature();
```

sedangkan kelembaban dibaca menggunakan:

```cpp
float kelembaban = dht.readHumidity();
```

Data tersebut kemudian ditampilkan melalui Serial Monitor menggunakan beberapa perintah `Serial.print()`.

Pada bagian akhir terdapat:

```cpp
delay(2000);
```

yang memberikan jeda selama 2000 milidetik atau 2 detik sebelum sensor melakukan pembacaan berikutnya.

---

## Penjelasan Percabangan / Conditional

Program menggunakan percabangan untuk memeriksa keberhasilan pembacaan sensor dan menentukan kondisi aktuator.

Percabangan pertama adalah:

```cpp
if (isnan(suhu) || isnan(kelembaban)) {
  Serial.println("Gagal membaca data sensor!");
}
```

Percabangan ini digunakan untuk mengecek apakah data suhu atau kelembaban gagal terbaca. Fungsi `isnan()` digunakan untuk memeriksa apakah nilai hasil pembacaan bukan merupakan angka yang valid. Jika salah satu data tidak valid, Serial Monitor akan menampilkan pesan bahwa pembacaan sensor gagal.

Apabila pembacaan sensor berhasil, program akan masuk ke bagian `else` dan menampilkan nilai suhu serta kelembaban.

Percabangan berikutnya digunakan untuk mengendalikan aktuator:

```cpp
if (suhu > suhuThreshold) {
  digitalWrite(RELAYPIN, HIGH);
  Serial.println("Aktuator: ON");
} else {
  digitalWrite(RELAYPIN, LOW);
  Serial.println("Aktuator: OFF");
}
```

Apabila suhu yang terbaca lebih besar dari threshold 30°C, relay akan diberi kondisi `HIGH` dan status aktuator menjadi ON.

Sebaliknya, jika suhu berada di bawah atau sama dengan 30°C, relay diberi kondisi `LOW` dan status aktuator menjadi OFF.

Secara sederhana logika program adalah:

```text
Suhu > 30°C  → Aktuator ON
Suhu <= 30°C → Aktuator OFF
```

---

## Library atau Dependencies yang Diperlukan

Library yang digunakan pada praktikum ini adalah:

```cpp
#include <DHT.h>
```

Library tersebut digunakan untuk membantu komunikasi antara ESP32 dengan sensor DHT11 serta menyediakan fungsi pembacaan suhu dan kelembaban.

Library dapat dipasang melalui Arduino IDE dengan membuka **Library Manager**, kemudian mencari:

```text
DHT sensor library
```

Selain library tersebut, digunakan board ESP32 pada Arduino IDE agar program dapat dikompilasi dan diunggah ke mikrokontroler ESP32.

---

## Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

### 1. Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?

Threshold diperlukan sebagai nilai acuan untuk menentukan kapan aktuator harus aktif atau tidak aktif berdasarkan hasil pembacaan sensor. Dengan adanya threshold, ESP32 dapat mengambil keputusan secara otomatis tanpa harus dikendalikan secara manual.

Pada praktikum ini digunakan threshold sebesar 30°C. Ketika suhu berada di bawah atau sama dengan 30°C, aktuator berada dalam kondisi OFF. Jika suhu melebihi 30°C, aktuator berubah menjadi ON.

### 2. Apa yang terjadi apabila `suhuThreshold` diturunkan menjadi sangat rendah, misalnya 20.0?

Jika nilai `suhuThreshold` diturunkan menjadi 20°C, maka aktuator akan lebih sering berada dalam kondisi ON karena suhu lingkungan pada saat percobaan berada di atas 20°C.

Sebagai contoh:

```text
Suhu = 28°C
Threshold = 20°C

28°C > 20°C
Aktuator = ON
```

Dengan demikian, semakin rendah nilai threshold dibandingkan suhu lingkungan, semakin mudah aktuator diaktifkan.

### 3. Apa perbedaan kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histerisis (dua ambang batas)?

Kendali kondisi tunggal hanya menggunakan satu nilai threshold untuk menentukan kondisi ON atau OFF pada aktuator.

Sementara itu, kendali menggunakan histerisis memakai dua nilai threshold, yaitu nilai batas atas untuk mengaktifkan aktuator dan nilai batas bawah untuk mematikannya.

Penggunaan histerisis dapat membuat sistem lebih stabil karena dapat mencegah aktuator terlalu sering berubah antara kondisi ON dan OFF ketika nilai suhu berada di sekitar threshold.

---

## Penjelasan Singkat Mengenai Detail Percobaan

Pada percobaan pertama dilakukan pengujian sensor DHT11 pada beberapa kondisi lingkungan untuk mengetahui perubahan pembacaan suhu dan kelembaban.

Hasil pengamatan yang diperoleh adalah sebagai berikut:

| No. | Waktu | Kondisi Sensor | Suhu (°C) | Kelembaban (%) | Status Pembacaan |
|---|---|---|---:|---:|---|
| 1 | 00.00 | Normal | 28 | 57 | Valid |
| 2 | 00.02 | Didekati tangan | 28.10 | 82 | Valid |
| 3 | 00.04 | Didekati AC | 27.10 | 60 | Valid |
| 4 | 00.10 | Didekati api | 28.90 | 53 | Valid |

Pada kondisi normal, sensor membaca suhu 28°C dan kelembaban 57%. Ketika sensor didekatkan ke tangan, suhu berubah menjadi 28,10°C sedangkan kelembaban meningkat menjadi 82%.

Ketika sensor didekatkan ke AC, suhu menurun menjadi 27,10°C dengan kelembaban sebesar 60%. Saat sensor didekatkan ke api, suhu meningkat menjadi 28,90°C dan kelembaban menjadi 53%.

Berdasarkan hasil tersebut, sensor DHT11 dapat membaca perubahan suhu dan kelembaban pada setiap kondisi dengan status pembacaan valid.

Pada percobaan berikutnya dilakukan pengujian sistem kendali aktuator dengan nilai threshold sebesar 30°C.

| Waktu | Suhu (°C) | Threshold (°C) | Kondisi Suhu | Status Aktuator | Kondisi LED/Relay |
|---|---:|---:|---|---|---|
| 00.00 | 28 | 30 | < threshold | OFF | Menyala |
| 00.02 | 30.20 | 30 | > threshold | ON | Menyala |
| 00.04 | 35.60 | 30 | > threshold | ON | Menyala |

Pada suhu 28°C, suhu masih berada di bawah nilai threshold sehingga status aktuator adalah OFF. Ketika suhu meningkat menjadi 30,20°C dan 35,60°C, suhu sudah melebihi threshold sehingga status aktuator berubah menjadi ON.

Pada saat pelaksanaan praktikum terdapat kendala ketika melakukan perakitan. Informasi konfigurasi pin pada datasheet tidak sepenuhnya sesuai dengan sensor DHT11 yang tersedia di laboratorium sehingga sempat terjadi kesalahan pemasangan pin dan sensor tidak dapat membaca data dengan baik.

Kendala lainnya terjadi pada pengujian relay. Tegangan keluaran ESP32 sekitar 3,3 V, sedangkan relay diperkirakan membutuhkan tegangan sekitar 5 V agar mekanisme saklarnya dapat bekerja dengan baik. Akibatnya, indikator relay dapat menyala, tetapi mekanisme saklar tidak berpindah dan tidak terdengar bunyi klik.

---

## Skematik / Diagram Rangkaian

Berikut merupakan skematik atau diagram rangkaian yang digunakan pada praktikum.

<img width="468" height="430" alt="Percobaan 2" src="https://github.com/user-attachments/assets/65b5318a-aef4-4756-9085-e432c70f7929" />

<img width="468" height="395" alt="Percobaan 1" src="https://github.com/user-attachments/assets/982bfe57-543d-4eba-a694-d70152291f8e" />

---

## Foto Proses Praktikum / Perangkaian

Berikut merupakan dokumentasi proses perangkaian dan pelaksanaan praktikum.

<img width="720" height="1280" alt="Proses Perangkaian Percobaan 1" src="https://github.com/user-attachments/assets/675bcb0d-2d7c-41bb-be36-d2c089c07d70" />

<img width="720" height="1280" alt="Proses Perangkaian Percobaan 2" src="https://github.com/user-attachments/assets/61bfb432-71ef-40bf-bb92-e1afbf45819c" />

---

### Pengujian Sensor DHT11

Berikut merupakan dokumentasi pengujian sensor DHT11 pada saat praktikum.

<img width="720" height="1280" alt="Pengujian Sensor DHT11 1" src="https://github.com/user-attachments/assets/21fef781-bddd-4aea-860a-9a177bba5043" />

<img width="720" height="1280" alt="Pengujian Sensor DHT11 2" src="https://github.com/user-attachments/assets/05f548e2-259e-41fc-b392-13f3d4834dfc" />

---

### Hasil Serial Monitor

Berikut merupakan hasil pembacaan sensor dan status aktuator yang ditampilkan melalui Serial Monitor.

<img width="374" height="276" alt="Serial Monitor Percobaan 1" src="https://github.com/user-attachments/assets/6fa24dac-ef57-4408-a5db-e3753cccf350" />

<img width="510" height="289" alt="Serial Monitor Percobaan 2" src="https://github.com/user-attachments/assets/fcca4d16-056d-4299-b769-82180213630e" />

---

