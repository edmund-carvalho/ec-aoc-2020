#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LINE_LEN 1024
#define FLOOR '.'
#define EMPTY_SEAT 'L'
#define OCCUPIED_SEAT '#'

typedef enum block_type{
  eFLOOR,
  eEMPTY_SEAT,
  eOCCUPIED_SEAT,
}block_type_t;

typedef enum direction{
  eNORTH,
  eNORTH_EAST,
  eEAST,
  eSOUTH_EAST,
  eSOUTH,
  eSOUTH_WEST,
  eWEST,
  eNORTH_WEST
}direction_t;

typedef enum scan_mode{
  eSCAN_ADJACENT,
  eSCAN_FIRST_LOS
}scan_mode_t;

int64_t y_dim_max_g;
int64_t x_dim_max_g;
int tolerence_g;
scan_mode_t scan_mode_g;

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


int get_first_line_len(char *file_path)
{
  FILE *fp = fopen(file_path, "r");
  int len = 0;
  char c;

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  while (c != EOF){ 
    c = fgetc(fp);
    if (c != '\n' && c != EOF){
      len++;
    } else {
      break;
    }
  }

err:
  fclose(fp);
  return (len);

  return(0);
}


int parse_input_file(char *file_path, int8_t **map)
{
  int i=0, j=0;
  int8_t c;
  FILE *fp = fopen(file_path, "r");

  if (fp == NULL) {
    perror("Failed to read line count");
    goto err;
  }

  while ((c = fgetc(fp))){
    if (c == '\n'){
      i++;
      j=0;
    } else if (c == EOF){
      break;
    } else {
      map[i][j] = c;
      j++;
    }
  }

err:
  fclose(fp);
  return(0);
}


void print_data(int8_t **map, int64_t y_dim_max, int64_t x_dim_max)
{
  printf("\n--------------------------------------------------\n");
  for (int i=0; i<y_dim_max; i++){
    for (int j=0; j<x_dim_max; j++){
      printf("%c", map[i][j]);
    }
    printf("\n");
  }
  printf("\n--------------------------------------------------\n");
}


int64_t seat_count_g[3]={0};
void _increment_block_type(int8_t seat_type)
{
  if (seat_type == OCCUPIED_SEAT){
    seat_count_g[eOCCUPIED_SEAT]+=1;
  } else if (seat_type == EMPTY_SEAT){
    seat_count_g[eEMPTY_SEAT]+=1;
  } else if (seat_type == FLOOR){
    seat_count_g[eFLOOR]+=1;
  }
}


bool _is_occupied_seats_matching(int8_t **map, int8_t **new_map)
{
  for (int i=0; i<y_dim_max_g; i++){
    for (int j=0; j<x_dim_max_g; j++){
      if (map[i][j] != new_map[i][j]) {
        return(false);
      }
    }
  }

  return(true);
}


void _copy_map(int8_t **src_map, int8_t **dest_map)
{
  for (int i=0; i<y_dim_max_g; i++){
    for (int j=0; j<x_dim_max_g; j++){
      dest_map[i][j] = src_map[i][j];
    }
  }
}


int64_t _get_num_seats_occupied(int8_t **map)
{
  int64_t ret=0;
  for (int i=0; i<y_dim_max_g; i++){
    for (int j=0; j<x_dim_max_g; j++){
      if(map[i][j] == OCCUPIED_SEAT) {
        ret++;
      }
    }
  }
  return(ret);
}


