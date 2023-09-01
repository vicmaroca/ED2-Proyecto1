// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void USART_RX_config (uint16_t baudrate);

void USART_TX_config (uint16_t baudrate);

void USART_write_char (char c);

char USART_read_char(void);

#endif	/* USART_H */

