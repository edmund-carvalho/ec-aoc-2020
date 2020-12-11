

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LINE_LEN 128

int64_t builtin_adapter_joltage_g=0;
int64_t charging_outlet_joltage_g=0;


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


void sort_adapter_list(int64_t *arr, int count)
{
  int i,j;
  int64_t tmp;

  for (i=0; i<count; i++){
    for (j=i+1; j<count; j++){  
      if (arr[i] > arr[j]){
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}


int64_t _get_device_adaptor_max_joltage(int64_t *list, int count)
{
  return(list[count-1]+3);
}


void _print_adaptor_list(int64_t *list, int count)
{
  for (int idx=0;idx<count;idx++){
    printf("adapter_list[%d] : %ld\n", idx, list[idx]);
  }
}


void process_adapter_chain(int64_t *list, int count)
{
  int64_t previous_adapter_joltage = charging_outlet_joltage_g;
  int64_t diff;
  int64_t jolt_1_diff=0; 
  int64_t jolt_2_diff=0; 
  int64_t jolt_3_diff=1; 

  for (int idx=0;idx<count;idx++){
    diff = list[idx]-previous_adapter_joltage;
    switch(diff){
      case 1:
        jolt_1_diff++;
        break;
      case 2:
        jolt_2_diff++;
        break;
      case 3:
        jolt_3_diff++;
        break;
      default:
        printf("Invalid Joltage diff=%ld\n",diff);
    }
    previous_adapter_joltage=list[idx];
  }

  printf("{Day 10 :: Challenge 1 solution} Jolt product : %ld \n",jolt_1_diff*jolt_3_diff);
}


void generate_child_chain(int *parent_chain, int offset, int count)
{
  int64_t previous_adapter_joltage = charging_outlet_joltage_g;
  int remaining_samples=count-offset;
  int64_t diff;

  if ((parent_chain==NULL)||(offset==0)){
    //first chain
    // new_node
  } else {
    // new_node
    memcpy(parent_chain, child_chain, sizeof(int)*offset-1);
    previous_adapter_joltage = child_chain[offset];
  }

  for (int idx=offset;idx<remaining_samples;idx++){
    //diff1
    child_chain[id] = 
  }
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

  int num_adapters = get_line_count(input_file_path);
  int64_t *adapter_list = calloc(num_adapters, sizeof(int64_t));
  parse_input_file(input_file_path, adapter_list);
  sort_adapter_list(adapter_list, num_adapters);
  builtin_adapter_joltage_g = _get_device_adaptor_max_joltage(adapter_list, num_adapters);
  printf("builtin_adapter_joltage_g : %ld\n", builtin_adapter_joltage_g);
  process_adapter_chain(adapter_list, num_adapters);

  free(adapter_list);

  return (0);
}
