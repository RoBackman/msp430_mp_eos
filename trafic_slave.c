#include <msp430.h>
#include <stdbool.h>
#include "trafic.h"
#include "ports.h"
#include "led.h"
#include "uart.h"
#include "project.h"

#define ACK 0x00

// Times in seconds
#define STARTING_BLINK_INTERVAL 1
#define STARTING_DURATION 10
#define RED_DURATION 10
#define RED_AND_AMBER_DURATION 1
#define GREEN_DURATION 10
#define AMBER_DURATION 1

//Global state variable
static light_state state;
//Global ack received variable
static bool ack_received;

static tLong time_in_state = 0;

void trafic_lights_init(const light_state START_STATE)
{
    state = START_STATE;
    ack_received = false;
}

void trafic_lights_update(void)
{

    switch (state)
    {
    case OUT_OF_ORDER:
	if (++time_in_state % STARTING_BLINK_INTERVAL == 0)
	{
	    led_red_toggle();
	    led_green_toggle();
	}
	break;
    case RED:
	//Other way arround to make this light opposite masters RED
	led_red_off();
	led_green_on();
	break;
    case RED_AND_AMBER:
	led_red_on();
	led_green_on();
	break;
    case GREEN:
	//Other way arround to make this light opposite masters GREEN
	led_red_on();
	led_green_off();
	break;
    case AMBER:
	led_red_on();
	led_green_on();
	break;
    }
}

void trafic_lights_uart(char message)
{
    state = message;
    uart_write_char(ACK);
}
