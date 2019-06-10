#include "cpu.h"
#include <stdio.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
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
    if (ir > 0b00111111) {
      operandA = cpu->ram[cpu->pc+1];
      num_operands++;
    }
    if (ir > 0b01111111) {
      operandB = cpu->ram[cpu->pc+2];
      num_operands++;
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch(ir) {
      // TODO PUT OUR INSTRUCTIONS HERE
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->pc += 3;
        break;
      
      case PRN:
        printf("%s\n", operandA);
        cpu->pc += 2;
        break;

      case HLT:
       running = 0;
       cpu->pc++;
       break;
      
      default:
        printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
    }
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

unsigned char cpu_ram_read(struct cpu *cpu)
{
  return cpu->ram[cpu->pc];
}

void cpu_ram_write(struct cpu *cpu, char input)
{
  cpu->ram[cpu->pc] = input;

}
