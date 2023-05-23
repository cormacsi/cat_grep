CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

.PHONY : all s21_cat cat tests clean \
	leaks style check rebuild

all : s21_cat

s21_cat : s21_cat.c s21_cat.h
	$(CC) $(FLAGS) $< -o $@

tests : s21_cat
	sh cat_tests.sh

clean : 
	@echo ==Cleaning..==
	@rm -rf *.o
	@rm -rf s21_cat
	@rm -rf .clang-format
	@echo ==CLANG REMOVED!==
	@echo ==Done!==

style : 
	@cp ../../materials/linters/.clang-format ./
	@echo =COPY DONE!=
	@clang-format -n s21_cat.c
	@clang-format -n s21_cat.h
	@echo =CHECK COMPLETED=
	@rm -rf .clang-format
	@echo =REMOVE DONE!=

leaks :
	leaks -atExit -- ./s21_cat -benstv cat_tests/*

check : 
	$(MAKE) style
	$(MAKE) leaks

rebuild : 
	$(MAKE) clean
	$(MAKE) all
