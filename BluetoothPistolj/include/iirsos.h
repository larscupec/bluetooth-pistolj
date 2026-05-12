/*
 * iirsos.h
 *
 *  Created on: 26. ožu 2026.
 *      Author: lars
 */

#ifndef INCLUDE_IIRSOS_H_
#define INCLUDE_IIRSOS_H_

#define NUM_SECTIONS 1

typedef struct {
    float a[NUM_SECTIONS][2];
    float b[NUM_SECTIONS][3];
    float w[NUM_SECTIONS][2];
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

#endif /* INCLUDE_IIRSOS_H_ */
