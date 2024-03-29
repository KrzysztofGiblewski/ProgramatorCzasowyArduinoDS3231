#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <DS3231.h>
// 4 lampy
#define PRZEKAZNIK 13
#define PRZEKAZNIK 10
#define PRZEKAZNIK 9
#define PRZEKAZNIK 8
//dla co2
#define PRZEKAZNIK 7

int minuty;
int godziny;
int sekundy;

int wlonczPier = 10;  // godzina włączenia pierwszej lampy
int wylonczPier = 20; //godzina wyłączenia lampy

int wlonczDrug = 11; //godzina włączenia drugiej lampy
int wylonczDrug = 19;

int wlonczTrze = 12; //godzina włączenia trzeciej lampy
int wylonczTrze = 18;

int wlonczCzwa = 13;  //godzina włączenia czwartej lampy
int wylonczCzwa = 17; //godzina wyłączenia czwartej lampy

int wlonczKarm1 = 11;
int wlonczKarm2 = 15;

int serwopozycja = 0;
int serwowloncz = 16;

int coIleMin = 3;     // co ile min co2
int przezSekund = 30; //przez ile sekund co2

int ekrany = 3; //zeby w zaleznosci od liczby wybrać różne ekrany

int ilosc = 3; //ile karmy podac ile razy posypać

Servo myservo; // create servo object to control a servo
DS3231 Clock;
RTClib RTC;

//modul zegara koniecznie 3.3V bo inaczej jest godzina 165-165-85

//VSS-  VDD+   VO-do potencjometru     RS do pinu 12    RW-   (en to E)E do 11 itd    A+     K-
const int rs = 12,
          en = 11,
          d4 = 5,
          d5 = 4,
          d6 = 3,
          d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
   lcd.begin(16, 2);
   Serial.begin(9600);

   // tu podpinam przekazniki do oświetlenia zeby mieć pewność żę przekażnik w dobrym zboczu :)
   pinMode(13, OUTPUT); //pierwsza lampa
   digitalWrite(13, LOW);
   delay(1000);
   pinMode(13, OUTPUT); //pierwsza lampa
   digitalWrite(13, HIGH);
   delay(1000);

   pinMode(10, OUTPUT); //druga lampa
   digitalWrite(10, LOW);
   delay(1000);
   pinMode(10, OUTPUT); //druga lampa
   digitalWrite(10, HIGH);
   delay(1000);

   pinMode(8, OUTPUT); //trzecia
   digitalWrite(8, LOW);
   delay(1000);
   pinMode(8, OUTPUT); //trzecia
   digitalWrite(8, HIGH);
   delay(1000);

   pinMode(9, OUTPUT); //czwarta lampa
   digitalWrite(9, LOW);
   delay(1000);
   pinMode(9, OUTPUT); //czwarta lampa
   digitalWrite(9, HIGH);
   delay(1000);

   pinMode(7, OUTPUT); //CO2
   digitalWrite(7, LOW);
   delay(1000);
   pinMode(7, OUTPUT); //CO2
   digitalWrite(7, HIGH);
   delay(1000);

   pinMode(14, INPUT_PULLUP); //przycisk zmiany ekranu

   //  pinMode(LED_BUILTIN, OUTPUT);               //wbudowana dioda led na plytce

   myservo.attach(6); //  pin 6 sygnal dla serwa servo
   delay(1000);
   myservo.write(0);  //  ustawiam serwo na kąt 0 stopni
   delay(1000);
   myservo.write(90);
   delay(1000);
   myservo.write(0);
   delay(1000);

   ////////////*********************************************************************************///////////
   // ******* to po kolei wybraną linie odkomentować żeby ustawić zegar a potem zakomentować,  prymitywne ale dziala ;)
   //*********koniecznie podłączyć 3,3V bo przy 5V wariuje
   //ROK
   //  Clock.setYear(18);
   //MIESIAC
   //   Clock.setMonth(6);
   //DZIEN
   //   Clock.setDate(11);
   //DZIEN TYGODNIA NP 1 TO PONIEDZIAŁEK
   //   Clock.setDoW(2);
   //GODZINA
   //   Clock.setHour(12);
   //MINUTY
   //   Clock.setMinute(50);
   //SEKUNDY
   //   Clock.setSecond(15);
   //////////////*********************************************************************************////////////
}

