/*
 * File:   pruebaDC.c
 * Author: pablo
 *
 * Created on August 22, 2023, 3:13 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

// LIBRERIAS
#include <xc.h>
#include <stdint.h>
#include "Interrupciones.h"

/*
 * Constantes
 */
#define _XTAL_FREQ 1000000

/*
 * Prototipos de funcion
 */
void setup(void);

void __interrupt() isr(void){
    
    if (INTCONbits.RBIF) {  // Comprobar si la bandera de interrupción del Puerto B está activada
        if (RB0 == 0) {  // Comprobar si RB0 está en bajo (botón presionado)
            __delay_ms(100);  // Debounce para evitar rebotes del botón
            if (RB0 == 0) {
                RB1 = 1;  // Activar el motor
                while (RB0 == 0);  // Esperar hasta que el botón se suelte
                RB1 = 0;  // Apagar el motor cuando se suelta el botón
            }
        }
        INTCONbits.RBIF = 0;  // Limpiar la bandera de interrupción del Puerto B
    }
    
}    

void main(void) {
    
    setup();
    RB1 = 0;     // Inicialmente, apagar el motor
    while(1){
       
    }
}

void setup(void){
    // CONFIGURACION DE ENTRADAS Y SALIDAS
    // Pines digitales
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    PORTA = 0;
    TRISC = 0;
    
    TRISB1 = 0;  // Configurar RB1 como salida (control del motor)
    ioc_init(0);
    
    return;
}
