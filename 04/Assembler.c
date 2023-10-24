/*
# Name: Clay Molitor
# Date: 10/23/2023
# Description: 
    My program transforms a SISA assembly file into machine code binary.
*/

#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"
#include "RegisterSet.h"
#include "InstructionOpcodes.h"

void write_opcode(       char* reg_num,   unsigned short* machineInstruction, unsigned short* leftShift);
void write_register_code(char* reg_num,   unsigned short* machineInstruction, unsigned short* leftShift);
void write_immediate(    char* immediate, unsigned short* machineInstruction, unsigned short* leftShift);
void write_machine_instruction(FILE* fptr_write, unsigned short machineInstruction);

const char *PADDING_CHARACTERS = ", []+\0";

void assemble(char* assemblyFilename, char* objectCodeFilename)
{
    char file_line[ASSEMBLY_STATEMENT_BUFFER_SIZE];
    
    FILE* fptr_read;
    FILE* fptr_write;

    fptr_read = fopen(assemblyFilename, "r");
    fptr_write = fopen(objectCodeFilename, "wb");

    char* result = fgets(file_line, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptr_read);

    while (result != 0)
    {
        // machineInstruction receives a line of assemble converted into 16 bits of binary.
        unsigned short machineInstruction = 0;
        unsigned short shift = sizeof(machineInstruction) * 8;

        // Set end of line chars to null in current line.
        file_line[strcspn(file_line, "\r")] = 0; // Linux
        file_line[strcspn(file_line, "\n")] = 0; // Windows

        printf("Translating assembly statement: %s\n", file_line);
        
        // Read each word in line of assembly. output to machineInstruction.
        for(char* chunk = strtok(file_line, PADDING_CHARACTERS); 
            chunk != NULL; 
            chunk = strtok(NULL, PADDING_CHARACTERS))
        {
            if(chunk[0] == 'R' && (chunk[1] >= '0' && chunk[1] <= '9' )) // If register
            {  
                write_register_code(chunk, &machineInstruction, &shift);
            }
            else if(chunk[0] == 'b') // If binary number
            {
                char* trimmed_immediate = strtok(chunk, "b");

                write_immediate(trimmed_immediate, &machineInstruction, &shift);
            }
            else // Opcode 
            {
                write_opcode(chunk, &machineInstruction, &shift);
            }
        } // End for loop


        write_machine_instruction(fptr_write, machineInstruction);

        // Read next line.
        result = fgets(file_line, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptr_read);
    }

    fclose(fptr_read);
    fclose(fptr_write);
}

// opcode is ASCII. See InstructionOpcodes.h for valid inputs.
// machineInstruction is combined with, opcode binary offset by leftShift.
// leftShift points to the start of where data should be written to machineInstruction.
// leftShift is then set to the address of remaining unwritten space in machineInstruction.
void write_opcode(char* opcode, unsigned short* machineInstruction, unsigned short* leftShift)
{
    unsigned short machineOpcode = 0;
    // Opcodes are 4 bits long, move shift to accommodate. 
    *leftShift -= 4;
    // Used for indexing machineInstruction.
    int startShift = *leftShift;

    // Convert Ascii assembly to matching binary instruction.
    if        (strcmp(opcode, MOVI) == 0) {
        machineOpcode = OPCODE_MOVI;
    } else if (strcmp(opcode, ADD)  == 0) {
        machineOpcode = OPCODE_ADD;
    }  else if (strcmp(opcode, SUB) == 0) {
        machineOpcode = OPCODE_SUB;
    } else if (strcmp(opcode, CMP)  == 0) {
        machineOpcode = OPCODE_CMP; 
    } else if (strcmp(opcode, MUL)  == 0) {
        machineOpcode = OPCODE_MUL; 
    } else if (strcmp(opcode, DIV)  == 0) {
        machineOpcode = OPCODE_DIV; 
    } else if (strcmp(opcode, LDR)  == 0) {
        machineOpcode = OPCODE_LDR; 
    } else if (strcmp(opcode, STR)  == 0) {
        machineOpcode = OPCODE_STR; 
    } else if (strcmp(opcode, BE)   == 0) {
        machineOpcode = OPCODE_BE; 
    } else if (strcmp(opcode, BNE)  == 0) {
        machineOpcode = OPCODE_BNE; 
    } else if (strcmp(opcode, BL)   == 0) {
        machineOpcode = OPCODE_BL; 
    } else if (strcmp(opcode, BG)   == 0) {
        machineOpcode = OPCODE_BG; 
    } else if (strcmp(opcode, BLE)  == 0) {
        machineOpcode = OPCODE_BLE; 
    } else if (strcmp(opcode, BGE)  == 0) {
        machineOpcode = OPCODE_BGE;
    } else if (strcmp(opcode, B)    == 0) {
        machineOpcode = OPCODE_B; 
        *leftShift -= 4; // 4 bit padding after opcode.
    } else if (strcmp(opcode, HALT) == 0) {
        machineOpcode = OPCODE_HALT;
        *leftShift -= 12; // 12 bit padding after opcode.
    } else {
        // invalid operator
        printf("\n\tInvalid Operator: %s \n", opcode);
    }

    machineOpcode = machineOpcode << startShift;
    *machineInstruction = *machineInstruction | machineOpcode;
}

