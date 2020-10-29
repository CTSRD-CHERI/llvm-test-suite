/* %%%%%%%%%%%%%%%%%%%% (c) William Landi 1991 %%%%%%%%%%%%%%%%%%%%%%%%%%%% */
/* Permission to use this code is granted as long as the copyright */
/* notice remains in place. */
/* ============================== op_tab =================================== */
/* Defines and allows for loop up in the OPERATOR TABLE                      */
#include <stdio.h>
#include <string.h>
#include "pseudo.h"

/* ---------------- Possible formats for instructions.                       */
enum formats {ONE,TWO,THREE_FOUR,PSEUDO,NOT_FOUND};
/* ---------------- Possible operand types --------------------------------- */
enum operands 
   {NONE,REG,REG_REG,MEM,REG_NUM,NUM,PSEUDO_LOC,PSEUDO_NOLOC,PSEUDO_ADDR};

/* ---------------- Structure of the operator table                          */
struct OP_ENTRY {
  char *MNEMONIC;                /* Name of the operator                     */
  enum formats FORMAT;           /* FORMAT of this operator                  */
  char *OPCODE;                  /* OPCODE for this operator                 */
  enum operands OPERAND;         /* Number and type of operands              */
  int FUNCTION;                  /* Function to execute is PASS1. Pseudo     */
                                 /* operators only.                          */
};


#define NUM_OPCODES               59  /* number of opcodes in the table */
                                      /* NOT counting the NOT_FOUND entry */

/* table (sorted array) of keywords used in EMITID */
struct OP_ENTRY OP_TABLE[] = 
   {{"ADD",THREE_FOUR,"18",MEM,0},    
    {"ADDR",TWO,"90",REG_REG,0},
    {"AND",THREE_FOUR,"40",MEM,0},    
    {"BYTE",PSEUDO,"  ",PSEUDO_LOC,PSEUDO_BYTE0},
    {"CLEAR",TWO,"B4",REG,0},         
    {"COMP",THREE_FOUR,"28",MEM,0},
    {"COMPR",TWO,"A0",REG_REG,0},     
    {"CSECT",PSEUDO,"  ",PSEUDO_ADDR,PSEUDO_CSECT0},
    {"DIV",THREE_FOUR,"24",MEM,0},    {"DIVR",TWO,"9C",REG_REG,0},
    {"END",PSEUDO,"  ",PSEUDO_LOC,PSEUDO_END0},
    {"EQU",PSEUDO,"  ",PSEUDO_ADDR,PSEUDO_EQU0},
    {"EXTDEF",PSEUDO,"  ",PSEUDO_NOLOC,PSEUDO_EXTDEF0},
    {"EXTREF",PSEUDO,"  ",PSEUDO_NOLOC,PSEUDO_EXTREF0},
    {"HIO",ONE,"F4",NONE,0},          {"J",THREE_FOUR,"3C",MEM,0},
    {"JEQ",THREE_FOUR,"30",MEM,0},    {"JGT",THREE_FOUR,"34",MEM,0},
    {"JLT",THREE_FOUR,"38",MEM,0},    {"JSUB",THREE_FOUR,"48",MEM,0},
    {"LDA",THREE_FOUR,"00",MEM,0},    {"LDB",THREE_FOUR,"68",MEM,0},
    {"LDCH",THREE_FOUR,"50",MEM,0},   {"LDL",THREE_FOUR,"08",MEM,0},
    {"LDS",THREE_FOUR,"6C",MEM,0},    {"LDT",THREE_FOUR,"74",MEM,0},
    {"LDX",THREE_FOUR,"04",MEM,0},    {"LPS",THREE_FOUR,"D0",MEM,0},
    {"MUL",THREE_FOUR,"20",MEM,0},    {"MULR",TWO,"98",REG_REG,0},
    {"OR",THREE_FOUR,"44",MEM,0},     {"RD",THREE_FOUR,"D8",MEM,0},
    {"RESB",PSEUDO,"  ",PSEUDO_LOC,PSEUDO_RESB0},
    {"RESW",PSEUDO,"  ",PSEUDO_LOC,PSEUDO_RESW0},
    {"RMO",TWO,"AC",REG_REG,0},       {"RSUB",THREE_FOUR,"4C",NONE,0},
    {"SHIFTL",TWO,"A4",REG_NUM,0},    {"SHIFTR",TWO,"A8",REG_NUM,0},
    {"SIO",ONE,"F0",NONE,0},          {"SSK",THREE_FOUR,"EC",MEM,0},
    {"STA",THREE_FOUR,"0C",MEM,0},    
    {"START",PSEUDO,"  ",PSEUDO_ADDR,PSEUDO_START0},
    {"STB",THREE_FOUR,"78",MEM,0},    {"STCH",THREE_FOUR,"54",MEM,0},
    {"STI",THREE_FOUR,"D4",MEM,0},    {"STL",THREE_FOUR,"14",MEM,0},
    {"STS",THREE_FOUR,"7C",MEM,0},    {"STSW",THREE_FOUR,"E8",MEM,0},
    {"STT",THREE_FOUR,"84",MEM,0},    {"STX",THREE_FOUR,"10",MEM,0},
    {"SUB",THREE_FOUR,"1C",MEM,0},    {"SUBR",TWO,"94",REG_REG,0},
    {"SVC",TWO,"B0",NUM,0},           {"TD",THREE_FOUR,"E0",MEM,0},
    {"TIO",ONE,"F8",NONE,0},          {"TIX",THREE_FOUR,"2C",MEM,0},
    {"TIXR",TWO,"B8",REG,0},          {"WD",THREE_FOUR,"DC",MEM,0},
    {"WORD",PSEUDO,"  ",PSEUDO_LOC,PSEUDO_WORD0},
/* the next entry must always be last                                        */
    {"*END OF TABLE*",NOT_FOUND,"00",NONE,0}};


/* ---------------------------- LOOK_UP_OP --------------------------------- */
/* Find operator in the table. Return the entry if it is found, else return  */
/* the not found entry. Uses BINARY SEARCH.                                  */
struct OP_ENTRY *LOOK_UP_OP(char OP[])
{
  int LOW = 0,           /* LOWest element in OP_TABLE that may be the OP    */
    HIGH = NUM_OPCODES-1,/* HIGHest element that may be the OP               */
    MIDDLE;              /* (LOW+HIGH)/2... next element considering         */

  int CMP;               /* stores the result of a string comparison;        */
			 /*  0 str1 = str2, 1 str1 > str2, -1 str1 < str2    */

  MIDDLE = 0;
  while (LOW <= HIGH) 
    { 
      MIDDLE = (HIGH + LOW)/2;
      if (!(CMP = strcmp(OP_TABLE[MIDDLE].MNEMONIC,OP)))
	return &OP_TABLE[MIDDLE];
      if (CMP > 0)
	HIGH = MIDDLE - 1;
      else LOW = MIDDLE + 1;
    }
  return &OP_TABLE[NUM_OPCODES];
}
