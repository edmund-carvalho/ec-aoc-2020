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


void _increment_block_type(int8_t seat_type, int64_t *seat_count_arr)
{
  if (seat_type == OCCUPIED_SEAT){
    seat_count_arr[eOCCUPIED_SEAT]+=1;
  } else if (seat_type == EMPTY_SEAT){
    seat_count_arr[eEMPTY_SEAT]+=1;
  } else if (seat_type == FLOOR){
    seat_count_arr[eFLOOR]+=1;
  }
}


bool _is_occupied_seats_matching(int8_t **map, int8_t **new_map, int64_t y_dim_max, int64_t x_dim_max)
{
  for (int i=0; i<y_dim_max; i++){
    for (int j=0; j<x_dim_max; j++){
      if (map[i][j] != new_map[i][j]) {
        return(false);
      }
    }
  }

  return(true);
}


void _copy_map(int8_t **src_map, int8_t **dest_map, int64_t y_dim_max, int64_t x_dim_max)
{
  for (int i=0; i<y_dim_max; i++){
    for (int j=0; j<x_dim_max; j++){
      dest_map[i][j] = src_map[i][j];
    }
  }
}


int64_t _get_num_seats_occupied(int8_t **map, int64_t y_dim_max, int64_t x_dim_max)
{
  int64_t ret=0;
  for (int i=0; i<y_dim_max; i++){
    for (int j=0; j<x_dim_max; j++){
      if(map[i][j] == OCCUPIED_SEAT) {
        ret++;
      }
    }
  }
  return(ret);
}


void process_floor_map(int8_t **map, int8_t **new_map, int64_t y_dim_max, int64_t x_dim_max)
{
  int64_t seat_count[3];
  int i,j;

  for (i=0; i<y_dim_max; i++){
    for (j=0; j<x_dim_max; j++){
      memset(seat_count, 0, 3*sizeof(int64_t));

      // 4 edge conditions
      if (i==0 && j==0 ){
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i+1][j], seat_count);
        _increment_block_type(map[i+1][j+1], seat_count);
      }
      else if (i==y_dim_max-1 && j==0){
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i-1][j], seat_count);
        _increment_block_type(map[i-1][j+1], seat_count);
      }
      else if (i==0 && j==x_dim_max-1){
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i+1][j], seat_count);
        _increment_block_type(map[i+1][j-1], seat_count);
      }
      else if (i==y_dim_max-1 && j==x_dim_max-1){
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i-1][j], seat_count);
        _increment_block_type(map[i-1][j-1], seat_count);
      }
      // 4 border conditions
      else if (i==0 && j>0){
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i+1][j+1], seat_count);
        _increment_block_type(map[i+1][j], seat_count);
        _increment_block_type(map[i+1][j-1], seat_count);
      }
      else if (i>0 && j==0){       
        _increment_block_type(map[i-1][j], seat_count);
        _increment_block_type(map[i-1][j+1], seat_count);
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i+1][j+1], seat_count);
        _increment_block_type(map[i+1][j], seat_count);
      }
      else if (i==y_dim_max-1 && j>0){
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i-1][j-1], seat_count);
        _increment_block_type(map[i-1][j], seat_count);
        _increment_block_type(map[i-1][j+1], seat_count);
      }
      else if (i>0 && j==x_dim_max-1){
        _increment_block_type(map[i+1][j], seat_count);
        _increment_block_type(map[i+1][j-1], seat_count);
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i-1][j-1], seat_count);
        _increment_block_type(map[i-1][j], seat_count);
      }  
      // The rest of the cases
      else {      
        _increment_block_type(map[i-1][j-1], seat_count);
        _increment_block_type(map[i-1][j], seat_count);
        _increment_block_type(map[i-1][j+1], seat_count);
        _increment_block_type(map[i][j-1], seat_count);
        _increment_block_type(map[i][j+1], seat_count);
        _increment_block_type(map[i+1][j-1] , seat_count);
        _increment_block_type(map[i+1][j], seat_count);
        _increment_block_type(map[i+1][j+1] , seat_count);
      }

      //printf("idx %d:%d seat_count[eOCCUPIED_SEAT]:%ld seat_count[eEMPTY_SEAT]:%ld \n",i,j,seat_count[eOCCUPIED_SEAT],seat_count[eEMPTY_SEAT] );
      if (map[i][j] == EMPTY_SEAT && (seat_count[eOCCUPIED_SEAT]==0)){
        new_map[i][j] = OCCUPIED_SEAT;
      } else if (map[i][j] == OCCUPIED_SEAT && (seat_count[eOCCUPIED_SEAT]>=4)){
        new_map[i][j] = EMPTY_SEAT;
      } else {
        new_map[i][j] = map[i][j];
      }
    }

  }

  //print_data(new_map, y_dim_max, x_dim_max);
  if (_is_occupied_seats_matching(map, new_map, y_dim_max, x_dim_max) == false ){
    _copy_map(new_map, map, y_dim_max, x_dim_max);
    process_floor_map(map, new_map, y_dim_max, x_dim_max);
  } else {
    printf("{Day 10 :: Challenge 1 solution} Number of occupied seats :%ld\n",_get_num_seats_occupied(new_map, y_dim_max, x_dim_max));
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

  int64_t num_rows = get_line_count(input_file_path);
  int64_t row_len = get_first_line_len(input_file_path);

  int8_t **floor_map_previous = calloc((num_rows), sizeof(int8_t*));
  int8_t **floor_map_updated = calloc((num_rows), sizeof(int8_t*));

  for (int i=0;i<num_rows;i++){
    floor_map_previous[i] = calloc(row_len, sizeof(int8_t));
    floor_map_updated[i] = calloc(row_len, sizeof(int8_t));
  }

  parse_input_file(input_file_path, floor_map_previous);
  //print_data(floor_map_previous, num_rows, row_len);
  process_floor_map(floor_map_previous, floor_map_updated, num_rows, row_len);


  for (int i=0;i<num_rows;i++){
    free(floor_map_previous[i]);
    free(floor_map_updated[i]);

  }
  free(floor_map_previous);
  free(floor_map_updated);

  return(0);
}
