#ifndef SRC_GREP_S21_GREP_H
#define SRC_GREP_S21_GREP_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_0 "\033[91mThis error was printed by <getopt.h> library\033[0m\n"
#define ERROR_1                                                           \
  "\033[91mUsage: \033[95mgrep \033[92m[-eivclnhsfo] \033[96m[template] " \
  "\033[93m[filename ...]\033[0m\n"
#define ERROR_2 "\033[91mPossible memory leaks: can't close the file\033[0m\n"
#define ERROR_3 "\033[91mNo such file or directory\033[0m\n"

typedef struct {
  FILE *file;
  char *filename;
  int num_files;
  char *pattern;
  bool pattern_index;
  bool f_line;
  bool error;
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} opt;

void s21_grep_cormacsi(int argc, char **argv);
void parcer(int argc, char **argv, opt *options);
void pattern_e(opt *options, char *optarg);
void pattern_f(opt *options, char *optarg);
void set_pattern(int argc, char **argv, int optind, opt *options);
void searcher(opt *options);
void small_printer(opt *options, int number);
void option_c(opt *options, int counter, int anticounter);
void option_l(opt *options, int counter, int anticounter);
void printer(opt *options, regex_t compiled, char *read_line,
             regmatch_t pmatch[], int line_num);
void option_o(opt *options, regex_t compiled, char *read_line,
              regmatch_t pmatch[], int line_num);
int end(FILE *file);
void print_error(int error);
void print_error_directory(char *filename);

#endif  // SRC_GREP_S21_GREP_H
