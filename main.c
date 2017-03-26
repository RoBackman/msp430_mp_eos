#include <msp430.h>
#include "sheduler.h"
#include "led.h"
#include "uart.h"
#include "trafic.h"

//Helper when task cannot be run with argument
void uart_write_count(void)
{
    uart_write("C\n\r");
}


void main(void)
{
    //Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    //Init sheduler
    SCH_Init();
    //Init led output
    led_init();
    //Init button
    ports_init();
    //Init uart output
    uart_init();

    //Init trfic state machine
    trafic_lights_init(OUT_OF_ORDER);

    //Tur on leds to show they are working
    //led_red_on();
    //led_green_on();

    //Add tasks to task list
    //SCH_Add_Task(led_red_toggle, 0, 100);
    //SCH_Add_Task(led_green_toggle, 0, 200);
    //SCH_Add_Task(uart_write_count, 500, 100);
    SCH_Add_Task(trafic_lights_update, 0, 50);


    while (1)
    {
        //Dispach tasks set to run
        SCH_Dispatch_Tasks();
    }
}