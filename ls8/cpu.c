#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6
#define SP cpu->registers[7] 

// helper functions for reading/writing ram
unsigned char cpu_ram_read(struct cpu *cpu, int address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, int address, char input)
{
  cpu->ram[address] = input;

}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // file pointer
  FILE *fp;
  // read buffer
  char line[1024];
  // ram address we'll write into
  int address = 0;

  // open the file passed in via CLI args
  fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  // loop through file and write each line into the cpu->ram
  while (fgets(line, 1024, fp) != NULL) {
    char *endptr;
    // use stroul to convert string to unsigned long
    unsigned char v = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

    cpu_ram_write(cpu, address, v);
    address++;
  

  }

  fclose(fp);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    case ALU_DIV:
      cpu->registers[regA] = cpu->registers[regA] / cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
    case ALU_SUB:
      cpu->registers[regA] = cpu->registers[regA] - cpu->registers[regB];
      break;
    case ALU_MOD:
      cpu->registers[regA] = cpu->registers[regA] % cpu->registers[regB];
      break;
    case ALU_CMP:
      if (cpu->registers[regA] == cpu->registers[regB]) {
        cpu->fl = 0b00000001;
      } else if (cpu->registers[regA] < cpu->registers[regB]) {
        cpu->fl = 0b00000100;
      } else {
        cpu->fl = 0b00000010;
      }
      break;
    default:
      printf("That operation isn't recognized by the ALU\n");
      exit(1);

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  // instruction register
  unsigned char ir;
  int num_operands = 0;
  unsigned char operandA;
  unsigned char operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    num_operands = (ir >> 6);
    if (num_operands > 0) {
      operandA = cpu->ram[cpu->pc+1];
    }
    if (num_operands > 1) {
      operandB = cpu->ram[cpu->pc+2];
    }

    // printf("TRACE: ir: %02x, pc: %d, SP:%02x, r0: %d, r1: %0d\n", ir, cpu->pc, SP, cpu->registers[0], cpu->registers[1]);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch(ir) {
      // TODO PUT OUR INSTRUCTIONS HERE
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        break;
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        break;
      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case HLT:
        running = 0;
        break;
      case PUSH:
        SP--;
        cpu_ram_write(cpu, SP, cpu->registers[operandA]);
        break;
      case POP:
        if (SP == 0xF4) {
          fprintf(stderr, "error: cannot pop from empty stack");
        } else {
          cpu->registers[operandA] = cpu_ram_read(cpu, SP);
          SP++;
        }
        break;
      case CALL:
        // address of instruction after call pushed onto stack
        SP--;
        cpu_ram_write(cpu, SP, cpu->pc + 2);
        // set pc to address stored in given register
        cpu->pc = cpu->registers[operandA];
        break;
      case JMP:
        cpu->pc = cpu->registers[operandA];
        break;
      case RET:
        cpu->pc = cpu_ram_read(cpu, SP);
        SP++;
        break;
      case ST:
      // Store value in registerB in the address stored in registerA
        cpu_ram_write(cpu, cpu->ram[cpu->registers[operandA]], cpu->registers[operandB]);
        break; 
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JEQ:
        if (cpu->fl == 0b00000001) {
          cpu->pc = cpu->registers[operandA];
        } else {
          cpu->pc+= num_operands + 1;
        }
        break;
      case JNE:
        if ((cpu->fl & 0b00000001) == 0) {
          cpu->pc = cpu->registers[operandA];
        } else {
          cpu->pc+= num_operands + 1;
        }
        break;
      default:
        printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
        exit(1);
    }

    if (ir != RET && ir != JMP && ir != CALL && ir != JEQ && ir != JNE) {
      cpu->pc += num_operands + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->fl = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
  // setting up the stack pointer
  SP = 0xF4;
}


