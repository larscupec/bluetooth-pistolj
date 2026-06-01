/*
 * play.c
 *
 *  Created on: 13. svi 2026.
 *      Author: lars
 */

#include <limits.h>
// #include <dsk6713_led.h>
#include <csl_stdinc.h>
#include <play.h>

#define GAIN 30000

extern const int SAMPLE_RATE;
extern int playedSound;
union {
    Uint32 uint;
    short channel[2];
} outputData;

void play(float (*sound)(int), int sampleCount, int repeat, int led)
{
    if (!repeat && playedSound)
    {
        return;
    }

    // DSK6713_LED_on(led);

    int sampleIndex;

    for (sampleIndex = 0; sampleIndex < (repeat ? sampleCount / 2 : sampleCount); sampleIndex++)
    {
        float sample = (*sound)(sampleIndex);

        // Apply gain
        sample *= GAIN;

        // Clip peaks
        if (sample > (float) SHRT_MAX)
        {
            sample = (float) SHRT_MAX;
        }
        else if (sample < (float) SHRT_MIN)
        {
            sample = (float) SHRT_MIN;
        }

        outputData.channel[0] = (short) sample;
        outputData.channel[1] = (short) sample;

        output_sample(outputData.uint);
    }

    // DSK6713_LED_off(led);

    playedSound = TRUE;
}
