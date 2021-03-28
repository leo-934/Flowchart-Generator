#include "draw_terminal.h"
#include "imgui.h"
#include "graphics.h"
#include <string.h>
#include "draw_log.h"
#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "file_operate.h"
bool is_terminal_input = FALSE;
int terminal_row=0;
char terminal_data[500][20];
char terminal_box[30];
char last_terminal[30];
void draw_terminal() {
	MovePen(1.5,0);
	DrawLine(0, 3);
	DrawLine(-1.5, 0);
	
	SetPenColor("Green");
	drawRectangle(0, 1.25, 1.5, 1.75, 1);
	SetPenColor("Red");
	SetPenColor("Gray");
	drawLabel(0.25, 1.05, "请在此处输入");
	SetPenColor("Green");
	textbox(GenUIID(0), 0.25, 0.75, 1, 0.25, terminal_box, 25);
	if (button(GenUIID(0), 0.5, 0.25, 0.5, 0.3,"OK")) {
		
		if (strlen(terminal_box) > 0) {
			add_terminal();
			is_terminal_input = TRUE;
			strcpy(last_terminal, terminal_box);
			add_log("数据输入成功");
			if (to_open_file) {
				open_a_file(get_last_terminal());
				to_open_file = FALSE;
			}
			if (to_new_file) {
				new_a_file(get_last_terminal());
				to_new_file = FALSE;
			}
		}

		terminal_box[0] = '\0';
		
		
	}

	double fh = 0.25;
	
	SetPenColor("Red");
	for (int i =terminal_row-1 ; i >= 0&&i>=terminal_row-7; i--) {
		drawLabel(0.1,1.3 + fh * (terminal_row - i - 1), terminal_data[i]);
	}

	
	
}
void add_terminal() {
	strcpy(terminal_data[terminal_row], terminal_box);
	terminal_row++;
	return;
}
char * get_last_terminal() {
	if (is_terminal_input) {
		is_terminal_input = FALSE;
		return last_terminal;
	}
	return NULL;
}



