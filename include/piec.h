#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>


#define button_plus 10 
#define button_minus 11
#define button_enter 12
#define LM35 A5 //do zmiany, póki co taki mam w domu do testowania algorytmu
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //inicjalizacja pinow dla ekranu



struct etap{ 
  unsigned int czas_wypalu; //czas trawania etapu w minutach
  unsigned char temp_wypalu; //docelowa temp etapu
  unsigned char narost; //narost temp podawany w 0.1°C
  etap *kolejny;        //wskaźnik do kolejnego etapu 
};


struct dane{
  bool plus;
  bool minus;
  bool zatwierdz;
  float temp_teraz;
  float temp_docelowa;
  String nazwa_etapu;
};


float obecna_temp(){
  return (analogRead(LM35)*5.0)/10.24;
}


void wyczysc(etap *glowa){
  etap *teraz = glowa;
  while(teraz!=NULL){
    etap *usun = teraz;
    teraz = teraz->kolejny;
    delete usun;
  }
  glowa = NULL;
}


void wypisz(String tekst1, String tekst2){  //wypisuje podane stringi, tekst1 w pierwszej linijce, tekst2 w drugiej
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(tekst1);
  lcd.setCursor(0, 1);
  lcd.print(tekst2);
}