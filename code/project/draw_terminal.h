#pragma once
#include "genlib.h"
extern bool is_terminal_input;
extern int terminal_row;
extern char terminal_data[500][20];
void draw_terminal();
void add_terminal();
char * get_last_terminal();