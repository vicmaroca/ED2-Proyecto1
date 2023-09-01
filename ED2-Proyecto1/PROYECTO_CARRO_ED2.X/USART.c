#include "USART.h"

void USART_RX_config (uint16_t baudrate){
    TXSTAbits.SYNC = 0;     //Asincrono
    TXSTAbits.BRGH = 1;     // (High Baud Rate Select bit) en High Speed
    BAUDCTLbits.BRG16 = 1;  // (Baud Rate Generator bit) 16-bit Baud Rate Generator bit
    
    if (baudrate == 300){// Tener en cuenta que estos valores dependen de FOSC = 8MHz
        SPBRG = 6666;
    }else if (baudrate == 1200){
        SPBRG = 1666;
    }else if (baudrate == 2400){
        SPBRG = 832;
    }else if (baudrate == 9600){
        SPBRG = 207;
    }else if (baudrate == 10417){
        SPBRG = 191;
    }else if (baudrate == 19200){
        SPBRG = 103;
    }else if (baudrate == 57600){
        SPBRG = 34;
    }else if (baudrate == 115200){
        SPBRG = 16;
    }
    
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;     //Serial Port enabled
    RCSTAbits.RX9 = 0;      // 8-bit reception
    RCSTAbits.CREN = 1;     //(Continuous Receive Enable bit) asynchronous mode:Enable receiver
}

void USART_TX_config (uint16_t baudrate){
    TXSTAbits.SYNC = 0;     //Asincrono
    TXSTAbits.BRGH = 1;     // (High Baud Rate Select bit) en High Speed
    BAUDCTLbits.BRG16 = 1;  // (Baud Rate Generator bit) 16-bit Baud Rate Generator bit
    
    if (baudrate == 300){// Tener en cuenta que estos valores dependen de FOSC = 8MHz
        SPBRG = 6666;
    }else if (baudrate == 1200){
        SPBRG = 1666;
    }else if (baudrate == 2400){
        SPBRG = 832;
    }else if (baudrate == 9600){
        SPBRG = 207;
    }else if (baudrate == 10417){
        SPBRG = 191;
    }else if (baudrate == 19200){
        SPBRG = 103;
    }else if (baudrate == 57600){
        SPBRG = 34;
    }else if (baudrate == 115200){
        SPBRG = 16;
    }
    
    SPBRGH = 0;
    
    TXSTAbits.TXEN = 1;     // Transmit enabled
}

void USART_write_char (char c){
        if (PIR1bits.TXIF) //Siempre está en 1, a menos que esté enviando datos
        {
            TXREG = c;
        }
}

char USART_read_char(void){
    return RCREG;
}
