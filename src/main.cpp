#include <piec.h>

time *clock = new time;
lcd_dis *display = new lcd_dis;
data *head = new data;
stage *first = NULL;

char phase;                               //B -> blokada, D -> wprowadzanie danychm, P -> pieczenie, K -> koniec
bool run;

ISR(TIMER1_OVF_vect){

  time_keeper(clock);
  run = true;

}

// buttons interupts here

int main(){

  setup(clock);

  phase = 'B';

  while(1){

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
    }

  }

  return 0;

}