#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    opt options = {0};
    int opt_ind = 0;
    if (!parcer(argc, argv, &options, &opt_ind)) {
      s21_cat(argc, argv, &options);
    }
  }
  return 0;
}

int parcer(int argc, char **argv, opt *options, int *opt_ind) {
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  int option = 0;
  int error = 0;
  while ((option = getopt_long(argc, argv, "+benstvET", long_options,
                               opt_ind)) != EOF &&
         error == 0) {
    if (option == 'b') {
      options->b += 1;
    } else if (option == 'e') {
      options->e += 1;
      options->v += 1;
    } else if (option == 'n') {
      options->n += 1;
    } else if (option == 's') {
      options->s += 1;
    } else if (option == 't') {
      options->t += 1;
      options->v += 1;
    } else if (option == 't') {
      options->v += 1;
    } else if (option == 'v') {
      options->v += 1;
    } else if (option == 'E') {
      options->e += 1;
    } else if (option == 'T') {
      options->t += 1;
    } else {
      print_error(0);
      error = 1;
      break;
    }
  }
  options->n *= (options->b == 0) ? 1 : 0;
  return error;
}

void s21_cat(int argc, char **argv, opt *options) {
  for (int x = 1; x < argc; x += 1) {
    options->file = fopen(argv[x], "r");
    if (options->file != NULL) {
      printer(options);
      if (!end(options->file)) {
        print_error(4);
      }
    }
  }
}

int printer(opt *options) {
  all flags = {0};
  flags.print_char = fgetc(options->file);
  flags.next_char = '\n';
  flags.number = 1;

  if (flags.print_char != EOF) {
    num_function(&flags, options);
    dollar(&flags, options);
    while ((flags.next_char = fgetc(options->file)) != EOF) {
      squeeze(&flags, options);
      print_function(&flags, options);

      if (flags.next_char != EOF) num_function(&flags, options);
      dollar(&flags, options);
      flags.print_char = flags.next_char;
    }
    if (flags.print_char != EOF) {
      print_function(&flags, options);
    }
  }
  return 0;
}

void squeeze(all *flags, opt *options) {
  if (options->s) {
    if (flags->print_char == '\n' && flags->next_char == '\n') {
      if (flags->first_letter != 0) printf("\n");
      while (flags->next_char == '\n') {
        flags->next_char = fgetc(options->file);
        (flags->s_minus)++;
      }
    }
    if (flags->s_minus) {
      (flags->squeeze)++;
      if (options->n) num_function(flags, options);
      dollar(flags, options);
    }
    flags->s_minus = 0;
  }
}

void print_function(all *flags, opt *options) {
  if (options->t && flags->print_char == '\t') {
    printf("^");
    flags->print_char = 'I';
  }
  if (options->v) {
    int ch = (int)flags->print_char;
    if (ch >= 0 && ch != 9 && ch != 10 && ch < 32) {
      printf("^");
      flags->print_char += 64;
    } else if (ch == 127) {
      printf("^");
      flags->print_char = '?';
    } else if (ch > 127 && ch < 160) {
      printf("M-^");
      flags->print_char &= ~(1 << 7);
    }
  }
  if (flags->print_char != EOF) printf("%c", flags->print_char);
  flags->first_letter += 1;
}

void num_function(all *flags, opt *options) {
  if (options->n && flags->first_letter == 0 && flags->print_char != '\n') {
    printf("%6d\t", flags->number);
    flags->number += 1;
  } else if (options->n && flags->first_letter == 0 && flags->number != 1) {
    if (flags->print_char != '\n') {
      printf("%6d\t", flags->number);
      flags->number += 1;
    }
  } else if (options->n && flags->print_char == '\n') {
    printf("%6d\t", flags->number);
    flags->number += 1;
  }

  if (options->b && flags->first_letter == 0 && flags->print_char != '\n') {
    printf("%6d\t", flags->number);
    flags->number += 1;
  } else if (options->b && flags->print_char == '\n' &&
             flags->next_char != '\n') {
    printf("%6d\t", flags->number);
    flags->number += 1;
  }
}

void dollar(all *flags, opt *options) {
  if (options->e) {
    if (flags->next_char == '\n' && flags->first_letter != 0)
      printf("$");
    else if (flags->print_char == '\n' && flags->s_minus &&
             flags->first_letter != 0)
      printf("$");
    else if (flags->first_letter == 0 && flags->next_char == '\n' &&
             flags->print_char == '\n')
      printf("$");
  }
}

int end(FILE *file) {
  int error = 0;
  if (!fclose(file)) {
    error = 1;
  }
  return error;
}

void print_error(int error) {
  if (error == 1) {
    fprintf(stderr, "%s\n", ERROR_1);
  } else if (error == 2) {
    fprintf(stderr, "%s\n", ERROR_2);
  } else if (error == 3) {
    fprintf(stderr, "%s\n", ERROR_3);
  } else if (error == 4) {
    fprintf(stderr, "%s\n", ERROR_4);
  } else {
    fprintf(stderr, "%s\n", ERROR_0);
  }
}
