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
    
    if (!ack_received)
    {
        state = OUT_OF_ORDER;
        time_in_state = 0;
    }
    

    switch (state)
    {
    case OUT_OF_ORDER:
        if (time_in_state % STARTING_BLINK_INTERVAL == 0)
        {
            led_red_toggle();
            led_green_toggle();
        }

        if (++time_in_state == STARTING_DURATION)
        {
            //UART_Write("Startup complete switching to RED\r\n");
            state = RED;
            time_in_state = 0;
        }
        break;
    case RED:
        led_red_on();
        led_green_off();

        if (++time_in_state == RED_DURATION)
        {
            //UART_Write("Switching state to RED_AND_AMBER\r\n");
            state = RED_AND_AMBER;
            time_in_state = 0;
        }

        if (isPressed() == BUTTON)
        {
            //UART_Write("Switching state to GREEN on user request\r\n");
            state = RED_AND_AMBER;
            time_in_state = 0;
        }
        break;
    case RED_AND_AMBER:
        led_red_on();
        led_green_on();

        if (++time_in_state == RED_AND_AMBER_DURATION)
        {
            //UART_Write("Switching state to GREEN\r\n");
            state = GREEN;
            time_in_state = 0;
        }
        break;
    case GREEN:
        led_red_off();
        led_green_on();

        if (++time_in_state == GREEN_DURATION)
        {
            //UART_Write("Switching state to AMBER\r\n");
            state = AMBER;
            time_in_state = 0;
        }
        break;
    case AMBER:
        led_red_on();
        led_green_on();

        if (++time_in_state == AMBER_DURATION)
        {
            //UART_Write("Switching state to RED\r\n");
            state = RED;
            time_in_state = 0;
        }
        break;
    }

    //Send current state to all slaves
    uart_write_char(state);
    ack_received = false;
}

void trafic_lights_uart(char message)
{
    if (message == ACK)
    {
        ack_received = true;
    }
}
