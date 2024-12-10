#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(char *file_path) {
  char *buffer;
  FILE *fp = fopen(file_path, "rb");
  if (!fp) {
    fprintf(stderr, "could not open file %s: %s\n", file_path, strerror(errno));
    exit(1);
  }
  long len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  buffer = (char *)malloc(len);
  fread(buffer, len, 1, fp);

  return buffer;
}

#define INITIAL_CAPACITY 1024 

typedef struct {
  int first;
  int second;
} PageOrderRule;

typedef struct {
  PageOrderRule *items;
  size_t capacity;
  size_t count;
} PageOrderRules;

typedef struct {
  int items[156];
  size_t count;
} PrintingOrder;

typedef struct {
  PrintingOrder *items;
  size_t capacity;
  size_t count;
} PrintingOrders;

void page_order_rules_append(PageOrderRules *rules, PageOrderRule item) {
  if (rules->capacity == 0) {
    rules->capacity = INITIAL_CAPACITY;
    rules->items = malloc(sizeof(item) * rules->capacity);
  }
  if (rules->count == rules->capacity) {
    rules->capacity = rules->capacity * 2;
    rules->items = realloc(rules->items, rules->capacity);
  }
  rules->items[rules->count++] = item;
}

// TODO: Figure out why malloc/realloc works iffy
void printing_orders_append(PrintingOrders *orders, PrintingOrder item) {
  if (orders->capacity == 0) {
    orders->capacity = 512;
    orders->items = malloc(sizeof(item) * 512);
  }
  if (orders->count == orders->capacity) {
    orders->capacity = orders->capacity * 2;
    orders->items = realloc(orders->items, orders->capacity);
  }
  orders->items[orders->count++] = item;
}

void parse_input(char *input, PageOrderRules *rules, PrintingOrders *orders) {
  (void)rules;
  (void)orders;
  size_t rule_stop_index = strstr(input, "\n\n") - input;
  char *tmp = input;
  size_t n = 0;
  size_t length = strlen(input);
  while (n < rule_stop_index) {
    while (*tmp != '\n') {
      tmp++;
      n++;
    }
    PageOrderRule rule = {0};
    char *separator = strchr(input, '|');
    long first = strtol(input, &separator, 10);
    long second = strtol(separator + 1, &tmp, 10);
    rule.first = first;
    rule.second = second;
    page_order_rules_append(rules, rule);
    tmp++;
    n++;
    input = tmp;
  }

  input += 1;
  n += 1;
  tmp = input;
  while (n < length) {
    size_t pages = 0;
    while (*tmp != '\n') {
      if (*tmp == ',') pages++;
      tmp++;
      n++;
    }
    pages++;
    PrintingOrder order = {0};
    order.count = pages;
    for (size_t i = 0; i < pages; i++) {
      char *start = input;
      while (*input >= '0' && *input <= '9') input++;
      int page = (int)strtol(start, &input, 10);
      order.items[i] = page;
      input++;
    }
    printing_orders_append(orders, order);

    tmp++;
    n++;
    input = tmp;
  }
}

void solve_part_1(char *input) {
  PageOrderRules rules = {0};
  PrintingOrders orders = {0};
  parse_input(input, &rules, &orders);

  int sum = 0;
  int visited[1024];
  for (size_t i = 0; i < orders.count; i++) {
    size_t page_count = orders.items[i].count;
    for (size_t vi = 0; vi < page_count; vi++) {
      visited[vi] = 0;
    }
    for (size_t j = 0; j < page_count; j++) {
      int page = orders.items[i].items[j];
      for (size_t x = 0; x < rules.count; x++) {
        if (rules.items[x].first == page) {
          for (size_t y = 0; y < page_count; y++) {
            if (visited[y] == rules.items[x].second) {
              goto invalid;
            }
          }
        }
      }
      visited[j] = page;
    }
    int middle = orders.items[i].items[orders.items[i].count / 2];
    sum += middle;
invalid:
  }
  printf("Middle sum: %d\n", sum);
}

int main(void) {
  char *input = read_file("src/input5.txt"); 
  solve_part_1(input);
  return 0;
}
