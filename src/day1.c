#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE "3   4\n" \
"4   3\n" \
"2   5\n" \
"1   3\n" \
"3   9\n" \
"3   3"

void get_left_right_numbers(char *line, long *left, long *right)
{
  char *endptr = line;
  // Consume first number
  while (*endptr <= '0' && *endptr <= '9') endptr++;
  *left = strtol(line, &endptr, 10);
  endptr++;
  
  // Skip whitespaces
  while (*endptr == ' ') endptr++;
  line = endptr;

  // Consume second number
  while (*endptr <= '0' && *endptr <= '9') endptr ++;
  *right = strtol(line, &endptr, 10);
}

int lcmp(const void *left, const void *right) {
  return (*(int *)left - *(int *)right);
}

int main(void)
{
  char *input;
  FILE *fp = fopen("src/input1.txt", "rb");
  fseek(fp, 0L,  SEEK_END);
  long size = ftell(fp);
  rewind(fp);
  input = calloc(1, size + 1);
  if (input) {
    fread(input, 1, size, fp);
  } else {
    exit(1);
  }
  size_t line_count = 1;
  size_t i = 0;
  for (; i < strlen(input); i++) {
    if (input[i] == '\n' || input[i] == EOF) line_count++;
  }
  int left[line_count];
  int right[line_count];
  const char* current_line = input;
  i = 0;
  for (i = 0; i < line_count; i++)
  {
    const char* next_line = strchr(current_line, '\n');
    size_t current_line_length = next_line ? (size_t)(next_line - current_line) : strlen(current_line); 
    char *tmp = (char *) malloc(current_line_length+1);
    if (tmp) {
      memcpy(tmp, current_line, current_line_length);
      tmp[current_line_length] = '\0';
      long l = 0;
      long r = 0;
      get_left_right_numbers(tmp, &l, &r);
      free(tmp);
      left[i] = l;
      right[i] = r;
    }
    current_line = next_line ? (next_line + 1) : NULL;
  }

  qsort(left, sizeof left / sizeof left[0], sizeof left[0], lcmp); 
  qsort(right, sizeof right / sizeof right[0], sizeof right[0], lcmp);

  int distance_sum = 0;
  for (i = 0; i < sizeof left / sizeof left[0]; i++) {
    distance_sum += abs(left[i] - right[i]);
  }

  printf("Part 1: %d\n", distance_sum);

  long similarity_score = 0;
  for (i = 0; i < line_count; i++) {
    int l = left[i];
    int delta_similarity = 0;
    for (size_t j = 0; j < line_count; j++) {
      if (l == right[j]) delta_similarity++;
    }
    similarity_score += l * delta_similarity;
  }

  printf("Part 2: %ld\n", similarity_score);

  return 0;
}
