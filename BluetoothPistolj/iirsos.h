/*
 * iirsos.h
 *
 *  Created on: 26. ožu 2026.
 *      Author: lars
 */

#ifndef IIRSOS_H_
#define IIRSOS_H_

#include "LPF.cof"

typedef struct {
    float (*a)[2];
    float (*b)[3];
    float (*w)[2];
} IIRFilter;

float iirsos(float input, IIRFilter* filter)
{
    int section; //index for section number
    float wn, yn; //intermediate and output values in each stage

    for (section = 0; section < NUM_SECTIONS; section++)
    {
        wn = input - filter->a[section][0] * filter->w[section][0]
                - filter->a[section][1] * filter->w[section][1];
        yn = filter->b[section][0] * wn + filter->b[section][1] * filter->w[section][0]
                + filter->b[section][2] * filter->w[section][1];
        filter->w[section][1] = filter->w[section][0];
        filter->w[section][0] = wn;
        input = yn; //output of current section will be input to next
    }

    return yn;
}

#endif /* IIRSOS_H_ */
