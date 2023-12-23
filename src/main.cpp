#include "piec.h"

stage *head = NULL;
stage *now;
data *przy = new data;

void button_check(void *parametr){
  buttons(przy);
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void stage_change(void *parametr){
  unsigned char stage_number = 0;
  przy->cooling = false;
  now = head;
  while(now != NULL){
    przy->to_end = now->stage_time;
    stage_number++;
    przy->stage_name = "E" + String(stage_number / 10) + String(stage_number % 10);
    przy->temp_aim = owen_temp();
    vTaskDelay(now->stage_time*60000 / portTICK_PERIOD_MS);
    now = now->next;
  }
  przy->cooling = true;
  przy->to_end = przy->cooling_time;
  przy->cooling_temp_change = (30.0 - przy->temp_now) / przy->cooling_time;
  przy->stage_name = "Chl";
}

void temp(void *parametr){
  przy->temp_now = owen_temp();
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void owen_controll(void *parametr){
  // if(przy->plus && przy->minus && przy->enter)
  //   some piece of code to change current stage or quit baking
  baking(przy);
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void aim_temperature_change(void *parametr){
  temp_change(przy, now);
  vTaskDelay(15000 / portTICK_PERIOD_MS);
}

void lcd_update(void *parametr){
  lcd_menager(przy, now);
}

void time_keeper(void *parametr){
  przy->to_end--;
  vTaskDelay(60000 / portTICK_PERIOD_MS);
}

void user_interface(void *parametr){
  //not done yet
}

void setup() { //owen setup
  lcd.begin(16, 2);
  pinMode(button_plus, INPUT_PULLUP);
  pinMode(button_minus, INPUT_PULLUP);
  pinMode(button_enter, INPUT_PULLUP);
  pinMode(owen, OUTPUT);
  pinMode(led_indicator, OUTPUT);

}

void loop() {
}