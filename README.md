# IoT Smart Home ESP32

Proyek ini mengontrol lampu dan kipas secara otomatis maupun manual via Blynk, serta memantau suhu, cahaya, dan gas.

## Fitur

- Monitoring suhu (DHT11), intensitas cahaya (LDR), dan kadar gas (MQ-2)
- Kontrol otomatis:
  - Lampu ON saat gelap
  - Kipas ON saat suhu tinggi
- Kontrol manual dari aplikasi Blynk:
  - Virtual Pin V5 untuk lampu
  - Virtual Pin V6 untuk kipas
- Alarm buzzer saat gas melewati ambang

## Mapping Pin ESP32

- DHT11 data: GPIO 4
- LDR analog: GPIO 34
- MQ-2 analog: GPIO 35
- Relay lampu: GPIO 2
- Relay kipas: GPIO 15
- Buzzer: GPIO 12

## Ambang Default di Kode

- `AMBANG_GELAP = 1500`
- `AMBANG_PANAS = 31.0`
- `AMBANG_GAS = 800`

Silakan sesuaikan nilai ambang berdasarkan kondisi sensor di tempat kamu.

## Library Arduino IDE

Install library berikut dari Library Manager:

- WiFi (bawaan ESP32 core)
- WiFiManager by tzapu
- Blynk
- DHT sensor library by Adafruit
- Adafruit Unified Sensor

## Konfigurasi Blynk

Project ini membaca token dari file lokal `secrets.h` (file ini tidak dipush ke GitHub).

1. Salin `secrets.example.h` menjadi `secrets.h`
2. Isi token kamu di `secrets.h`

```cpp
#define BLYNK_AUTH_TOKEN "ISI_TOKEN_KAMU"
```

Virtual pin yang dipakai:

- V0: Suhu
- V1: LDR
- V2: Gas
- V3: Status lampu
- V4: Status kipas
- V5: Tombol manual lampu
- V6: Tombol manual kipas

## Cara Upload

1. Buka `esp32_smarthome.ino` di Arduino IDE.
2. Pilih board ESP32 yang sesuai.
3. Pilih COM port.
4. Pastikan semua library sudah terpasang.
5. Pastikan file `secrets.h` sudah dibuat dan berisi token.
6. Upload sketch.
7. Saat pertama kali menyala, ESP32 akan membuat portal `SmartHome-Setup` (WiFiManager) untuk konfigurasi WiFi.

## Catatan

- Relay di kode ini aktif pada logika LOW.
- Jika pembacaan DHT gagal, data tidak akan dikirim pada siklus tersebut.
