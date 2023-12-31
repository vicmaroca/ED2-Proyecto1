/*
 * File:   internet.c
 * Author: pablo
 *
 * Created on August 19, 2023, 10:56 PM
 */

/*
  Distance meter using HC-SR04 ultrasonic sensor and PIC16F887 CCS C code.
  Echo and trigger pins of the HC-SR04 are connected to RB0 and RB1 respectively.
*/
 
////LCD module connections
//#define LCD_RS_PIN     PIN_D0
//#define LCD_RW_PIN     PIN_D1
//#define LCD_ENABLE_PIN PIN_D2
//#define LCD_DATA4      PIN_D3
//#define LCD_DATA5      PIN_D4
//#define LCD_DATA6      PIN_D5
//#define LCD_DATA7      PIN_D6
//End LCD module connections
 
#include <16F887.h>
#fuses INTRC_IO, NOMCLR, NOBROWNOUT, NOLVP
#use delay(clock = 8MHz)
#use fast_io(B)
#include <lcd.c>
 
int16 i, distance;
int1 wait_sensor(){
  i = 0;
  set_timer1(0);                                 // Reset Timer1
  while(!input(PIN_B0) && (i < 1000))
    i = get_timer1();                            // Read Timer1 and store its value in i
  if(i > 990)
    return 0;
  else
    return 1;
}
int16 get_distance(){
  i = 0;
  set_timer1(0);
  while(input(PIN_B0) && (i < 25000))
    i = get_timer1();
  return i;
}
void main(){
  setup_oscillator(OSC_8MHZ);                    // Set internal oscillator to 8MHz
  output_b(0);
  set_tris_b(1);                                 // Configure RB0 pin as input
  delay_ms(1000);
  lcd_init();                                    // Initialize LCD module
  lcd_putc('\f');                                // Clear LCD
  lcd_gotoxy(4, 1);                              // Go to column 4 row 1
  lcd_putc("Distance:");
  SETUP_TIMER_1(T1_INTERNAL | T1_DIV_BY_2);      // Configure Timer 1 to increment by 1 every 1 us
  while(TRUE){
    // Send 10us pulse to HC-SR04 Trigger pin
    output_high(PIN_B1);
    delay_us(10);
    output_low(PIN_B1);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor()){
      distance = get_distance();
      PORTA = distance;
//      if(distance > 24990){
////        lcd_gotoxy(3, 2);                        // Go to column 3 row 2
////        lcd_putc("Out Of Range");
//      }
//      else {
//        distance = i/58;                         // Calculate the distance
////        lcd_gotoxy(3, 2);                        // Go to column 3 row 2
////        lcd_putc("       cm   ");
////        lcd_gotoxy(6, 2);                        // Go to column 6 row 2
////        printf(lcd_putc, "%3Lu", distance);
//      }
//    }
////    else {
////      lcd_gotoxy(3, 2);                          // Go to column 3 row 2
////      lcd_putc("  Time Out  ");
////    }
  delay_ms(100);
  }
}
// End of code