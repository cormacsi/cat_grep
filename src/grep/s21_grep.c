#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    s21_grep_cormacsi(argc, argv);
  } else {
    print_error(1);
  }
  return 0;
}

void parcer(int argc, char **argv, opt *options) {
  int option = 0;
  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != EOF &&
         !options->error) {
    if (option == 'e') {
      pattern_e(options, optarg);
      options->e = 1;
    } else if (option == 'i') {
      options->i = 1;
    } else if (option == 'v') {
      options->v = 1;
    } else if (option == 'c') {
      options->c = 1;
    } else if (option == 'l') {
      options->l = 1;
    } else if (option == 'n') {
      options->n = 1;
    } else if (option == 'h') {
      options->h = 1;
    } else if (option == 's') {
      options->s = 1;
    } else if (option == 'f') {
      pattern_f(options, optarg);
      options->f = 1;
    } else if (option == 'o') {
      options->o = 1;
    }
  }
  set_pattern(argc, argv, optind, options);
}

void set_pattern(int argc, char **argv, int optind, opt *options) {
  if (options->v)
    options->o = 0;
  else if (options->c)
    options->o = 0;
  else if (options->l)
    options->o = 0;
  if (!options->error) {
    if (options->e || options->f) {
      options->pattern_index = 1;
    } else if ((optind + 1) != argc) {
      strcpy(options->pattern, argv[optind]);
      options->pattern_index = 1;
      optind++;
    }
  }
  options->num_files = argc - optind;
}

void pattern_e(opt *options, char *optarg) {
  if (options->e == 0 && options->f_line == 0) {
    strcpy(options->pattern, optarg);
  } else if (options->e != 0 || options->f_line != 0) {
    strcat(strcat(options->pattern, "|"), optarg);
  }
}

void pattern_f(opt *options, char *optarg) {
  FILE *pattern_file = NULL;
  if ((pattern_file = fopen(optarg, "r")) != NULL) {
    char *next_line = NULL;
    size_t pattern_len = 0;
    int char_new_line = 0;
    int last_line_char = 10;
    while (getline(&next_line, &pattern_len, pattern_file) != EOF) {
      int last = strlen(next_line) - 1;
      if (next_line[0] != '\n') {
        if (next_line[last] == 10) next_line[last] = '\0';
        pattern_e(options, next_line);
        options->f_line = 1;
        last_line_char = next_line[0];
      } else {
        char_new_line = 1;
      }
    }
    if (char_new_line && last_line_char != '\n') pattern_e(options, ".");

    options->f_line = 0;
    if (next_line) free(next_line);
    if (pattern_file) fclose(pattern_file);
  } else if (!options->s) {
    print_error_directory(optarg);
    options->error = 1;
  }
}

void s21_grep_cormacsi(int argc, char **argv) {
  opt options = {0};
  options.pattern = (char *)malloc(2048 * sizeof(char));
  if (options.pattern) {
    parcer(argc, argv, &options);
    if (argc - optind > 0 && options.pattern_index != 0) {
      for (int i = argc - options.num_files; i < argc; i++) {
        if (strcmp(".", argv[i]) == 0) {
          continue;
        }
        options.file = fopen(argv[i], "r");
        options.filename = argv[i];
        if (options.file != NULL) {
          searcher(&options);
          end(options.file);
        } else if (!options.s) {
          print_error_directory(options.filename);
        }
      }
    } else if (!options.error) {
      print_error(1);
    }
    free(options.pattern);
  } else {
    print_error(2);
  }
}

void searcher(opt *options) {
  char *read_line = NULL;
  regex_t compiled;
  regmatch_t pmatch[1];
  const size_t nmatch = 1;
  int status;
  size_t len = 0;
  int counter = 0, anticounter = 0, line_num = 0;
  int reg_flag = options->i ? REG_ICASE : REG_EXTENDED;
  regcomp(&compiled, options->pattern, reg_flag);
  while (getline(&read_line, &len, options->file) != EOF) {
    status = regexec(&compiled, read_line, nmatch, pmatch, 0);
    if (read_line[0] == '\n' && options->pattern[0] == '.')
      status = REG_NOMATCH;
    line_num += 1;
    if (status == 0 && !options->v) {
      counter += 1;
      printer(options, compiled, read_line, pmatch, line_num);
    } else if (status != 0 && options->v) {
      anticounter += 1;
      printer(options, compiled, read_line, pmatch, line_num);
    }
  }
  option_c(options, counter, anticounter);

  option_l(options, counter, anticounter);

  if (read_line != NULL) free(read_line);
  regfree(&compiled);
}

void small_printer(opt *options, int number) {
  if (options->num_files == 1 || options->h)
    printf("%d\n", number);
  else
    printf("%s:%d\n", options->filename, number);
}

void option_c(opt *options, int counter, int anticounter) {
  if (options->c && !options->l) {
    if (options->v) {
      small_printer(options, anticounter);
    } else if (options->v == 0) {
      small_printer(options, counter);
    }
  }
}

void option_l(opt *options, int counter, int anticounter) {
  if (options->l) {
    if (options->v && anticounter > 0) {
      if (options->c) {
        small_printer(options, anticounter > 0);
      }
      if (anticounter > 0) printf("%s\n", options->filename);
    } else if (options->v == 0) {
      if (options->c) {
        small_printer(options, counter > 0);
      }
      if (counter > 0) printf("%s\n", options->filename);
    }
  }
}

void printer(opt *options, regex_t compiled, char *read_line,
             regmatch_t pmatch[], int line_num) {
  if (!options->c && !options->l) {
    if (options->num_files > 1 && options->h == 0)
      printf("%s:", options->filename);
    if (!options->o) {
      if (options->n) printf("%d:", line_num);
      printf("%s", read_line);
      int str_len = strlen(read_line) - 1;
      if ((read_line)[str_len] != '\n') printf("\n");
    } else if (options->o) {
      option_o(options, compiled, read_line, pmatch, line_num);
    }
  }
}

void option_o(opt *options, regex_t compiled, char *read_line,
              regmatch_t pmatch[], int line_num) {
  if (options->n) printf("%d:", line_num);
  while (regexec(&compiled, read_line, 1, pmatch, 0) == options->v) {
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
      printf("%c", (read_line)[i]);
    }
    printf("\n");
    read_line += pmatch[0].rm_eo;
  }
}

int end(FILE *file) {
  int error = 0;
  if (file) {
    if (fclose(file) != 0) print_error(2);
  }
  return error;
}

void print_error(int error) {
  if (error == 1) {
    fprintf(stderr, "%s", ERROR_1);
  } else if (error == 2) {
    fprintf(stderr, "%s", ERROR_2);
  } else {
    fprintf(stderr, "%s", ERROR_0);
  }
}

void print_error_directory(char *filename) {
  if (filename) {
    fprintf(stderr, "grep: %s: %s", filename, ERROR_3);
  } else {
    fprintf(stderr, "%s", ERROR_1);
  }
}
