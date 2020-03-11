#include "sunsensor.h"

int get_angle(float *dest, unsigned short *src)
{
    SunSensor tmp;
    tmp.output[0] = src[0] * POWER_SUPPLY / 1024;
    tmp.output[1] = src[1] * POWER_SUPPLY / 1024;
    tmp.output[2] = src[2] * POWER_SUPPLY / 1024;
    tmp.output[3] = src[3] * POWER_SUPPLY / 1024;

    int status = A60_Algorithm(&tmp);

    dest[0] = tmp.angle[0];
    dest[1] = tmp.angle[1];

    return status;
}