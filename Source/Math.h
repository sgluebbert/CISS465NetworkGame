#ifndef MATH_H
#define MATH_H



#include <cmath>



static double PI = 3.141592654;
static double TWOPI = 6.283185307;
static double TRIG_TABLE[72][2];

static void Initialize_Trig_Table() {
    for (int i = 0; i < 72; i++) {
        double temp = 2 * PI * i / 72;
        TRIG_TABLE[i][0] = sin(temp);
        TRIG_TABLE[i][1] = cos(temp);
    }
}



#endif