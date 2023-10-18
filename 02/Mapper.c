/*
# Name: Clay Molitor
# Date: 10/10/2023
# Description: 
	OutputAsHexCharacters takes an int value and prints it in hex format.
*/

#include "Mapper.h"
#include <stdio.h>

// Input 32 bit int.
// Output: None, Prints value to in hex format string.
void OutputAsHexCharacters(unsigned int value)
{
	// This function masks off 4 bits at a time, from greatest to least bit, 
	// and passes the masked value to MapNibbleToHexCharacter, printing the returned value.
	const int NIBBLES_PER_BYTE = 2;
	const int NIBBLE_COUNT = sizeof(unsigned int) * NIBBLES_PER_BYTE;
	const int BITS_PER_NIBBLE = 4;

	const int BITS_PER_INT = NIBBLE_COUNT * BITS_PER_NIBBLE; // 32
	unsigned int mask = 0x0000000F;

	printf("\n");


	for (int count = 0; count < NIBBLE_COUNT; count++)
	{
		// 28 to 0, increments of 4.
		unsigned int bits_to_shift = BITS_PER_INT - ((count + 1) * BITS_PER_NIBBLE); 
		// Shift target nibble to the right 4 bits and mask off everything but the nibble
		unsigned int nibble = (value >> bits_to_shift) & mask;
		
	
		
		printf("Hexidecimal character: %c \n", MapNibbleToHexCharacter(nibble));
	}
	
	printf("\n");
}

// Takes a value from 0 to 15 (4 bits) and returns an ascii hex char
// Returns char '\0' (number 0) if input is invalid. 
unsigned char MapNibbleToHexCharacter(unsigned int value)
{
	// I was having segfaults when passing numbers bigger than 15 
	// So I changed this function. Now it handles invalid inputs more gracefully.
	char hex;

	if(value <= 9) {
		hex = value + '0';
	}
	else if (value <= 0xF) {
		hex = value - 10 + 'A';
	}
	else {
		// Invalid input.
	}
	return hex;
}
