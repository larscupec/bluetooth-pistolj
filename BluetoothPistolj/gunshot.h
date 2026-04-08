/*
 * gunshot.h
 *
 *  Created on: 26. ožu 2026.
 *      Author: lars
 */

#ifndef GUNSHOT_H_
#define GUNSHOT_H_

#include <limits.h>
#include "iirsos.h"

#define SAMPLE_COUNT 9600
#define ADSR_A_SAMPLE_COUNT 480
#define ADSR_D_SAMPLE_COUNT 480
#define ADSR_R_SAMPLE_COUNT 8640
#define ADSR_A_STEP 0.002087682672223382f
#define ADSR_D_STEP 0.001461377870564f
#define ADSR_R_STEP 0.000034722222222f
#define GAIN 10000
#define GUNSHOT_MODE_1 0
#define GUNSHOT_MODE_2 1
#define GUNSHOT_MODE_3 2
#define GUNSHOT_MODE_4 3

short sampleIndex = 0;
float adsr = 0.0f;

short filter(float sample, short sampleIndex, short mode)
{
    float filteredSample = 0.0f;

    switch (mode)
    {
    case GUNSHOT_MODE_1:
        filteredSample = iirsos(sample, a_1000, b_1000);
        break;
    case GUNSHOT_MODE_2:
        filteredSample = iirsos(sample, a_700, b_700);
        break;
    case GUNSHOT_MODE_3:
        filteredSample = iirsos(sample, a_400, b_400);
        break;
    case GUNSHOT_MODE_4:
        filteredSample = iirsos(sample, a_100, b_100);
    }

    // Apply ADSR envelope
    if (sampleIndex < ADSR_A_SAMPLE_COUNT)
    {
        filteredSample *= adsr;
        adsr += ADSR_A_STEP;
    }
    else if (sampleIndex >= ADSR_A_SAMPLE_COUNT
            && sampleIndex < ADSR_A_SAMPLE_COUNT + ADSR_D_SAMPLE_COUNT)
    {
        filteredSample *= adsr;
        adsr -= ADSR_D_STEP;
    }
    else if (sampleIndex >= ADSR_A_SAMPLE_COUNT + ADSR_D_SAMPLE_COUNT
            && sampleIndex < SAMPLE_COUNT)
    {
        filteredSample *= adsr;
        adsr -= ADSR_R_STEP;
    }

    // Apply gain
    filteredSample *= GAIN;

    // Clip peaks
    if (filteredSample > (float) SHRT_MAX)
    {
        filteredSample = (float) SHRT_MAX;
    }
    else if (filteredSample < (float) SHRT_MIN)
    {
        filteredSample = (float) SHRT_MIN;
    }

    return (short) filteredSample;
}

void playGunshot(short mode)
{
    // Reset parameters
    adsr = 0.0f;

    for (sampleIndex = 0; sampleIndex < SAMPLE_COUNT; sampleIndex++)
    {
        float sample = -1 + 2 * (float) rand() / RAND_MAX; // Random number between -1 and 1
        output_left_sample(filter(sample, sampleIndex, mode));
    }
}

#endif /* GUNSHOT_H_ */
