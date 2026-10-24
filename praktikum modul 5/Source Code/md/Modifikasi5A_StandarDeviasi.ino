#include <math.h> // sin() dan sqrt()

// Modul 5 - Modifikasi Percobaan 5A - Kelompok 4
// Nama: Farhan Nur Sahid | NIM: H1H024057
// Menambahkan simpangan baku sampel dari buffer Moving Average 5 data.

unsigned long jumlahBaca = 0;

float bacaSuhuMock() {
  jumlahBaca++;
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);
  float noise = random(-5, 6) * 0.1;
  float suhu = suhuDasar + noise;
  if (jumlahBaca % 25 == 0) suhu += 8.0;
  return suhu;
}

const int N = 5;
float buffer[N];
int indeks = 0;
int jumlah = 0;
float suhuMin = 1000.0;
float suhuMax = -1000.0;

float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N;
  if (jumlah < N) jumlah++;

  float total = 0;
  for (int i = 0; i < jumlah; i++) total += buffer[i];
  return total / jumlah;
}

// MODIFIKASI: menghitung simpangan baku dari data yang ada di buffer
float hitungSimpanganBaku() {
  if (jumlah < 2) return 0.0; // Simpangan baku sampel butuh >= 2 data

  float total = 0.0;
  for (int i = 0; i < jumlah; i++) total += buffer[i];
  float rataRata = total / jumlah;

  float jumlahKuadratSelisih = 0.0;
  for (int i = 0; i < jumlah; i++) {
    float selisih = buffer[i] - rataRata;
    jumlahKuadratSelisih += selisih * selisih;
  }

  // Dibagi jumlah-1: standar deviasi sampel
  return sqrt(jumlahKuadratSelisih / (jumlah - 1));
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());
  Serial.println("Modul 5A - Kelompok 4 - MA5, Min, Max, Std");
}

void loop() {
  float suhu = bacaSuhuMock();
  float suhuFilter = hitungMovingAverage(suhu);
  float standarDeviasi = hitungSimpanganBaku();

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
  Serial.print(suhuMax, 2);
  Serial.print(" | Std: ");
  Serial.println(standarDeviasi, 2);

  delay(2000);
}
