/*
 * iirsos.h
 *
 *  Created on: 26. ožu 2026.
 *      Author: lars
 */

#ifndef IIRSOS_H_
#define IIRSOS_H_

#include "LPF_1000.cof"

float w[NUM_SECTIONS][2] = { 0 };

float iirsos(float input)
{
    int section; //index for section number
    float wn, yn; //intermediate and output values in each stage

    for (section = 0; section < NUM_SECTIONS; section++)
    {
        wn = input - a[section][0] * w[section][0]
                - a[section][1] * w[section][1];
        yn = b[section][0] * wn + b[section][1] * w[section][0]
                + b[section][2] * w[section][1];
        w[section][1] = w[section][0];
        w[section][0] = wn;
        input = yn; //output of current section will be input to next
    }

    return yn;
}

#endif /* IIRSOS_H_ */
