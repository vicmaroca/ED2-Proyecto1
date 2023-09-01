/*
 * File:   us_prueba.c
 * Author: pablo
 *
 * Created on August 20, 2023, 9:53 AM
 */

////*****************************************************************************
//// Palabras de configuración
////*****************************************************************************
//// CONFIG1
//#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
//#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
//#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
//#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
//#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
//#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
//#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
//#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
//#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
//
//// CONFIG2
//#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
//#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//
//// #pragma config statements should precede project file includes.
//// Use project enums instead of #define for ON and OFF.
//
////*****************************************************************************
//// Definición e importación de librerías
////*****************************************************************************
//#include <stdint.h>
//#include <xc.h>
//#include "LCD_8b.h"
//#include "Interrupciones.h"
////*****************************************************************************
//// Definición de variables
////*****************************************************************************
//#define _XTAL_FREQ 8000000
//#define RS RE0
//#define EN RE1
//#define D0 RD0
//#define D1 RD1
//#define D2 RD2
//#define D3 RD3
//#define D4 RD4
//#define D5 RD5
//#define D6 RD6
//#define D7 RD7
//volatile int a;
////*****************************************************************************
//// Definición de funciones para que se puedan colocar después del main de lo 
//// contrario hay que colocarlos todas las funciones antes del main
////*****************************************************************************
//void setup(void);
////*****************************************************************************
//// Código de Interrupción 
////*****************************************************************************
//void __interrupt() isr(void){
//    if(INTCONbits.RBIF)
//    {
//        INTCONbits.RBIE = 0;    //Deshabilitando On-Change Interrupt
//        
//        if (PORTBbits.RB4 == 1){    //Si ECHO en HIGH
//            TMR1ON = 1;     //Empieza TMR1
//        }
//        if (PORTBbits.RB4 == 0){    //Si ECHO en LOW
//            TMR1ON = 0;     //Para TMR1
//            a = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            PORTA = TMR1H;
//        }
//        INTCONbits.RBIF = 0;       //Clear PORTB On-Change Interrupt flag
//        INTCONbits.RBIE = 1;       //Enable PORTB On-Change Interrupt
//    } 
//}
////*****************************************************************************
//// Main
////*****************************************************************************
//void main(void) {
//    setup();
//    Lcd_Clear(); 
//    //*************************************************************************
//    // Loop infinito
//    //*************************************************************************
//    while(1){
//        TMR1H = 0;      //Inicializando TMR1H en 0
//        TMR1L = 0;      //Inicializando TMR1L en 0
//        
//        PORTBbits.RB0 = 1;  //TRIGGER en HIGH
//        __delay_us(10);     //10us delay
//        PORTBbits.RB0 = 0;  //TRIGGER en LOW
//        
//        __delay_ms(100);     //Esperando el ECHO
//        
//        a = a + 1;                        //Error Correction Constant
//
//        if(a>=2 && a<=400)                //Check whether the result is valid or not
//        {
//          Lcd_Clear();
//          Lcd_Set_Cursor(1,1);
//          Lcd_Write_String("Distance = ");
//
//          Lcd_Set_Cursor(1,14);
//          Lcd_Write_Char(a%10 + 48);
//
//          a = a/10;
//          Lcd_Set_Cursor(1,13);
//          Lcd_Write_Char(a%10 + 48);
//
//          a = a/10;
//          Lcd_Set_Cursor(1,12);
//          Lcd_Write_Char(a%10 + 48);
//
//          Lcd_Set_Cursor(1,15);
//          Lcd_Write_String("cm");
//        }
//        else
//        {
//          Lcd_Clear();
//          Lcd_Set_Cursor(1,1);
//          Lcd_Write_String("Out of Range");
//        }
//        __delay_ms(400);
//        
//        
//    }
//    return;
//}
////*****************************************************************************
//// Función de Inicialización
////*****************************************************************************
//void setup(void){
//    ANSEL = 0;
//    ANSELH = 0;             // I/O Digitales
//    
//    TRISBbits.TRISB4 = 1;   //RB4 como Input (ECHO)
//    TRISBbits.TRISB0 = 0;   // RBO como Output (TRIGGER)
//    
//    TRISD = 0;      // PINES de LCD como Output
//    TRISE = 0;      // Enable & RW de LCD
//    
//    TRISA = 0;
//    PORTA = 0;
//    
//    ioc_init(4);
//    
//    // Configuracion oscilador interno
//    OSCCONbits.IRCF = 0b111; // 8MHz
//    OSCCONbits.SCS = 1;
//    
//    Lcd_Init();     //Inicializar el la LCD
//    T1CON = 0x10;   //Initialize Timer Module
////    T1CONbits.T1CKPS = 0b01;    //1:2 Prescaler Value
////    T1CONbits.TMR1CS = 0;       //Internal clock (FOSC/4)
////    T1CONbits.TMR1ON = 0;       //TMR1 off
//}

