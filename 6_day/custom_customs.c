#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LINE_LEN 32
#define CHECK_LIST_FIELD_COUNT 26


int set_checklist_field(bool *form, char field)
{
  int offset = field-97;
  //printf("%s[%d]\n",__func__,offset);
  form[offset] = true;
  return (0);
}


void _reset_form(bool *form)
{
  for(int i=0;i<CHECK_LIST_FIELD_COUNT; i++){
    form[i]=false;
  }
}


int _form_checksum(bool *form)
{
  int ret=0;
  for(int i=0;i<CHECK_LIST_FIELD_COUNT; i++){
    if(form[i]==true){
      ret++;
    }
  }
  //printf("Current user checksum %d\n", ret);
  return(ret);
}

int parse_input_file(char *file_path)
{
  FILE *fp = fopen(file_path, "r");
  char c;
  bool newline_flag = false;
  int checksum=0;

  bool current_group_form[CHECK_LIST_FIELD_COUNT] = {false};

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
      //printf("End of group ... \n");
      checksum += _form_checksum(current_group_form);
      _reset_form(current_group_form);
      break;
    } else if (c == '\n' && newline_flag==true){
      /*
       * Group ended, compute sum of "checklist==yes"
       */
      //printf("End of group ... \n");
      checksum += _form_checksum(current_group_form);
      _reset_form(current_group_form);
      newline_flag = false;
    } else if (c == '\n'){
      newline_flag = true;
    } else {
      //printf("{field:%c} ",c);
      set_checklist_field(current_group_form, c);
      newline_flag = false;
    }
  }

  printf("{Day 6 :: Challenge 1 solution} Checksum : %d \n", checksum);

err:
  fclose(fp);
  return (checksum);
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

  parse_input_file(input_file_path);

  return (0);
}
