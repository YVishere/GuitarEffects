#include "DSP_Config.h" 
#include "Echo.h" 
#include "math.h"
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1
#define R 96

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;

/* add any global variables here */
float xLeft, xRight, yLeft, yRight;
Uint32 oldest = 0; // index for buffer value
Uint32 index = 0; //Index of beta
#define BUFFER_LENGTH   96 // buffer length in samples
#pragma DATA_SECTION (buffer, "CE0"); // put "buffer" in SDRAM
volatile float buffer[2][BUFFER_LENGTH]; // space for left + right
volatile float gain = 0.75; /* set gain value for echoed sample */
extern volatile float Beta[96000];

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
	if (++oldest >= BUFFER_LENGTH){ // implement circular buffer
		oldest = 0;
	}


	int Bn = (int)round(Beta[index]);
	int offset = R - Bn;
	// use either FIR or IIR lines below
	int offset2 = offset+oldest;


	if (++index >= 96000){
	    index = 0;
	}
	if (offset2 >= BUFFER_LENGTH){ // implement circular buffer
	            offset2 = offset2 - BUFFER_LENGTH;
	        }

	// for FIR comb filter effect, uncomment next two lines
	yLeft = xLeft + (gain * buffer[LEFT][offset2]);
	//  yRight = xRight + (gain * buffer[RIGHT][oldest]);
    
//	buffer[LEFT][newest] = xLeft + (gain * buffer[LEFT][offset2+1]);
//	yLeft = buffer[LEFT][oldest];  // or use newest

	CodecDataOut.Channel[LEFT] = yLeft;   // setup the LEFT value
	CodecDataOut.Channel[RIGHT] = xRight; // setup the RIGHT value -----> Talk through
	/*****************************/
	/* end your code here */

	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
}

