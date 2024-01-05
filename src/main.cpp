#include "piec.h"

stage *head = NULL, *now;
data *przy = new data;

TaskHandle_t button_checker;
TaskHandle_t main_tasker;
TaskHandle_t temp_measurer;
TaskHandle_t owen_controller;
TaskHandle_t temp_changer;
TaskHandle_t data_enter;
TaskHandle_t lcd_updater;

void button_check(void *parametr){
  while(1){
    buttons(przy);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void main_task(void *parametr){
  while(1){

    vTaskSuspend(temp_measurer);        //sttoping all not needed now tasks
    vTaskSuspend(owen_controller);
    vTaskSuspend(temp_changer);
    vTaskSuspend(lcd_updater);
    vTaskSuspend(data_enter);


    print_to_lcd("Piec zablokowany", "Odb:ENTER + PLUS" ); //owen blokade, enter + plus -> owen unblock
    while(1){
      vTaskDelay(100 / portTICK_PERIOD_MS);
      if(przy->enter && przy->plus)
        break;
    }
      
    print_to_lcd(" Milego wypalu!", "" );       //greatings
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    


    przy->data_ready = false;
    vTaskResume(data_enter);
    //entering wypał data here
    while(!przy->data_ready)
      vTaskDelay(500 / portTICK_PERIOD_MS);
      
    vTaskSuspend(data_enter);
    


    przy->stage_number = 0;     //initialization of nedded values, preparing to bake
    przy->cooling = false;
    now = head;    

    vTaskResume(temp_measurer);     //resuming tasks
    vTaskResume(owen_controller);
    vTaskResume(temp_changer);
    vTaskResume(lcd_updater);

    while(now != NULL){                         //if now == NULL then we have reached the end of all stages -> while loop ends
      przy->to_end = now->stage_time;           //setting stage values, such as name, time and start temp
      przy->stage_number++;
      przy->stage_name = "E" + String(przy->stage_number / 10) + String(przy->stage_number % 10);
      przy->temp_aim = owen_temp();

      while(przy->to_end>0){                    //keeping track on time
        przy->to_end--;
        vTaskDelay(60000 / portTICK_PERIOD_MS);
      }
      
      now = now->next; 
    }

    if(przy->cooling_time != 0){                //cooling time set to 0 -> no cooling

      przy->cooling = true;                 //seting up cooling values
      przy->to_end = przy->cooling_time;
      przy->cooling_temp_change = (30.0 - przy->temp_now) / przy->cooling_time;
      przy->stage_name = "Chl";
      while(przy->to_end>0){
        przy->to_end--;
        vTaskDelay(60000 / portTICK_PERIOD_MS);
      }

    }

    vTaskSuspend(temp_measurer);          //after all baking and cooling suspends all not needed tasks
    vTaskSuspend(owen_controller);
    vTaskSuspend(temp_changer);
    vTaskSuspend(lcd_updater);
    

    print_to_lcd("Koniec pieczenia", "Smacznego ciacha");   //prints end text and waits to press enter
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

void data_input(void *parametr){
  while(1){
    przy->stage_number = 1;

    while(!przy->enter){
      print_to_lcd("Ile etapow?", String(przy->stage_number));

      if(przy->plus){
        if(przy->stage_number == 255)
          przy->stage_number = 0;
        else 
          przy->stage_number++;
      }

      if(przy->minus){
        if(przy->stage_number == 0)
          przy->stage_number = 255;
        else 
          przy->stage_number--;
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    for(int i=0; i<przy->stage_number; i++){
      if(i==0){
        head = new stage;
        now = head;
      }
      else{
        now->next = new stage;
        now = now->next;
      }
      /*
      maszyna stanow poczas wprowadzania
      jeszcze jest watchdog i zmiana w tarkcie do zrobienia
      */
    }


    przy->data_ready = true;
  }
}

void setup() { //owen setup
  lcd.begin(16, 2);
  pinMode(button_plus, INPUT_PULLUP);
  pinMode(button_minus, INPUT_PULLUP);
  pinMode(button_enter, INPUT_PULLUP);
  pinMode(owen, OUTPUT);
  pinMode(led_indicator, OUTPUT);

  xTaskCreate(main_task, "Boss", 100, NULL, 10, &main_tasker);
  xTaskCreate(data_input, "Stage info input", 100, NULL, 8, &data_enter);
  xTaskCreate(temp_measure, "Temp measuring", 100, NULL, 7, &temp_measurer);
  xTaskCreate(aim_temperature_change, "Temp changing", 100, NULL, 7, &temp_changer);
  xTaskCreate(owen_controll, "Owen controlling", 100, NULL, 6, &owen_controller);
  xTaskCreate(button_check, "Button checking", 100, NULL, 5, &button_checker);
  xTaskCreate(lcd_update, "LCD updating", 100, NULL, 0, &lcd_updater);

}

void loop() {
}