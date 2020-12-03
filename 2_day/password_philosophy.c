/*  
 *  https://adventofcode.com/2020/day/1
 *  
 *  --- Day 2: Password Philosophy ---
 *  
 *  Your flight departs in a few days from the coastal airport; the easiest way down to the coast from here is via toboggan.
 *  The shopkeeper at the North Pole Toboggan Rental Shop is having a bad day. 
 *  "Something's wrong with our computers; we can't log in!" You ask if you can take a look.
 *  Their password database seems to be a little corrupted: 
 *  some of the passwords wouldn't have been allowed by the Official Toboggan Corporate Policy that was in effect when they were chosen.
 *  To try to debug the problem, they have created a list (your puzzle input) of passwords 
 *  (according to the corrupted database) and the corporate policy when that password was set.
 *  
 *  For example, suppose you have the following list:
 *  1-3 a: abcde
 *  1-3 b: cdefg
 *  2-9 c: ccccccccc
 *  
 *  Each line gives the password policy and then the password.
 *  The password policy indicates the lowest and highest number of times a given letter must appear for the password to be valid. 
 *  For example, 1-3 a means that the password must contain a at least 1 time and at most 3 times.
 *  In the above example, 2 passwords are valid. 
 *  The middle password, cdefg, is not; it contains no instances of b, but needs at least 1. 
 *  The first and third passwords are valid: they contain one a or nine c, both within the limits of their respective policies.
 *  How many passwords are valid according to their policies?
 *  
 *  
 *  --- Part Two ---
 *  While it appears you validated the passwords correctly, they don't seem to be what the Official Toboggan Corporate Authentication System is expecting.
 *  The shopkeeper suddenly realizes that he just accidentally explained the password policy rules from his old job
 *  at the sled rental place down the street! The Official Toboggan Corporate Policy actually works a little differently.
 *  Each policy actually describes two positions in the password, where 1 means the first character, 2 means the second character, and so on.
 *  (Be careful; Toboggan Corporate Policies have no concept of "index zero"!) Exactly one of these positions must contain the given letter. 
 *  Other occurrences of the letter are irrelevant for the purposes of policy enforcement.
 *  
 *  Given the same example list from above:
 *      1-3 a: abcde is valid: position 1 contains a and position 3 does not.
 *      1-3 b: cdefg is invalid: neither position 1 nor position 3 contains b.
 *      2-9 c: ccccccccc is invalid: both position 2 and position 9 contain c.
 *  How many passwords are valid according to the new interpretation of the policies?
 */  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LINE_LEN 1024
#define MAX_PASSWORD_STR_LEN 256

#define USER_DB_FORMAT "%d-%d %c: %s"

typedef struct user_db {
  char password[MAX_PASSWORD_STR_LEN];
  uint8_t policy_key;
  int32_t min_count;
  int32_t max_count;
  uint32_t password_len;
}user_db_t;


typedef enum db_policy_type {
  eSLED_RENTAL,
  eTOBOGGAN
}db_policy_type_t;


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


int parse_input_file(char *file_path, user_db_t *db)
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
    status = sscanf(line, USER_DB_FORMAT, &db[idx].min_count, &db[idx].max_count, &db[idx].policy_key, db[idx].password);
    if (status != 4) {
      printf("ERROR::LINE %d \n", idx);
      status = -2;
      break;
    } else {
      db[idx].password_len = strlen(db[idx].password);
    }
    idx++;
  }

err:
  fclose(fp);
  return (idx);
}


void dump_db(user_db_t *db, int user_count)
{
  for (int idx=0; idx<user_count; idx++){
    printf("[%4d] %3d %3d %c %s pwd_len(%d)\n", idx, db[idx].min_count, db[idx].max_count, db[idx].policy_key, db[idx].password, db[idx].password_len);
  }
}


static bool _validate_password_sled_rental(user_db_t *usr){
  bool is_valid=true;
  uint32_t count=0;

  for(int i=0; i<usr->password_len; i++){
    if(usr->password[i] == usr->policy_key){
      count++;
      if(count > usr->max_count){
        // Not really required minor optimization
        break; 
      }
    }
  }

  if ((count < usr->min_count) || (count > usr->max_count)){
    is_valid = false;
  }

  return(is_valid);
}


static bool _validate_password_toboggan(user_db_t *usr){
  bool is_valid=true;
  uint32_t count=0;

  if(usr->password[usr->min_count-1] == usr->policy_key){
    count++;
  }

  if(usr->password[usr->max_count-1] == usr->policy_key){
    count++;
  }

  if (count!=1){
    is_valid = false;
  }

  return(is_valid);
}


bool _validate_password(user_db_t *usr, db_policy_type_t policy_key_type)
{
  bool is_valid=true;

  if (usr->min_count <=0 || (usr->max_count <= 0) ){
    printf("Unhandled case_1 !!!\n");
    is_valid = false;
    exit(-1);
  } else if (usr->min_count == usr->max_count) {
    printf("Unhandled case_2 !!!\n");
    is_valid = false;
    exit(-1);
  }

  switch(policy_key_type){
    case eSLED_RENTAL:
      is_valid = _validate_password_sled_rental(usr);
      break; 
    case eTOBOGGAN:
      is_valid = _validate_password_toboggan(usr);
      break; 
    default:
      printf("Unsupported policy type !!");
      is_valid = false;
  }

  return(is_valid);
}


uint32_t validate_db(user_db_t *db, uint64_t user_count)
{
  uint32_t invalid_password_count=0;

  for (int idx=0; idx<user_count; idx++){
    if(_validate_password(&db[idx], eSLED_RENTAL) == false){
      //printf("Corrupted password on line %d !!\n", idx+1);
      invalid_password_count++;
    }
  }

  printf("{Day 2 :: Challenge 1 solution} Found <%lu> valid passwords. \n", (user_count-invalid_password_count));

  invalid_password_count=0;

  for (int idx=0; idx<user_count; idx++){
    if(_validate_password(&db[idx], eTOBOGGAN) == false){
      //printf("Corrupted password on line %d !!\n", idx+1);
      invalid_password_count++;
    }
  }

  printf("{Day 2 :: Challenge 2 solution} Found <%lu> valid passwords. \n", (user_count-invalid_password_count));

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

  uint64_t user_count = get_line_count(input_file_path);
  user_db_t *data = calloc(user_count, sizeof(user_db_t));
  parse_input_file(input_file_path, data);
  //dump_db(data, user_count);
  validate_db(data, user_count);

  free(data);

  return (0);
}