void _scan_direction(int8_t **map, int64_t y_cordinate, int64_t x_cordinate, direction_t pointing)
{
  int64_t i = y_cordinate;
  int64_t j = x_cordinate;


  if(pointing == eNORTH){
    while(i>0){
      i--;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eNORTH Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eNORTH_EAST){
    while ((i>0) && (j<x_dim_max_g-1)){
      i--;
      j++;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eNORTH_EAST Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eNORTH_WEST){
    while ((i>0) && (j>0)){
      i--;
      j--;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eNORTH_WEST Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eSOUTH){
    while ((i<y_dim_max_g-1)){
      i++;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eSOUTH Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eSOUTH_EAST){
    while ((i<y_dim_max_g-1) && (j<x_dim_max_g-1)){
      i++;
      j++;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eSOUTH_EAST Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eSOUTH_WEST){
    while ((i<y_dim_max_g-1) && (j>0)){
      i++;
      j--;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eSOUTH_WEST Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eEAST){
    while (j>=0){
      j++;
      //printf(">> i%ld j:%ld\n",i,j);
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eEAST Found seat at map[%ld][%ld] : 0x%x\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }
  else if(pointing == eWEST){ 
    while (j<x_dim_max_g-1){
      j--;
      if (map[i][j] == OCCUPIED_SEAT || map[i][j] == EMPTY_SEAT){
        //printf("Scan eWEST Found seat at map[%ld][%ld] : %c\n",i,j,map[i][j]);
        _increment_block_type(map[i][j]);
        break;
      }
      if (scan_mode_g ==  eSCAN_ADJACENT){
        break;
      }
    }
  }

}


void process_floor_map(int8_t **map, int8_t **new_map)
{
  int i,j;

  for (i=0; i<y_dim_max_g; i++){
    for (j=0; j<x_dim_max_g; j++){
      memset(seat_count_g, 0, 3*sizeof(int64_t));

      // 4 edge conditions
      if (i==0 && j==0 ){
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i+1][j]);
        _increment_block_type(map[i+1][j+1]);
      }
      else if (i==y_dim_max_g-1 && j==0){
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i-1][j]);
        _increment_block_type(map[i-1][j+1]);
      }
      else if (i==0 && j==x_dim_max_g-1){
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i+1][j]);
        _increment_block_type(map[i+1][j-1]);
      }
      else if (i==y_dim_max_g-1 && j==x_dim_max_g-1){
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i-1][j]);
        _increment_block_type(map[i-1][j-1]);
      }
      // 4 border conditions
      else if (i==0 && j>0){
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i+1][j+1]);
        _increment_block_type(map[i+1][j]);
        _increment_block_type(map[i+1][j-1]);
      }
      else if (i>0 && j==0){       
        _increment_block_type(map[i-1][j]);
        _increment_block_type(map[i-1][j+1]);
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i+1][j+1]);
        _increment_block_type(map[i+1][j]);
      }
      else if (i==y_dim_max_g-1 && j>0){
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i-1][j-1]);
        _increment_block_type(map[i-1][j]);
        _increment_block_type(map[i-1][j+1]);
      }
      else if (i>0 && j==x_dim_max_g-1){
        _increment_block_type(map[i+1][j]);
        _increment_block_type(map[i+1][j-1]);
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i-1][j-1]);
        _increment_block_type(map[i-1][j]);
      }  
      // The rest of the cases
      else {      
        _increment_block_type(map[i-1][j-1]);
        _increment_block_type(map[i-1][j]);
        _increment_block_type(map[i-1][j+1]);
        _increment_block_type(map[i][j-1]);
        _increment_block_type(map[i][j+1]);
        _increment_block_type(map[i+1][j-1]);
        _increment_block_type(map[i+1][j]);
        _increment_block_type(map[i+1][j+1]);
      }

      //printf("idx %d:%d seat_count_g[eOCCUPIED_SEAT]:%ld seat_count_g[eEMPTY_SEAT]:%ld \n",i,j,seat_count_g[eOCCUPIED_SEAT],seat_count_g[eEMPTY_SEAT]);
      if (map[i][j] == EMPTY_SEAT && (seat_count_g[eOCCUPIED_SEAT]==0)){
        new_map[i][j] = OCCUPIED_SEAT;
      } else if (map[i][j] == OCCUPIED_SEAT && (seat_count_g[eOCCUPIED_SEAT]>=4)){
        new_map[i][j] = EMPTY_SEAT;
      } else {
        new_map[i][j] = map[i][j];
      }
    }

  }

  //print_data(new_map, y_dim_max, x_dim_max);
  if (_is_occupied_seats_matching(map, new_map) == false ){
    _copy_map(new_map, map);
    process_floor_map(map, new_map);
  } else {
    printf("{Day 10 :: Challenge 1 solution} Number of occupied seats :%ld\n",_get_num_seats_occupied(new_map));
  }

  return;
}



