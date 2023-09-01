/*
 * File:   PIC_ESP32.c
 * Author: pablo
 *
 * Created on August 24, 2023, 10:15 PM
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
#include <xc.h>
#include "LCD_8b.h"
#include "Interrupciones.h"
#include "map.h"
#include "USART.h"
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

uint8_t ultra_slave;
uint8_t c_ultra_slave;
uint8_t d_ultra_slave;
uint8_t u_ultra_slave;

uint8_t photo_slave;
uint8_t c_photo_slave;
uint8_t d_photo_slave;
uint8_t u_photo_slave;

uint8_t rain_slave;
uint8_t c_rain_slave;
uint8_t d_rain_slave;
uint8_t u_rain_slave;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint8_t readSlave(uint8_t s_addrs);
void writeSlave(uint8_t s_addrs, uint8_t data);
void USART_write_string (uint8_t L, char c[]);
//uint8_t readRTC(uint8_t r_addrs);
//uint8_t int2char(uint8_t num);
//void writeRTC(uint8_t r_addrs, uint8_t data);
//******************************************************************************
// Vector de Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    if(INTCONbits.RBIF)
    {
        if(PIR1bits.RCIF)
        {
            PORTB = USART_read_char();
        }
    } 
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Clear();
    while(1){
            // *****************************************************************
            //LEYENDO DATA DEL ESCLAVO CON ULTRASÓNICO
            ultra_slave = readSlave(0x50);  //Leyendo el esclavo con direccion 0x50
            //Separando unidades, decenas y centenas
            c_ultra_slave = ultra_slave / 100;
            d_ultra_slave = (ultra_slave / 10) % 10;
            u_ultra_slave = ultra_slave % 10;
            char esclavo_ultra[] = {c_ultra_slave+48, d_ultra_slave+48, u_ultra_slave+48, '\0'};
            //IMPLEMENTANDO LÓGICA DE FRENADO
            if (ultra_slave >= 2 && ultra_slave <= 15){
//                PORTA = 0b11000011;         //Se enciende luces de frenado
                writeSlave(0x50, 60);      //Enviando dato para que apague motor
                
                Lcd_Clear();                //Se limpia la pantalla        
                Lcd_Set_Cursor(1,5);
                Lcd_Write_String("E-BRAKE!");    // Frenado de emergencia
                __delay_ms(1000);
                Lcd_Clear(); 
            }else{
//                PORTA = 0;
                writeSlave(0x50, 129);      //Enviando dato para que encienda motor
                //MOSTRANDO EN LCD DATOS SENSOR ULTRASONICO
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("SS: ");       //Security System
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String(esclavo_ultra);
            }
            //******************************************************************
            
            
            //******************************************************************
            //LEYENDO DATA DEL ESCLAVO CON FOTORESISTENCIA
            photo_slave = readSlave(0x60);  //Leyendo el esclavo con direccion 0x60
            photo_slave = (int) mapRange(photo_slave, 0, 185, 0, 100.0);
            //Separando unidades, decenas y centenas
            c_photo_slave = photo_slave / 100;
            d_photo_slave = (photo_slave / 10) % 10;
            u_photo_slave = photo_slave % 10;
            char esclavo_foto[] = {c_photo_slave+48, d_photo_slave+48, u_photo_slave+48, '\0'};
            //MOSTRANDO EN LCD DATOS SENSOR DE LUZ
            Lcd_Set_Cursor(1,5);
            Lcd_Write_String("LS:");       //Photo resistor
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String(esclavo_foto);
            //IMPLEMENTANDO LOGICA DE LA LUZ
            if (photo_slave >= 0 && photo_slave <= 33){
                PORTA = 0b00001111;         //Se enciende luces de frenado
            }
            if (photo_slave >= 34 && photo_slave <= 66){
                PORTA = 0b00000011;         //Se enciende luces de frenado
            }
            if (photo_slave >= 67 && photo_slave <= 110){
                PORTA = 0b00000000;         //Se enciende luces de frenado
            }
            //******************************************************************
            
            
            //******************************************************************
            //LEYENDO DATA DEL ESCLAVO CON SENSOR DE LLUVIA
            rain_slave = readSlave(0x70);   //Leyendo el esclavo con direccion 0x60
            rain_slave = (int) mapRange(rain_slave, 0, 255, 0, 100.0);
            //Separando unidades, decenas y centenas
            c_rain_slave = rain_slave / 100;
            d_rain_slave = (rain_slave / 10) % 10;
            u_rain_slave = rain_slave % 10;
            char esclavo_rain[] = {c_rain_slave+48, d_rain_slave+48, u_rain_slave+48, '\0'};
            //IMPLEMENTANDO LÓGICA DEL LIMPIAPARABRISAS
            if (rain_slave >= 34 && rain_slave <= 66){
                CCPR1L = 125;
                CCPR2L = 125;
                __delay_ms(1000);
                CCPR1L = 250;
                CCPR2L = 250;
                __delay_ms(1000);
                Lcd_Set_Cursor(1,13);
                Lcd_Write_String("LOW");       //Rain Sensor
            }
            if (rain_slave >= 67 && rain_slave <= 100){
                CCPR1L = 125;
                CCPR2L = 125;
                __delay_ms(500);
                CCPR1L = 250;
                CCPR2L = 250;
                __delay_ms(500);
                Lcd_Set_Cursor(1,13);
                Lcd_Write_String("HIG");       //Rain Sensor
            }
            if (rain_slave >= 0 && rain_slave <= 33){
                Lcd_Set_Cursor(1,13);
                Lcd_Write_String("   ");
                CCPR1L = 125;
                CCPR2L = 125;
            }
            //MOSTRANDO EN LCD DATOS SENSOR DE LLUVIA
            Lcd_Set_Cursor(1,9);
            Lcd_Write_String("RS: ");       //Rain Sensor
            Lcd_Set_Cursor(2,9);
            Lcd_Write_String(esclavo_rain);
            //******************************************************************
            
            
            //******************************************************************
            // ENVIANDO LOS DATOS A LA ESP32
            char men_ultra[] = {'D','1','-', c_ultra_slave+48, d_ultra_slave+48, u_ultra_slave+48, '\n'};
            USART_write_string (7, men_ultra);
            char men_foto[] = {'D','2','-',c_photo_slave+48, d_photo_slave+48, u_photo_slave+48, '\n'};
            USART_write_string (7, men_foto);
            char men_rain[] = {'D','3','-',c_rain_slave+48, d_rain_slave+48, u_rain_slave+48, '\n'};
            USART_write_string (7, men_rain);
            __delay_ms(1000);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    TRISE = 0;      // PINES PARA LCD
    TRISD = 0x00;   // PINES PARA LCD
    
    TRISA = 0;
    PORTA = 0;
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 1;
    
    // Configuracion PWM
    // PASO 1
    TRISCbits.TRISC2 = 1;   //RC2/CCP1 como entrada
    TRISCbits.TRISC1 = 1;   //RC1/CCP2 como entrada
    // PASO 2
    PR2 = 250;              // Periodo del tmr2 (2ms)
    // PASO 3
    CCP1CONbits.P1M = 0;    // mode single output (P1A modulated; P1B, P1C, P1D assigned as port pins) CCP1
    CCP1CONbits.CCP1M = 0b00001100;    // Modo PWM CCP1
    CCP2CONbits.CCP2M = 0b00001100;    // Modo PWM CCP2
    // PASO 4
    CCPR1L = 0x0f;          // Duty cycle inicial CCP1
    CCPR2L = 0x0f;          // Duty cycle inicial CCP2
    CCP1CONbits.DC1B1 = 0;  // bits menos significativos CCP1
    CCP1CONbits.DC1B0 = 0;  // bits menos singificativos CCP1
    CCP2CONbits.DC2B1 = 0;  // bits menos singificativos CCP2
    CCP2CONbits.DC2B0 = 0;  // bits menos singificativos CCP2
    // PASO 5  
    PIR1bits.TMR2IF = 0;    
    T2CONbits.T2CKPS = 0b11;    // Prescaler 1:16
    T2CONbits.TMR2ON = 1;       // Prender TMR2
    // PASO 6
    while (!PIR1bits.TMR2IF);   // esperar un ciclo
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC2 = 0;       // Habilitar salida CCP1
    TRISCbits.TRISC1 = 0;       // Habilitar salida CCP2
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C (Standar Mode)
    
    //INICIALIZANDO COMUNICACIÓN SERIAL
    USART_RX_config(9600);
    USART_TX_config(9600);
    
    Lcd_Init();                     //Inicializar el la LCD
}
//******************************************************************************
// Funcion para leer del RTC
//******************************************************************************
uint8_t readRTC(uint8_t r_addrs){
    uint8_t temp;
    I2C_Master_Start();
    I2C_Master_Write(0xD0); //Slave Address en modo write
    I2C_Master_Write(r_addrs); //Acceder a la dirección del registro de interes ej. Hr, Min, Sec...
    I2C_Master_Stop();
    __delay_ms(10);
       
    I2C_Master_Start();
    I2C_Master_Write(0xD1); //Leer los datos
    temp = I2C_Master_Read(0); // ACK 
    I2C_Master_Stop();
    __delay_ms(10);
    return temp;
}
//******************************************************************************
// Funcion para escribir al RTC
//******************************************************************************
void writeRTC(uint8_t r_addrs, uint8_t data){
    I2C_Master_Start();
    I2C_Master_Write(0xD0); //Slave Address en modo write
    I2C_Master_Write(r_addrs); //Acceder a la dirección del registro de interes ej. Hr, Min, Sec...
    I2C_Master_Write(data);    //Dato a escribir
    I2C_Master_Stop();
    __delay_ms(10);
}
//******************************************************************************
// Funcion para leer del PIC esclavo
//******************************************************************************
uint8_t readSlave(uint8_t s_addrs){
    uint8_t temp;
    I2C_Master_Start();
    I2C_Master_Write(s_addrs+1); //Slave address for reading
    temp = I2C_Master_Read(0); //Reading data 
    I2C_Master_Stop();
    __delay_ms(10);
    return temp;
}
//******************************************************************************
// Funcion para escribir al PIC esclavo
//******************************************************************************
void writeSlave(uint8_t s_addrs, uint8_t data){
    I2C_Master_Start();
    I2C_Master_Write(s_addrs); //Slave addres for writing
    I2C_Master_Write(data); //Writing data
    I2C_Master_Stop();
    __delay_ms(10);
}
//******************************************************************************
// Funcion para enviar datos serialmente
//******************************************************************************
void USART_write_string (uint8_t L, char c[]){
    for (int i=0; i<L; i++){
            __delay_ms(10);
            USART_write_char(c[i]);
            }
}