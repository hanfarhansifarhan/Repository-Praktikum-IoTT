"""Modul 5 - Percobaan 5B dan modifikasi Moving Average 5.
Kelompok 4 | Farhan Nur Sahid | H1H024057
Menerima data MQTT, menyimpan CSV, menghitung statistik dan z-score,
serta membuat grafik suhu mentah dan Moving Average.

Install: python -m pip install paho-mqtt pandas matplotlib
Jalankan: python modul5_analisis_data.py
"""
import json
import math
import time

import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompok4/data"
JUMLAH_SAMPEL = 30

CSV_FILE = "data_suhu.csv"
GRAFIK_FILE = "grafik_suhu.png"
data = []


def on_connect(client, userdata, flags, reason_code, properties):
    print("Terhubung ke broker, kode:", reason_code)
    if reason_code == 0:
        client.subscribe(TOPIC)
        print("Subscribe topic:", TOPIC)
        print("Menunggu data suhu dari ESP8266...")


def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode("utf-8"))
        suhu = float(payload["suhu"])
        if not math.isfinite(suhu):
            raise ValueError("Suhu bukan angka valid")
    except (ValueError, KeyError, TypeError, UnicodeDecodeError):
        print("Data tidak valid, diabaikan.")
        return

    waktu = time.strftime("%Y-%m-%d %H:%M:%S")
    data.append({"waktu": waktu, "suhu": suhu})
    print(f"[{len(data)}/{JUMLAH_SAMPEL}] Waktu: {waktu} | Suhu: {suhu:.2f} °C")

    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()


def hitung_zscore(seri):
    """Z-score dari satu seri; bila simpangan baku 0, hasil 0."""
    std = seri.std()  # Standar deviasi sampel
    if pd.isna(std) or std == 0:
        return pd.Series(0.0, index=seri.index)
    return (seri - seri.mean()) / std


def main():
    client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    print("Menghubungkan ke broker MQTT...")

    try:
        client.connect(BROKER, PORT, keepalive=60)
        client.loop_forever()
    except KeyboardInterrupt:
        print("\nPengambilan data dihentikan oleh pengguna.")
        client.disconnect()
    except (OSError, RuntimeError) as error:
        print("Terjadi kesalahan koneksi MQTT:", error)

    if not data:
        print("Tidak ada data yang diterima. Cek broker dan topic.")
        return

    df = pd.DataFrame(data)
    df["zscore"] = hitung_zscore(df["suhu"])
    df["anomali"] = df["zscore"].abs() > 2

    # MODIFIKASI 5B: filter MA5 pada data sebelum dihitung z-score-nya
    df["moving_average_5"] = df["suhu"].rolling(window=5, min_periods=1).mean()
    df["zscore_ma5"] = hitung_zscore(df["moving_average_5"])
    df["anomali_ma5"] = df["zscore_ma5"].abs() > 2

    df.to_csv(CSV_FILE, index=False, float_format="%.4f")

    print("\n========= HASIL ANALISIS DATA =========")
    print(f"Jumlah data         : {len(df)}")
    print(f"Rata-rata           : {df['suhu'].mean():.2f} °C")
    print(f"Standar deviasi     : {df['suhu'].std():.2f} °C")
    print(f"Minimum             : {df['suhu'].min():.2f} °C")
    print(f"Maksimum            : {df['suhu'].max():.2f} °C")
    print(f"Anomali mentah      : {int(df['anomali'].sum())}")
    print(f"Anomali setelah MA5 : {int(df['anomali_ma5'].sum())}")
    print("Data tersimpan di", CSV_FILE)
    print("\n20 DATA PERTAMA UNTUK TABEL DAPENG 5.2:")
    print(df[["waktu", "suhu", "zscore", "anomali"]].head(20).to_string(index=False))

    plt.figure(figsize=(10, 4))
    plt.plot(df.index + 1, df["suhu"], marker="o", label="Suhu mentah")
    plt.plot(df.index + 1, df["moving_average_5"], label="Moving Average (5)")
    anom = df[df["anomali"]]
    plt.scatter(anom.index + 1, anom["suhu"], color="red", s=100,
                label="Anomali (|z| > 2)", zorder=3)
    plt.axhline(df["suhu"].mean(), color="gray", linestyle="--", label="Rata-rata")
    plt.xlabel("Urutan data")
    plt.ylabel("Suhu (°C)")
    plt.title("Modul 5 Kelompok 4 - Suhu dan Deteksi Anomali")
    plt.legend()
    plt.tight_layout()
    plt.savefig(GRAFIK_FILE, dpi=160)
    print("Grafik tersimpan di", GRAFIK_FILE)
    plt.show()


if __name__ == "__main__":
    main()
