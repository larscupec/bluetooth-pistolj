/*
 * laser.h
 *
 *  Created on: 25. tra 2026.
 *      Author: lars
 */

#ifndef LASER_H_
#define LASER_H_

#include <math.h>
#include "play.h"

#define LASER_SAMPLE_COUNT 7200
#define M_PI (3.14159265358979323846264338327950288)

extern const int SAMPLE_RATE;
float f_start = 2000.0f;
float f_end = 200.0f;
float phase = 0.0f;
float two_pi = 2.0f * M_PI;

float laser(int sampleIndex, int square)
{
    float t = (float) sampleIndex / SAMPLE_RATE;

    // Linear frequency sweep
    float f = f_start
            + (f_end - f_start) * ((float) sampleIndex / LASER_SAMPLE_COUNT);

    // Vibrato
    double vib = 50.0 * sin(2.0 * M_PI * 30.0 * t);
    f += vib;

    // Phase accumulation
    phase += two_pi * f / SAMPLE_RATE;

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

#endif /* LASER_H_ */
