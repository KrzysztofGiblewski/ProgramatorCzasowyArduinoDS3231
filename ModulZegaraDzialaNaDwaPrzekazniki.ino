#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <DS3231.h>

#define PRZEKAZNIK 13
#define PRZEKAZNIK 10
#define PRZEKAZNIK 8
#define PRZEKAZNIK 9

 int minuty ;
 int godziny ;
 int sekundy ;
 
 int wloncz =6; 
 int wyloncz =18;
 
 int wlonczz =7; 
 int wylonczz =19;
 
 int wlonczKarm1 =10;
 int wlonczKarm2 =15;
 
 int serwopozycja =0;
 int serwowloncz=16;
 
 int ilosc=3;     //ile karmy podac ile razy posypać

 Servo myservo;  // create servo object to control a servo
  DS3231 Clock;
  RTClib RTC; //modul zegara
  DateTime  now= RTC.now();

//RS do pinu 12 (en to E) E do 11 itd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  // tu podpinam przekazniki do oświetlenia 
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  
  pinMode(10, OUTPUT);
  digitalWrite(10,HIGH);
  
  pinMode(8, OUTPUT);
  digitalWrite(8,HIGH);
  
  pinMode(9, OUTPUT);
  digitalWrite(9,HIGH);
  
  pinMode(LED_BUILTIN, OUTPUT);               //wbudowana dioda led na plytce

   myservo.attach(9);  //  pin 9 sygnal dla serwa servo 
   myservo.write(0);    //  ustawiam serwo na kąt 0 stopni          

////////////*********************************************************************************/////////// 
// ******* to po kolei wybraną linie odkomentować żeby ustawić zegar a potem zakomentować,  prymitywne ale dziala ;)
//ROK
//   Clock.setYear(19);
//MIESIAC
//   Clock.setMonth(6);
//DZIEN
//   Clock.setDate(11);
//DZIEN TYGODNIA NP 1 TO PONIEDZIAŁEK
//   Clock.setDoW(2);
//GODZINA
//   Clock.setHour(12);
//MINUTY
//   Clock.setMinute(54);
//SEKUNDY
//   Clock.setSecond(15);
//////////////*********************************************************************************////////////
}
 
void loop(){
    now= RTC.now();
    godziny=now.hour();
    minuty=now.minute();
    sekundy=now.second();

 delay(500);
 {
    //wyświetla na monitorze portu szeregowego
    Serial.print("godzina: ");
    Serial.print(godziny);
    Serial.print(" : ");  
    Serial.print(minuty);
    Serial.print(" : ");
    Serial.println(sekundy);
      
       digitalWrite(LED_BUILTIN, HIGH); //dodałem migającą diodę wbudowanaą na płytce
       delay(500);
       digitalWrite(LED_BUILTIN,LOW);

//tu pulapki czasowe dla aktywacji przekaznikow
   if (godziny==wloncz){
      digitalWrite(13,LOW);  
    }
   if (godziny==wyloncz){
      digitalWrite(13,HIGH);
    }
   if (godziny==wlonczz){
      digitalWrite(10,LOW);  
     }
    if (godziny==wylonczz){
        digitalWrite(10,HIGH);
    }
 // pułapka dla serwa karmienie
  if (godziny==wlonczKarm1 && minuty ==0 && sekundy==0)
  // i ilosc to ilosc przechylen serwa sypniec karmy
    for(int i=0; i<ilosc; i++)
    {
     myservo.write(180);              
     delay(1000);
     myservo.write(0); 
     delay(1000);
        }

  if (godziny==wlonczKarm2 && minuty ==0 && sekundy==0)
    for(int i=0; i<ilosc; i++)
    {
     myservo.write(180);              
     delay(1000);
     myservo.write(0); 
     delay(1000);
         }

    lcd.setCursor(0,0);
    if (godziny <10)
    lcd.print(0);
    lcd.print(godziny);
    lcd.print(":");
    if (minuty <10)
    lcd.print(0);
    lcd.print(minuty );
    lcd.print(":");
    if (sekundy <10)
    lcd.print(0);
    lcd.print(sekundy);
    lcd.print(" Karmie ");
    lcd.setCursor(0,1);
//     lcd.print(" ");
//     lcd.print(wloncz);
//     lcd.print("+");
//     lcd.print(wyloncz);
//     lcd.print(" +");
//     lcd.print(wlonczz);
//     lcd.print(" -");
//     lcd.print(wylonczz);
     lcd.print("o ");
     lcd.print(wlonczKarm1);
     lcd.print(" i o ");
     lcd.print(wlonczKarm2);
     lcd.print(" *");
     lcd.print(ilosc);
     lcd.print("syp");
  
 }
 
 }    
