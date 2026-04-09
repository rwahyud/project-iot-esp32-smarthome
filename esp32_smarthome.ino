#include <WiFi.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDR_PIN 34
#define MQ2_PIN 35
#define RELAY_LAMPU 2
#define RELAY_KIPAS 15
#define BUZZER 12

const int AMBANG_GELAP = 1500;
const float AMBANG_PANAS = 31.0;
const int AMBANG_GAS = 800;

// Variabel status manual
bool modeManualLampu = false;
bool modeManualKipas = false;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Fungsi kontrol manual Lampu dari HP
BLYNK_WRITE(V5) {
  int value = param.asInt();
  modeManualLampu = (value == 1);
  digitalWrite(RELAY_LAMPU, value ? LOW : HIGH); // ON jika 1, OFF jika 0
  Blynk.virtualWrite(V3, value); // Update indikator status
}

// Fungsi kontrol manual Kipas dari HP
BLYNK_WRITE(V6) {
  int value = param.asInt();
  modeManualKipas = (value == 1);
  digitalWrite(RELAY_KIPAS, value ? LOW : HIGH);
  Blynk.virtualWrite(V4, value); // Update indikator status
}

void updateSistem() {
  float h = dht.readTemperature();
  int ldr = analogRead(LDR_PIN);
  int gas = analogRead(MQ2_PIN);

  if (isnan(h)) return;

  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, ldr);
  Blynk.virtualWrite(V2, gas);

  // Logika Lampu: Hanya otomatis jika TIDAK sedang dikontrol manual
  if (!modeManualLampu) {
    if (ldr > AMBANG_GELAP) {
      digitalWrite(RELAY_LAMPU, LOW);
      Blynk.virtualWrite(V3, 1);
    } else {
      digitalWrite(RELAY_LAMPU, HIGH);
      Blynk.virtualWrite(V3, 0);
    }
  }

  // Logika Kipas: Hanya otomatis jika TIDAK sedang dikontrol manual
  if (!modeManualKipas) {
    if (h > AMBANG_PANAS) {
      digitalWrite(RELAY_KIPAS, LOW);
      Blynk.virtualWrite(V4, 1);
    } else {
      digitalWrite(RELAY_KIPAS, HIGH);
      Blynk.virtualWrite(V4, 0);
    }
  }

  if (gas > AMBANG_GAS) tone(BUZZER, 1000);
  else noTone(BUZZER);

  Serial.printf("Suhu: %.2fC | LDR: %d | Gas: %d | Manual: L:%d K:%d\n", h, ldr, gas, modeManualLampu, modeManualKipas);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_LAMPU, OUTPUT);
  pinMode(RELAY_KIPAS, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RELAY_LAMPU, HIGH);
  digitalWrite(RELAY_KIPAS, HIGH);
  dht.begin();

  WiFiManager wm;
  if (wm.autoConnect("SmartHome-Setup")) {
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect();
  }
  timer.setInterval(2000L, updateSistem);
}

void loop() {
  Blynk.run();
  timer.run();
}
