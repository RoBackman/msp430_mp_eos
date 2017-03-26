#include <msp430.h>
#include "led.h"
#include "project.h"

void led_init()
{
    P1DIR |= (RED_LED | GREEN_LED);
    P1OUT &= ~(RED_LED | GREEN_LED);
}

void led_red_on()
{
    P1OUT |= RED_LED;
}

void led_red_off()
{
    P1OUT &= ~RED_LED;
}

void led_red_toggle()
{
    P1OUT ^= RED_LED;
}

void led_green_on()
{
    P1OUT |= GREEN_LED;
}

void led_green_off()
{
    P1OUT &= ~GREEN_LED;
}

void led_green_toggle()
{
    P1OUT ^= GREEN_LED;
}