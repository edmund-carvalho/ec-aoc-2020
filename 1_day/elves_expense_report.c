/*
 *  https://adventofcode.com/2020/day/1
 *
 *  --- Day 1: Report Repair ---
 *  --- Part Two ---
 *  The Elves in accounting just need you to fix your expense report (your puzzle input); apparently, something isn't quite adding up.
 *  Specifically, they need you to find the two entries that sum to 2020 and then multiply those two numbers together.
 *  For example, suppose your expense report contained the following:
 *  
 *  1721
 *  979
 *  366
 *  299
 *  675
 *  1456
 *  
 *  In this list, the two entries that sum to 2020 are 1721 and 299. Multiplying them together produces 1721 * 299 = 514579, so the correct answer is 514579.
 *  Of course, your expense report is much larger. Find the two entries that sum to 2020; what do you get if you multiply them together?
 *
 *  --- Part Two ---
 *  Find three numbers in your expense report that meet the same criteria.
 *  Using the above example again, the three entries that sum to 2020 are 979, 366, and 675. Multiplying them together produces the answer, 241861950.
 *  In your expense report, what is the product of the three entries that sum to 2020?
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define INPUT_EX1 "aoc_day1_input1.txt"
#define MAX_LINE_LEN 10

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


int parse_input_file(char *file_path, int *arr)
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
    status = sscanf(line, "%d", &arr[idx]);
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


int process_expense_report(int *arr, int len)
{
  for (int i = 0; i < len; i++) {
    for (int j = i + 1; j < len; j++) {
      if ((arr[i] + arr[j]) == 2020) {
        printf("{Day 1 :: Challenge 1 solution} [i:%d j:%d] -> i*j = %d \n", arr[i], arr[j], arr[i] * arr[j]);
      }
      for (int k = j + 1; k < len; k++) {
        if ((arr[i] + arr[j]  + arr[k]) == 2020) {
          printf("{Day 1 :: Challenge 2 solution} [i:%d j:%d k:%d] i*j*k = %d \n", arr[i], arr[j], arr[k], (arr[i]*arr[j]*arr[k]));
        }
      }
    }
  }

  return (0);
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
  int *data = malloc(line_count * sizeof(int));
  parse_input_file(input_file_path, data);
  process_expense_report(data, line_count);

  free(data);

  return (0);
}
