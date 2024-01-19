#include "piec.h"


stage *head = NULL, *now;
data *przy = new data;


TaskHandle_t temp_measurer;
TaskHandle_t owen_controller;
TaskHandle_t temp_changer;
TaskHandle_t data_enter;
TaskHandle_t lcd_updater;
TaskHandle_t jokes;

void button_change_enter(){
  przy->enter = !przy->enter;
}

void button_change_plus(){
  przy->plus = !przy->plus;
}

void button_change_minus(){
  przy->minus = !przy->minus;
}


void temp_measure(void *parametr){
  while(1){
    przy->temp_now = owen_temp();
  }
}

void owen_controll(void *parametr){
  while(1){
    if(przy->plus && przy->minus && !przy->enter){
      vTaskSuspend(lcd_updater);
      print_to_lcd("Restart?", "- ->NIE  + ->TAK");
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      while(1){
        if(przy->plus)
          resetFunc();

        if(przy->minus)
          break;

        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
      vTaskResume(lcd_updater);
    }

    if(!baking_manual(przy))
      baking_auto(przy);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void aim_temperature_change(void *parametr){
  while(1){
    temp_change(przy, now);
    vTaskDelay(14000 / portTICK_PERIOD_MS);
  }
}

void lcd_update(void *parametr){
  while(1)
    lcd_menager(przy, now);
}

void data_input(void *parametr){
  przy->stage_number = 1;

  while(!przy->enter){
    print_to_lcd("Ile etapow?", String(przy->stage_number));

    przy->stage_number = value_change(przy->stage_number, 10, 0, przy);

    vTaskDelay(250 / portTICK_PERIOD_MS);
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


    now->stage_temp = 30;
    now->stage_time = 5;
    now->temp_grow = 1;
    now->next = NULL;

    vTaskDelay(500 / portTICK_PERIOD_MS);

    unsigned char state = 0;
    
    while(state < 3){

      switch(state){
        case(0):
          print_to_lcd("Czas etapu " + String(i+1) + ":", String(now->stage_time) + "min");

          now->stage_time = value_change(now->stage_time, 255, 1, przy);
          
          if(przy->enter){
            state = 1;
            vTaskDelay(500 / portTICK_PERIOD_MS);
          }
            
          vTaskDelay(150 / portTICK_PERIOD_MS);

          break;

        case(1):
          print_to_lcd("Temp etapu " + String(i+1) + ":", String(now->stage_temp) + "*C");

          now->stage_temp = value_change(now->stage_temp, 125, 30, przy);
          
          if(przy->enter){
            state = 2;
            vTaskDelay(500 / portTICK_PERIOD_MS);
          }

          vTaskDelay(250 / portTICK_PERIOD_MS);

          break;

          
        case(2):
          print_to_lcd("Narost etapu " + String(i+1) + ":", String(now->temp_grow/10) + "," + String(now->temp_grow%10) + "*C/min");

          now->temp_grow = value_change(now->temp_grow, 30, 1, przy);
          
          if(przy->enter){
            state = 3;
            vTaskDelay(500 / portTICK_PERIOD_MS);
          }

          vTaskDelay(250 / portTICK_PERIOD_MS);

          break;
      }
    }
  }
  
  przy->cooling_time = 1;

  przy->stage_number = 0;

  vTaskDelay(500 / portTICK_PERIOD_MS);

  while(!przy->enter){
    print_to_lcd("Czas chlodzenia", String(przy->cooling_time)+"min, 0->brak");

    przy->cooling_time = value_change(przy->cooling, 255, 0, przy);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }

  przy->data_ready = true;
  vTaskSuspend(data_enter);

}

void joker(void *parametr){
  while(1){
    if(przy->enter && przy->plus && przy->minus){
      vTaskSuspend(lcd_updater);
      let_the_fun_begin();
      vTaskResume(lcd_updater);
    }
    else
      vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

int main() { //owen setup

  wdt_enable(WDTO_1S); //watchdog initialization with 1sec timer

  lcd.begin(16, 2);                     //pins setup
  pinMode(button_plus, INPUT_PULLUP);
  pinMode(button_minus, INPUT_PULLUP);
  pinMode(button_enter, INPUT_PULLUP);
  pinMode(owen, OUTPUT);
  pinMode(led_indicator, OUTPUT);


  if(button_enter == LOW)
    przy->enter = true;
  else
    przy->enter = false;
  if(button_plus == LOW)
    przy->plus = true;
  else
    przy->plus = false;
  if(button_minus == LOW)
    przy->minus = true;
  else
    przy->minus = false;


  attachInterrupt(digitalPinToInterrupt(button_enter), button_change_enter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button_plus), button_change_enter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button_minus), button_change_enter, CHANGE);

  
  digitalWrite(owen, LOW);
  digitalWrite(led_indicator, LOW);


  xTaskCreate(temp_measure, "Temp measuring", 100, NULL, 3, &temp_measurer);

  print_to_lcd("Piec zablokowany", "Odb:ENTER + PLUS" );        //owen blokade, enter + plus -> owen unblock
  while(1){
    delay(100);
    if(przy->enter && przy->plus)
      break;
  }

  print_to_lcd(" Milego wypalu!", "" );       //greatings
  delay(3000);


  przy->data_ready = false;
  xTaskCreate(data_input, "Stage info input", 100, NULL, 1, &data_enter);
  while(!przy->data_ready)
    delay(500);


  vTaskDelete(data_enter);
  przy->stage_number = 0;     //initialization of nedded values, preparing to bake
  przy->cooling = false;
  now = head;


  xTaskCreate(aim_temperature_change, "Temp changing", 100, NULL, 2, &temp_changer);
  xTaskCreate(owen_controll, "Owen controlling", 100, NULL, 1, &owen_controller);
  xTaskCreate(lcd_update, "LCD updating", 100, NULL, 0, &lcd_updater);
  xTaskCreate(joker, "jokes", 100, NULL, 0, &jokes);


  unsigned long time;


  while(now != NULL){                         //if now == NULL then we have reached the end of all stages -> while loop ends
    przy->to_end = now->stage_time;           //setting stage values, such as name, time and start temp
    przy->stage_number++;
    przy->stage_name = "E" + String(przy->stage_number / 10) + String(przy->stage_number % 10);
    przy->temp_aim = owen_temp();


    time = millis();
    while(przy->to_end>0){                    //keeping track on time
      if((millis()-time)>60000){
        przy->to_end--;
        time = millis();
        if(przy->to_end > 0) 
          delay(50000);
      }
        
      delay(100);
    }
    
    now = now->next; 
  }

  if(przy->cooling_time != 0){                //cooling time set to 0 -> no cooling


    przy->cooling = true;                 //seting up cooling values
    przy->to_end = przy->cooling_time;
    przy->cooling_temp_change = (30.0 - przy->temp_now) / (przy->cooling_time*4.0);
    przy->stage_name = "Chl";


    time = millis();
    while(przy->to_end>0){
      if((millis()-time)>60000){
        przy->to_end--;
        time = millis();
        if(przy->to_end > 0) 
          delay(50000);
      }
        
      delay(100);
    }

  }


  vTaskDelete(owen_controller);          //after all baking and cooling Deletes all not needed tasks
  vTaskDelete(temp_changer);
  vTaskDelete(lcd_updater);
  vTaskDelete(jokes);


  digitalWrite(owen, LOW);
  digitalWrite(led_indicator, LOW);



  print_to_lcd("Koniec pieczenia", "T:");   //prints end text and waits to press enter
  while(!przy->enter){
    lcd.setCursor(12,0);
    lcd.print(przy->temp_now);
    delay(500);
  }
    

  delay(200);

  resetFunc();

}