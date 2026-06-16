/* PROJE: Gelişmiş Bluetooth Kontrollü Araç (DHT11 & V Komutu Entegreli) */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h> // DHT11 kütüphanesini ekledik

LiquidCrystal_I2C lcd(0x27, 16, 2); 

#define DHTPIN 10       // DHT11 sensörünün bağlandığı pin
#define DHTTYPE DHT11   // Sensör tipi DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- Pinler ---
const int sol_ileri = 5, sol_geri = 4, sag_ileri = 7, sag_geri = 6;
const int buzPin = 2, ledPin = 13, arkaLedPin = 9; // Flaşör orijinal isteğin üzerine 9. pine alındı
const int ekstraLedPin = 11; 
const int ldrPin = A0; // ButonPin iptal edildi

// --- Değişkenler ---
int valSpeed = 255;    
bool flasorAktif = false;
bool ekstraFlasorModu = false; 
unsigned long eskiZaman = 0;
unsigned long ekstraLedEskiZaman = 0; 
const long aralik = 150; 

unsigned long lcdEskiZaman = 0;
unsigned long motivasyonZamani = 0;
unsigned long ekranDonguZamani = 0;
bool motivasyonAktif = false;
int mesajIndex = 0;
int kaymaPos = 0;
unsigned long kaymaZamani = 0;

// Ekran geçişleri için yeni durum değişkeni
int ekranDurumu = 0; 

const char* mesajlar[] = {
   "Pes Etme! ", "Basarabilirsin ", 
  "Gelecek Sensin ", "Gulumse :) ", "Cok Iyisin ", "NHWS Durdurulamaz "
};
int toplamMesaj = 7;

byte gulenYuz[8] = {
  0b00000, 0b01010, 0b01010, 0b00000, 0b10001, 0b01110, 0b00000, 0b00000
};

byte kalp[8] = {
  0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000
};

void setup() {
    Serial.begin(38400);
    lcd.init(); lcd.backlight();
    dht.begin(); // DHT sensörünü başlattık
    
    lcd.createChar(0, gulenYuz); 
    lcd.createChar(1, kalp); 

    pinMode(sol_ileri, OUTPUT); pinMode(sol_geri, OUTPUT);
    pinMode(sag_ileri, OUTPUT); pinMode(sag_geri, OUTPUT);
    pinMode(buzPin, OUTPUT); pinMode(ledPin, OUTPUT); 
    pinMode(arkaLedPin, OUTPUT);
    pinMode(ekstraLedPin, OUTPUT); 

    lcd.setCursor(0,0); lcd.print("Sistem Hazir "); lcd.write(0);
    delay(1500); lcd.clear();
    ekranDonguZamani = millis();
}

void loop() {
    unsigned long suankiZaman = millis();

    // --- EKRAN YÖNETİMİ (Saniyeler İçinde Akıcı Dönüşüm) ---
    if (motivasyonAktif) {
        if (suankiZaman - motivasyonZamani >= 10000) { // 10 saniye sonra motivasyondan çık
            motivasyonAktif = false;
            lcd.clear();
            ekranDonguZamani = suankiZaman; 
            ekranDurumu = 0;
        } else {
            scrollMesaj("MOTIVASYON:", mesajlar[mesajIndex]);
        }
    } 
    else {
        // Her 3 saniyede bir (3000 ms) ekran durumunu değiştirir
        if (suankiZaman - ekranDonguZamani >= 3000) {
            ekranDonguZamani = suankiZaman;
            ekranDurumu = (ekranDurumu + 1) % 3; // 3 farklı ekran modu arasında döner
            lcd.clear();
            kaymaPos = 0; // Kayan yazı sayacını sıfırla
        }

        if (ekranDurumu == 0) {
            // 1. DÜZEY: Sistem Aktif Kayan Yazı
            scrollMesaj("Sistem Aktif", "--- NH+NWS :) <3 PROJE --- "); 
        } 
        else if (ekranDurumu == 1) {
            // 2. DÜZEY: LDR Gece - Gündüz Durumu
            normalEkran();
        } 
        else if (ekranDurumu == 2) {
            // 3. DÜZEY: DHT11 Sıcaklık ve Nem Durumu
            dhtEkran();
        }
    }

    // --- ARKA LED FLAŞÖR (Pin 9) ---
    if (flasorAktif && (suankiZaman - eskiZaman >= aralik)) {
        eskiZaman = suankiZaman;
        digitalWrite(arkaLedPin, !digitalRead(arkaLedPin));
    }

    // --- EKSTRA LED FLAŞÖR (Pin 11) ---
    if (ekstraFlasorModu && (suankiZaman - ekstraLedEskiZaman >= aralik)) {
        ekstraLedEskiZaman = suankiZaman;
        digitalWrite(ekstraLedPin, !digitalRead(ekstraLedPin));
    }
    
    bluetoothKontrol();
}

