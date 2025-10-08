#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
  puts("A simple CouNTeR v" VERSION);
  puts("Stores a number (int) in a .cntr file in the current directory and "
       "allows you to increment and decrement it using simple commands");
  puts("Exit codes: [0] if the cntr is positive, [1] if it'z 0 or negative");
  puts("Usage: cntr [FLAG] [ACTION]\n");
  puts("FLAGS:");
  puts("\t-r   print just the raw number");
  puts("\t          (for scripts)");
  puts("\t-q   do not print anything\n");
  puts("ACTIONS:");
  puts("\tnone shows the current value");
  puts("\t+    increments the counter by 1");
  puts("\t+N   increments the counter by N");
  puts("\t-    decrements the counter by 1");
  puts("\t-N   decrements the counter by N");
  puts("\t=    sets the counter to 0");
  puts("\t=N   sets the counter to N");
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

  bool quiet = false;
  bool raw = false;

  if (argc > 1 && strcmp(argv[1], "-q") == 0) {
    quiet = true;
    argc--;
    argv++;
  }
  if (argc > 1 && strcmp(argv[1], "-r") == 0) {
    raw = true;
    quiet = true;
    argc--;
    argv++;
  }

  int value = load(fp);
  int new_value = get_new_value(argc, argv, value);

  if (!quiet) printf("\t>> %d <<\n", value);


  if (new_value != value) {
    if (!quiet) printf("\t== %d ==\n", new_value);
    save(new_value, fp);
  }

  if (raw) printf("%d", new_value);
  
  fclose(fp);
  return (int)(new_value <= 0);
}
