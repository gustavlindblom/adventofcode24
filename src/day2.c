#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE "7 6 4 2 1\n" \
"1 2 7 8 9\n"                 \
"9 7 6 2 1\n"                 \
"1 3 2 4 5\n"                 \
"8 6 4 4 1\n"                 \
"1 3 6 7 9"                 

int sign(int input) {
  return ((input > 0) - (input < 0));
}

char* trim_whitespace(char *input) {
  while (*input == ' ') input++;
  return input;
}

char* trim_newline(char *input) {
  while (*input != '\n' && strlen(input) > 0) input++;
  return strlen(input) == 0 || strlen(input) == 1 ? NULL : input + 1; 
}

size_t count_numbers(char *input) {
  size_t numbers = 0;
  while (*input != '\n' && *input != '\0') {
    if (*input == ' ') numbers++;
    input++;
  }
  return numbers + 1;
}

char* get_next_number(char *input, int *result) {
  input = trim_whitespace(input);
  char *start = input;
  while (*input >= '0' && *input <= '9') input++;
  *result = strtol(start, &input, 10);
  return input;
}

bool is_line_safe(char *input) {
  size_t numbers = count_numbers(input);
  int levels[numbers];
  for (size_t i = 0; i < numbers; i++) { 
    int result;
    input = get_next_number(input, &result);
    levels[i] = result;
  }
  int d1 = levels[0] - levels[1];
  int s1 = sign(d1);

  for (size_t i = 1; i < numbers; i++) {
    int r = levels[i-1] - levels[i];
    int ds = sign(r);
    if (ds != s1) return false;
    if (abs(r) < 1 || abs(r) > 3) return false;
  }
  return true;
}

void solve_example_2() {
  printf("Example 2: %d\n", 0);
}

void solve_part_1(char *input) {
  int safe_reports = 0;
  int line = 1;
  while (input) {
    if (is_line_safe(input)) {
      printf("%d: Safe\n", line);
    } else {
      printf("%d: Unsafe\n", line);
    }
    safe_reports += is_line_safe(input) ? 1 : 0;
    input = trim_newline(input);
    line++;
  }
  printf("Part 1: %d\n", safe_reports);
  
}

int main(void) {
  char *input;
  long length;
  FILE *file = fopen("src/input2.txt", "rb");
  if (!file) {
    perror("could not open input file");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);
  input = malloc(length);
  if (!input) {
    perror("could not allocate");
    exit(1);
  }

  fread(input, 1, length, file);
  fclose(file);

  printf("EXAMPLE\n");
  solve_part_1(EXAMPLE);
  printf("REAL\n");
  solve_part_1(input);
  return 0;
}

