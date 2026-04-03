/* codec support */
#include <dsk6713_aic23.h>
/* LED */
#include <dsk6713_led.h>
/* DIP switch */
#include <dsk6713_dip.h>
/* GPIO */
#include <csl_gpio.h>
#include <csl_gpiohal.h>
#include <csl_irq.h>
/* Standard C libs */
#include <stdlib.h>
#include <time.h>
/* gunshot sound effect */
#include "gunshot.h"

Uint32 fs = DSK6713_AIC23_FREQ_24KHZ;
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint32 inputsource = DSK6713_AIC23_INPUT_LINE;

void main(void)
{
    comm_poll();
    DSK6713_LED_init();
    DSK6713_DIP_init();

    srand(time(0));

    while (1)
    {
        if (DSK6713_DIP_get(0) == 0)
        {
            DSK6713_LED_on(0);
            playGunshot();
            DSK6713_LED_off(0);
        }
    }
}
