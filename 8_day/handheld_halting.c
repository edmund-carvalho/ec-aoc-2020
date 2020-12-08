/*  
 *  
 *  https://adventofcode.com/2020/day/8
 *  
 *  --- Day 8: Handheld Halting ---
 *  
 *  Your flight to the major airline hub reaches cruising altitude without incident.
 *  While you consider checking the in-flight menu for one of those drinks that come with a little umbrella, 
 *  you are interrupted by the kid sitting next to you.
 *  
 *  Their handheld game console won't turn on! They ask if you can take a look.
 *  
 *  You narrow the problem down to a strange infinite loop in the boot code (your puzzle input) of the device.
 *  You should be able to fix it, but first you need to be able to run the code in isolation.
 *  
 *  The boot code is represented as a text file with one instruction per line of text. 
 *  Each instruction consists of an operation (acc, jmp, or nop) and an argument (a signed number like +4 or -20).
 *  
 *      acc increases or decreases a single global value called the accumulator by the value given in the argument. 
 *      For example, acc +7 would increase the accumulator by 7. The accumulator starts at 0. 
 *      After an acc instruction, the instruction immediately below it is executed next.
 *      jmp jumps to a new instruction relative to itself. 
 *      The next instruction to execute is found using the argument as an offset from the jmp instruction;
 *      for example, jmp +2 would skip the next instruction, jmp +1 would continue to the instruction immediately below it, 
 *      and jmp -20 would cause the instruction 20 lines above to be executed next.
 *      nop stands for No OPeration - it does nothing. The instruction immediately below it is executed next.
 *  
 *  For example, consider the following program:
 *  
 *  nop +0
 *  acc +1
 *  jmp +4
 *  acc +3
 *  jmp -3
 *  acc -99
 *  acc +1
 *  jmp -4
 *  acc +6
 *  
 *  These instructions are visited in this order:
 *  
 *  nop +0  | 1
 *  acc +1  | 2, 8(!)
 *  jmp +4  | 3
 *  acc +3  | 6
 *  jmp -3  | 7
 *  acc -99 |
 *  acc +1  | 4
 *  jmp -4  | 5
 *  acc +6  |
 *  
 *  First, the nop +0 does nothing. 
 *  Then, the accumulator is increased from 0 to 1 (acc +1) and jmp +4 sets the next instruction to the other acc +1 near the bottom. 
 *  After it increases the accumulator from 1 to 2, jmp -4 executes, setting the next instruction to the only acc +3. 
 *  It sets the accumulator to 5, and jmp -3 causes the program to continue back at the first acc +1.
 *  
 *  This is an infinite loop: with this sequence of jumps, the program will run forever. 
 *  The moment the program tries to run any instruction a second time, you know it will never terminate.
 *  
 *  Immediately before the program would run an instruction a second time, the value in the accumulator is 5.
 *  
 *  Run your copy of the boot code. Immediately before any instruction is executed a second time, what value is in the accumulator?
 *  
 *  
 *  --- Part Two ---
 *  
 *  After some careful analysis, you believe that exactly one instruction is corrupted.
 *  
 *  Somewhere in the program, either a jmp is supposed to be a nop, or a nop is supposed to be a jmp. 
 *  (No acc instructions were harmed in the corruption of this boot code.)
 *  
 *  The program is supposed to terminate by attempting to execute an instruction immediately after the last instruction in the file. 
 *  By changing exactly one jmp or nop, you can repair the boot code and make it terminate correctly.
 *  
 *  For example, consider the same program from above:
 *  
 *  nop +0
 *  acc +1
 *  jmp +4
 *  acc +3
 *  jmp -3
 *  acc -99
 *  acc +1
 *  jmp -4
 *  acc +6
 *  
 *  If you change the first instruction from nop +0 to jmp +0, it would create a single-instruction infinite loop, 
 *  never leaving that instruction. If you change almost any of the jmp instructions, the program will still eventually find another jmp instruction and loop forever.
 *  
 *  However, if you change the second-to-last instruction (from jmp -4 to nop -4), the program terminates! 
 *  The instructions are visited in this order:
 *  
 *  nop +0  | 1
 *  acc +1  | 2
 *  jmp +4  | 3
 *  acc +3  |
 *  jmp -3  |
 *  acc -99 |
 *  acc +1  | 4
 *  nop -4  | 5
 *  acc +6  | 6
 *  
 *  After the last instruction (acc +6), the program terminates by attempting to run the instruction below the last instruction
 *  in the file. With this change, after the program terminates, the accumulator contains the value 8 (acc +1, acc +1, acc +6).
 *  
 *  Fix the program so that it terminates normally by changing exactly one jmp (to nop) or nop (to jmp). 
 *  What is the value of the accumulator after the program terminates?
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE_LEN 10
int64_t accumulator_g=0;

typedef struct instruction{
  char mnemonic[MAX_LINE_LEN];
  int opcode;
  bool visited;
} instruction_t;


int get_line_count(char *file_path)
{
  FILE *fp = fopen(file_path, "r");
  int count = 0;
  char line[MAX_LINE_LEN];

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  while (fgets(line, MAX_LINE_LEN, fp)) {
    count++;
  }

err:
  fclose(fp);
  return (count);
}


int parse_input_file(char *file_path, instruction_t *stack)
{
  FILE *fp = fopen(file_path, "r");
  int idx = 0;
  char line[MAX_LINE_LEN];
  int status = 0;

  if (fp == NULL) {
    perror("Failed to read line count");
  }

  while (fgets(line, MAX_LINE_LEN, fp)) {
    status = sscanf(line, "%s %d", stack[idx].mnemonic, &stack[idx].opcode);
    if (status != 2) {
      printf("ERROR::LINE %d \n", idx);
      break;
    } 
    idx++;
  }

  fclose(fp);
  return (idx);
}


void print_stack(instruction_t *stack, int stack_depth)
{
  for(int i=0;i<stack_depth;i++){
    printf("%s %d\n", stack[i].mnemonic, stack[i].opcode);
  }
}


bool is_stack_corrupted(instruction_t *stack, int stack_depth)
{
  int pc=0;

  for(int i=0;i<stack_depth;i++){
    stack[i].visited = false;
  }

  while(1){
    if (pc == stack_depth){
      return(false);
    }

    stack[pc].visited=true;

    if(stack[pc].mnemonic[0] == 'n' && stack[pc].mnemonic[1] == 'o' && stack[pc].mnemonic[2] == 'p' ){
      pc++;
    }else if(stack[pc].mnemonic[0] == 'a' && stack[pc].mnemonic[1] == 'c' && stack[pc].mnemonic[2] == 'c' ){
      accumulator_g += stack[pc].opcode;
      pc++;
    } else if(stack[pc].mnemonic[0] == 'j' && stack[pc].mnemonic[1] == 'm' && stack[pc].mnemonic[2] == 'p' ){
      pc += stack[pc].opcode;

      if (stack[pc].visited==true){
        /*
         * Loop detected !!
         */
        break;
      }

    } else {
      printf("ERROR : Unsupported mnemonic %s %d\n", stack[pc].mnemonic, stack[pc].opcode);
    }

    if(pc > stack_depth){
      printf("ERROR Tried to access stack out of bounds !!\n");
    }

  }

  return(true);
}


