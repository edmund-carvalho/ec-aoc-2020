#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define CHECK_LIST_FIELD_COUNT 26

typedef enum check_sum_type{
  eCHALLENGE_1=1,
  eCHALLENGE_2
} check_sum_type_t;


void _set_checklist_field(int32_t *form, char field)
{
  int offset = field-97;
  form[offset] += 1;
}


void _reset_form(int32_t *form)
{
  for(int i=0;i<CHECK_LIST_FIELD_COUNT; i++){
    form[i]=0;
  }
}


int _group_form_checksum(int32_t *form, int group_head_count, uint8_t check_sum_type_id)
{
  int ret=0;

  for(int i=0;i<CHECK_LIST_FIELD_COUNT; i++){
    if(check_sum_type_id == eCHALLENGE_1){
      if(form[i]>0){
        ret++;
      }
    } else if (check_sum_type_id == eCHALLENGE_2){
      if(form[i]==group_head_count){
        ret++;
      }
    } else {
      printf("Unsupported check sumtype !!");
      // ASSERT 
    }
  }

  return(ret);
}


int process_custom_customs_form(char *file_path, int cid)
{
  FILE *fp = fopen(file_path, "r");
  char c;
  bool newline_flag = false;
  int checksum=0;
  int group_head_count=0;
  int32_t current_group_form[CHECK_LIST_FIELD_COUNT] = {0};

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  while (true) {
    c = fgetc(fp);

    if (feof(fp)){
      /*
       * Last group will end with EOF !
       */
      checksum += _group_form_checksum(current_group_form, group_head_count, cid);
      _reset_form(current_group_form);
      break;
    } else if (c == '\n' && newline_flag==true){
      /*
       * Group ended, compute group checksum"
       */
      checksum += _group_form_checksum(current_group_form, group_head_count, cid);
      _reset_form(current_group_form);
      newline_flag = false;
      group_head_count=0;
    } else if (c == '\n'){
      /*
       * Finished reading current persons form
       */
      newline_flag = true;
      group_head_count++;
    } else {
      /*
       * Got next field in persons form
       */
      _set_checklist_field(current_group_form, c);
      newline_flag = false;
    }
  }

  printf("{Day 6 :: Challenge_%d solution} Checksum : %d \n", cid, checksum);

err:
  fclose(fp);
  return (checksum);
}


int main(int argc, char *argv[])
{
  char input_file_path[1024]={'\0'};
  int cid=0;
  const char check_sum_type_list[2][8] = {"chksum1", "chksum2"};

  if (argc == 3) {
    strcpy(input_file_path, argv[1]);
    printf("Using input file : %s\n", input_file_path);
  } else {
    printf("You forgot to provide an input file !! \nUsage:\t\t%s input_file_name.txt [chksum1 | chksum2]\n", argv[0]);
    exit(-1);
  }

  if (strcmp(check_sum_type_list[0], argv[2]) == 0){
    cid=eCHALLENGE_1;
  } else if (strcmp(check_sum_type_list[1], argv[2]) == 0){
    cid=eCHALLENGE_2;
  } else {
    printf("Unsupported check sum type :%s\n",argv[2]);
    exit(-1);
  }

  process_custom_customs_form(input_file_path, cid);

  return (0);
}
