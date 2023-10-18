/*
# Name: Clay Molitor
# Date: 10/17/2023
# Description: 
	assemble: Takes an input file name and an output filename.
        The input file must be SISA assembly.
        The output file will be the input converted to bytecode.
    write_operator_code: Writes leading opcodes to file.
        Opcode passed as string is written as binary opcode to the output file.
    write_register_code: Included by prof.
        Register passed as string is written as binary register to the output file.
    write_binary_number: Writes a binary number to file.
        Binary passed as string is written to the output file.

*/

#pragma warning(disable : 4996)
// https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"
#include "RegisterSet.h"
#include "InstructionOpcodes.h"

void write_register_code(char *reg_num, FILE *fptr_write);
void write_operator_code(char *op_code, FILE *fptr_write);
void write_binary_number(char* number,  FILE *fptr_write);

const char *PADDING_CHARACTERS = ", []+\0";


void assemble(char *assemblyFilename, char *objectCodeFilename)
{
    char file_line[ASSEMBLY_STATEMENT_BUFFER_SIZE];

    FILE *fptr_read;
    FILE *fptr_write;

    fptr_read = fopen(assemblyFilename, "r");
    fptr_write = fopen(objectCodeFilename, "w");

    char *result = fgets(file_line, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptr_read);

    while (result != 0)
    {

        // Set end of line chars to null in current line.
        file_line[strcspn(file_line, "\r")] = 0; // Linux
        file_line[strcspn(file_line, "\n")] = 0; // Windows

        printf("Translating assembly statement: %s\n", file_line);

        //Chunks are opcode mnemonic, registers, or binary numbers.
        char *chunk; 
        chunk = strtok(file_line, PADDING_CHARACTERS);
        write_operator_code(chunk, fptr_write);

        while(chunk != NULL)
        {
            if(chunk[0] == 'R') // If register
            {
                write_register_code(chunk, fptr_write);
            }
            else if(chunk[0] == 'b') // If binary number
            {
                write_binary_number(chunk, fptr_write);
            }
            else
            {
                // Invalid chunk 
            }
            // Get next chunk
            chunk = strtok(NULL, PADDING_CHARACTERS);
        }

        // Print windows style endline.
        fprintf(fptr_write, "\r\n");


        // Read next line.
        result = fgets(file_line, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptr_read);
    }

    fclose(fptr_read);
    fclose(fptr_write);
}

// number: ascii string of binary number with leading 'b'.
// fptr_write: output file's handle.
// Removes 'b' and prints number to file as ascii string.
void write_binary_number(char* number,  FILE *fptr_write)
{
    // Remove 'b' and print number to file.
    fprintf(fptr_write, strtok(number, "b"));
}

// op_code: ascii string, see InstructionOpcodes.h for valid inputs.
// fptr_write: output file's handle.
// Writes 4 bit opcode to file.
// Prints additional 12 zero bits for HALT.
// Prints additional 4 zeros bits for B.
void write_operator_code(char *op_code, FILE *fptr_write)
{
    if (strcmp(op_code, MOVI) == 0)
    {
        fprintf(fptr_write, OPCODE_MOVI);
    }
    else if (strcmp(op_code, ADD) == 0)
    {
        fprintf(fptr_write, OPCODE_ADD);
    }
    else if (strcmp(op_code, SUB) == 0)
    {
        fprintf(fptr_write, OPCODE_SUB);
    }
    else if (strcmp(op_code, CMP) == 0)
    {
        fprintf(fptr_write, OPCODE_CMP);
    }
    else if (strcmp(op_code, MUL) == 0)
    {
        fprintf(fptr_write, OPCODE_MUL);
    }
    else if (strcmp(op_code, DIV) == 0)
    {
        fprintf(fptr_write, OPCODE_DIV);
    }
    else if (strcmp(op_code, LDR) == 0)
    {
        fprintf(fptr_write, OPCODE_LDR);
    }
    else if (strcmp(op_code, STR) == 0)
    {
        fprintf(fptr_write, OPCODE_STR);
    }
    else if (strcmp(op_code, BE) == 0)
    {
        fprintf(fptr_write, OPCODE_BE);
    }
    else if (strcmp(op_code, BNE) == 0)
    {
        fprintf(fptr_write, OPCODE_BNE);
    }
    else if (strcmp(op_code, BL) == 0)
    {
        fprintf(fptr_write, OPCODE_BL);
    }
    else if (strcmp(op_code, BG) == 0)
    {
        fprintf(fptr_write, OPCODE_BG);
    }
    else if (strcmp(op_code, BLE) == 0)
    {
        fprintf(fptr_write, OPCODE_BLE);
    }
    else if (strcmp(op_code, BGE) == 0)
    {
        fprintf(fptr_write, OPCODE_BGE);
    }
    else if (strcmp(op_code, B) == 0)
    {
        fprintf(fptr_write, OPCODE_B);
        fprintf(fptr_write, MCIA_PADDING);
    }
    else if (strcmp(op_code, HALT) == 0)
    {
        fprintf(fptr_write, OPCODE_HALT);
        fprintf(fptr_write, MCIA_PADDING);
        fprintf(fptr_write, MCIA_PADDING);
        fprintf(fptr_write, MCIA_PADDING);
    }
    else
    {
        // invalid operator
    }
}
// reg_num: ascii string, see InstructionOpcodes.h for valid inputs.
// fptr_write: output file's handle.
// Writes 4 bit register.
void write_register_code(char *reg_num, FILE *fptr_write)
{
    int cmp_result = strcmp(reg_num, ACRO_R0);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R0);
    cmp_result = strcmp(reg_num, ACRO_R1);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R1);
    cmp_result = strcmp(reg_num, ACRO_R2);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R2);
    cmp_result = strcmp(reg_num, ACRO_R3);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R3);
    cmp_result = strcmp(reg_num, ACRO_R4);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R4);
    cmp_result = strcmp(reg_num, ACRO_R5);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R5);
    cmp_result = strcmp(reg_num, ACRO_R6);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R6);
    cmp_result = strcmp(reg_num, ACRO_R7);
    if (cmp_result == 0)
        fprintf(fptr_write, MCRO_R7);
}
