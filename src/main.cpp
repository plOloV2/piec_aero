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
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void main_task(void *parametr){

  digitalWrite(owen, LOW);
  digitalWrite(led_indicator, LOW);

  vTaskSuspend(owen_controller);        //sttoping all not needed now tasks
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
  vTaskDelay(3000 / portTICK_PERIOD_MS);


  przy->data_ready = false;
  vTaskResume(data_enter);
  while(!przy->data_ready)
    vTaskDelay(500 / portTICK_PERIOD_MS);


  przy->stage_number = 0;     //initialization of nedded values, preparing to bake
  przy->cooling = false;
  now = head;

  vTaskResume(temp_measurer);     //resuming tasks
  vTaskResume(owen_controller);
  vTaskResume(temp_changer);
  vTaskResume(lcd_updater);

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
          vTaskDelay(50000 / portTICK_PERIOD_MS);
      }
        
      vTaskDelay(100 / portTICK_PERIOD_MS);
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
          vTaskDelay(50000 / portTICK_PERIOD_MS);
      }
        
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

  }

  digitalWrite(owen, LOW);
  digitalWrite(led_indicator, LOW);

  vTaskSuspend(owen_controller);          //after all baking and cooling suspends all not needed tasks
  vTaskSuspend(temp_changer);
  vTaskSuspend(lcd_updater);
  

  mem_clean(head);


  print_to_lcd("Koniec pieczenia", "T: " + String(przy->temp_now) + "C");   //prints end text and waits to press enter
  lcd.setCursor(0,1);

  while(!przy->enter){
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    lcd.setCursor(0,1);
    lcd.print("T: " + String(przy->temp_now) + "C");
  }

  vTaskDelay(200);
  pinMode(resetPin, OUTPUT);

}

void temp_measure(void *parametr){
  while(1){
    przy->temp_now = owen_temp();
  }
}

void owen_controll(void *parametr){
  while(1){
    if(przy->plus && przy->minus){
      vTaskSuspend(lcd_updater);
      print_to_lcd("Restart?", "- ->NIE  + ->TAK");
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      while(1){
        if(przy->plus)
          pinMode(resetPin, OUTPUT);

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
  while(1){
    przy->stage_number = 1;

    while(!przy->enter){
      print_to_lcd("Ile etapow?", String(przy->stage_number));

      if(przy->plus){
        if(przy->stage_number == 10)
          przy->stage_number = 0;
        else 
          przy->stage_number++;
      }

      if(przy->minus){
        if(przy->stage_number == 0)
          przy->stage_number = 10;
        else 
          przy->stage_number--;
      }

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


      now->stage_temp = 80;
      now->stage_time = 30;
      now->temp_grow = 10;
      now->next = NULL;
      vTaskDelay(500 / portTICK_PERIOD_MS);

      unsigned char state = 0;
      while(state < 3){

        switch(state){
          case(0):
            print_to_lcd("Czas etapu " + String(i+1) + ":", String(now->stage_time) + "min");

            if(przy->plus){
              if(now->stage_time == 9999)
                now->stage_time = 0;
              else 
                now->stage_time++;
            }

            if(przy->minus){
              if(now->stage_time == 0)
                now->stage_time = 9999;
              else 
                now->stage_time--;
            }
            
            if(przy->enter){
              state = 1;
              vTaskDelay(500 / portTICK_PERIOD_MS);
            }
              

            vTaskDelay(150 / portTICK_PERIOD_MS);

            break;

          case(1):
            print_to_lcd("Temp etapu " + String(i+1) + ":", String(now->stage_temp) + "*C");

            if(przy->plus){
              if(now->stage_temp == 150)
                now->stage_temp = 30;
              else 
                now->stage_temp++;
            }

            if(przy->minus){
              if(now->stage_temp == 30)
                now->stage_temp = 150;
              else 
                now->stage_temp--;
            }
            
            if(przy->enter){
              state = 2;
              vTaskDelay(500 / portTICK_PERIOD_MS);
            }

            vTaskDelay(250 / portTICK_PERIOD_MS);

            break;

            
          case(2):
            print_to_lcd("Narost etapu " + String(i+1) + ":", String(now->temp_grow/10) + "," + String(now->temp_grow%10) + "*C/min");

            if(przy->plus){
              if(now->temp_grow == 30)
                now->temp_grow = 1;
              else 
                now->temp_grow++;
            }

            if(przy->minus){
              if(now->temp_grow == 1)
                now->temp_grow = 30;
              else 
                now->temp_grow--;
            }
            
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

      if(przy->plus){
        if(przy->cooling_time == 9999)
          przy->cooling_time = 0;
        else 
          przy->cooling_time++;
      }

      if(przy->minus){
        if(przy->cooling_time == 0)
          przy->cooling_time = 9999;
        else 
          przy->cooling_time--;
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    przy->data_ready = true;
    vTaskSuspend(data_enter);
  }
}

void setup() {                                                                          //owen setup

  lcd.begin(16, 2);                                                                     //pins setup
  pinMode(button_plus, INPUT_PULLUP);
  pinMode(button_minus, INPUT_PULLUP);
  pinMode(button_enter, INPUT_PULLUP);
  pinMode(owen, OUTPUT);
  pinMode(led_indicator, OUTPUT);

  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);

  digitalWrite(owen, LOW);
  digitalWrite(led_indicator, LOW);


  xTaskCreate(main_task, "Boss", 128, NULL, 10, &main_tasker);                            //task creation
  xTaskCreate(button_check, "Button checking", 100, NULL, 4, &button_checker);
  xTaskCreate(temp_measure, "Temp measuring", 100, NULL, 3, &temp_measurer);
  xTaskCreate(aim_temperature_change, "Temp changing", 100, NULL, 2, &temp_changer);
  xTaskCreate(owen_controll, "Owen controlling", 100, NULL, 1, &owen_controller);
  xTaskCreate(data_input, "Stage info input", 100, NULL, 1, &data_enter);
  xTaskCreate(lcd_update, "LCD updating", 100, NULL, 0, &lcd_updater);


}

void loop() {}