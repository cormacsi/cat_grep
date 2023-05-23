CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

.PHONY : all s21_grep tests clean \
	leaks style check rebuild

all : s21_grep

s21_grep : s21_grep.c s21_grep.h
	$(CC) $(FLAGS) $< -o $@

tests : s21_grep
	sh grep_tests.sh

clean : 
	@echo ==Cleaning..==
	@rm -rf *.o
	@rm -rf s21_grep
	@rm -rf .clang-format
	@echo ==CPP REMOVED!==
	@echo ==Done!==

style : 
	@cp ../../materials/linters/.clang-format ./
	@echo =COPY DONE!=
	@clang-format -n s21_grep.c
	@clang-format -n s21_grep.h
	@echo =CHECK COMPLETED=
	@rm -rf .clang-format
	@echo =REMOVE DONE!=

leaks :
	leaks -atExit -- ./s21_grep -n -f grep_tests/grep_test_1.txt grep_tests/grep_test_2.txt

check : 
	$(MAKE) style
	$(MAKE) leaks

rebuild : 
	$(MAKE) clean
	$(MAKE) all
