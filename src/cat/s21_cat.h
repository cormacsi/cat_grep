#ifndef SRC_CAT_S21_CAT_H
#define SRC_CAT_S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_0 "\033[91mThis error was printed by <getopt.h> library\033[0m"
#define ERROR_1 "\033[91mUsage: cat [-benstv] [filename ...]\033[0m"
#define ERROR_2 "\033[91mCat: illegal option -- \033[0m"
#define ERROR_3 "\033[91mNo such file or directory\033[0m"
#define ERROR_4 "\033[91mPossible memory leaks: can't close the file\033[0m"

typedef struct {
  FILE *file;
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;

typedef struct {
  int print_char;
  int next_char;
  int first_letter;
  int squeeze;
  int s_minus;
  int number;
} all;

int parcer(int argc, char **argv, opt *options, int *opt_ind);
void s21_cat(int argc, char **argv, opt *options);
int printer(opt *options);
void squeeze(all *flags, opt *options);
void print_function(all *flags, opt *options);
void num_function(all *flags, opt *options);
void dollar(all *flags, opt *options);
int end(FILE *file);
void print_error(int error);

#endif  // SRC_CAT_S21_CAT_H
