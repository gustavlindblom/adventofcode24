#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EXAMPLE "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"

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
  return 0;
}
