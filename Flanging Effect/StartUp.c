#include "DSP_Config.h"
#include "Echo.h"
#include "math.h"

volatile float Beta[96000]; //N----> bufferlength
#pragma DATA_SECTION (Beta, "CE0");

void StartUp()
{
    // startup code required for Echo project
    ZeroBuffer();   // set all buffer values to zero
    float fnaught = 0.5;
    float Fs = 48000.0;
    float R = 96;
    float arg;
    int N = 96000;
    int i;
    for (i=0; i<N; i++){
        arg = 2*3.14*(fnaught/Fs)*i;
        Beta[i] = R*(1 - cos(arg))/2;
    }
}
