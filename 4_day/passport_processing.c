/*  
 *  
 *  https://adventofcode.com/2020/day/1
 *  
 *  --- Day 4: Passport Processing ---
 *  
 *  You arrive at the airport only to realize that you grabbed your North Pole Credentials instead of your passport.
 *  While these documents are extremely similar, North Pole Credentials aren't issued by a country 
 *  and therefore aren't actually valid documentation for travel in most of the world.
 *  
 *  It seems like you're not the only one having problems, though; 
 *  a very long line has formed for the automatic passport scanners, 
 *  and the delay could upset your travel itinerary.
 *  
 *  Due to some questionable network security, you realize you might be able to solve both of these problems at the same time.
 *  
 *  The automatic passport scanners are slow because they're having trouble detecting which passports have all required fields. 
 *  The expected fields are as follows:
 *  
 *      byr (Birth Year)
 *      iyr (Issue Year)
 *      eyr (Expiration Year)
 *      hgt (Height)
 *      hcl (Hair Color)
 *      ecl (Eye Color)
 *      pid (Passport ID)
 *      cid (Country ID)
 *  
 *  Passport data is validated in batch files (your puzzle input).
 *  Each passport is represented as a sequence of key:value pairs separated by spaces or newlines.
 *  Passports are separated by blank lines.
 *  
 *  Here is an example batch file containing four passports:
 *  
 *  ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
 *  byr:1937 iyr:2017 cid:147 hgt:183cm
 *  
 *  iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
 *  hcl:#cfa07d byr:1929
 *  
 *  hcl:#ae17e1 iyr:2013
 *  eyr:2024
 *  ecl:brn pid:760753108 byr:1931
 *  hgt:179cm
 *  
 *  hcl:#cfa07d eyr:2025 pid:166559648
 *  iyr:2011 ecl:brn hgt:59in
 *  
 *  The first passport is valid - all eight fields are present. The second passport is invalid - it is missing hgt (the Height field).
 *  
 *  The third passport is interesting; the only missing field is cid, so it looks like data from North Pole Credentials,
 *  not a passport at all! Surely, nobody would mind if you made the system temporarily ignore missing cid fields. Treat this "passport" as valid.
 *  The fourth passport is missing two fields, cid and byr. Missing cid is fine, but missing any other field is not, so this passport is invalid.
 *  According to the above rules, your improved system would report 2 valid passports.
 *  Count the number of valid passports - those that have all required fields. Treat cid as optional. In your batch file, how many passports are valid?
 *  
 *  Your puzzle answer was 226.
 *  
 *  
 *  --- Part Two ---
 *  
 *  The line is moving more quickly now, but you overhear airport security talking about how passports with invalid data are getting through.
 *  Better add some data validation, quick!
 *  
 *  You can continue to ignore the cid field, but each other field has strict rules about what values are valid for automatic validation:
 *  
 *      byr (Birth Year) - four digits; at least 1920 and at most 2002.
 *      iyr (Issue Year) - four digits; at least 2010 and at most 2020.
 *      eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
 *      hgt (Height) - a number followed by either cm or in:
 *          If cm, the number must be at least 150 and at most 193.
 *          If in, the number must be at least 59 and at most 76.
 *      hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
 *      ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
 *      pid (Passport ID) - a nine-digit number, including leading zeroes.
 *      cid (Country ID) - ignored, missing or not.
 *  
 *  Your job is to count the passports where all required fields are both present and valid according to the above rules. Here are some example values:
 *  
 *  byr valid:   2002
 *  byr invalid: 2003
 *  
 *  hgt valid:   60in
 *  hgt valid:   190cm
 *  hgt invalid: 190in
 *  hgt invalid: 190
 *  
 *  hcl valid:   #123abc
 *  hcl invalid: #123abz
 *  hcl invalid: 123abc
 *  
 *  ecl valid:   brn
 *  ecl invalid: wat
 *  
 *  pid valid:   000000001
 *  pid invalid: 0123456789
 *  
 *  Here are some invalid passports:
 *  
 *  eyr:1972 cid:100
 *  hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926
 *  
 *  iyr:2019
 *  hcl:#602927 eyr:1967 hgt:170cm
 *  ecl:grn pid:012533040 byr:1946
 *  
 *  hcl:dab227 iyr:2012
 *  ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277
 *  
 *  hgt:59cm ecl:zzz
 *  eyr:2038 hcl:74454a iyr:2023
 *  pid:3556412378 byr:2007
 *  
 *  Here are some valid passports:
 *  
 *  pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
 *  hcl:#623a2f
 *  
 *  eyr:2029 ecl:blu cid:129 byr:1989
 *  iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm
 *  
 *  hcl:#888785
 *  hgt:164cm byr:2001 iyr:2015 cid:88
 *  pid:545766238 ecl:hzl
 *  eyr:2022
 *  
 *  iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719
 *  
 *  Count the number of valid passports - those that have all required fields and valid values. 
 *  Continue to treat cid as optional. In your batch file, how many passports are valid?
 *  
 *  Your puzzle answer was 160.
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LINE_LEN 1024

typedef struct passport_t{
  char byr[16]; // (Birth Year)
  char iyr[16]; // (Issue Year)
  char eyr[16]; // (Expiration Year)
  char hgt[16]; // (Height)
  char hcl[16]; // (Hair Color)
  char ecl[16]; // (Eye Color)
  char pid[16]; // (Passport ID)
  char cid[16]; // (Country ID)
}passport_t;

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


int replace_space_with_newline(char * file_path)
{  
  FILE *src_fp = fopen(file_path, "rb+");
  char c;

  if (src_fp == NULL) {
    perror("Failed to open input file ");
    return(0);
  }

  while ((c = fgetc(src_fp))){
    if (c == ' '){
      fseek(src_fp, -1, SEEK_CUR);
      fputc('\n', src_fp);
    } else if (c == EOF){
      break;
    } else {
      // Do nothing
    }
  }

  fclose(src_fp);
  return(0);

}


int64_t get_passport_count(char* file_path, int line_count)
{
  FILE *fp = fopen(file_path, "r");
  int64_t count = 1;
  char line[MAX_LINE_LEN];
  int len=0;

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  for(int i=0;i<line_count;i++){
    fgets(line, MAX_LINE_LEN, fp);
    len = strlen(line);
    if(len <= 1 && line[0] == '\n'){
      count++;
    }
  }

  printf("Number of passports : %lu\n",count);

err:
  fclose(fp);
  return (count);
}


int parse_input_file(char *file_path, int line_count, passport_t *arr)
{
  FILE *fp = fopen(file_path, "r");
  int idx = 0;
  char line[MAX_LINE_LEN]={'\0'};
  int count=0;
  int len=0;

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  for(int i=0;i<line_count;i++){
    fgets(line, MAX_LINE_LEN, fp);
    len = strlen(line);
    //printf("{%d}line:%s", len, line);
    if(len <= 1 && line[0] == '\n'){ // Blankline new passport !
      count++;
      idx++;
    } else {
      if (line[0] =='b' && line[1]=='y' && line[2]=='r'){
        strcpy(arr[idx].byr, &line[4]);
      }else if (line[0] =='i' && line[1]=='y' && line[2]=='r'){
        strcpy(arr[idx].iyr, &line[4]);
      }else if (line[0] =='e' && line[1]=='y' && line[2]=='r'){
        strcpy(arr[idx].eyr, &line[4]);
      }else if (line[0] =='h' && line[1]=='g' && line[2]=='t'){
        strcpy(arr[idx].hgt, &line[4]);
      }else if (line[0] =='h' && line[1]=='c' && line[2]=='l'){
        strcpy(arr[idx].hcl, &line[4]);
      }else if (line[0] =='e' && line[1]=='c' && line[2]=='l'){
        strcpy(arr[idx].ecl, &line[4]);
      }else if (line[0] =='p' && line[1]=='i' && line[2]=='d'){
        strcpy(arr[idx].pid, &line[4]);
      }else if (line[0] =='c' && line[1]=='i' && line[2]=='d'){
        strcpy(arr[idx].cid, &line[4]);
      } else {
        printf("CORRUPTED INPUT line : %d idx %d\n",i,idx);
      }
    }
  }

err:
  fclose(fp);
  return (idx);
}


void print_immigration_db(passport_t *db, int count)
{
  for (int i=0; i<count;i++){
    printf("\t[%d].byr = %s", i, db[i].byr);
    printf("\t[%d].iyr = %s", i, db[i].iyr);
    printf("\t[%d].eyr = %s", i, db[i].eyr);
    printf("\t[%d].hgt = %s", i, db[i].hgt);
    printf("\t[%d].hcl = %s", i, db[i].hcl);
    printf("\t[%d].ecl = %s", i, db[i].ecl);
    printf("\t[%d].pid = %s", i, db[i].pid);
    printf("\t[%d].cid = %s", i, db[i].cid);
    printf("\n------------------------------\n");
  }
}




bool _isvalid_byr(char * byr_str)
{
  bool ret=false;

  if (strlen(byr_str) == 0){
    return (false);
  }

  int byr = atoi(byr_str);
  
  if (byr>=1920 && byr<=2002){
    return(true);
  }

  //printf("[DEBUG] byr : %d ret->%d\n", byr, ret) ;
  return(ret);
}

bool _isvalid_iyr(char * iyr_str)
{
  bool ret=false;

  if (strlen(iyr_str) == 0){
    return (false);
  }

  int iyr = atoi(iyr_str);
  
  if (iyr>=2010 && iyr<=2020){
    return(true);
  }

  //printf("[DEBUG] iyr : %d ret->%d\n", iyr, ret) ;
  return(ret);
}

bool _isvalid_eyr(char * eyr_str)
{
  bool ret=false;

  if (strlen(eyr_str) == 0){
    return (false);
  }

  int byr = atoi(eyr_str);
  
  if (byr>=2020 && byr<=2030){
    ret = true;
  }
  return(ret);
}

bool _isvalid_hgt(char * hgt_str)
{
  bool ret=false;
  int len = strlen(hgt_str);
  char arr[8]={'\0'};
  int height;
  int i;

  if (len == 0){
    return (false);
  }

  for (i=0; i<len; i++){
    if (hgt_str[i]>47 && hgt_str[i]<58){
      arr[i] = hgt_str[i];
    } else {
      break;
    }
  }
  
  height = atoi(arr);

  if (hgt_str[i] == 'i' && hgt_str[i+1] == 'n'){
    if (height >58 && height<77){
      return(true);
    }
  }else if (hgt_str[i] == 'c' && hgt_str[i+1] == 'm'){
    if (height > 149 && height<194){
      return(true);
    }
  }

  //printf("[ERROR] Decoding height : %d arr[%d]->%c arr[%d]->%c\n", height, i, hgt_str[i], i+1, hgt_str[i+1]);

  return(ret);
}

bool _isvalid_hcl(char * hcl_str)
{
  bool ret=false;

  if (strlen(hcl_str) == 0){
    return (false);
  }

  if (hcl_str[0] != '#'){
    return (false);
  }

  for (int i=1; i<7; i++){
    /*
     * ASCII codes rabges for character values 
     * [0-9] -> 48 through 57
     * [A-Z] -> 65 through 90
     * [a-z] -> 97 through 122
     */
    //if ((hcl_str[i] > 47 && hcl_str[i] < 58) || (hcl_str[i] > 64 && hcl_str[i] < 91) || (hcl_str[i] > 96 && hcl_str[i] < 123)){
    if ((hcl_str[i] > 47 && hcl_str[i] < 58) || (hcl_str[i] > 96 && hcl_str[i] < 103)){
      ret = true;
    } else {
      return(false);
    }
  }

  return(ret);
}

