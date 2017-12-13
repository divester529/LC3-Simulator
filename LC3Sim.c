#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/* 16 LC3 operations */
#define ADD 1
#define AND 2
#define BR 3
#define JMP 4
#define JSR 5
#define LD 6
#define LDI 7
#define LDR 8
#define LEA 9
#define NOT 10
#define RET 11
#define RTI 12
#define ST 13
#define STI 14
#define STR 15
#define TRAP 16

/* LC3 Data types */
typedef unsigned char BYTE;
typedef unsigned long WORD;

/* LC3 Machine */
typedef struct
{
  WORD R[8]; /* 8 4-Bit data registers */
  WORD IR; /* 16-Bit instruction register */ 
  WORD PC; /* 16-Bit program counter */
  
  WORD MAR; /* 16-Bit memory address register */
  WORD MDR; /* 16-Bit memory data register */
  
  WORD *mem; /* 2^16 bits of memory */
} LC3;

/* Initialize memory and zero registers */
void initMachine(LC3* lc3);

/* Free machine */
void killMachine(LC3* lc3);

/* Show current state of machine */
void showStatus(LC3* lc3)
{
  int i;
  printf("-----------------LC3 STATUS------------------\n");
 
  printf("PC:  0x%04lX\t\tIR:  0x%04lX\n", lc3->PC, lc3->IR);
  printf("MAR: 0x%04lX\t\tMDR: 0x%04lX\n", lc3->MAR, lc3->MDR);
  
  for(i = 0; i < 8; i+=4)
    {
      printf("r%d: 0x%04lX\tr%d: 0x%04lX\tr%d: 0x%04lX\tr%d: 0x%04lX\n", i, lc3->R[i], i+1, lc3->R[i+1], i+2, lc3->R[i+2], i+3, lc3->R[i+3]);
    }
  
  printf("-----------------------------------------------\n");
}

/* Set the program counter */
void setPC(LC3* lc3, WORD _PC)
{
  lc3->PC=_PC;
}

void next(LC3* lc3);

WORD fetch(LC3* lc3);

BYTE decode(WORD instruc);

int main(int argc, char* args[])
{
  LC3 lc3;

  initMachine(&lc3);

  WORD instruc = 0x99E;
  
  lc3.mem[0]=instruc;

  showStatus(&lc3);
 
  next(&lc3);

  showStatus(&lc3);
  
  killMachine(&lc3);
}

void next(LC3* lc3)
{
  BYTE opcode;

  lc3->IR = fetch(lc3);
  opcode=decode(lc3->IR);

  printf("%d\n", opcode);
}

WORD fetch(LC3* lc3)
{
  lc3->MAR = lc3->PC;
  lc3->MDR = lc3->mem[lc3->PC++];

  return lc3->MDR;
}

BYTE decode(WORD instruc)
{
  BYTE opcode = instruc >> 11; // Bit-shift to get opcode of instruction
  
  /* OPCODES
   * 0001 0x01 ADD
   * 0101 0x11 AND
   * 0000 0x00 BR
   * 1100 0xB0 JMP
   * 0100 0x10 
   */
  switch(opcode)
    {
    case 0x01:
      return ADD;
      break;
    case 0x11:
      return AND;
      break;
    case 0x00:
      return BR;
    break;
    default:
      return -1;
    }
}

void initMachine(LC3* lc3)
{
  int i=0;

  for(i = 0; i < 8; i++)
    lc3->R[i]=0;

  lc3->MAR=0;
  lc3->MDR=0;
  lc3->IR=0;
  
  lc3->mem=malloc(sizeof(WORD)*pow(2, 16));
  setPC(lc3, 0);
}

void killMachine(LC3* lc3)
{
  free(lc3->mem);
}
