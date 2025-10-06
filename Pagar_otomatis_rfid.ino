// Masukan Library Wemos D1 mini + Blynk + MFRC522 + Relay + Buzzer

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <MFRC522.h>

char auth[] = "BLYNK_AUTH_TOKEN"; // ganti token Blynk
char ssid[] = "YOUR_SSID";        // ganti SSID
char pass[] = "YOUR_PASS";        // ganti password

// Pin (Wemos D1 mini)
const uint8_t RST_PIN    = D1; // MFRC522 RST
const uint8_t SS_PIN     = D2; // MFRC522 SDA(SS)
const uint8_t RELAY_PIN  = D4; // Relay control
const uint8_t BUZZER_PIN = D0; // Buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN);

const unsigned long starterPulseMs = 2000;
unsigned long lastStart = 0;
const unsigned long cooldownMs = 10000;

// Kartu rfid atau kode uid yang diizinkan (format "AA:BB:CC:DD")
const char* allowedUIDs[] = { "DE:AD:BE:EF", "01:23:45:67" };

BlynkTimer timer;

String uidToString(MFRC522::Uid *uid) {
    String s;
    for (byte i = 0; i < uid->size; i++) {
        if (uid->uidByte[i] < 0x10) s += "0";
        s += String(uid->uidByte[i], HEX);
        if (i + 1 != uid->size) s += ":";
    }
    s.toUpperCase();
    return s;
}

bool isAuthorized(const String &u) {
    for (auto &a : allowedUIDs) if (u.equalsIgnoreCase(a)) return true;
    return false;
}

void beep(int times=1, int onMs=80) {
    for (int i=0;i<times;i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(onMs);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);
    }
}

void startMotor(const char *source) {
    if (millis() - lastStart < cooldownMs) {
        beep(2,50);
        Blynk.notify(String("Cooldown: ") + source);
        return;
    }
    lastStart = millis();
    digitalWrite(RELAY_PIN, HIGH);
    Blynk.notify(String("Starter ON: ") + source);
    beep(1,120);
    delay(starterPulseMs);
    digitalWrite(RELAY_PIN, LOW);
    Blynk.notify(String("Starter OFF: ") + source);
}

// Virtual button V1 untuk remote start via Blynk
BLYNK_WRITE(V1) {
    if (param.asInt() == 1) startMotor("Blynk");
}

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT); digitalWrite(RELAY_PIN, LOW);
    pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW);

    SPI.begin();
    mfrc522.PCD_Init();

    Blynk.begin(auth, ssid, pass);
    timer.setInterval(1000L, [](){ Blynk.virtualWrite(V0, digitalRead(RELAY_PIN)); });
    beep(2,60);
}

void loop() {
    Blynk.run();
    timer.run();

    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    String uid = uidToString(&mfrc522.uid);
    Serial.println("UID: " + uid);
    if (isAuthorized(uid)) startMotor("RFID"); else { beep(3,60); Blynk.notify(String("RFID tidak diizinkan: ") + uid); }
    mfrc522.PICC_HaltA();
}