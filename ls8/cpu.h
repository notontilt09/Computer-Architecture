#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  int pc;
  // flag register
  int fl;
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
  ALU_MOD,
  ALU_CMP,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_NOT,
  ALU_SHL,
  ALU_SHR,
  ALU_ADDI
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
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define JMP  0b01010100
#define RET  0b00010001
#define ST   0b10000100
#define CMP  0b10100111
#define JEQ  0b01010101
#define JNE  0b01010110
#define JGE  0b01011010
#define JGT  0b01010111
#define JLE  0b01011001
#define JLT  0b01011000
#define AND  0b10101000
#define OR   0b10101010
#define XOR  0b10101011
#define NOT  0b01101001
#define SHL  0b10101100
#define SHR  0b10101101

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char* filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
