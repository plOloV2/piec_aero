#include <WString.h>                            //lots of work to be done here...
#include <delay.h>

/*
    HD44780U data sheet: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf

    LCD pins 4 - 13
    RS -> 4
    EN -> 5
    DB7 -> 6
    DB6 -> 7
    DB5 -> 8
    DB4 -> 9
    DB3 -> 10
    DB2 -> 11
    DB1 -> 12
    DB0 -> 13

    8 data pins mode
    16x2 display mode
*/

void pulse_enable(){        //wait command for LCD to proces instruction

  PORTD |= 0b00100000;      //sets EN pin to HIGH

  delayMicroseconds(1);     //enable pulse must be >450ns

  PORTD &= !0b00100000;     //sets EN to LOW -> LCD begins executing instructions

  delayMicroseconds(100);   // commands need >37 us to settle
}

void LCD_setup_1(){       //resets LCD into listening to setup commands, must be  3 times

  DDRD |= 0b11110000;     //sets pins 4-13 as output
  DDRB |= 0b00111111;

  PORTD &= !0b11110000;   //sets pins 4-7 as LOW 
  PORTB &= !0b00111111;   //sets pins 8-13 as LOW

  PORTD &= !0b10110000;   //sets pins 4,6,7 as LOW
  PORTB |= 0b00000011;    //sets pins 8 and 9 to HIGH
  
  pulse_enable();
}

void LCD_setup_2(){       //sets 8pin mode and display & font size
  
  PORTD &= !0b10110000;   //sets pins 4,6,7 as LOW
  PORTB |= 0b00000111;    //sets pins 8, 9, 10 to HIGH
  PORTB &= !0b00001000;   //sets pins 11 to LOW

  pulse_enable();
}

void LCD_setup_3(){       //turn display and cursor on

  PORTD &= !0b10110000;   //sets pins 4,6,7 as LOW
  PORTB |= 0b00011100;    //sets pins 10,11,12 to HIGH
  PORTB &= !0b00100011;   //sets pins 8, 9, 13 to LOW

  pulse_enable();
}

void LCD_setup_4(){       //sets display to write from left to right

  PORTD |= 0b00100000;    //sets EN pin to HIGH
  PORTD &= !0b10110000;   //sets pins 4,6,7 as LOW
  PORTB |= 0b00011000;    //sets pins 11,12 to HIGH
  PORTB &= !0b00100111;   //sets pins 8, 9, 10, 13 to LOW

}

void LCD_write(char letter, bool * ){

}

void set_row(unsigned char row){

}

void lcd_clear(){

}


void lcd_setup(){             //initializes LCD page 45, figure 23

  delayMicroseconds(50000);   //waits >40ms for LCD to power up

  LCD_setup_1();              //first time
  delayMicroseconds(4500);    //waits >4.1ms

  LCD_setup_1();              //seccond time
  delayMicroseconds(150);     //waits >100us

  LCD_setup_1();              //third time is a charm

  LCD_setup_2();              //display mode setup first part

  LCD_setup_3();              //display mode setup second part

  LCD_setup_4();              //display mode setup third part

}


void lcd_print(String row1, String row2){

  for(unsigned char i = 0; i < 16; i++){

  }
  
}