#pragma once
#include "genlib.h"
#include <stdio.h>
extern bool is_file_open;
extern FILE * fp;
extern bool to_open_file;
extern bool to_new_file;
void open_a_file(char *str);
void new_a_file(char *str);
void close_a_file();