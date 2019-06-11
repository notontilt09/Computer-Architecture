#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  int pc;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	// Add more here
  ALU_DIV,
  ALU_ADD,
  ALU_SUB,
  ALU_MOD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define ADD  0b10100000
#define SUB  0b10100001
#define DIV  0b10100011
#define MOD  0b10100100
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char* filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
