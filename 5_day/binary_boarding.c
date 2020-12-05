/*  
 *  
 *  https://adventofcode.com/2020/day/5
 *  
 *  --- Day 5: Binary Boarding ---
 *  
 *  You board your plane only to discover a new problem: you dropped your boarding pass!
 *  You aren't sure which seat is yours, and all of the flight attendants are 
 *  busy with the flood of people that suddenly made it through passport control.
 *  
 *  You write a quick program to use your phone's camera to scan all of 
 *  the nearby boarding passes (your puzzle input); perhaps you can find your seat through process of elimination.
 *  
 *  Instead of zones or groups, this airline uses binary space partitioning to seat people.
 *  A seat might be specified like FBFBBFFRLR, where F means "front", B means "back", L means "left", and R means "right".
 *  
 *  The first 7 characters will either be F or B; 
 *  these specify exactly one of the 128 rows on the plane (numbered 0 through 127).
 *  Each letter tells you which half of a region the given seat is in. 
 *  Start with the whole list of rows; the first letter indicates whether the seat is in the front (0 through 63) or the back (64 through 127).
 *  The next letter indicates which half of that region the seat is in, and so on until you're left with exactly one row.
 *  
 *  For example, consider just the first seven characters of FBFBBFFRLR:
 *  
 *      Start by considering the whole range, rows 0 through 127.
 *      F means to take the lower half, keeping rows 0 through 63.
 *      B means to take the upper half, keeping rows 32 through 63.
 *      F means to take the lower half, keeping rows 32 through 47.
 *      B means to take the upper half, keeping rows 40 through 47.
 *      B keeps rows 44 through 47.
 *      F keeps rows 44 through 45.
 *      The final F keeps the lower of the two, row 44.
 *  
 *  The last three characters will be either L or R; 
 *  these specify exactly one of the 8 columns of seats on the plane (numbered 0 through 7). 
 *  The same process as above proceeds again, this time with only three steps. 
 *  L means to keep the lower half, while R means to keep the upper half.
 *  
 *  For example, consider just the last 3 characters of FBFBBFFRLR:
 *  
 *      Start by considering the whole range, columns 0 through 7.
 *      R means to take the upper half, keeping columns 4 through 7.
 *      L means to take the lower half, keeping columns 4 through 5.
 *      The final R keeps the upper of the two, column 5.
 *  
 *  So, decoding FBFBBFFRLR reveals that it is the seat at row 44, column 5.
 *  
 *  Every seat also has a unique seat ID: multiply the row by 8, then add the column. In this example, the seat has ID 44 * 8 + 5 = 357.
 *  
 *  Here are some other boarding passes:
 *  
 *      BFFFBBFRRR: row 70, column 7, seat ID 567.
 *      FFFBBBFRRR: row 14, column 7, seat ID 119.
 *      BBFFBBFRLL: row 102, column 4, seat ID 820.
 *  
 *  As a sanity check, look through your list of boarding passes. What is the highest seat ID on a boarding pass?
 *  
 *  
 *  --- Part Two ---
 *  
 *  Ding! The "fasten seat belt" signs have turned on. Time to find your seat.
 *  
 *  It's a completely full flight, so your seat should be the only missing boarding pass in your list.
 *  However, there's a catch: some of the seats at the very front and back of the plane don't exist on this aircraft,
 *  so they'll be missing from your list as well.
 *  Your seat wasn't at the very front or back, though; the seats with IDs +1 and -1 from yours will be in your list.
 *  What is the ID of your seat?
 */  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define MAX_LINE_LEN 16

typedef struct boarding_pass {
  char str[MAX_LINE_LEN];
  uint8_t row;
  uint8_t col;
  uint32_t seat_id;
}boarding_pass_t;

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


int32_t _get_seat_id(uint8_t row, uint8_t col)
{
  return ((8*row)+col);
}


void _set_row_col_offsets(boarding_pass_t *p){
  int8_t i=0;
  int8_t offset = 0;

  for (i=0; i<7;i++){
    offset = abs(i-7);
    if (p->str[i] == 'F'){
    } else if (p->str[i] == 'B'){
      p->row |= (1<<(offset-1));
    } else {
      printf("Unsupported char i:%d char:%c !!\n",i,p->str[i]);
      break;
    }
  }

  for (; i<10; i++){
    offset = abs(i-10);
    if (p->str[i] == 'R'){
      p->col |= (1<<(offset-1));
    } else if (p->str[i] == 'L'){
    } else {
      printf("Unsupported char i:%d char:%c !!\n", i, p->str[i]);
      break;
    }
  }
}


int parse_input_file(char *file_path, boarding_pass_t *arr)
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
    status = sscanf(line, "%s", arr[idx].str);
    if (status != 1) {
      printf("ERROR::LINE %d \n", idx);
      break;
    }

    _set_row_col_offsets(&arr[idx]);
    arr[idx].seat_id = _get_seat_id(arr[idx].row,arr[idx].col);

    idx++;
  }

err:
  fclose(fp);
  return (idx);
}


void print_passes(boarding_pass_t *arr, int count)
{
  for (int idx=0;idx<count;idx++){
    printf("str:%s col%d row:%d seat_id:%d\n", arr[idx].str, arr[idx].col, arr[idx].row, arr[idx].seat_id);
  }
}


int32_t sort_boarding_passes(boarding_pass_t *arr, int count)
{
  boarding_pass_t p;
  int i,j;

  for (i=0; i<count; i++){
    for (j=i+1; j<count; j++){  
      if (arr[i].seat_id > arr[j].seat_id){
        p = arr[i];
        arr[i] = arr[j];
        arr[j] = p;
      }
    }
  }

  return(0);
}


int32_t get_max_seat_id_from_sorted_list(boarding_pass_t *arr, int count)
{
  return(arr[count-1].seat_id);
}


int32_t get_missing_seat_id(boarding_pass_t *arr, int count){
  int32_t previous_seat_id = arr[0].seat_id;

  for (int idx=1; idx<count; idx++){
    if ((arr[idx].seat_id - previous_seat_id) != 1){
      return(arr[idx].seat_id-1);
    }
    previous_seat_id = arr[idx].seat_id;
  }

  return(0);
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

  int passenger_count = get_line_count(input_file_path);
  boarding_pass_t *boarding_pass_arr = calloc(passenger_count, sizeof(boarding_pass_t)); 

  parse_input_file(input_file_path, boarding_pass_arr);
  sort_boarding_passes(boarding_pass_arr, passenger_count);
  printf("Max seat ID : %d \n", get_max_seat_id_from_sorted_list(boarding_pass_arr, passenger_count));    
  printf("Missing seat id : %d\n", get_missing_seat_id(boarding_pass_arr, passenger_count));

  free(boarding_pass_arr);

  return (0);
}