//*****************************************************************************
// Palabras de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <xc.h>
#include "LCD_8b.h"
#include "Interrupciones.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
volatile int a;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
    if(INTCONbits.RBIF)
    {
        INTCONbits.RBIE = 0;    //Deshabilitando On-Change Interrupt
        
        if (PORTBbits.RB4 == 1){    //Si ECHO en HIGH
            TMR1ON = 1;     //Empieza TMR1
        }
        if (PORTBbits.RB4 == 0){    //Si ECHO en LOW
            TMR1ON = 0;     //Para TMR1
            a = (TMR1L | (TMR1H<<8))*0.017; //Calculate Distance
            PORTA = TMR1H;
        }
        INTCONbits.RBIF = 0;       //Clear PORTB On-Change Interrupt flag
        INTCONbits.RBIE = 1;       //Enable PORTB On-Change Interrupt
    } 
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Clear(); 
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        TMR1H = 0;      //Inicializando TMR1H en 0
        TMR1L = 0;      //Inicializando TMR1L en 0
        
        PORTBbits.RB0 = 1;  //TRIGGER en HIGH
        __delay_us(10);     //10us delay
        PORTBbits.RB0 = 0;  //TRIGGER en LOW
        
        __delay_ms(100);     //Esperando el ECHO
        
        a = a + 1;                        //Error Correction Constant

        if(a>=2 && a<=25)                //Check whether the result is valid or not
        {
          Lcd_Clear();
          Lcd_Set_Cursor(1,1);
          Lcd_Write_String("Distance = ");

          Lcd_Set_Cursor(1,14);
          Lcd_Write_Char(a%10 + 48);

          a = a/10;
          Lcd_Set_Cursor(1,13);
          Lcd_Write_Char(a%10 + 48);

          a = a/10;
          Lcd_Set_Cursor(1,12);
          Lcd_Write_Char(a%10 + 48);

          Lcd_Set_Cursor(1,15);
          Lcd_Write_String("cm");
        }
        else
        {
          Lcd_Clear();
          Lcd_Set_Cursor(1,1);
          Lcd_Write_String("Out of Range");
        }
        __delay_ms(400);
        
        
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;             // I/O Digitales
    
    TRISBbits.TRISB4 = 1;   //RB4 como Input (ECHO)
    TRISBbits.TRISB0 = 0;   // RBO como Output (TRIGGER)
    
    TRISD = 0;      // PINES de LCD como Output
    TRISE = 0;      // Enable & RW de LCD
    
    TRISA = 0;
    PORTA = 0;
    
    ioc_init(4);
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 1;
    
    Lcd_Init();     //Inicializar el la LCD
    T1CON = 0x10;   //Initialize Timer Module
//    T1CONbits.T1CKPS = 0b01;    //1:2 Prescaler Value
//    T1CONbits.TMR1CS = 0;       //Internal clock (FOSC/4)
//    T1CONbits.TMR1ON = 0;       //TMR1 off
}
