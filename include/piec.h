#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <avr/wdt.h>


#define owen A4                        //output to owen
#define led_indicator A5               //output indicator, HIGH if owen is told to heat 
#define button_plus 3                  //plus button
#define button_minus 2                 //minus button
#define button_enter 1                 //enter button
#define termistor A0                   //termistor pin

LiquidCrystal lcd(4, 5, 6, 7, 8, 9, 10, 11, 12, 13);    //LCD pinout initialization
void(* resetFunc) (void) = 0;


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
  bool data_ready;                 //set to true if data input has been finishead
  double temp_now;                 //last measured temperature
  double temp_aim;                 //temperature wanted in owne
  double cooling_temp_change;      //temperature decreas during cooling
  unsigned int cooling_time;       //colling duration in minutes
  unsigned int to_end;             //time to end of stage in minutes
  unsigned char stage_number;      //current stage number in baking, also used to determin how many stages there will be when enttering data
  String stage_name;               //name of current stage
};


void buttons(data *przy){               //updates buttons status at given data pointer, takes pointer to data struct as parameter

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


void temp_change(data *przy, stage *now){     //changes aimed temperature, called every 15s, takes pointers to data struct and to current stage struct as parameters
  if(przy->cooling){                          //if cooling == TRUE, lowers designated temp by earier calculated ammount
    przy->temp_aim += przy->cooling_temp_change;
  }
  else{                                       //if cooling == FALSE, ads to designated temp temp grow specified by user
    if(przy->temp_aim + now->temp_grow/40.0 < now->stage_temp)
      przy->temp_aim += now->temp_grow/40.0;
    else 
      przy->temp_aim = now->stage_temp;   //if addition will make designated temp higher than specified by user in current stage sets designated temp to that specified by user
  }
}


bool baking_manual(data *przy){              //manual controll over owen, return true if owen is manually controlled

  if(przy->enter && przy->plus){      //enter + plus -> owen heating ON
    digitalWrite(owen, HIGH);
    digitalWrite(led_indicator, HIGH);
    return true;
  }

  if(przy->enter && przy->minus){      //enter + minus -> owen heating OFF
    digitalWrite(owen, LOW);
    digitalWrite(led_indicator, LOW);
    return true;
  }

  return false;
}


void baking_auto(data *przy){
  if(przy->temp_now < przy->temp_aim){
    digitalWrite(owen, HIGH);
    digitalWrite(led_indicator, HIGH);
  }

  else{
    digitalWrite(owen, LOW);
    digitalWrite(led_indicator, LOW);
  }

  return;
}


void mem_clean(stage *head){            //clears memory allocated to stage struct, takes pointer to first stage struct as parameter
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


void lcd_menager(data *przy, stage *now){       //prints to lcd baking info, updates current temp every 1s, whole screen updates every 2s, takes pointers to data struct and to current stage struct as parameters

  print_to_lcd(przy->stage_name + " Temp: "+String(przy->temp_now)+"C", "Zostalo: "+String(przy->to_end)+"min");
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  lcd.setCursor(10,0);
  lcd.print(przy->temp_now);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  if(!przy->cooling){                           //only during baking, every other screen update will show baking info (temp grow, designated temp)

    print_to_lcd(przy->stage_name + " Temp: "+String(przy->temp_now)+"C", "T: "+String(now->stage_temp)+"C  N: "+String(now->temp_grow/10)+"."+String(now->temp_grow%10)+"C");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    lcd.setCursor(10,0);
    lcd.print(przy->temp_now);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  
}


double owen_temp(){    

  double temp;                    /* to read the value 4 times*/

  temp = analogRead(termistor);
  vTaskDelay(250 / portTICK_PERIOD_MS);

  temp += analogRead(termistor);
  vTaskDelay(250 / portTICK_PERIOD_MS);

  temp += analogRead(termistor);
  vTaskDelay(250 / portTICK_PERIOD_MS);

  temp += analogRead(termistor);
      
  temp /= 4.0;                    /* find the average analog value from those data*/


  temp *= 5;
  temp /=1024.0;
  temp = (5 / temp) - 1;
  temp *= 980;                    /*resistance value*/

  Serial.println(temp);

  temp = (1/(0.03537299663 - 0.008631998552*log(temp) + 0.00008232071957*pow(log(temp), 3))) - 277.15;
  
  vTaskDelay(250 / portTICK_PERIOD_MS);

  return temp;                  
}