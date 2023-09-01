/*
 * File:   us_prueba.c
 * Author: pablo
 *
 * Created on August 22, 2023, 9:01 PM
 */

//*****************************************************************************
// Palabra de configuración
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
#include <pic16f887.h>
#include "I2C.h"
#include "Interrupciones.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
uint8_t adc_val;
volatile int distancia;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
//            SSPBUF = distancia;
            SSPBUF = PORTA;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
    if(INTCONbits.RBIF)
    {
        INTCONbits.RBIE = 0;    //Deshabilitando On-Change Interrupt
        
        if (PORTBbits.RB4 == 1){    //Si ECHO en HIGH
            TMR1ON = 1;     //Empieza TMR1
        }
        if (PORTBbits.RB4 == 0){    //Si ECHO en LOW
            TMR1ON = 0;     //Para TMR1
            distancia = (TMR1L | (TMR1H<<8))*0.017; //Calculate Distance
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
        
        distancia = distancia + 1;                        //Error Correction Constant
        
        PORTA = distancia;
        
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
    
//    TRISD = 0;      // PINES de LCD como Output
//    TRISE = 0;      // Enable & RW de LCD
    
    TRISA = 0;
    PORTA = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    ioc_init(4);
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 1;
    
    T1CON = 0x10;   //Initialize Timer Module
    
    I2C_Slave_Init(0x50);   
}