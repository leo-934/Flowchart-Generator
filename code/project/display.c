#include "display.h"
#include "main.h"
#include "enum.h"
#include "link.h"
#include "draw_button.h"
#include "draw_menu.h"
#include "draw_log.h"
#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "mousecallback.h"
#include "genlib.h"
#include "draw_terminal.h"
void display() {
	DisplayClear();
	//clear the graphic window
	draw_terminal();
	draw_menu();
	draw_button();
	draw_log();
	char *prev_color = GetPenColor();
	SetPenColor("Red");
	drawRectangle(10, window_height - menu_height, 3, menu_height, 1);//挡住溢出的log
	SetPenColor("Green");
	drawLabel(10.1, 0.05 + window_height - menu_height, "Log");
	SetPenColor(prev_color);
	if (is_draw_circle) {

		//drawRectangle(circle1x, circle2y, circle2x - circle1x, circle1y - circle2y, 0);
		MovePen(circle2x, (circle1y + circle2y) / 2);
		DrawEllipticalArc((circle2x - circle1x) / 2, (circle1y - circle2y) / 2, 0, 360);

	}
	else if (is_draw_rectangle) {
		drawRectangle(rectangle1x, rectangle2y, rectangle2x - rectangle1x, rectangle1y - rectangle2y, 0);
	}
	else if (is_draw_triangle) {
		drawTriangle(triangle1x, triangle2y, triangle2x - triangle1x, triangle1y - triangle2y);
	}
	else if (is_draw_parallelogram) {
		drawParallelogram(parallelogram1x, parallelogram1y, parallelogram2x, parallelogram2y, 0);
	}
	else if (is_draw_diamond) {
		drawDiamond(diamond1x, diamond1y, diamond2x, diamond2y, 0);
	}
	if (button(GenUIID(0), 0, window_height - menu_height - icon_height * 2, 1, icon_height, "input")) {
		if (is_add_figure == TRUE) {
			type_choose = input;
			add_log("语句种类选择成功，请开始构建图形");
		}
	}
	if (button(GenUIID(0), 0, window_height - menu_height - icon_height * 3, 1, icon_height, "output")) {
		if (is_add_figure == TRUE) {
			type_choose = output;
			add_log("语句种类选择成功，请开始构建图形");
		}
	}
	if (button(GenUIID(0), 0, window_height - menu_height - icon_height * 4, 1, icon_height, "condition")) {
		if (is_add_figure == TRUE) {
			type_choose = condition;
			add_log("语句种类选择成功，请开始构建图形");
		}
	}
	if (button(GenUIID(0), 0, window_height - menu_height - icon_height * 5, 1, icon_height, "loop")) {
		if (is_add_figure == TRUE) {
			type_choose = loop;
			add_log("语句种类选择成功，请开始构建图形");
		}
	}
	traverse(NULL, list);
	tra_i = 0;
	tra_j = -1;
	/*
	Link tmp=list;//for traversal
	Link prev = NULL;
	double x1, y1, x2, y2;
	int i = 0,j=0;//to generate different GenUIID in the same line
	while (tmp) {
		x1 = tmp->figure1x;
		x2 = tmp->figure2x;
		y1 = tmp->figure1y;
		y2 = tmp->figure2y;
		int text_size0 = strlen(tmp->text[0]);
		int text_size1 = strlen(tmp->text[1]);
		int text_size2 = strlen(tmp-> text[2]);


		switch (tmp->type) {
		case empty:
			textbox(GenUIID(i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
			break;
		case input:
			textbox(GenUIID(i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 + GetFontHeight()) / 2, text_size0*0.1+0.1, GetFontHeight(), tmp->text[0], 30);
			drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 - GetFontHeight()) / 2, "=");
			textbox(GenUIID(i), (x1 + x2 - text_size1 * 0.1) / 2, (y1 + y2 - GetFontHeight()*3) / 2, text_size1*0.1+0.1, GetFontHeight(), tmp->text[1], 30);
			break;
		case output:
			textbox(GenUIID(i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1+0.1, GetFontHeight(), tmp->text, 30);
			break;
		case condition:
			drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 + GetFontHeight()) / 2, "IF");
			textbox(GenUIID(i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()*3) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
			break;
		case loop:
			drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 + GetFontHeight()) / 2, "WHILE");
			textbox(GenUIID(i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight() * 3) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
			break;
		}


		switch (tmp->kind) {
		case circle:
			MovePen(x2, (y1 + y2) / 2);
			DrawEllipticalArc((x2 - x1) / 2, (y1 - y2) / 2, 0, 360);
			break;
		case rectangle:
			drawRectangle(x1, y2, x2 - x1, y1 - y2,0);
			break;
		case triangle:
			drawTriangle(x1, y2, x2 - x1, y1 - y2);
			break;
		case parallelogram:
			drawParallelogram(x1, y1, x2, y2, 0);
			break;
		case diamond:
			drawDiamond(x1, y1, x2, y2, 0);
			break;
		}

		if (prev) {
			MovePen((prev->figure1x + prev->figure2x) / 2, prev->figure2y);
			DrawLine((x1 + x2) / 2 - (prev->figure1x + prev->figure2x) / 2, y1 - prev->figure2y);
			if (button(GenUIID(j), (x1 + x2 + prev->figure1x + prev->figure2x) / 4 - 0.125, (prev->figure2y + y1) / 2, 0.25, 0.25, "+")) {
				insert_place = tmp;
				is_add_figure = TRUE;
				add_log("请点击左上角图标按钮选择图形");
			}

		}
		i++;
		j++;
		prev = tmp;
		tmp = tmp->next;
	}*/



}