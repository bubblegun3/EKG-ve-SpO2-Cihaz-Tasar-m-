#include <Wire.h>
#include "MAX30100_PulseOximeter.h"  //Max30100 kütüphanesi eklendi
#include "Adafruit_SSD1306.h"        //Ekranın kütüphanesi eklendi

// OLED ekran genişliği ve yüksekliği (piksel olarak)
int genislik = 128;  // OLED ekranın genişliği
int yukseklik = 32;  // OLED ekranın yüksekliği
int adres = 0x3C;    // OLED ekranın I2C adresi

Adafruit_SSD1306 ekran(genislik, yukseklik, &Wire);  // OLED ekran nesnesi oluşturulmakta

#define RAPORLAMA_SURESI_MS 1000  // Veri raporlama süresi milisaniye cinsinden belirlenmiştir

PulseOximeter pox;  // PulseOximeter nesnesi oluşturulmakta

uint32_t sonRaporZamani = 0;  // Son rapor zamanı için değişken tanımlanmıştır

void nabizTespitEdildi()
{
    Serial.println("Nabız!");  // Nabız tespit edildiğinde seri monitöre "Nabız!" yazılmakta
}

void setup()
{
    ekran.begin(SSD1306_SWITCHCAPVCC, adres);  // OLED ekran başlatılmakta
    ekran.clearDisplay();  // Ekran temizlenmekte
    ekran.display();  // Ekran güncellenmekte

    Serial.begin(9600);  // Seri iletişim başlatılmakta

    Serial.print("Nabız oksimetresini başlatıyor...");

    if (!pox.begin()) {  // Pulse oksimetre başlatılmakta
        Serial.println("BAŞARISIZ");  // Başarısız olursa "BAŞARISIZ" yazılmakta
        for(;;);  // Sonsuz döngüye girilmekte
    } else {
        Serial.println("BAŞARILI");  // Başarılı olursa "BAŞARILI" yazılmakta
    }

    pox.setOnBeatDetectedCallback(nabizTespitEdildi);  // Nabız tespiti callback fonksiyonu ayarlanmakta
}

void loop()
{
    pox.update();  // Pulse oksimetre güncellenmekte

    if (millis() - sonRaporZamani > RAPORLAMA_SURESI_MS) {  // Eğer raporlama süresi geçmişse
        int nabiz = (int)pox.getHeartRate();  // Nabız hızı alınmakta
        int spo2 = (int)pox.getSpO2();  // SpO2 değeri alınmakta

        Serial.print("Nabız:");  // Nabız hızı seri monitöre yazılmakta
        Serial.print(nabiz);
        Serial.print(" bpm / SpO2:");
        Serial.print(spo2);
        Serial.println("%");

        ekran.clearDisplay();  // Ekran temizlenmekte
        ekran.setTextSize(1);  // Metin boyutu ayarlanmakta
        ekran.setTextColor(SSD1306_WHITE);  // Metin rengi beyaz yapılmakta
        ekran.setCursor(0, 0);  // İmleç başlangıç konumuna ayarlanmakta
        ekran.print("Nabiz:");  // Nabız değeri ekrana yazılmakta
        ekran.print(nabiz);
        ekran.println(" bpm");
        ekran.print("SpO2:");  // SpO2 değeri ekrana yazılmakta
        ekran.print(spo2);
        ekran.println("%");
        ekran.display();  // Ekran güncellenmekte

        sonRaporZamani = millis();  // Son rapor zamanı güncellenmekte
    }
}