int64_t try_brute_force_bug_find(instruction_t *stack, int stack_depth)
{
  bool nop_swap=false;
  bool jmp_swap=false; //This flag is redundant , but looks nice so im keeping it XD .

  for (int i=0; i<stack_depth; i++){
    accumulator_g=0; //Reset accumulator before next run

    if (stack[i].mnemonic[0] == 'n' && stack[i].mnemonic[1] == 'o' && stack[i].mnemonic[2] == 'p' ){
      strcpy(stack[i].mnemonic , "jmp");
      nop_swap=true;
    } else if (stack[i].mnemonic[0] == 'j' && stack[i].mnemonic[1] == 'm' && stack[i].mnemonic[2] == 'p' ){
      strcpy(stack[i].mnemonic , "nop");
      jmp_swap=true;
    }

    if (is_stack_corrupted(stack, stack_depth) == true){
      if (jmp_swap){
        strcpy(stack[i].mnemonic , "jmp");
        jmp_swap=false;
      } else if (nop_swap){
        strcpy(stack[i].mnemonic , "nop");
        nop_swap=false;
      }
    } else {
      return(i+1);
    }
  }

  return(-1);
}


int main(int argc, char *argv[])
{
  char input_file_path[1024]={'\0'};

  if (argc == 2) {
    strcpy(input_file_path, argv[1]);
    printf("Using input file : %s\n", input_file_path);
  } else {
    printf("You forgot to provide an input file !! \nUsage:\t\t%s input_file_name.txt\n", argv[0]);
    exit(-1);
  }

  int line_count = get_line_count(input_file_path);
  instruction_t *callstack = calloc(line_count, sizeof(instruction_t));
  parse_input_file(input_file_path, callstack);

  if (is_stack_corrupted(callstack, line_count) == true){
    printf("{Day 8 :: Challenge 1 solution} Stack corrupted; accumulator value before loop : %ld\n", accumulator_g);
  } else {
    printf("No BUG in stack :P\n");
    return(-1);
  }

  int64_t ret_val=0;
  if ((ret_val = try_brute_force_bug_find(callstack, line_count)) >= 0){
    printf("{Day 8 :: Challenge 2 solution} Foung BUG at offset %ld !! Stack repaired :D; accumulator value at the end of stack : %ld\n", ret_val, accumulator_g);
  } else {
    printf("Unable to fix bug in stack :(\n");
    return(-2);
  }

  free(callstack);

  return (0);
}
