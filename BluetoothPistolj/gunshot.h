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
#include "play.h"

#define GUNSHOT_SAMPLE_COUNT 9600
#define ADSR_A_SAMPLE_COUNT 480
#define ADSR_D_SAMPLE_COUNT 480
#define ADSR_R_SAMPLE_COUNT 8640
#define ADSR_A_STEP 0.002087682672223382
#define ADSR_D_STEP 0.001461377870564
#define ADSR_R_STEP 0.000034722222222

float adsr = 0.0f;
IIRFilter LPF_1000 = {a_1000, b_1000, NULL};
IIRFilter LPF_500 = {a_500, b_500, NULL};

float gunshot(IIRFilter* filter, int sampleIndex)
{
    // Random number between -1 and 1
    float sample = -1 + 2 * (float) rand() / RAND_MAX;

    // Apply LPF
    float filteredSample = iirsos(sample, filter);

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
            && sampleIndex < GUNSHOT_SAMPLE_COUNT)
    {
        filteredSample *= adsr;
        adsr -= ADSR_R_STEP;
    }

    // Reset ADSR
    if (sampleIndex == GUNSHOT_SAMPLE_COUNT - 1)
    {
        adsr = 0.0f;
    }

    return filteredSample;
}

float gunshotA(int sampleIndex)
{
    return gunshot(&LPF_1000, sampleIndex);
}

float gunshotB(int sampleIndex)
{
    return gunshot(&LPF_500, sampleIndex);
}

#endif /* GUNSHOT_H_ */
