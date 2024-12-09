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

bool parse_line_safety(int levels[], size_t count) {
  int d1 = levels[0] - levels[1];
  int s1 = sign(d1);

  for (size_t i = 1; i < count; i++) {
    int r = levels[i-1] - levels[i];
    int ds = sign(r);
    if (ds != s1 || abs(r) < 1 || abs(r) > 3) return false;
  }
  return true;
}

bool is_line_safe(char *input, bool tolerance) {
  size_t numbers = count_numbers(input);
  int levels[numbers];
  for (size_t i = 0; i < numbers; i++) { 
    int result;
    input = get_next_number(input, &result);
    levels[i] = result;
  }

  bool is_line_safe = parse_line_safety(levels, numbers);
  if (!tolerance) return is_line_safe;
  if (is_line_safe) return is_line_safe;

  for (size_t i = 0; i < numbers; i++) {
    int temp[numbers-1];
    size_t j = 0;
    for (size_t k = 0; k < numbers; k++) {
      if (k != i) temp[j++] = levels[k];
    }
    if (parse_line_safety(temp, numbers-1)) return true;
  }
  return false;
}

void solve_part_1(char *input) {
  int safe_reports = 0;
  int line = 1;
  while (input) {
    safe_reports += is_line_safe(input, false) ? 1 : 0;
    input = trim_newline(input);
    line++;
  }
  printf("Part 1: %d\n", safe_reports);
}

void solve_part_2(char *input) {
  int safe_reports = 0;
  int line = 1;
  while (input) {
    safe_reports += is_line_safe(input, true) ? 1 : 0;
    input = trim_newline(input);
    line++;
  }
  printf("Part 2: %d\n", safe_reports);
}

int main(void) {
  char *input, *input2;
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
  input2 = malloc(length);
  if (!input || !input2) {
    perror("could not allocate");
    exit(1);
  }

  fread(input, 1, length, file);
  fseek(file, 0, SEEK_SET);
  fread(input2, 1, length, file);
  fclose(file);

  printf("EXAMPLE\n");
  solve_part_1(EXAMPLE);
  printf("REAL\n");
  solve_part_1(input);
  printf("\n=========\n");
  printf("EXAMPLE\n");
  solve_part_2(EXAMPLE);
  printf("REAL\n");
  solve_part_2(input2);
  return 0;
}