// reg_num is ASCII. See RegisterSet.h for valid inputs.
// machineInstruction is combined with, opcode binary offset by leftShift.
// leftShift points to the start of where data should be written to machineInstruction.
// leftShift is then set to the address of remaining unwritten space in machineInstruction.
void write_register_code(char* reg_num, unsigned short* machineInstruction, unsigned short* leftShift)
{
    unsigned short machineRegister = 0;
    // Register are 4 bits or longer, move shift to accommodate.
    *leftShift -= 4;

    // Convert Ascii registers to matching binary equivalent.
    if        (strcmp(reg_num, ACRO_R0) == 0) {
        machineRegister = MCRO_R0;
    } else if (strcmp(reg_num, ACRO_R1) == 0) {
        machineRegister = MCRO_R1; 
    } else if (strcmp(reg_num, ACRO_R2) == 0) {
        machineRegister = MCRO_R2; 
    } else if (strcmp(reg_num, ACRO_R3) == 0) {
        machineRegister = MCRO_R3; 
    } else if (strcmp(reg_num, ACRO_R4) == 0) {
        machineRegister = MCRO_R4; 
    } else if (strcmp(reg_num, ACRO_R5) == 0) {
        machineRegister = MCRO_R5; 
    } else if (strcmp(reg_num, ACRO_R6) == 0) {
        machineRegister = MCRO_R6; 
    } else if (strcmp(reg_num, ACRO_R7) == 0) {
        machineRegister = MCRO_R7; 
    } else if (strcmp(reg_num, ACRO_R8) == 0) {
        machineRegister = MCRO_R8;
    } else {
        // Invalid register.
        printf("\n\tInvalid register: %s\n", reg_num);
    }

    machineRegister = machineRegister << *leftShift;
    *machineInstruction = *machineInstruction | machineRegister;
}

// immediate is a binary number in ASCII of 4 or 8 bits. Remove 'b' before passing.
// machineInstruction is combined with opcode binary.
// leftShift points to the start of where data should be written to machineInstruction.
// leftShift is then set to the address of remaining unwritten space in machineInstruction.
void write_immediate(char* immediate, unsigned short* machineInstruction, unsigned short* leftShift)
{
    // immediate should be 4 or 8 chars
    *leftShift -= strlen(immediate);

    unsigned short machineImmediate = (unsigned short)strtol(immediate, NULL, 2);;
    machineImmediate = machineImmediate << *leftShift;
    *machineInstruction = *machineInstruction | machineImmediate;
}

void write_machine_instruction(FILE* fptr_write, unsigned short machineInstruction)
{
    int result = fwrite(&machineInstruction, sizeof(unsigned short), 1, fptr_write);
}