void process_floor_map_challenge2(int8_t **map, int8_t **new_map)
{
  int64_t i,j;

  for (i=0; i<y_dim_max_g; i++){
    for (j=0; j<x_dim_max_g; j++){
      memset(seat_count_g, 0, 3*sizeof(int64_t));

      // 4 edge conditions
      //printf("Scanning from location map[%ld][%ld] : 0x%x\n",i,j, map[i][j]);
      if (i==0 && j==0 ){
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eSOUTH_EAST);
        _scan_direction(map, i, j, eSOUTH);
      }
      else if (i==y_dim_max_g-1 && j==0){
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eNORTH);
        _scan_direction(map, i, j, eNORTH_EAST);
      }
      else if (i==0 && j==x_dim_max_g-1){
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eSOUTH);
        _scan_direction(map, i, j, eSOUTH_WEST);
      }
      else if (i==y_dim_max_g-1 && j==x_dim_max_g-1){
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eNORTH);
        _scan_direction(map, i, j, eNORTH_WEST);
      }

      // 4 border conditions
      else if (i==0 && j>0){
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eSOUTH_EAST);
        _scan_direction(map, i, j, eSOUTH);
        _scan_direction(map, i, j, eSOUTH_WEST);
      }
      else if (i>0 && j==0){       
        _scan_direction(map, i, j, eNORTH);
        _scan_direction(map, i, j, eNORTH_EAST);
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eSOUTH_EAST);
        _scan_direction(map, i, j, eSOUTH);
      }
      else if (i==y_dim_max_g-1 && j>0){
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eNORTH_WEST);
        _scan_direction(map, i, j, eNORTH);
        _scan_direction(map, i, j, eNORTH_EAST);
      }
      else if (i>0 && j==x_dim_max_g-1){
        _scan_direction(map, i, j, eSOUTH);
        _scan_direction(map, i, j, eSOUTH_WEST);
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eNORTH_WEST);
        _scan_direction(map, i, j, eNORTH);
      }

      // The rest of the cases
      else {      
        _scan_direction(map, i, j, eNORTH);
        _scan_direction(map, i, j, eNORTH_EAST);
        _scan_direction(map, i, j, eEAST);
        _scan_direction(map, i, j, eSOUTH_EAST);
        _scan_direction(map, i, j, eSOUTH);
        _scan_direction(map, i, j, eSOUTH_WEST);
        _scan_direction(map, i, j, eWEST);
        _scan_direction(map, i, j, eNORTH_WEST);
      }

      //printf("idx %ld:%ld seat_count_g[eOCCUPIED_SEAT]:%ld seat_count_g[eEMPTY_SEAT]:%ld \n",i,j,seat_count_g[eOCCUPIED_SEAT],seat_count_g[eEMPTY_SEAT] );
      if (map[i][j] == EMPTY_SEAT && (seat_count_g[eOCCUPIED_SEAT]==0)){
        new_map[i][j] = OCCUPIED_SEAT;
      } else if (map[i][j] == OCCUPIED_SEAT && (seat_count_g[eOCCUPIED_SEAT]>=tolerence_g)){
        new_map[i][j] = EMPTY_SEAT;
      } else {
        new_map[i][j] = map[i][j];
      }
    }

  }

  //print_data(new_map, y_dim_max, x_dim_max);
  if (_is_occupied_seats_matching(map, new_map) == false ){
    _copy_map(new_map, map);
    process_floor_map_challenge2(map, new_map);
  } else {
    printf("{Day 10 :: Challenge 1 solution} Number of occupied seats :%ld\n",_get_num_seats_occupied(new_map));
  }

  return;
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

  y_dim_max_g = get_line_count(input_file_path);
  x_dim_max_g = get_first_line_len(input_file_path);

  int8_t **floor_map_previous = calloc((y_dim_max_g), sizeof(int8_t*));
  int8_t **floor_map_updated = calloc((y_dim_max_g), sizeof(int8_t*));

  for (int i=0;i<y_dim_max_g;i++){
    floor_map_previous[i] = calloc(x_dim_max_g, sizeof(int8_t));
    floor_map_updated[i] = calloc(x_dim_max_g, sizeof(int8_t));
  }

  parse_input_file(input_file_path, floor_map_previous);
  //print_data(floor_map_previous, num_rows, row_len);

  scan_mode_g = eSCAN_ADJACENT;
  tolerence_g = 4;

  process_floor_map(floor_map_previous, floor_map_updated);

  // This is broken !!
  // TODO Manually test boundary conditions of _scan_direction() !!
  // TODO Recheck calls to _scan_direction() for boundary conditions !!

  //scan_mode_g = eSCAN_FIRST_LOS;
  //tolerence_g = 5;
  //process_floor_map_challenge2(floor_map_previous, floor_map_updated);


  for (int i=0;i<y_dim_max_g;i++){
    free(floor_map_previous[i]);
    free(floor_map_updated[i]);
  }
  free(floor_map_previous);
  free(floor_map_updated);

  return(0);
}
