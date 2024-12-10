#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(char *file_path) {
  char *buffer;
  FILE *fp = fopen(file_path, "rb");
  long len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  buffer = (char *)malloc(len);
  fread(buffer, len, 1, fp);

  return buffer;
}

void count_rows_and_columns(char *input, int *rows, int *columns) {
  int r = 0;
  int c = 0;

  bool count_columns = true;
  for (size_t i = 0; i < strlen(input); i++) {
    if (*(input + i) == '\n') {
      count_columns = false;
      r++;
    }
    else if (count_columns) c++;
  }

  *rows = r;
  *columns = c;
}

void solve_part_1(char *input) {
  int rows, columns;
  count_rows_and_columns(input, &rows, &columns);
  char word_search[rows][columns];
  char word_search_solved[rows][columns];
  
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      int idx = (y * rows) + x /* skip newline characters */ + (1 * y); 
      word_search[y][x] = input[idx];
      word_search_solved[y][x] = '.';
    }
  }
  int words_found = 0;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      if (word_search[y][x] == 'X') {
        for (int dy = -1; dy < 2; dy++) {
          if (dy + y < 0 || dy + y == rows) continue;
          for (int dx = -1; dx < 2; dx++) {
            if (dx + x < 0 || dx + x == columns) continue;
            if (dy == 0 && dx == 0) continue;

            if (word_search[dy + y][dx + x] == 'M') {
              if (dy*2+y < 0 || dy*2+y == rows) continue;
              if (dx*2+x < 0 || dx*2+x == columns) continue;
              if (word_search[dy*2 + y][dx*2 + x] == 'A') {
                if (dy*3+y < 0 || dy*3+y == rows) continue;
                if (dx*3+x < 0 || dx*3+x == columns) continue;
                if (word_search[dy*3 + y][dx*3 + x] == 'S') {
                  words_found++;
                  word_search_solved[dy*3+y][dx*3+x] = 'S';
                  word_search_solved[dy*2+y][dx*2+x] = 'A';
                  word_search_solved[dy+y][dx+x] = 'M';
                  word_search_solved[y][x] = 'X';
                }
              }
            }
          }
        }
      }
    }
  }
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      printf("%c", word_search_solved[y][x]);
    }
    printf("\n");
  }
  printf("words found: %d\n", words_found);
}

void solve_part_2(char *input) {
  int rows, columns;
  count_rows_and_columns(input, &rows, &columns);
  char word_search[rows][columns];
  bool grid_usage[rows][columns];
  char word_search_solved[rows][columns];
  
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      int idx = (y * rows) + x /* skip newline characters */ + (1 * y); 
      word_search[y][x] = input[idx];
      word_search_solved[y][x] = '.';
    }
  }
  int words_found = 0;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      char current = word_search[y][x];
      if ((current == 'M' || current == 'S') && !grid_usage[y][x]) {
        char next = 'A';
        char last = current == 'M' ? 'S' : 'M';
        if (y - 1 > 0 && x - 1 > 0 && word_search[y-1][x-1] == next && !grid_usage[y-1][x-1]) {
          if (y - 2 > 0 && x - 2 > 0 && word_search[y-2][x-2] == last) {
            char dC = word_search[y][x-2];
            if (dC == 'M' || dC == 'S') {
              char dL = dC == 'M' ? 'S' : 'M';
              if (word_search[y-2][x] == dL) {
                words_found++;
                word_search_solved[y][x] = current;
                word_search_solved[y-1][x-1] = 'A';
                word_search_solved[y-2][x-2] = last;
                word_search_solved[y][x-2] = dC;
                word_search_solved[y-2][x] = dL;
                grid_usage[y][x] = true;
                grid_usage[y-1][x-1] = true;
                grid_usage[y-2][x-2] = true;
              }
            }
          }
        }
        if (y + 1 < rows && x + 1 < columns && word_search[y+1][x+1] == next && !grid_usage[y+1][x+1]) {
          if (y + 2 < rows && x + 2 < columns && word_search[y+2][x+2] == last) {
            char dC = word_search[y][x+2];
            if (dC == 'M' || dC == 'S') {
              char dL = dC == 'M' ? 'S' : 'M';
              if (word_search[y+2][x] == dL) {
                words_found++;
                word_search_solved[y][x] = current;
                word_search_solved[y+1][x+1] = 'A';
                word_search_solved[y+2][x+2] = last;
                word_search_solved[y][x+2] = dC;
                word_search_solved[y+2][x] = dL;
                grid_usage[y][x] = true;
                grid_usage[y+1][x+1] = true;
                grid_usage[y+2][x+2] = true;
              }
            }
          }
        }
      }
    }
  }
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      printf("%c", word_search_solved[y][x]);
    }
    printf("\n");
  }
  printf("X-MAS found: %d\n", words_found);
}

int main(void) {
  char *input = read_file("src/input4.txt");
  // char *example = read_file("src/example4.txt");
  printf("=== PART 1 ===\n");
  solve_part_1(input);
  printf("\n=== PART 2 ===\n");
  solve_part_2(input);
  return 0;
}
