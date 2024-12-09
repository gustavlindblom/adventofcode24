#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))" "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"

typedef struct {
  int start;
  int end;
} ValidInterval;

void solve_part_2(char* input) {
  char dont_marker[] = "don't()";
  char do_marker[] = "do()";
  ValidInterval intervals[100] = {{0}};
  char *first_stop = strstr(input, dont_marker);
  ValidInterval start = {
    .start = 0,
    .end =  first_stop - input
  };
  first_stop++;
  intervals[0] = start;
  int count = 1;
  const char *tmp = first_stop;
  const char *tmp1 = first_stop;
  while ((tmp = strstr(tmp, do_marker)))
  {
    long offset = tmp - input;
    tmp1 = tmp1 ? strstr(tmp1, dont_marker) : input + strlen(input);
    if (tmp1) {
      tmp1 = strstr(tmp, dont_marker);
    } else {
      tmp1 = input + strlen(input) - 1;
    }
    ValidInterval interval = {
      .start = offset,
      .end = tmp1 - input
    };
    intervals[count] = interval;
    count++;
    tmp++;
    tmp1++;
  }

  regex_t regex;
  const char* pattern = "mul\\(([0-9]+),([0-9]+)\\)";
  int ret = regcomp(&regex, pattern, REG_EXTENDED);
  if (ret != 0) {
    perror("failed to compile regex");
    exit(1);
  }
  regmatch_t matches[3];
  int offset = 0;
  long mul_sum = 0;
  while ((ret = regexec(&regex, input + offset, 3, matches, 0)) == 0) {
    int actual_offset = offset + matches[1].rm_so;
    size_t ix = 0;
    bool valid = false;
    while (intervals[ix].end != 0) {
      if (intervals[ix].start <= actual_offset && intervals[ix].end >= actual_offset) {
        valid = true;
        break;
      }
      ix++;
    }
    if (!valid) {
      offset += matches[0].rm_eo;
      continue;
    }
    char *left  = strndup(input + offset + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
    char *right = strndup(input + offset + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
    long x = strtol(left, NULL, 10);
    long y = strtol(right, NULL, 10);
    free(left);
    free(right);
    mul_sum += x * y;
    offset += matches[0].rm_eo;
  }

  regfree(&regex);
  printf("%ld\n", mul_sum);
}

void solve_part_1(char* input) {
  regex_t regex;
  const char* pattern = "mul\\(([0-9]+),([0-9]+)\\)";
  int ret = regcomp(&regex, pattern, REG_EXTENDED);
  if (ret != 0) {
    perror("failed to compile regex");
    exit(1);
  }
  regmatch_t matches[3];
  int offset = 0;
  long mul_sum = 0;
  while ((ret = regexec(&regex, input + offset, 3, matches, 0)) == 0) {
    char *left  = strndup(input + offset + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
    char *right = strndup(input + offset + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
    long x = strtol(left, NULL, 10);
    long y = strtol(right, NULL, 10);
    free(left);
    free(right);
    mul_sum += x * y;
    offset += matches[0].rm_eo;
  }

  regfree(&regex);
  printf("%ld\n", mul_sum);

}

int main(void) {
  char *input1, *input2;
  long length;
  FILE *file = fopen("src/input3.txt", "rb");
  if (!file) {
    perror("could not open input file");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);
  input1 = malloc(length);
  input2 = malloc(length);
  if (!input1 || !input2) {
    perror("could not allocate");
    exit(1);
  }

  fread(input1, 1, length, file);
  fseek(file, 0, SEEK_SET);
  fread(input2, 1, length, file);
  fclose(file);

  printf("=== PART 1 ===\n");
  printf("Example: ");
  solve_part_1(EXAMPLE);
  printf("Real: ");
  solve_part_1(input1);
  printf("=== PART 2 ===\n");
  printf("Example: ");
  solve_part_2(EXAMPLE);
  printf("Real: ");
  solve_part_2(input2);
  return 0;
}
