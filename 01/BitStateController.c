#include "BitStateController.h"
/*
# Name: Clay Molitor
# Date: 10/2/2023
# Description: 
	I want to make sure I'm doing things the right way.

	BitStateControllerSetBitState sets the state of an individual bit inside a byte.
	BitStateControllerGetBitState reads the state of an individual bit inside a byte.
*/

// Sets the specified bit in the passed byte to the passed bitState
// bitStateControllerPtr: pointer to the byte to be twiddled.
// bit: The bit to be set, 1 based indexing.
// bitState: Desired state of the bit, 0 or 1.
void BitStateControllerSetBitState(BitStateControllerPtr bitStateControllerPtr, enum Bit bit, enum BitState bitState)
{
	bit = bit - 1; // Changing 1 based index to zero base
	uint8_t mask;

    mask = 1 << bit; // byte with "bit"th bit set true.

	if(bitState == On)
	{
		bitStateControllerPtr->bitStates |= mask;
	}
	else if(bitState == Off)
	{
		bitStateControllerPtr->bitStates &= ~mask;
	}
	else
	{
		// Invalid input.
	}

}

// Reads the specified bit in the passed byte to the passed bitState
// bitStateControllerPtr: pointer to the byte to be read.
// bit: The bit in the byte to be read
// return: The state of the specified bit, 0 or 1.
enum BitState BitStateControllerGetBitState(BitStateControllerPtr bitStateControllerPtr, enum Bit bit)
{
	bit = bit - 1; // changing 1 based index to zero base
	// Shift bitStates so lowest bit is "bit"th bit.
	// return 0x01 if this lowest bit is 1.

	return (bitStateControllerPtr->bitStates >> bit) & 0x01;
}