bool _isvalid_ecl(char * ecl_str)
{
  bool ret=false; 
  char *eye_clr[7] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

  if (strlen(ecl_str) == 0){
    return (false);
  }

  for (int i=0; i<7; i++){
    if ((ecl_str[0] == eye_clr[i][0]) && (ecl_str[1] == eye_clr[i][1]) && (ecl_str[2] == eye_clr[i][2])){
      return(true);
    }
  }

  return(ret);
}

bool _isvalid_pid(char * pid_str)
{
  bool ret=true;

  if (strlen(pid_str) == 0){
    return (false);
  }

  for (int i=0; i<9; i++){
    if ((pid_str[i] < 48 || pid_str[i] > 57)){
      return(false);
    }
  }
  return(ret);
}

bool _isvalid_cid(char * cid_str)
{
  return(true);
}


void isvalid_immigration_db(passport_t *db, int count)
{
  int invalid_passport_count=0;

  for (int i=0; i<count;i++){
    //printf("\n------------------------------\n");
    //printf("\t[%d].byr = %s", i, db[i].byr);
    //printf("\t[%d].iyr = %s", i, db[i].iyr);
    //printf("\t[%d].eyr = %s", i, db[i].eyr);
    //printf("\t[%d].hgt = %s", i, db[i].hgt);
    //printf("\t[%d].hcl = %s", i, db[i].hcl);
    //printf("\t[%d].ecl = %s", i, db[i].ecl);
    //printf("\t[%d].pid = %s", i, db[i].pid);
    //printf("\t[%d].cid = %s", i, db[i].cid);
    //printf("\n------------------------------\n");

    if((_isvalid_byr(db[i].byr)) == false){
      printf("[%d] Invalid byr : %s\n", i, db[i].byr);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_iyr(db[i].iyr)) == false){
      printf("[%d] Invalid iyr : %s\n", i, db[i].iyr);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_eyr(db[i].eyr)) == false){
      printf("[%d] Invalid eyr : %s\n", i, db[i].eyr);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_hgt(db[i].hgt)) == false){
      printf("[%d] Invalid hgt : %s\n", i, db[i].hgt);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_hcl(db[i].hcl)) == false){
      printf("[%d] Invalid hcl : %s\n", i, db[i].hcl);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_ecl(db[i].ecl)) == false){
      printf("[%d] Invalid ecl : %s\n", i, db[i].ecl);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_pid(db[i].pid)) == false){
      printf("[%d] Invalid pid : %s\n", i, db[i].pid);
      invalid_passport_count++;
      continue;
    }
    if((_isvalid_cid(db[i].cid)) == false){
      printf("[%d] Invalid cid : %s\n", i, db[i].cid);
      invalid_passport_count++;
      continue;
    }
  }
  printf("Valid passport count %d\n",(count-invalid_passport_count));
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

  replace_space_with_newline(input_file_path);
  int line_count = get_line_count(input_file_path);
  int64_t passport_count = get_passport_count(input_file_path, line_count);
  passport_t *immigration_db = calloc(passport_count, sizeof(passport_t));

  parse_input_file(input_file_path, line_count, immigration_db);
  isvalid_immigration_db(immigration_db, passport_count);
  //print_immigration_db(immigration_db, passport_count);
  
  return(0);
}

