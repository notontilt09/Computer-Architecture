#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  while (fgets(line, 1024, fp) != NULL) {
    char *endptr;
    unsigned char v = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

    cpu->ram[address] = v;
    address++;
  }


  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu)
{
  return cpu->ram[cpu->pc];
}

void cpu_ram_write(struct cpu *cpu, char input)
{
  cpu->ram[cpu->pc] = input;

}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char ir;
  int num_operands = 0;
  unsigned char operandA;
  unsigned char operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    num_operands = (ir >> 6);
    if (num_operands > 0) {
      operandA = cpu->ram[cpu->pc+1];
    }
    if (num_operands > 1) {
      operandB = cpu->ram[cpu->pc+2];
    }
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

      case HLT:
        running = 0;
        break;
      
      default:
        printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
        exit(1);
    }
    cpu->pc += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(cpu));
  cpu->pc = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}


