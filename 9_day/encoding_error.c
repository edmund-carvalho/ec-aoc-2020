/*  
 *  https://adventofcode.com/2020/day/9
 *  
 *  --- Day 9: Encoding Error ---
 *  
 *  With your neighbor happily enjoying their video game,
 *  you turn your attention to an open data port on the little screen in the seat in front of you.
 *  
 *  Though the port is non-standard, 
 *  you manage to connect it to your computer through the clever use of several paperclips.
 *  Upon connection, the port outputs a series of numbers (your puzzle input).
 *  
 *  The data appears to be encrypted with the eXchange-Masking Addition System (XMAS) which, 
 *  conveniently for you, is an old cypher with an important weakness.
 *  
 *  XMAS starts by transmitting a preamble of 25 numbers.
 *  After that, each number you receive should be the sum of any two of the 25 immediately previous numbers.
 *  The two numbers will have different values, and there might be more than one such pair.
 *  
 *  For example, suppose your preamble consists of the numbers 1 through 25 in a random order.
 *  To be valid, the next number must be the sum of two of those numbers:
 *  
 *      26 would be a valid next number, as it could be 1 plus 25 (or many other pairs, like 2 and 24).
 *      49 would be a valid next number, as it is the sum of 24 and 25.
 *      100 would not be valid; no two of the previous 25 numbers sum to 100.
 *      50 would also not be valid; although 25 appears in the previous 25 numbers, the two numbers in the pair must be different.
 *  
 *  Suppose the 26th number is 45, and the first number (no longer an option,
 *  as it is more than 25 numbers ago) was 20. 
 *  Now, for the next number to be valid, 
 *  there needs to be some pair of numbers among 1-19, 21-25, or 45 that add up to it:
 *  
 *      26 would still be a valid next number, as 1 and 25 are still within the previous 25 numbers.
 *      65 would not be valid, as no two of the available numbers sum to it.
 *      64 and 66 would both be valid, as they are the result of 19+45 and 21+45 respectively.
 *  
 *  Here is a larger example which only considers the previous 5 numbers (and has a preamble of length 5):
 *  
 *  35
 *  20
 *  15
 *  25
 *  47
 *  40
 *  62
 *  55
 *  65
 *  95
 *  102
 *  117
 *  150
 *  182
 *  127
 *  219
 *  299
 *  277
 *  309
 *  576
 *  
 *  In this example, after the 5-number preamble, almost every number is the sum of two of the previous 5 numbers;
 *  the only number that does not follow this rule is 127.
 *  
 *  The first step of attacking the weakness in the XMAS data is to find the first number in the list (after the preamble)
 *  which is not the sum of two of the 25 numbers before it. What is the first number that does not have this property?
 *  
 *  
 *  --- Part Two ---
 *  
 *  
 *  The final step in breaking the XMAS encryption relies on the invalid number you just found:
 *  you must find a contiguous set of at least two numbers in your list which sum to the invalid number from step 1.
 *  
 *  Again consider the above example:
 *  
 *  35
 *  20
 *  15
 *  25
 *  47
 *  40
 *  62
 *  55
 *  65
 *  95
 *  102
 *  117
 *  150
 *  182
 *  127
 *  219
 *  299
 *  277
 *  309
 *  576
 *  
 *  In this list, adding up all of the numbers from 15 through 40 produces the invalid number from step 1, 127. 
 *  (Of course, the contiguous set of numbers in your actual list might be much longer.)
 *  
 *  To find the encryption weakness, add together the smallest and largest number in this contiguous range; 
 *  in this example, these are 15 and 47, producing 62.
 *  
 *  What is the encryption weakness in your XMAS-encrypted list of numbers?
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LINE_LEN 128
int preamble_length_g=25; //TODO Get this from command line args 

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


int parse_input_file(char *file_path, int64_t *arr)
{
  FILE *fp = fopen(file_path, "r");
  int idx = 0;
  char line[MAX_LINE_LEN];
  int status = 0;

  if (fp == NULL) {
    perror("Failed to read line count");
    status = -1;
    goto err;
  }

  while (fgets(line, MAX_LINE_LEN, fp)) {
    status = sscanf(line, "%ld", &arr[idx]);
    if (status != 1) {
      printf("ERROR::LINE %d \n", idx);
      status = -2;
      break;
    }
    idx++;
  }

err:
  fclose(fp);
  return (idx);
}


void print_input(int64_t *arr, int count)
{
  int i;
  for(i=0; i<count; i++){
    printf("%ld\n",arr[i]);
  }
}


int64_t process_xmas_data(int64_t *arr, int len)
{
  int i,j,k;
  bool flag=false;

  for (i = preamble_length_g; i < len; i++) {
    flag=false;
    for (j = i-preamble_length_g; j < i; j++) {
      for(k=j+1; k<i; k++){
        if (arr[j]+arr[k] == arr[i]){
          flag = true;
          goto next;
        }
      }
    }

next:
    if (flag==false){
      printf("{Day 9 :: Challenge 1 solution} Following number does not allign to the xmas protocol %ld \n",arr[i]);
      return(arr[i]);
    }
  }

  return (0);
}


int64_t _min_custom(int64_t *arr, int start_idx, int end_idx)
{
  int64_t min_val=arr[start_idx];
  
  for(int i=start_idx+1;i<end_idx; i++){
    if (arr[i] < min_val){
      min_val = arr[i];
    }
  }

  return(min_val);
}


int64_t _max_custom(int64_t *arr, int start_idx, int end_idx)
{
  int64_t max_val=arr[start_idx];

  for(int i=start_idx+1;i<end_idx; i++){
    if (arr[i] > max_val){
      max_val = arr[i];
    }
  }

  return(max_val);
}


int64_t exploit_xmas_data(int64_t *arr, int len, int key)
{
  int start_idx=0;
  int sum=0;
  int64_t exploit_key;

  for(int i=start_idx; i<len;){
    sum+=arr[i];
    if (sum==key){
      exploit_key = _min_custom(arr, start_idx, i) + _max_custom(arr, start_idx, i);
      printf("{Day 9 :: Challenge 2 solution} Weakness key : %ld \n", exploit_key);
      break;
    } else if (sum>key){
      sum=0;
      start_idx++;
      i = start_idx;
      continue;
    }
    i++;
  }

  return(exploit_key);
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
  int64_t *xmas_data_stream = calloc(line_count, sizeof(int64_t));
  parse_input_file(input_file_path, xmas_data_stream);

  exploit_xmas_data(xmas_data_stream, line_count, process_xmas_data(xmas_data_stream, line_count));

  free(xmas_data_stream);

  return (0);
}
