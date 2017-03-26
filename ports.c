#include <msp430.h>
#include "ports.h"
#include "project.h"

void ports_init() {
	P1DIR &= ~BUTTON;
	P1OUT |= BUTTON;
	P1REN |= BUTTON;
}

int isPressed() {
	return (~P1IN & BUTTON);
}