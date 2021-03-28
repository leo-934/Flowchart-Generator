#include "main.h"
#include "enum.h"
#include "draw_menu.h"
#include "draw_log.h"
#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "genlib.h"
#include "file_operate.h"
#include "inputbox.h"
#include <Windows.h>
#include "draw_terminal.h"
#include "link.h"
void draw_menu() {
	static char* menu_list_file[] = {
		"File",
		"New  | Ctrl-N",
		"Open | Ctrl-O",
		"Close| Ctrl-C",
		"Exit | Ctrl-E",
	};
	/*static char* menu_list_edit[] = {
		"Edit",
		"Select",
		"Copy",
		"Paste",
		"delete:"
	};*/
	static char* menu_list_plot[] = {
		"Sentence",
		"input",
		"output",
		"condition",
		"loop"
	};
	static char* menu_list_help[] = {
	"Other",
	"Help",
	"About",
	"Get something fantastic"
	};
	static char* select_label = NULL;
	double font_height = GetFontHeight();
	//the default font height is 0.1666
	double x = 7.5;
	double y = window_height;
	double h = font_height * 1.5;//control height
	menu_height = h;
	double w = TextStringWidth(menu_list_file[0]) * 3;//control width
	double wlist = TextStringWidth(menu_list_file[1])*1.2;//subcontrol width
	double xindent = window_height / 20;//indentation
	int selection;
	drawMenuBar(0, y - h, window_width, h);

	//the function menuList will help us to diaplay the menu and menulist
	//according to the messgage of user's mouse

	//set menu color
	setMenuColors("Orange", "Black", "Green", "Red", 1);
	//file menu
	selection = menuList(GenUIID(0), x+0.4, y - h, w-0.4, wlist, h, menu_list_file, sizeof(menu_list_file) / sizeof(menu_list_file[0]));
	if (selection > 0)//which means that user have click the menu
	{
		select_label = menu_list_file[selection];
		switch (selection) {
		case 1:
			to_new_file = TRUE;
			break;
		case 2:
			to_open_file=TRUE;
			break;
		case 3:
			fclose(fp);
			init_list(&list);
			break;
		case 4:
			exit(0);
			break;

		}

	}
	//edit menu
	/*selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menu_list_edit, sizeof(menu_list_edit) / sizeof(menu_list_edit[0]));
	if (selection > 0)
		select_label = menu_list_edit[selection];
		*/
	//plot menu
	selection = menuList(GenUIID(0), x +  w, y - h, w, wlist, h, menu_list_plot, sizeof(menu_list_plot) / sizeof(menu_list_plot[0]));
	if (selection > 0) {
		select_label = menu_list_plot[selection];

		switch (selection) {
		case 1:
			type_choose = input;
			add_log("语句种类选择成功，请开始构建图形");
			break;
		case 2:
			type_choose = output;
			add_log("语句种类选择成功，请开始构建图形");
			break;
		case 3:
			type_choose = condition;
			add_log("语句种类选择成功，请开始构建图形");
			break;
		case 4:
			type_choose = loop;
			add_log("语句种类选择成功，请开始构建图形");
			break;
		}
	}
	//help menu
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menu_list_help, sizeof(menu_list_help) / sizeof(menu_list_help[0]));
	if (selection > 0) {
		switch (selection) {
		case 1:
			system("notepad ..\\project\\help.txt");
			break;
		case 2:
			system("notepad ..\\project\\about.txt");
			break;

		}
	}
	//used for test
	/*x = window_width / 15;
	y = window_height / 8 * 7;
	SetPenColor("Blue");
	drawLabel(x, y -= h, "Most recently selected menu is:");
	SetPenColor("Red");
	drawLabel(x + xindent, y -= h, select_label);
	*/
}
