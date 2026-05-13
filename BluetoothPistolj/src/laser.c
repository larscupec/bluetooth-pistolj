/*
 * laser.c
 *
 *  Created on: 13. svi 2026.
 *      Author: lars
 */

#include <math.h>
#include <csl_stdinc.h>
#include <laser.h>

#define M_PI (3.14159265358979323846264338327950288)
#define F_START 2000.0
#define F_END 200.0
#define TWO_PI (2.0 * M_PI)

extern const int SAMPLE_RATE;
float phase = 0.0f;

float laser(int sampleIndex, int square)
{
    float t = (float) sampleIndex / SAMPLE_RATE;

    // Linear frequency sweep
    float f = F_START
            + (F_END - F_START) * ((float) sampleIndex / LASER_SAMPLE_COUNT);

    // Vibrato
    double vib = 50.0 * sin(2.0 * M_PI * 30.0 * t);
    f += vib;

    // Phase accumulation
    phase += TWO_PI * f / SAMPLE_RATE;

    // Oscillator
    float sample;

    if (square)
    {
        sample = (sin(phase) > 0) ? 1.0f : -1.0f;
    }
    else
    {
        sample = sin(phase);
    }

    // Exponential decay envelope
    float env = exp(-10.0 * t);

    return sample * env;
}

float laserA(int sampleIndex)
{
    return laser(sampleIndex, FALSE);
}

float laserB(int sampleIndex)
{
    return laser(sampleIndex, TRUE);
}
