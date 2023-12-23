#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>

#define owen 8                          //output to owen
#define led_indicator 9                 //output indicator, HIGH if owen is told to heat 
#define button_plus 10                  //plus button
#define button_minus 11                 //minus button
#define button_enter 12                 //enter button
#define LM35 A5                         //TO BE CHANGED to thermocouple, thermoresistor for now
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);    //LCD pinout initialization


struct stage{ 
  unsigned int stage_time;        //stage time in minutes
  unsigned char stage_temp;       //stage temperature in °C
  unsigned char temp_grow;        //tempetature grow in 0.1°C
  stage *next;                    //pointer to next stage 
};


struct data{
  bool plus;                       //plus button status
  bool minus;                      //minus button status
  bool enter;                      //enter button status
  bool cooling;                    //cooling switch
  float temp_now;                  //last measured temperature
  float temp_aim;                  //temperature wanted in owne
  float cooling_temp_change;       //
  unsigned int cooling_time;       //colling duration in minutes
  unsigned int to_end;             //time to end of stage in minutes
  String stage_name;               //name of current stage
};


void buttons(data *przy){               //updates buttons status at given data pointer
  if(digitalRead(button_plus) == LOW)
    przy->plus = true;
  else 
    przy->plus = false;
  if(digitalRead(button_minus) == LOW)
    przy->minus = true;
  else 
    przy->minus = false;
  if(digitalRead(button_enter) == LOW)
    przy->enter = true;
  else 
    przy->enter = false;
}


void temp_change(data *przy, stage *now){     //changes aimed temperature
  if(przy->cooling){
    przy->temp_aim += przy->cooling_temp_change;
  }
  else{
    if(przy->temp_aim + now->temp_grow/40.0 < now->stage_temp)
      przy->temp_aim += now->temp_grow/40.0;
    else 
      przy->temp_aim = now->stage_temp;
  }
}


void baking(data *przy){              //determines actions in owen

  if(przy->enter && przy->plus){      //enter + plus -> owne heating ON
    digitalWrite(owen, HIGH);
    digitalWrite(led_indicator, HIGH);
    return;
  }

  if(przy->enter && przy->minus){      //enter + minus -> owne heating OFF
    digitalWrite(owen, LOW);
    digitalWrite(led_indicator, LOW);
    return;
  }

  if(przy->temp_now < przy->temp_aim){  //if currnet temperature is lower than wanted temperature owen heating ON
    digitalWrite(owen, HIGH);
    digitalWrite(led_indicator, HIGH);
  }

  else{                                 //if else owen heating is OFF
    digitalWrite(owen, LOW);
    digitalWrite(led_indicator, LOW);
  }
}


float owen_temp(){                      //measures temperature in owen
  return (analogRead(LM35)*5.0)/10.24;
}


void mem_clean(stage *head){            //clears stage memory
  stage *now = head;
  while(now!=NULL){
    stage *del = now;
    now = now->next;
    delete del;
  }
  head = NULL;
}


void print_to_lcd(String text1, String text2){  //prints strings to lcd, text1 in first row, text2 in second
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text1);
  lcd.setCursor(0, 1);
  lcd.print(text2);
}


void lcd_menager(data *przy, stage *now){

  print_to_lcd(przy->stage_name + " Temp: "+String(przy->temp_now)+"C", "Zostalo: "+String(przy->to_end)+"min");
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  lcd.setCursor(10,0);
  lcd.print(przy->temp_now);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  if(!przy->cooling){
    print_to_lcd(przy->stage_name + " Temp: "+String(przy->temp_now)+"C", "T: "+String(now->stage_temp)+"C  N: "+String(now->temp_grow/10)+"."+String(now->temp_grow%10)+"C");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    lcd.setCursor(10,0);
    lcd.print(przy->temp_now);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  
}