#include <math.h>  // Fungsi matematika sin() untuk simulasi suhu

// Modul 5 - Percobaan 5A - Kelompok 4
// Nama: Farhan Nur Sahid | NIM: H1H024057
// Board: ESP8266 NodeMCU 1.0 (ESP-12E Module)
// Data suhu mock (tanpa DHT fisik). Serial Monitor: 115200 baud.

unsigned long jumlahBaca = 0; // Menghitung banyak pembacaan suhu

// Simulasi pembacaan sensor suhu
float bacaSuhuMock() {
  jumlahBaca++;
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);
  float noise = random(-5, 6) * 0.1;
  float suhu = suhuDasar + noise;

  if (jumlahBaca % 25 == 0) {
    suhu += 8.0; // Lonjakan untuk menguji respons filter
  }
  return suhu;
}

const int N = 5;       // Banyak data dalam jendela Moving Average
float buffer[N];       // Buffer melingkar suhu terbaru
int indeks = 0;        // Posisi penulisan berikutnya
int jumlah = 0;        // Banyak data valid dalam buffer
float suhuMin = 1000.0;
float suhuMax = -1000.0;

float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N; // Kembali ke awal setelah posisi ke-5
  if (jumlah < N) jumlah++;

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  return total / jumlah;
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());
  Serial.println("Modul 5A - Kelompok 4 - Pengolahan Data Suhu Mock");
}

void loop() {
  float suhu = bacaSuhuMock();
  float suhuFilter = hitungMovingAverage(suhu);

  if (suhu < suhuMin) suhuMin = suhu;
  if (suhu > suhuMax) suhuMax = suhu;

  Serial.print("Pembacaan ke-");
  Serial.print(jumlahBaca);
  Serial.print(" | Mentah: ");
  Serial.print(suhu, 2);
  Serial.print(" | Filter: ");
  Serial.print(suhuFilter, 2);
  Serial.print(" | Min: ");
  Serial.print(suhuMin, 2);
  Serial.print(" | Max: ");
  Serial.println(suhuMax, 2);

  delay(2000); // Pembacaan tiap dua detik
}