void loop()
{
   DateTime now = RTC.now();
   godziny = now.hour();
   minuty = now.minute();
   sekundy = now.second();

   if (digitalRead(14) == LOW)
   {
      delay(50);
      zmienEkran();
   }

   delay(1000);
   {
      //wyświetla na monitorze portu szeregowego
      Serial.print("godzina: ");
      Serial.print(godziny);
      Serial.print(" : ");
      Serial.print(minuty);
      Serial.print(" : ");
      Serial.println(sekundy);

      //tu pulapki czasowe dla aktywacji przekaznikow
      if (godziny >= wlonczPier)
      {
         digitalWrite(13, LOW); //pierwsza lampa ON
      }
      if (godziny >= wylonczPier)
      {
         digitalWrite(13, HIGH); //pierwsza lampa OFF
      }
      if (godziny >= wlonczDrug)
      {
         digitalWrite(10, LOW); //druga ON
      }
      if (godziny >= wylonczDrug)
      {
         digitalWrite(10, HIGH); //druga OFF
      }
      if (godziny >= wlonczTrze)
      { //trzecia ON
         digitalWrite(9, LOW);
      }
      if (godziny >= wylonczTrze)
      { //trzecia OFF
         digitalWrite(9, HIGH);
      }
      if (godziny >= wlonczCzwa)
      { //Czwarta ON
         digitalWrite(8, LOW);
      }
      if (godziny >= wylonczCzwa)
      { //czwarta OFF
         digitalWrite(8, HIGH);
      }

      if (godziny > wlonczPier && minuty % coIleMin == 0)
      {
         digitalWrite(7, LOW); //CO2 ON co iles minut
      }
      if (godziny > wlonczPier && minuty % coIleMin != 0 || sekundy > przezSekund)
      { //CO2 OFF co iles minut przez iles sekund
         digitalWrite(7, HIGH);
      }

      if (godziny > wylonczDrug)
      {
         digitalWrite(7, HIGH); //CO2 OFF na noc razem z drugim swiatlem na wszelki wypadek
      }

      // pułapka dla serwa karmienie
      if (godziny == wlonczKarm1 && minuty == 0 && sekundy == 0)
         karmRyby();

      if (godziny == wlonczKarm2 && minuty == 0 && sekundy == 0)
         karmRyby();

      lcd.setCursor(0, 0);
      if (godziny < 10) //jak godziny od 0 do 9 to trzeba zero dopisac zeby ładnie było
         lcd.print(0);
      lcd.print(godziny);
      lcd.print(":");
      if (minuty < 10) //jak minuty od 0 do 9 to trzeba zero dopisac
         lcd.print(0);
      lcd.print(minuty);
      lcd.print(":");
      if (sekundy < 10) //jak sekundy od 0 do 9 to trzeba zero dopisac
         lcd.print(0);
      lcd.print(sekundy);

      switch (ekrany)
      {
      case 0:
      {
         lcd.print(" Karma ");
         lcd.setCursor(0, 1);
         lcd.print("o ");
         lcd.print(wlonczKarm1);
         lcd.print(" i o ");
         lcd.print(wlonczKarm2);
         lcd.print(" * ");
         lcd.print(ilosc);
         lcd.print("  ");
         break;
      }
      case 1:
      {
         lcd.print(" La ON ");
         lcd.setCursor(0, 1);
         lcd.print(wlonczPier);
         lcd.print(" ");
         lcd.print(wlonczDrug);
         lcd.print(" ");
         lcd.print(wlonczTrze);
         lcd.print(" ");
         lcd.print(wlonczCzwa);
         lcd.print(" godz");
         break;
      }
      case 2:
      {
         lcd.print(" La OFF");
         lcd.setCursor(0, 1);
         lcd.print(wylonczPier);
         lcd.print(" ");
         lcd.print(wylonczDrug);
         lcd.print(" ");
         lcd.print(wylonczTrze);
         lcd.print(" ");
         lcd.print(wylonczCzwa);
         lcd.print(" godz");
         break;
      }
      case 3:
      {
         lcd.print("   CO2 ");
         lcd.setCursor(0, 1);
         lcd.print(" co2 co ");
         lcd.print(coIleMin);
         lcd.print(" minuty ");
         break;
      }
      }
   }
}

void zmienEkran()
{
   ekrany++;
   if (ekrany > 3)
      ekrany = 0;
}

void karmRyby()
{
   // i ilosc to ilosc przechylen serwa sypniec karmy
   for (int i = 0; i < ilosc; i++)
   {
      myservo.write(180);
      delay(1000);
      myservo.write(0);
      delay(1000);
   }
}
