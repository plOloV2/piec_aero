#include <WString.h>                            //lots of work to be done here...

/*
    LCD pins 4 - 13
    8 data pins mode
    16x2 display mode
*/

void LCD_setup_1(){   

}

void LCD_setup_2(){   

}

void LCD_setup_3(){   

}

void LCD_write(String row1, String row2){

}

/*
void LCDreset()             // Reset LCD into 4-bit mode. Only needs to be called once at start up. 
{ 
  // Data sheet calls this initialization by instruction. Send 0x3 three times, then 0x2 to reset into 4-bit mode.
  PORTD &= B00000011;       // 01. Clear D7-D4, En, RS (digital pins 7-2, respectively) to 0 as a neutral starting point, leave pins 1,0 unchanged
  PORTD |= B00111000;       // 02. Write upper nibble 0x3, En=1
  PORTD &= B11110111;       // 03. En=0. Command is sent on trailing edge of enable
  _delay_ms(5);             // 04. Wait >4.1ms for command to process
  PORTD |= B00111000;       // 05. Write upper nibble 0x3, En=1
  PORTD &= B11110111;       // 06. En=0. Command is sent on trailing edge of enable
  _delay_us(150);           // 07. Wait >100us for command to process
  PORTD |= B00111000;       // 08. Write upper nibble 0x3, En = 1
  PORTD &= B11110111;       // 09. En=0. Command is sent on trailing edge of enable
  _delay_us(150);           // 10. Wait >100us for command to process
  PORTD &= B00000011;       // 11. Clear PORTD to overwrite next instruction in buffer
  PORTD |= B00101000;       // 12. Write upper nibble 0x2, En = 1
  PORTD &= B11110111;       // 13. En=0. Command is sent on trailing edge of enable
  _delay_us(150);           // 14. Wait >100us for command to process
  // At this point LCD is reset and listening for 4-bit commands.
}

void LCD4BitWriteCMD(byte CMD)            // Write a command byte to the LCD one nibble at a time using 4 bit mode.
{
  byte upperNibble = CMD & 0xF0;          // 01. Mask upper nibble of CMD, clear lower nibble so result takes the form: 0xUUUU0000
  byte lowerNibble = (CMD & 0x0F) << 4;   // 02. Mask lower nibble of CMD, clear upper nibble then shift left 4 so result takes the form 0xLLLL0000
  PORTD &= B00000011;                     // 03. Clear LCD buffer, enable, and register select
  PORTD |= upperNibble;                   // 04. Write upperNibble to LCD buffer
  PORTD |= B00001000;                     // 05. En=1, RS = 0
  PORTD &= B11110111;                     // 06. En=0. Command is sent on trailing edge of enable
  _delay_us(1600);                        // 07. Wait >40us for command to process
  PORTD &= B00000011;                     // 08. Clear LCD buffer, enable, and register select
  PORTD |= lowerNibble;                   // 09. Write lowerNibble to LCD buffer
  PORTD |= B00001000;                     // 10. En=1, RS = 0
  PORTD &= B11110111;                     // 11. En=0. Command is sent on trailing edge of enable
  _delay_us(1600);                        // 12. Wait >40us for command to process
}


void LCD4BitWriteDAT(byte DAT)            // Write a data byte to the LCD one nibble at a time using 4 bit mode.
{
  byte upperNibble = DAT & 0xF0;          // 01. Mask upper nibble of DAT, clear lower nibble so result takes the form: 0xUUUU0000
  byte lowerNibble = (DAT & 0x0F) << 4;   // 02. Mask lower nibble of DAT, clear upper nibble then shift left 4 so result takes the form 0xLLLL0000
  PORTD &= B00000011;                     // 03. Clear LCD buffer, enable, and register select 
  PORTD |= upperNibble;                   // 04. Write upperNibble to LCD buffer
  PORTD |= B00001100;                     // 05. En=1, RS = 1
  PORTD &= B11110111;                     // 06. En=0. Command is sent on trailing edge of enable
  _delay_us(1600);                        // 07. Wait >40us for command to process
  PORTD &= B00000011;                     // 08. Clear LCD buffer, enable, and register select
  PORTD |= lowerNibble;                   // 09. Write lowerNibble to LCD buffer
  PORTD |= B00001100;                     // 10. En=1, RS = 1
  PORTD &= B11110111;                     // 11. En=0. Command is sent on trailing edge of enable
  _delay_us(1600);                        // 12. Wait >40us for command to process
}
*/