void scrollMesaj(const char* ustSatir, const char* altSatir) {
    if (millis() - kaymaZamani > 300) {
        kaymaZamani = millis();
        lcd.setCursor(0, 0); lcd.print(ustSatir);
        
        String temp = String(altSatir);
        if (temp.indexOf("<3") != -1) {
          temp.replace("<3", "\1"); // Kalp karakterini yerleştirir
        }

        int len = temp.length();
        String displayStr = "";
        for (int i = 0; i < 16; i++) {
            char c = temp[(kaymaPos + i) % len];
            displayStr += c;
        }
        
        lcd.setCursor(0, 1);
        lcd.print(displayStr);
        lcd.setCursor(15, 0); lcd.write(0); // Gülen yüzü köşeye basar
        
        kaymaPos++;
        if (kaymaPos >= len) kaymaPos = 0;
    }
}

void normalEkran() {
    int ldr = analogRead(ldrPin);
    
    lcd.setCursor(0, 0);
    lcd.print("NH+NWS PROJE   ");
    
    lcd.setCursor(0, 1);
    if (ldr < 400) {
        lcd.print("DRM: GECE      ");
    } else {
        lcd.print("DRM: GUNDUZ    ");
    }
    
    lcd.setCursor(15, 0); lcd.write(0); 
}

void dhtEkran() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    lcd.setCursor(0, 0);
    lcd.print("Sicaklik: ");
    if (isnan(t)) {
        lcd.print("--C  ");
    } else {
        lcd.print((int)t);
        lcd.print("C   ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Nem: %");
    if (isnan(h)) {
        lcd.print("--   ");
    } else {
        lcd.print((int)h);
        lcd.print("    ");
    }
    lcd.setCursor(15, 0); lcd.write(0);
}

void bluetoothKontrol() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch(command) {
            case 'F': ileri(); break;
            case 'B': geri(); break;
            case 'R': sag(); break;
            case 'L': sol(); break;
            case 'S': dur(); break;
            case 'Y': digitalWrite(buzPin, HIGH); delay(200); digitalWrite(buzPin, LOW); break;
            case 'U': digitalWrite(ledPin, HIGH); break;
            case 'u': digitalWrite(ledPin, LOW); break;
            case 'W': ekstraFlasorModu = true; break; 
            case 'w': ekstraFlasorModu = false; digitalWrite(ekstraLedPin, LOW); break;
            case 'X': flasorAktif = !flasorAktif; 
                      if (!flasorAktif) digitalWrite(arkaLedPin, LOW); 
                      break;
            case 'V': // Uygulamadan 'V' harfi geldiğinde motivasyon mesajlarını tetikle
                      mesajIndex = (mesajIndex + 1) % toplamMesaj; 
                      kaymaPos = 0; 
                      motivasyonAktif = true;
                      motivasyonZamani = millis(); 
                      lcd.clear();
                      break;
            case '1': valSpeed = 100; break;
            case '2': valSpeed = 150; break;
            case '3': valSpeed = 200; break;
            case '4': valSpeed = 255; break;
        }
    }
}

void ileri() { analogWrite(sol_ileri, valSpeed); digitalWrite(sol_geri, LOW); analogWrite(sag_ileri, valSpeed); digitalWrite(sag_geri, LOW); }
void geri() { digitalWrite(sol_ileri, LOW); analogWrite(sol_geri, valSpeed); digitalWrite(sag_ileri, LOW); analogWrite(sag_geri, valSpeed); }
void sag() { analogWrite(sol_ileri, valSpeed); digitalWrite(sol_geri, LOW); digitalWrite(sag_ileri, LOW); analogWrite(sag_geri, valSpeed); }
void sol() { digitalWrite(sol_ileri, LOW); analogWrite(sol_geri, valSpeed); analogWrite(sag_ileri, valSpeed); digitalWrite(sag_geri, LOW); }
void dur() { digitalWrite(sol_ileri, LOW); digitalWrite(sol_geri, LOW); digitalWrite(sag_ileri, LOW); digitalWrite(sag_geri, LOW); }