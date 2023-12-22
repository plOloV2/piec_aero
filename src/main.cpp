#include "piec.h"

etap *glowa = NULL;
etap *teraz;
dane *przy = new dane;

void czy_wciskane(void *parametr){
  if(digitalRead(10) == LOW)
    przy->plus = true;
  else 
    przy->plus = false;
  if(digitalRead(11) == LOW)
    przy->minus = true;
  else 
    przy->minus = false;
  if(digitalRead(12) == LOW)
    przy->zatwierdz = true;
  else 
    przy->zatwierdz = false;
  vTaskDelay(20 / portTICK_PERIOD_MS);
}

void temperatura(void *parametr){
  przy->temp_teraz = obecna_temp();
  vTaskDelay(20 / portTICK_PERIOD_MS);
}

void grzaly(void *parametr){
  if(przy->zatwierdz && przy->plus){
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  if(przy->zatwierdz && przy->minus){
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  if(przy->temp_teraz < przy->temp_docelowa){
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  else{
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  vTaskDelay(20 / portTICK_PERIOD_MS);
}

void zmiana_etapu(void *parametr){
  teraz = glowa;
  while(teraz != NULL){
    przy->temp_docelowa = obecna_temp();
    vTaskDelay(teraz->czas_wypalu*60000 / portTICK_PERIOD_MS);
    teraz = teraz->kolejny;
  }
}

void narost(void *parametr){
  if(przy->temp_docelowa + teraz->narost/4.0 < teraz->temp_wypalu)
    przy->temp_docelowa += teraz->narost/4.0;
  else 
    przy->temp_docelowa = teraz->temp_wypalu;
  vTaskDelay(15000 / portTICK_PERIOD_MS);
}

// void ekran_wypal(void *parametr){
  
// }

// void ekran(void *parametr){
  
// }

void setup() {
  lcd.begin(16, 2); //16 znakow max w linijce, 2 linijki
  pinMode(button_plus, INPUT_PULLUP); //przycisk "+"
  pinMode(button_minus, INPUT_PULLUP); //przycisk "-"
  pinMode(button_enter, INPUT_PULLUP); //przycisk "zatwierdź"
  pinMode(8, OUTPUT); //grzały
  pinMode(9, OUTPUT); //lampka

}

void loop() {
}
