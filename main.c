#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CNTR_FILE ".cntr"
#define VERSION "0.0.1"

int load(FILE *fp) {
  int value;
  rewind(fp);
  fread(&value, sizeof(value), 1, fp);
  return value;
}

void save(int value, FILE *fp) {
  rewind(fp);
  fwrite(&value, sizeof(value), 1, fp);
}

void help() {
  puts("A simple CouNTeR " VERSION);
  puts("Stores a number (int) in a .cntr file in the current directory and "
       "allows you to increment and decrement it using simple commands");
  puts("Usage:");
  puts("\tcntr      shows the current value");
  puts("\tcntr +    increments the counter by 1");
  puts("\tcntr +N   increments the counter by N");
  puts("\tcntr -    decrements the counter by 1");
  puts("\tcntr -N   decrements the counter by N");
  puts("\tcntr =N   sets the counter to N");
  exit(2);
}

int get_new_value(int argc, char *argv[], int current) {
  if (argc == 1)
    return current;
  else if (argc > 2)
    help();

  int delta = 0;
  switch (argv[1][0]) {
  case '+':
    delta = 1;
    break;
  case '-':
    delta = -1;
    break;
  case '=':;
    char *endptr;
    int value = strtol(argv[1] + 1, &endptr, 10);
    if (endptr != argv[1] + strlen(argv[1]))
      help();
    return value;
  default:
    help();
  }
  if (strlen(argv[1]) > 1) {
    char *endptr;
    delta *= strtol(argv[1] + 1, &endptr, 10);
    if (endptr != argv[1] + strlen(argv[1]))
      help();
  }
  return current + delta;
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen(CNTR_FILE, "r+");
  if (fp == NULL) {
    fp = fopen(CNTR_FILE, "w+");
    if (fp == NULL) {
      perror("Failed to create " CNTR_FILE);
      return -1;
    }
    int default_value = 0;
    fwrite(&default_value, sizeof(default_value), 1, fp);
    rewind(fp);
  }

  int value = load(fp);
  int new_value = get_new_value(argc, argv, value);

  printf("\t>> %d <<\n", value);

  if (new_value != value) {
    printf("\t== %d ==\n", new_value);
    save(new_value, fp);
  }
  fclose(fp);
  return 0;
}
