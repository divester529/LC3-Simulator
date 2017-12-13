#define DEBUG

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/* LC3 Operations */
#define ADD 0x001

/* LC3 Data types */
typedef char BYTE;
typedef unsigned int WORD;

/* LC3 Machine */
typedef struct
{
  WORD R[8]; /* 8 16-Bit data registers */
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
  printf("-----------------LC3 STATUS--------------------\n");
 
  printf("PC:  0x%04X\t\tIR:  0x%04X\n", lc3->PC, lc3->IR);
  printf("MAR: 0x%04X\t\tMDR: 0x%04X\n", lc3->MAR, lc3->MDR);
  
  for(i = 0; i < 8; i+=4)
    {
      printf("r%d: 0x%04X\tr%d: 0x%04X\tr%d: 0x%04X\tr%d: 0x%04X\n", i, lc3->R[i], i+1, lc3->R[i+1], i+2, lc3->R[i+2], i+3, lc3->R[i+3]);
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

void execute(LC3* lc3, BYTE opcode);

int main(int argc, char* args[])
{
  LC3 lc3;

  initMachine(&lc3);

  WORD instruc = 0x193E;
  
  lc3.mem[0]=instruc;
  lc3.mem[1]=instruc;
  lc3.mem[2]=instruc;
  
  showStatus(&lc3);
 
  next(&lc3);
  next(&lc3);
  next(&lc3);
  
  showStatus(&lc3);
  
  killMachine(&lc3);
}

void next(LC3* lc3)
{
  BYTE opcode;

  lc3->IR = fetch(lc3);
  opcode=decode(lc3->IR);

  execute(lc3, opcode);
}

WORD fetch(LC3* lc3)
{
  lc3->MAR = lc3->PC;
  lc3->MDR = lc3->mem[lc3->PC++];

  return lc3->MDR;
}

BYTE decode(WORD instruc)
{
  BYTE opcode = instruc >> 12; // Bit-shift to get opcode of instruction
  
  return opcode;
}

void execute(LC3* lc3, BYTE opcode)
{
  switch(opcode)
    {
    case ADD:
      printf("0x%02X ADD\n", opcode);
      /* Bit mask IR[16:8] with 0xF8 and then subtract 0xF8 to isolate the 3 bits for the register number */
      BYTE DR = (lc3->IR >> 9 | 0xF8)-0xF8; 
      BYTE SR = (lc3->IR >> 6 | 0xF8)-0xF8;
      /* If mode bit IR[5]=1 then immediate mode, if 0, register mode*/
      BYTE MODE = (lc3->IR >> 5 | 0xFE)-0xFE;

      if(MODE){
	/* Sign bit IR[4] */
	BYTE SIGN = (lc3->IR >> 4 | 0xFE)-0xFE; 
	/* Immediate value IR[3:0] */
	BYTE IM = (lc3->IR | 0xF0)-0xF0;

	if(SIGN)
	  IM=IM+0xF0;
	
	lc3->R[DR]=lc3->R[SR]+IM;

	#ifdef DEBUG
	printf("Dest: r%d\tSrc: r%d\nSign %c\t\tImm: %d\n", DR, SR, SIGN?'-':'+', IM);
	printf("r%d: %d\n", DR, (int)lc3->R[DR]);
	#endif
      }else{
	

      }
      break;
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
