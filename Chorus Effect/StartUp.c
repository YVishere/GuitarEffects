#include "DSP_Config.h"
#include "Echo.h"
#include "math.h"

volatile float Beta[24000]; //N----> bufferlength
volatile float Beta1[25264];
volatile float Beta2[25806];
#pragma DATA_SECTION (Beta, "CE0");
#pragma DATA_SECTION (Beta1, "CE0");
#pragma DATA_SECTION (Beta2, "CE0");

void StartUp()
{
    // startup code required for Echo project
    ZeroBuffer();   // set all buffer values to zero
    float fnaught = 2;
    float Fs = 48000.0;
    float R = 1200.0;
    float arg, arg1, arg2;
    int N = 24000;
    int N1 = 25264;
    int N2 = 25806;
    int i;
//    for (i=0; i<N; i++){
//        arg = 2*3.14*(fnaught/Fs)*i;
//
//        Beta[i] = R*(1 - cos(arg))/2;
//        Beta1[i] = R*(1-cos(arg))/2;
//        Beta2[i] = R*(1 - cos(arg))/2;
//    }

    for (i=0; i<N; i++){
            arg = 2*3.14*(fnaught/Fs)*i;
            Beta[i] = R*(1 - cos(arg))/2;
        }

    for (i=0; i<N1; i++){
                arg1 = 2*3.14*(fnaught*0.95/Fs)*i;
                Beta1[i] = R*0.98*(1 - cos(arg1))/2;
            }

    for (i=0; i<N2; i++){
                arg2 = 2*3.14*(fnaught*0.93/Fs)*i;
                Beta2[i] = R*1.05*(1 - cos(arg2))/2;
            }
}
