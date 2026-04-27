/*
 * play.h
 *
 *  Created on: 25. tra 2026.
 *      Author: lars
 */

#ifndef PLAY_H_
#define PLAY_H_

#define GAIN 30000

extern const int SAMPLE_RATE;
int playedSound = FALSE;
union {
    Uint32 uint;
    short channel[2];
} outputData;

void play(float (*sound)(int), int sampleCount, int repeat)
{
    if (!repeat && playedSound)
    {
        return;
    }

    DSK6713_LED_on(0);

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

    DSK6713_LED_off(0);

    playedSound = TRUE;
}

#endif /* PLAY_H_ */
