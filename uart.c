#include <msp430.h>
#include "uart.h"
#include "project.h"
#include "trafic.h"

void uart_init(void)
{
    //Initzialize USCI state machine
    UCA0CTL1 |= UCSWRST;
    //Set clock to SMCLK
    UCA0CTL1 |= UCSSEL_2;

    //Set 9600 baud @ 16MHz
    UCA0BR0 = 0x83;
    UCA0BR1 = 0x06;

    //Set 9600 baud @ 1MHz (not used for now)
    //UCA0BR0 = 0x68;
    //UCA0BR1 = 0x00;

    //Set P1.1 = RXD and P1.2 = TXD
    P1SEL |= RXD + TXD;
    P1SEL2 |= RXD + TXD;

    //Remove UART from reset
    UCA0CTL1 &= ~UCSWRST;

#ifndef MASTER
    //Enable USCI_A0 RX interrupt
    UC0IE |= UCA0RXIE;
#endif
}

void uart_write_char(char byte)
{
    //Write char to buffer
    UCA0TXBUF = byte;
}

void uart_write(char *str)
{
    //While not terminated send char and increment pointer
    while (*str){
        uart_write_char(*str++);
    }
}

char uart_read(void)
{
    //Return a char from buffer
    return UCA0RXBUF;
}

__attribute__((interrupt(USCIAB0RX_VECTOR)))void USCI0RX_ISR(void)
{
    //Forward received message to be handeled by tarfic logic
    trafic_lights_uart(UCA0RXBUF);
}