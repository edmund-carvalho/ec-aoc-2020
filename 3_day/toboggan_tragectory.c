 /*
  *  https://adventofcode.com/2020/day/1
  *
  *  --- Day 3: Toboggan Trajectory ---
  *  
  *  With the toboggan login problems resolved, you set off toward the airport. 
  *  While travel by toboggan might be easy, it's certainly not safe: there's very minimal steering and the area is covered in trees.
  *  You'll need to see which angles will take you near the fewest trees.
  *  Due to the local geology, trees in this area only grow on exact integer coordinates in a grid.
  *  You make a map (your puzzle input) of the open squares (.) and trees (#) you can see. For example:
  *  
  *  ..##.......
  *  #...#...#..
  *  .#....#..#.
  *  ..#.#...#.#
  *  .#...##..#.
  *  ..#.##.....
  *  .#.#.#....#
  *  .#........#
  *  #.##...#...
  *  #...##....#
  *  .#..#...#.#
  *  
  *  These aren't the only trees, though; due to something you read about once involving arboreal genetics and biome stability,
  *  the same pattern repeats to the right many times:
  *  
  *  ..##.........##.........##.........##.........##.........##.......  --->
  *  #...#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
  *  .#....#..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
  *  ..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
  *  .#...##..#..#...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
  *  ..#.##.......#.##.......#.##.......#.##.......#.##.......#.##.....  --->
  *  .#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
  *  .#........#.#........#.#........#.#........#.#........#.#........#
  *  #.##...#...#.##...#...#.##...#...#.##...#...#.##...#...#.##...#...
  *  #...##....##...##....##...##....##...##....##...##....##...##....#
  *  .#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#  --->
  *  
  *  You start on the open square (.) in the top-left corner and need to reach the bottom (below the bottom-most row on your map).
  *  
  *  The toboggan can only follow a few specific slopes (you opted for a cheaper model that prefers rational numbers);
  *  start by counting all the trees you would encounter for the slope right 3, down 1:
  *  
  *  From your starting position at the top-left, check the position that is right 3 and down 1.
  *  Then, check the position that is right 3 and down 1 from there, and so on until you go past the bottom of the map.
  *  The locations you'd check in the above example are marked here with O where there was an open square and X where there was a tree:
  *  
  *  ..##.........##.........##.........##.........##.........##.......  --->
  *  #..O#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
  *  .#....X..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
  *  ..#.#...#O#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
  *  .#...##..#..X...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
  *  ..#.##.......#.X#.......#.##.......#.##.......#.##.......#.##.....  --->
  *  .#.#.#....#.#.#.#.O..#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
  *  .#........#.#........X.#........#.#........#.#........#.#........#
  *  #.##...#...#.##...#...#.X#...#...#.##...#...#.##...#...#.##...#...
  *  #...##....##...##....##...#X....##...##....##...##....##...##....#
  *  .#..#...#.#.#..#...#.#.#..#...X.#.#..#...#.#.#..#...#.#.#..#...#.#  --->
  *  
  *  In this example, traversing the map using this slope would cause you to encounter 7 trees.
  *  Starting at the top-left corner of your map and following a slope of right 3 and down 1, how many trees would you encounter?
  *  
  *  
  *  --- Part Two ---
  *  
  *  Time to check the rest of the slopes - you need to minimize the probability of a sudden arboreal stop, after all.
  *  Determine the number of trees you would encounter if, for each of the following slopes, you start at the top-left corner and traverse the map all the way to the bottom:
  *  
  *      Right 1, down 1.
  *      Right 3, down 1. (This is the slope you already checked.)
  *      Right 5, down 1.
  *      Right 7, down 1.
  *      Right 1, down 2.
  *  
  *  In the above example, these slopes would find 2, 7, 3, 4, and 2 tree(s) respectively; multiplied together, these produce the answer 336.
  *  
  *  What do you get if you multiply together the number of trees encountered on each of the listed slopes?
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LINE_LEN 1024
#define TREE '#'


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


void print_data(int8_t **map, int y_dim_max, int x_dim_max)
{
  for (int i=0; i<y_dim_max; i++){
    for (int j=0; j<x_dim_max; j++){
      printf("%c", map[i][j]);
    }
    printf("\n");
  }
}


int64_t compute_tree_collision(int8_t **map, int y_dim_max, int x_dim_max, int slope_x, int slope_y)
{
  int row=0;
  int col=0;
  int tree_count=0;

  for (row=slope_y; row<y_dim_max; row+=slope_y){
    col+=slope_x;
    if (map[row][col%x_dim_max] == TREE){
      tree_count++;
    }
  }

  return(tree_count);
}


int main(int argc, char *argv[])
{
  char input_file_path[1024]={'\0'};
  uint64_t longitude_mod_max = 0;
  uint64_t latitude_max = 0 ;

  if (argc == 2) {
    strcpy(input_file_path, argv[1]);
    printf("Using input file : %s\n", input_file_path);
  } else {
    printf("You forgot to provide an input file !! \nUsage:\t\t%s input_file_name.txt\n", argv[0]);
    exit(-1);
  }

  latitude_max = get_line_count(input_file_path);
  longitude_mod_max = get_first_line_len(input_file_path);

  int8_t **toboggan_map = calloc((latitude_max), sizeof(int8_t*));
  for (int i=0;i<latitude_max;i++){
    toboggan_map[i] = calloc(longitude_mod_max, sizeof(int8_t));
  }

  parse_input_file(input_file_path, toboggan_map);
  //print_data(toboggan_map, latitude_max, longitude_mod_max);

  printf("{Day 3 :: Challenge 1 solution} Trees hit : <%lu>. \n", compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 3, 1));

  uint64_t tree_hit_count=1;
  uint64_t total_tree_hit_count=1;

  if ((tree_hit_count = compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 1, 1))>0){
    total_tree_hit_count *= tree_hit_count;
  }
  if ((tree_hit_count = compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 3, 1))>0){
    total_tree_hit_count *= tree_hit_count;
  }
  if ((tree_hit_count = compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 5, 1))>0){
    total_tree_hit_count *= tree_hit_count;
  }
  if ((tree_hit_count = compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 7, 1))>0){
    total_tree_hit_count *= tree_hit_count;
  }
  if ((tree_hit_count = compute_tree_collision(toboggan_map, latitude_max, longitude_mod_max, 1, 2))>0){
    total_tree_hit_count *= tree_hit_count;
  }
  
  printf("{Day 3 :: Challenge 2 solution} Trees hit : <%lu>. \n", total_tree_hit_count);

  return (0);
}
