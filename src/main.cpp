#include <piec.h>

time *clock = new time;
data *info = new data;
buttons *butt = new buttons;
lcd_dis *display = new lcd_dis;
stage *first = NULL;

char phase;                               //B -> blokada, D -> wprowadzanie danychm, P -> pieczenie, K -> koniec
bool run, end;


ISR(TIMER1_OVF_vect){

  buttons_check(butt);

  run = true;

  time_keeper(clock);

}


void lcd_setup(){

  LCD_setup_1();
  run = false;
  while (run);

  LCD_setup_2();
  run = false;
  while (run);

  LCD_setup_3();
  run = false;
  while (run);
  
}



int main(){


  buttons_settup();                   //setup section
  owen_output_setup();
  clock->lcd_setup = true;
  setup(clock);
  
  lcd_setup();
  
  end = false;
  phase = 'B';


  while(!end){                      //running section

    if(run) switch (phase){
      case 'B':

        run = false;
        break;
      
      case 'D':

        run = false;
        break;

      case 'P':

        run = false;
        break;

      case 'K':

        run = false;
        break;
      
      default:

        display->row1 = "     ERROR!     ";
        display->row2 = "Shuting down...";

        end = true;

    }

  }

  owen_switch(false);

  return 0;

}
