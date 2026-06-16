# 🚗 Gelişmiş Bluetooth Kontrollü Araç (NH+NWS Projesi)

Bu proje; Arduino tabanlı, Bluetooth (HC-05/HC-06) üzerinden akıllı telefon uygulaması ile kontrol edilebilen, üzerinde anlık veri takibi için LCD ekran, sıcaklık/nem sensörü (DHT11) ve ışık sensörü (LDR) barındıran gelişmiş bir otonom/uzaktan kumandalı araç projesidir.

Projenin en ayırt edici özelliği, asenkron (`millis()`) kod mimarisi ve özel olarak entegre edilmiş **"Motivasyon Modu (V Komutu)"**dur.

---

**Hazırlayan:** Namık Hasan

## 🚀 Öne Çıkan Özellikler

* **Asenkron Ekran Yönetimi:** Kodda `delay()` kullanılmadığı için araç hareket ederken aynı anda LCD ekrandaki yazılar akıcı bir şekilde kayar ve her 3 saniyede bir ekran modları arasında (Sistem Bilgisi -> LDR Durumu -> DHT11 Verileri) otomatik geçiş yapılır.
* **Motivasyon Modu (V Komutu):** Bluetooth üzerinden 'V' komutu geldiğinde, ekrandaki döngü durur ve 10 saniye boyunca ekranda rastgele/sıralı motivasyon mesajları ve özel karakterler (`❤️`, `:)`) belirir.
* **Çift Flaşör Sistemi:** Arka LED (Pin 9) ve Ekstra LED (Pin 11) bağımsız zaman aralıklarıyla flaşör modunda çalışabilir.
* **Akıllı Işık Algılama:** LDR sensörü ile ortam ışığı ölçülerek LCD ekranda anlık olarak "GECE" veya "GUNDUZ" durumu raporlanır.

---

## 🛠️ Kullanılan Donanımlar

* **Mikrokontrolcü:** Arduino (UNO / MEGA)
* **Ekran:** 16x2 I2C LCD Ekran
* **Sensörler:** DHT11 Sıcaklık ve Nem Sensörü, LDR (Işık Sensörü)
* **Haberleşme:** HC-05 veya HC-06 Bluetooth Modülü
* **Sürücü & Motorlar:** DC Motorlar ve Motor Sürücü (L298N vb.)
* **Diğer:** Buzzer, Flaşör LED'ler, Ön Far LED'i

---

## 📌 Bağlantı Şeması (Pinout)

| Bileşen / Pin Adı | Arduino Pin Numarası | Açıklama |
| :--- | :--- | :--- |
| **Sol Motor İleri** | Pin 5 (PWM) | Sol motor ileri hareket kontrolü |
| **Sol Motor Geri** | Pin 4 | Sol motor geri hareket kontrolü |
| **Sağ Motor İleri** | Pin 7 (PWM) | Sağ motor ileri hareket kontrolü |
| **Sağ Motor Geri** | Pin 6 | Sağ motor geri hareket kontrolü |
| **DHT11 Data** | Pin 10 | Sıcaklık ve Nem verisi girişi |
| **LDR Sensör** | Analog A0 | Ortam ışık seviyesi girişi |
| **Buzzer** | Pin 2 | Korna / Uyarı ses çıkışı |
| **Ön Far LED** | Pin 13 | Açılıp kapatılabilen ön aydınlatma |
| **Arka Flaşör LED** | Pin 9 | Sinyal / Çakar LED çıkışı |
| **Ekstra Flaşör LED**| Pin 11 | İkinci çakar LED çıkışı |
| **I2C LCD** | SDA (A4) / SCL (A5) | Ekran haberleşme hatları |
| **Bluetooth RX/TX** | RX -> TX / TX -> RX | Seri haberleşme (38400 Baud) |

---

## 📲 Bluetooth Komut Seti

Araç, bir Bluetooth terminali veya özelleştirilmiş bir robot kontrol uygulaması üzerinden aşağıdaki harf komutlarıyla yönetilir:

* **F** : İleri Git
* **B** : Geri Git
* **L** : Sola Dön
* **R** : Sağa Dön
* **S** : Dur
* **Y** : Korna Çal (Buzzer - 200ms)
* **U / u** : Ön Farları Aç / Kapat
* **X** : Arka Flaşörü (Pin 9) Aktif Et / Kapat
* **W / w** : Ekstra Flaşörü (Pin 11) Aktif Et / Kapat
* **V** : **Motivasyon Modunu Tetikle!** (10 Saniye boyunca özel mesajlar döner)
* **1, 2, 3, 4** : Hız Ayarı (1: %40 Hız ... 4: %100 Tam Hız)

---

## 💻 Kurulum ve Çalıştırma

1. Bu depoyu (repository) bilgisayarınıza indirin veya klonlayın.
2. `uzaktankumadali_araba.ino` dosyasını Arduino IDE ile açın.
3. `LiquidCrystal_I2C` ve `DHT` kütüphanelerinin IDE'nizde yüklü olduğundan emin olun.
4. Kart tipinizi ve Portunuzu seçip kodu yükleyin.
5. *Not: Kod yüklerken Bluetooth modülünün RX/TX pinlerini sökmeyi unutmayın (seri port çakışmasını önlemek için).*

---

## 🎯 Gelecek Geliştirmeler & Yol Haritası

* [ ] Fritzing veya Proteus kullanılarak hazırlanacak olan detaylı **Devre Şeması (Schematic Diagram)** sonraki aşamada projeye eklenecektir.
* [ ] Şasi tasarımı ve 3D modelleme detayları paylaşılacaktır.

---
✨ **NH** tarafından sevgiyle ve mühendislikle geliştirilmiştir.
