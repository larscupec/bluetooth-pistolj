/* codec support */
#include <dsk6713_aic23.h>
/* LED */
#include <dsk6713_led.h>
/* GPIO */
#include <csl_gpio.h>
#include <csl_gpiohal.h>
#include <csl_irq.h>
/* Standard C libs */
#include <stdlib.h>
#include <time.h>
/* sound effects */
#include <include/gunshot.h>
#include <include/laser.h>
#include <include/play.h>

Uint32 fs = DSK6713_AIC23_FREQ_24KHZ;
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint32 inputsource = DSK6713_AIC23_INPUT_LINE;
GPIO_Handle hGpio;
GPIO_Config GPIOCfg = { 0x00000000, /* gpgc */
                        0x00000F00, /* gpen (set pins 8-11 as inputs) */
                        0x00000000, /* gdir */
                        0x00000000, /* gpval */
                        0x00000000, /* gphm */
                        0x00000000, /* gplm */
                        0x00000000 /* gppol */
};
const unsigned int statePins = GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11;
const int SAMPLE_RATE = 24000;
int playedSound = FALSE;
#define LED_1 0
#define LED_2 1
#define LED_3 2
#define LED_4 3

void main(void)
{
    comm_poll();
    DSK6713_LED_init();

    hGpio = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_config(hGpio, &GPIOCfg);

    srand(time(0));

    while (1)
    {
        unsigned int pistolState = GPIO_read(hGpio, statePins);

        // Buduci da funkcija GPIO_read vraca stanje registra koje izgleda
        // npr. 1111 0000 0000, moramo pomaknuti bitove 8-11 na pocetak
        // tako da ih shiftamo osam puta u desno da bi dobili zeljena
        // stanja pistolja.
        pistolState >>= 8;

        /*
         * Pistol States:
         *
         *  0 - MOD=1,RAF=0,TRIG=0
         *  1 - MOD=1,RAF=0,TRIG=1
         *  2 - MOD=1,RAF=1,TRIG=0
         *  3 - MOD=1,RAF=1,TRIG=1

         *  4 - MOD=2,RAF=0,TRIG=0
         *  5 - MOD=2,RAF=0,TRIG=1
         *  6 - MOD=2,RAF=1,TRIG=0
         *  7 - MOD=2,RAF=1,TRIG=1

         *  8 - MOD=3,RAF=0,TRIG=0
         *  9 - MOD=3,RAF=0,TRIG=1
         * 10 - MOD=3,RAF=1,TRIG=0
         * 11 - MOD=3,RAF=1,TRIG=1

         * 12 - MOD=4,RAF=0,TRIG=0
         * 13 - MOD=4,RAF=0,TRIG=1
         * 14 - MOD=4,RAF=1,TRIG=0
         * 15 - MOD=4,RAF=1,TRIG=1
         *
         */

        switch (pistolState)
        {
        case 0:
        case 4:
        case 8:
        case 12:
            playedSound = FALSE;
            break;
        case 1:
            play(gunshotA, GUNSHOT_SAMPLE_COUNT, FALSE, LED_1);
            break;
        case 3:
            play(gunshotA, GUNSHOT_SAMPLE_COUNT, TRUE, LED_1);
            break;
        case 5:
            play(gunshotB, GUNSHOT_SAMPLE_COUNT, FALSE, LED_2);
            break;
        case 7:
            play(gunshotB, GUNSHOT_SAMPLE_COUNT, TRUE, LED_2);
            break;
        case 9:
            play(laserA, LASER_SAMPLE_COUNT, FALSE, LED_3);
            break;
        case 11:
            play(laserA, LASER_SAMPLE_COUNT, TRUE, LED_3);
            break;
        case 13:
            play(laserB, LASER_SAMPLE_COUNT, FALSE, LED_4);
            break;
        case 15:
            play(laserB, LASER_SAMPLE_COUNT, TRUE, LED_4);
        }
    }
}
