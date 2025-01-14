#include "DSP_Config.h" 
#include "Echo.h" 
#include "math.h"
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1
#define R 1200
#define R1 1176
#define R2 1260

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;

/* add any global variables here */
float xLeft, xRight, yLeft, yRight;
Uint32 oldest = 0; // index for buffer value
Uint32 oldest1 = 0; // index for buffer value
Uint32 oldest2 = 0; // index for buffer value

Uint32 index = 0; //Index of beta
Uint32 index1 = 0; //Index of beta
Uint32 index2 = 0; //Index of beta

#define BUFFER_LENGTH   1200 // buffer length in samples
#define BUFFER_LENGTH1   1176 // buffer length in samples
#define BUFFER_LENGTH2   1260 // buffer length in samples

#pragma DATA_SECTION (buffer, "CE0"); // put "buffer" in SDRAM
#pragma DATA_SECTION (buffer1, "CE0"); // put "buffer" in SDRAM
#pragma DATA_SECTION (buffer2, "CE0"); // put "buffer" in SDRAM

volatile float buffer[2][BUFFER_LENGTH]; // space for left + right
volatile float buffer1[2][BUFFER_LENGTH1]; // space for left + right
volatile float buffer2[2][BUFFER_LENGTH2]; // space for left + right

volatile float gain = 0.75; /* set gain value for echoed sample */
volatile float gain1 = 0.8;
volatile float gain2 = 0.95;

extern volatile float Beta[24000];
extern volatile float Beta1[25264];
extern volatile float Beta2[25806];

void ZeroBuffer()
///////////////////////////////////////////////////////////////////////
// Purpose:   Initial fill of all buffer locations with 0.0
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    int i;

    for(i=0; i < BUFFER_LENGTH; i++)  {
        buffer[LEFT][i] = 0.0;
        buffer[RIGHT][i] = 0.0;  
        }

    for(i=0; i < BUFFER_LENGTH1; i++)  {
        buffer1[LEFT][i] = 0.0;
        buffer1[RIGHT][i] = 0.0;
        }

    for(i=0; i < BUFFER_LENGTH2; i++)  {
        buffer2[LEFT][i] = 0.0;
        buffer2[RIGHT][i] = 0.0;
        }
}

interrupt void Codec_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{                    
	/* add any local variables here */

 	if(CheckForOverrun())					// overrun error occurred (i.e. halted DSP)
		return;								// so serial port is reset to recover

  	CodecDataIn.UINT = ReadCodecData();		// get input data samples
	
	/* add your code starting here */

	/****************************
	ECHO ROUTINE BEGINS HERE
	****************************/
	xLeft = CodecDataIn.Channel[LEFT];   // current LEFT input value to float
	xRight = CodecDataIn.Channel[RIGHT];   // current RIGHT input value to float

	buffer[LEFT][oldest] = xLeft;
	buffer[RIGHT][oldest] = xRight;

	buffer1[LEFT][oldest1] = xLeft;
    buffer1[RIGHT][oldest1] = xRight;

    buffer2[LEFT][oldest2] = xLeft;
    buffer2[RIGHT][oldest2] = xRight;

	if (++oldest >= BUFFER_LENGTH){ // implement circular buffer
		oldest = 0;
	}
	if (++oldest1 >= BUFFER_LENGTH1){ // implement circular buffer
	        oldest1 = 0;
	    }
	if (++oldest2 >= BUFFER_LENGTH2){ // implement circular buffer
	        oldest2 = 0;
	    }


	int Bn = (int)round(Beta[index]);
	int Bn1 = (int)round(Beta1[index1]);
	int Bn2 = (int)round(Beta2[index2]);

	int offset = R - Bn;
	int offset11 = R1 -Bn1;
	int offset21 = R2 - Bn2;
	// use either FIR or IIR lines below
	int offset2 = offset+oldest;
	int offset12 = offset11 + oldest1;
	int offset22 = offset21 + oldest2;


	if (++index >= 24000){
	    index = 0;
	}
	if (++index1 >= 25264){
	        index1 = 0;
	    }
	if (++index2 >= 25806){
	        index2 = 0;
	    }

	if (offset2 >= BUFFER_LENGTH){ // implement circular buffer
	            offset2 = offset2 - BUFFER_LENGTH;
	        }
	if (offset12 >= BUFFER_LENGTH1){ // implement circular buffer
	                offset12 = offset12 - (BUFFER_LENGTH1);
	            }
	if (offset22 >= BUFFER_LENGTH2){ // implement circular buffer
	                offset22 = offset22 - (BUFFER_LENGTH2);
	            }

	// for FIR comb filter effect, uncomment next two lines
	yLeft = xLeft + (xLeft + (gain * buffer[LEFT][offset2])) + (xLeft + (gain1 * buffer1[LEFT][offset12])) + (xLeft + (gain2 * buffer2[LEFT][offset22]));
	//  yRight = xRight + (gain * buffer[RIGHT][oldest]);
    
//	buffer[LEFT][newest] = xLeft + (gain * buffer[LEFT][offset2+1]);
//	yLeft = buffer[LEFT][oldest];  // or use newest

	CodecDataOut.Channel[LEFT] = yLeft;   // setup the LEFT value
	CodecDataOut.Channel[RIGHT] = xRight; // setup the RIGHT value -----> Talk through
	/*****************************/
	/* end your code here */

	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
}

