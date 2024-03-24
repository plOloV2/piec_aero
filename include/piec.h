#include <avr/interrupt.h>
#include <jokes.h>

#define time_ticks 62410             //65535 - (16M / (256 * 20,)) -> 1/20 sec

struct time{
    unsigned char frag_sec;
    unsigned char sec;
    unsigned char min;
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

struct lcd_dis{
    String row1;
    String row2;
};

struct stage{ 
  unsigned int stage_time;          //stage time in minutes
  unsigned char stage_temp;         //stage temperature in °C
  unsigned char temp_grow;          //tempetature grow in 0.1°C
  stage *next;                      //pointer to next stage 
};


void setup(time *clock){
    DDRB |= _BV(DDB5);

    TCNT1 = time_ticks;

    TCCR1B = (1 << CS12);

    TCCR1A = 0x00;

    TIMSK1 = (1 << TOIE1);

    clock->sec = 0;
    clock->min = 0;

    sei();
}

void time_keeper(time *clock){

    TCNT1 = time_ticks;

    clock->frag_sec++;

    if(clock->frag_sec == 20){
        PORTB ^= _BV(PORTB5);
        clock->sec++;
        if(clock->sec == 60){
            clock->min++;
            clock->sec = 0;
        }
        clock->frag_sec = 0;
    }

    
}

void joke(lcd_dis *display){

    display->row1 = joke_row1(TCNT1%3);     //DOSTOSOWAĆ DO ILOŚCI OPCJI

    display->row2 = joke_row2(TCNT1%6);     //DOSTOSOWAĆ DO ILOŚCI OPCJI

}