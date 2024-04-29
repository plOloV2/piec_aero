#include <avr/interrupt.h>
#include <jokes.h>
#include <LCD.h>

#define time_ticks 49910                //65535 - (16M / (256 * 4)) -> 1/4 sec
#define time_ticks_lcd 64285            //65535 - (16M / (256 * 50)) -> 1/50 sec

struct time{
    bool lcd_setup;
    unsigned char frag_sec;
    unsigned char sec;
    unsigned char min;
};


struct buttons{
    bool plus;                       //plus button status
    bool minus;                      //minus button status
    bool enter;                      //enter button status
};


struct data{
    bool data_ready;                 //set to true if data input has been finishead
    double temp_now;                 //last measured temperature
    double temp_aim;                 //temperature wanted in owne
    unsigned int to_end;             //time to end of stage in minutes
    unsigned char stage_number;      //current stage number in baking, also used to determin how many stages there will be when enttering data
    String stage_name;               //name of current stage
};


struct lcd_dis{
    String row1;
    String row2;
};


struct stage{
    bool cooling;                       //true if this stage is cooling stage
    unsigned int stage_time;            //stage time in minutes
    unsigned char stage_temp;           //stage temperature in °C
    unsigned char temp_grow;            //tempetature grow in 0.1°C
    stage *next;                        //pointer to next stage 
};


void setup(time *clock){                //needs description

    TCNT1 = time_ticks_lcd;

    TCCR1B = (1 << CS12);

    TCCR1A = 0x00;

    TIMSK1 = (1 << TOIE1);

    clock->sec = 0;
    clock->min = 0;

    sei();
}


void time_keeper(time *clock){              //needs description

    if(clock->lcd_setup)
        TCNT1 = time_ticks_lcd;

    else{

        clock->frag_sec++;

            if(clock->frag_sec == 4){

                clock->sec++;

                if(clock->sec == 60){

                    clock->min++;

                    clock->sec = 0;

                }

                clock->frag_sec = 0;
            }

        TCNT1 = time_ticks;
    }
        
}


void buttons_settup(){

    DDRD &= !0b00001110;        //sets pins 1-3 as inputs

    PORTD |= 0b00001110;        //sets pins 1-3 to high
}


void owen_output_setup(){

    DDRC |= 0b00110000;             //sets pins A4 and A5 as outputs

    PORTC &= !0b00110000;           //sets pins A4 and A5 as LOW

}


void buttons_check(buttons *butt){                  //updates buttons status at given data pointer, takes pointer to data struct as parameter

    if(PIND & 0b00000010)                           //checks pin 1, if pin is low, sets button plus value to true
        butt->plus = false;
    else 
        butt->plus = true;
    
    if(PIND & 0b000000100)                          //checks pin 2, if pin is low, sets button minus value to true
        butt->minus = false;
    else 
        butt->minus = true;

    if(PIND & 0b000001000)                          //checks pin 3, if pin is low, sets button enter value to true
        butt->enter = false;
    else 
        butt->enter = true;

}


void owen_switch(bool ON){              //switcches owen output and indicator LED

    if(ON)
        PORTC &= !0b00110000;           //sets pins A4 and A5 as LOW

    else
        PORTC |= 0b00110000;            //sets pins A4 and A5 as HIGH
    
}


void joke(lcd_dis *display, time *clock){            //prints jokes to screeen wheen activated
    
    int random = clock->min + clock->frag_sec*clock->sec;

    display->row1 = joke_row1(random%3);     //DOSTOSOWAĆ DO ILOŚCI OPCJI

    display->row2 = joke_row2(random%6);     //DOSTOSOWAĆ DO ILOŚCI OPCJI

}