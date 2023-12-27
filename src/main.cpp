#include "piec.h"

stage *head = NULL, *now;
data *przy = new data;

TaskHandle_t button_checker;
TaskHandle_t main_tasker;
TaskHandle_t temp_measurer;
TaskHandle_t owen_controller;
TaskHandle_t temp_changer;
TaskHandle_t time_holder;
TaskHandle_t lcd_updater;

void button_check(void *parametr){
  while(1){
    buttons(przy);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void main_task(void *parametr){
  while(1){

    vTaskSuspend(temp_measurer);
    vTaskSuspend(owen_controller);
    vTaskSuspend(temp_changer);
    vTaskSuspend(time_holder);
    vTaskSuspend(lcd_updater);


    print_to_lcd("Piec zablokowany", "Odb:ENTER + PLUS" ); //owen blokade, enter + plus -> owen unblock
    while(!przy->enter && !przy->plus)
      vTaskDelay(100 / portTICK_PERIOD_MS);


    print_to_lcd(" Milego wypalu!", "" );
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    /*
    
    entering wypał data here
    
    */



    vTaskResume(temp_measurer);
    vTaskResume(owen_controller);
    vTaskResume(temp_changer);
    vTaskResume(time_holder);
    vTaskResume(lcd_updater);
    
    przy->stage_number = 0;
    przy->cooling = false;
    now = head;

    while(now != NULL){
      przy->to_end = now->stage_time;
      przy->stage_number++;
      przy->stage_name = "E" + String(przy->stage_number / 10) + String(przy->stage_number % 10);
      przy->temp_aim = owen_temp();
      vTaskDelay(now->stage_time*60000 / portTICK_PERIOD_MS);
      now = now->next;
    }


    przy->cooling = true;
    przy->to_end = przy->cooling_time;
    przy->cooling_temp_change = (30.0 - przy->temp_now) / przy->cooling_time;
    przy->stage_name = "Chl";
    vTaskDelay(przy->cooling_time*60000 / portTICK_PERIOD_MS);


    vTaskSuspend(temp_measurer);
    vTaskSuspend(owen_controller);
    vTaskSuspend(temp_changer);
    vTaskSuspend(time_holder);
    vTaskSuspend(lcd_updater);

    print_to_lcd("Koniec pieczenia", "Smacznego ciacha");
    while(!przy->enter)
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void temp_measure(void *parametr){
  while(1){
    przy->temp_now = owen_temp();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void owen_controll(void *parametr){
  while(1){
    // if(przy->plus && przy->minus && przy->enter)
    //   some piece of code to change current stage or quit baking
    baking(przy);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void aim_temperature_change(void *parametr){
  while(1){
    temp_change(przy, now);
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}

void lcd_update(void *parametr){
  while(1)
    lcd_menager(przy, now);
}

void time_keeper(void *parametr){
  while(1){
    przy->to_end--;
    vTaskDelay(60000 / portTICK_PERIOD_MS);
  }
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

  xTaskCreate(main_task, "Boss", 50, NULL, 10, &main_tasker);
  xTaskCreate(temp_measure, "Temp measuring", 50, NULL, 7, &temp_measurer);
  xTaskCreate(aim_temperature_change, "Temp changing", 50, NULL, 7, &temp_changer);
  xTaskCreate(owen_controll, "Owen controlling", 50, NULL, 6, &owen_controller);
  xTaskCreate(button_check, "Button checking", 50, NULL, 5, &button_checker);
  xTaskCreate(time_keeper, "Time keeping", 50, NULL, 1, &time_holder);
  xTaskCreate(lcd_update, "LCD updating", 50, NULL, 0, &lcd_updater);
  //xTaskCreate(temp_measure, "Temp measuring", 50, NULL, 7, &temp_measurer);
}

void loop() {
}