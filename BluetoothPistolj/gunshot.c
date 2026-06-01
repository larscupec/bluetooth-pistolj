/*
 * gunshot.c
 *
 *  Created on: 12. svi 2026.
 *      Author: lars
 */

#include <gunshot.h>
#include <iirsos.h>
#include <stdlib.h>

#define ADSR_A_SAMPLE_COUNT 480
#define ADSR_D_SAMPLE_COUNT 480
#define ADSR_R_SAMPLE_COUNT 8640
#define ADSR_A_STEP 0.002087682672223382
#define ADSR_D_STEP 0.001461377870564
#define ADSR_R_STEP 0.000034722222222

IIRFilter LPF_1000 = { { { -1.63299316E+00, 6.90598923E-01 } }, { {
        1.44014403E-02, 2.88028807E-02, 1.44014403E-02 } },
                       { 0 } };
IIRFilter LPF_500 = { { { -1.81534108E+00, 8.31005589E-01 } }, { {
        3.91612666E-03, 7.83225332E-03, 3.91612666E-03 } },
                      { 0 } };
float adsr = 0.0f;

float gunshot(IIRFilter* filter, int sampleIndex)
{
    // Reset ADSR
    if (sampleIndex == 0)
    {
        adsr = 0.0f;
    }

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
