#include "mousecallback.h"
#include "enum.h"
#include "link.h"
#include <string.h>
#include "draw_log.h"
#include "display.h"
#include "main.h"
#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "genlib.h"
#include <stdlib.h>
#include "file_operate.h"
#include "generate_id.h"
#include "draw_terminal.h"
struct variable variable_data[100];
int variable_num = 0;
bool is_exec = FALSE;
Link current_exec;
bool is_create_circle = FALSE;//等待创建圆形模式
bool is_draw_circle = FALSE;
bool is_create_rectangle = FALSE;//同上
bool is_draw_rectangle = FALSE;
bool is_create_triangle = FALSE;
bool is_draw_triangle = FALSE;
bool is_create_parallelogram = FALSE;
bool is_draw_parallelogram = FALSE;
bool is_create_diamond = FALSE;
bool is_draw_diamond = FALSE;
bool is_move_figure = FALSE;//是否正在移动图形
bool is_add_figure = FALSE;//确定用户画图之前是否点击加号，没点不能画
double circle1x, circle1y, circle2x, circle2y;//circle1是用户画圆时候的左上角，circle2是右下角figure
double rectangle1x, rectangle1y, rectangle2x, rectangle2y;
double triangle1x, triangle1y, triangle2x, triangle2y;
double parallelogram1x, parallelogram1y, parallelogram2x, parallelogram2y;
double diamond1x, diamond1y, diamond2x, diamond2y;

/****************************************************************/ 

#define is_digit(ch) ('0'<=(ch) && (ch)<='9')

int Char_To_Int(char *number)
{
	int value=0;
	for(char *ch = number; *ch != '\0'; ++ch) value = (value*10)+(*ch-'0');
	return value;
}
bool Is_Constant(char *name) 
{
	for(char *ch = name; *ch != '\0'; ++ch) if(!is_digit(*ch)) return 0;
	return 1;
}
/****************************************************************/ 

void MouseEventProcess(int x, int y, int button, int event) {

	uiGetMouse(x, y, button, event);
	static double mx, my;
	mx = ScaleXInches(x);
	my = ScaleYInches(y);
	static double omx, omy;//上一次进入时候的x和y
	switch (event) {
	case BUTTON_DOWN:
		if (button == LEFT_BUTTON) {
			if (type_choose != empty) {
				if (is_create_circle) {
					if (in_box(mx, my, 1.5, 10, 0, 6.75)) {
						is_draw_circle = TRUE;//开始画圆
						circle1x = circle2x = mx;
						circle1y = circle2y = my;
						add_log("开始构建圆形，按住鼠标左键拖动画图");
					}
				}
				else if (is_create_rectangle) {
					if (in_box(mx, my, 1.5, 10, 0, 6.75)) {
						is_draw_rectangle = TRUE;
						rectangle1x = rectangle2x = mx;
						rectangle1y = rectangle2y = my;
						add_log("开始构建矩形，按住鼠标左键拖动画图");
					}

				}
				else if (is_create_triangle) {
					if (in_box(mx, my, 1.5, 10, 0, 6.75)) {
						is_draw_triangle = TRUE;
						triangle1x = triangle2x = mx;
						triangle1y = triangle2y = my;
						add_log("开始构建三角形，按住鼠标左键拖动画图");
					}

				}
				else if (is_create_parallelogram) {
					if (in_box(mx, my, 1.5, 10, 0, 6.75)) {
						is_draw_parallelogram = TRUE;
						parallelogram1x = parallelogram2x = mx;
						parallelogram1y = parallelogram2y = my;
						add_log("开始构建平行四边形，按住鼠标左键拖动画图");
					}
				}
				else if (is_create_diamond) {
					if (in_box(mx, my, 1.5, 10, 0, 6.75)) {
						is_draw_diamond = TRUE;
						diamond1x = diamond2x = mx;
						diamond1y = diamond2y = my;
						add_log("开始构建菱形，按住鼠标左键拖动画图");
					}

				}

			}
			if (is_in_a_figure(list, mx, my)) {
				is_move_figure = TRUE;
				omx = mx;
				omy = my;
				add_log("已进入拖动模式，按住鼠标拖动图形");
			}
		}
		break;
	case BUTTON_UP:
		if (button == RIGHT_BUTTON) {
			if (is_in_a_figure(list, mx, my)) {
				write_delete(figure_to_move);
				delete(&list, figure_to_move);
			}

		}
		if (button == LEFT_BUTTON) {
			if (is_add_figure) {
				if (in_box(mx, my, 0, icon_width, window_height - menu_height - icon_height, window_height - menu_height)) {//判断是否点击第一个按钮
					is_create_circle = TRUE;//进入等待创建圆形模式
					type_choose = empty;
					add_log("已选择：圆形，请选择语句种类");
				}
				else if (in_box(mx, my, icon_width, icon_width * 2, window_height - menu_height - icon_height, window_height - menu_height)) {
					is_create_rectangle = TRUE;
					type_choose = empty;
					add_log("已选择：矩形，请选择语句种类");
				}
				else if (in_box(mx, my, icon_width * 2, icon_width * 3, window_height - menu_height - icon_height, window_height - menu_height)) {
					is_create_triangle = TRUE;
					type_choose = empty;
					add_log("已选择：三角形，请选择语句种类");
				}
				else if (in_box(mx, my, icon_width * 3, icon_width * 4, window_height - menu_height - icon_height, window_height - menu_height)) {
					is_create_parallelogram = TRUE;
					type_choose = empty;
					add_log("已选择：平行四边形，请选择语句种类");
				}
				else if (in_box(mx, my, icon_width * 4, icon_width * 5, window_height - menu_height - icon_height, window_height - menu_height)) {
					is_create_diamond = TRUE;
					type_choose = empty;
					add_log("已选择：菱形，请选择语句种类");
				}
			}
			if (is_move_figure) {
				is_move_figure = FALSE;//停止拖拽
				add_log("已结束拖动");
			}
			if (is_draw_circle) {//处于画圆状态下抬起鼠标，意味着画完了
				circle2x = mx;
				circle2y = my;
				Link node_circle = (Link)malloc(sizeof(struct node));
				node_circle->kind = circle;
				node_circle->figure1x = circle1x;
				node_circle->figure1y = circle1y;
				node_circle->figure2x = circle2x;
				node_circle->figure2y = circle2y;
				node_circle->type = type_choose;
				node_circle->next_extra = NULL;
				switch (type_choose) {
				case input:
					strcpy(node_circle->text[0], "variable");
					strcpy(node_circle->text[1], "integer");

					break;
				case output:
					strcpy(node_circle->text[0], "variable");

					break;
				case condition:
					strcpy(node_circle->text[0], "表达式");

					break;
				case loop:
					strcpy(node_circle->text[0], "表达式");
					node_circle->next_extra = (Link)malloc(sizeof(struct node));
					node_circle->next_extra->kind = circle;
					
					node_circle->next_extra->figure1x = node_circle->figure2x + 1;
					node_circle->next_extra->figure2x = node_circle->figure2x + 2;
					node_circle->next_extra->figure1y = node_circle->figure1y + 0.5;
					node_circle->next_extra->figure2y = node_circle->figure1y;
					node_circle->next_extra->type = loop_start;
					strcpy(node_circle->next_extra->text[0], "loop_start");
					node_circle->next_extra->next = (Link)malloc(sizeof(struct node));
					node_circle->next_extra->next->kind = circle;
					node_circle->next_extra->next->figure1x = node_circle->figure2x + 1;
					node_circle->next_extra->next->figure2x = node_circle->figure2x + 2;
					node_circle->next_extra->next->figure1y = node_circle->figure2y;
					node_circle->next_extra->next->figure2y = node_circle->figure2y-0.5;
					node_circle->next_extra->next_extra = NULL;
					node_circle->next_extra->serial = generate_id();
					node_circle->next_extra->next->serial = generate_id();
					node_circle->next_extra->next->type = loop_end;
					node_circle->next_extra->next->to_loop = node_circle;
					strcpy(node_circle->next_extra->next->text[0],"loop_end");
					node_circle->next_extra->next->next = NULL;
					node_circle->next_extra->next->next_extra = NULL;
					break;

				}

				node_circle->next = NULL;
				figure_num++;
				node_circle->serial = generate_id();
				write_insert(insert_prev, node_circle, insert_place, insert_flag);
				insert_in(&list, node_circle, insert_prev, insert_place);
				is_draw_circle = FALSE;//结束画圆
				is_create_circle = FALSE;//退出等待创建圆形模式
				is_add_figure = FALSE;
				add_log("结束创建圆形，自动连线成功");
			}
			else if (is_draw_rectangle) {
				rectangle2x = mx;
				rectangle2y = my;
				Link node_rectangle = (Link)malloc(sizeof(struct node));
				node_rectangle->kind = rectangle;
				node_rectangle->figure1x = rectangle1x;
				node_rectangle->figure1y = rectangle1y;
				node_rectangle->figure2x = rectangle2x;
				node_rectangle->figure2y = rectangle2y;
				for (int i = 0; i < 3; i++) strcpy(node_rectangle->text[i], "\0");
				node_rectangle->type = type_choose;
				node_rectangle->next_extra = NULL;
				switch (type_choose) {
				case input:
					strcpy(node_rectangle->text[0], "variable");
					strcpy(node_rectangle->text[1], "integer");

					break;
				case output:
					strcpy(node_rectangle->text[0], "variable");

					break;
				case condition:
					strcpy(node_rectangle->text[0], "表达式");
					break;
				case loop:
					strcpy(node_rectangle->text[0], "表达式");
					node_rectangle->next_extra = (Link)malloc(sizeof(struct node));
					node_rectangle->next_extra->kind = circle;
					node_rectangle->next_extra->figure1x = node_rectangle->figure2x + 1;
					node_rectangle->next_extra->figure2x = node_rectangle->figure2x + 2;
					node_rectangle->next_extra->figure1y = node_rectangle->figure1y + 0.5;
					node_rectangle->next_extra->figure2y = node_rectangle->figure1y;
					node_rectangle->next_extra->type = loop_start;
					strcpy(node_rectangle->next_extra->text[0], "loop_start");
					node_rectangle->next_extra->next = (Link)malloc(sizeof(struct node));
					node_rectangle->next_extra->next->kind = circle;
					node_rectangle->next_extra->next->figure1x = node_rectangle->figure2x + 1;
					node_rectangle->next_extra->next->figure2x = node_rectangle->figure2x + 2;
					node_rectangle->next_extra->next->figure1y = node_rectangle->figure2y;
					node_rectangle->next_extra->next->figure2y = node_rectangle->figure2y - 0.5;
					node_rectangle->next_extra->next_extra = NULL;
					node_rectangle->next_extra->serial = generate_id();
					node_rectangle->next_extra->next->serial = generate_id();

					node_rectangle->next_extra->next->type = loop_end;
					node_rectangle->next_extra->next->to_loop = node_rectangle;

					strcpy(node_rectangle->next_extra->next->text[0], "loop_end");
					node_rectangle->next_extra->next->next = NULL;
					node_rectangle->next_extra->next->next_extra = NULL;
					break;

				}

				node_rectangle->next = NULL;
				figure_num++;
				node_rectangle->serial = generate_id();
				write_insert(insert_prev, node_rectangle, insert_place, insert_flag);
				insert_in(&list, node_rectangle, insert_prev, insert_place);
				is_draw_rectangle = FALSE;
				is_create_rectangle = FALSE;
				is_add_figure = FALSE;
				add_log("结束创建矩形，自动连线成功");
			}
			else if (is_draw_triangle) {
				triangle2x = mx;
				triangle2y = my;
				Link node_triangle = (Link)malloc(sizeof(struct node));
				node_triangle->kind = triangle;
				node_triangle->figure1x = triangle1x;
				node_triangle->figure1y = triangle1y;
				node_triangle->figure2x = triangle2x;
				node_triangle->figure2y = triangle2y;
				for (int i = 0; i < 3; i++) strcpy(node_triangle->text[i], "\0");
				node_triangle->type = type_choose;
				node_triangle->next_extra = NULL;
				switch (type_choose) {
				case input:
					strcpy(node_triangle->text[0], "variable");
					strcpy(node_triangle->text[1], "integer");

					break;
				case output:
					strcpy(node_triangle->text[0], "variable");

					break;
				case condition:
					strcpy(node_triangle->text[0], "表达式");
					break;
				case loop:
					strcpy(node_triangle->text[0], "表达式");
					node_triangle->next_extra = (Link)malloc(sizeof(struct node));
					node_triangle->next_extra->kind = circle;
					node_triangle->next_extra->figure1x = node_triangle->figure2x + 1;
					node_triangle->next_extra->figure2x = node_triangle->figure2x + 2;
					node_triangle->next_extra->figure1y = node_triangle->figure1y + 0.5;
					node_triangle->next_extra->figure2y = node_triangle->figure1y;
					node_triangle->next_extra->type = loop_start;
					strcpy(node_triangle->next_extra->text[0], "loop_start");
					node_triangle->next_extra->next = (Link)malloc(sizeof(struct node));
					node_triangle->next_extra->serial = generate_id();
					node_triangle->next_extra->next->serial = generate_id();

					node_triangle->next_extra->next->kind = circle;
					node_triangle->next_extra->next->figure1x = node_triangle->figure2x + 1;
					node_triangle->next_extra->next->figure2x = node_triangle->figure2x + 2;
					node_triangle->next_extra->next->figure1y = node_triangle->figure2y;
					node_triangle->next_extra->next->figure2y = node_triangle->figure2y - 0.5;
					node_triangle->next_extra->next_extra = NULL;
					node_triangle->next_extra->next->type = loop_end;
					node_triangle->next_extra->next->to_loop = node_triangle;

					strcpy(node_triangle->next_extra->next->text[0], "loop_end");
					node_triangle->next_extra->next->next = NULL;
					node_triangle->next_extra->next->next_extra = NULL;
					break;

				}

				node_triangle->next = NULL;
				figure_num++;
				node_triangle->serial = generate_id();
				write_insert(insert_prev, node_triangle, insert_place, insert_flag);
				insert_in(&list, node_triangle, insert_prev, insert_place);
				is_draw_triangle = FALSE;
				is_create_triangle = FALSE;
				is_add_figure = FALSE;
				add_log("结束创建三角形，自动连线成功");
			}
			else if (is_draw_parallelogram) {
				parallelogram2x = mx;
				parallelogram2y = my;
				Link node_parallelogram = (Link)malloc(sizeof(struct node));
				node_parallelogram->kind = parallelogram;
				node_parallelogram->figure1x = parallelogram1x;
				node_parallelogram->figure1y = parallelogram1y;
				node_parallelogram->figure2x = parallelogram2x;
				node_parallelogram->figure2y = parallelogram2y;
				for (int i = 0; i < 3; i++) strcpy(node_parallelogram->text[i], "\0");
				node_parallelogram->type = type_choose;
				node_parallelogram->next_extra = NULL;
				switch (type_choose) {
				case input:
					strcpy(node_parallelogram->text[0], "variable");
					strcpy(node_parallelogram->text[1], "integer");

					break;
				case output:
					strcpy(node_parallelogram->text[0], "variable");

					break;
				case condition:
					strcpy(node_parallelogram->text[0], "表达式");
					break;
				case loop:
					strcpy(node_parallelogram->text[0], "表达式");
					node_parallelogram->next_extra = (Link)malloc(sizeof(struct node));
					node_parallelogram->next_extra->kind = circle;
					node_parallelogram->next_extra->figure1x = node_parallelogram->figure2x + 1;
					node_parallelogram->next_extra->figure2x = node_parallelogram->figure2x + 2;
					node_parallelogram->next_extra->figure1y = node_parallelogram->figure1y + 0.5;
					node_parallelogram->next_extra->figure2y = node_parallelogram->figure1y;
					node_parallelogram->next_extra->type = loop_start;
					strcpy(node_parallelogram->next_extra->text[0], "loop_start");
					node_parallelogram->next_extra->next = (Link)malloc(sizeof(struct node));

					node_parallelogram->next_extra->next->kind = circle;
					node_parallelogram->next_extra->serial = generate_id();
					node_parallelogram->next_extra->next->serial = generate_id();

					node_parallelogram->next_extra->next->figure1x = node_parallelogram->figure2x + 1;
					node_parallelogram->next_extra->next->figure2x = node_parallelogram->figure2x + 2;
					node_parallelogram->next_extra->next->figure1y = node_parallelogram->figure2y;
					node_parallelogram->next_extra->next->figure2y = node_parallelogram->figure2y - 0.5;
					node_parallelogram->next_extra->next_extra = NULL;
					node_parallelogram->next_extra->next->type = loop_end;
					node_parallelogram->next_extra->next->to_loop = node_parallelogram;

					strcpy(node_parallelogram->next_extra->next->text[0], "loop_end");
					node_parallelogram->next_extra->next->next = NULL;
					node_parallelogram->next_extra->next->next_extra = NULL;
					break;

				}

				node_parallelogram->next = NULL;
				figure_num++;
				node_parallelogram->serial = generate_id();
				write_insert(insert_prev, node_parallelogram, insert_place, insert_flag);
				insert_in(&list, node_parallelogram, insert_prev, insert_place);
				is_draw_parallelogram = FALSE;
				is_create_parallelogram = FALSE;
				is_add_figure = FALSE;
				add_log("结束创建平行四边形，自动连线成功");
			}
			else if (is_draw_diamond) {
				diamond2x = mx;
				diamond2y = my;
				Link node_diamond = (Link)malloc(sizeof(struct node));
				node_diamond->kind = diamond;
				node_diamond->figure1x = diamond1x;
				node_diamond->figure1y = diamond1y;
				node_diamond->figure2x = diamond2x;
				node_diamond->figure2y = diamond2y;
				for (int i = 0; i < 3; i++) strcpy(node_diamond->text[i], "\0");
				node_diamond->type = type_choose;
				node_diamond->next_extra = NULL;
				switch (type_choose) {
				case input:
					strcpy(node_diamond->text[0], "variable");
					strcpy(node_diamond->text[1], "integer");

					break;
				case output:
					strcpy(node_diamond->text[0], "variable");

					break;
				case condition:
					strcpy(node_diamond->text[0], "表达式");
					break;
				case loop:
					strcpy(node_diamond->text[0], "表达式");
					node_diamond->next_extra = (Link)malloc(sizeof(struct node));
					node_diamond->next_extra->kind = circle;
					node_diamond->next_extra->figure1x = node_diamond->figure2x + 1;
					node_diamond->next_extra->figure2x = node_diamond->figure2x + 2;
					node_diamond->next_extra->figure1y = node_diamond->figure1y + 0.5;
					node_diamond->next_extra->figure2y = node_diamond->figure1y;
					node_diamond->next_extra->type = loop_start;
					strcpy(node_diamond->next_extra->text[0], "loop_start");
					node_diamond->next_extra->next = (Link)malloc(sizeof(struct node));
					node_diamond->next_extra->next->kind = circle;
					node_diamond->next_extra->serial = generate_id();
					node_diamond->next_extra->next->serial = generate_id();

					node_diamond->next_extra->next->figure1x = node_diamond->figure2x + 1;
					node_diamond->next_extra->next->figure2x = node_diamond->figure2x + 2;
					node_diamond->next_extra->next->figure1y = node_diamond->figure2y;
					node_diamond->next_extra->next->figure2y = node_diamond->figure2y - 0.5;
					node_diamond->next_extra->next_extra = NULL;
					node_diamond->next_extra->next->type = loop_end;
					node_diamond->next_extra->next->to_loop = node_diamond;

					strcpy(node_diamond->next_extra->next->text[0], "loop_end");
					node_diamond->next_extra->next->next = NULL;
					node_diamond->next_extra->next->next_extra = NULL;
					break;

				}

				node_diamond->next = NULL;
				figure_num++;
				node_diamond->serial = generate_id();
				write_insert(insert_prev, node_diamond, insert_place, insert_flag);
				insert_in(&list, node_diamond, insert_prev, insert_place);
				is_draw_diamond = FALSE;
				is_create_diamond = FALSE;
				is_add_figure = FALSE;
				add_log("结束创建菱形，自动连线成功");
			}
		}
		break;
	case MOUSEMOVE:
		if (is_draw_circle) {
			circle2x = mx;
			circle2y = my;

		}
		else if (is_draw_rectangle) {
			rectangle2x = mx;
			rectangle2y = my;
		}
		else if (is_draw_triangle) {
			triangle2x = mx;
			triangle2y = my;
		}
		else if (is_draw_parallelogram) {
			parallelogram2x = mx;
			parallelogram2y = my;
		}
		else if (is_draw_diamond) {
			diamond2x = mx;
			diamond2y = my;
		}
		else if (is_move_figure) {
			figure_to_move->figure1x += mx - omx;//移动图形
			figure_to_move->figure2x += mx - omx;
			figure_to_move->figure1y += my - omy;
			figure_to_move->figure2y += my - omy;
			omx = mx;//更新记录
			omy = my;//更新记录
		}


		break;
	}
	display();

}
bool is_in_a_figure(Link tmp, double x, double y) {

	if (!tmp) return FALSE;
	if (in_box(x, y, tmp->figure1x, tmp->figure2x, tmp->figure2y, tmp->figure1y)) {
		figure_to_move = tmp;
		return TRUE;
	}
	return is_in_a_figure(tmp->next, x, y) || is_in_a_figure(tmp->next_extra, x, y);
}
bool in_box(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

void CharEventProcess(char ch) {
	uiGetChar(ch);//GUI get the event
	display();//refresh graphics window when event occurs
}//char callback function
void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	display();
}//keyboard callback function

void exec_input(Link p,int n) {
	int find_flag=0;
	for (int i = 0; i < variable_num; i++) {
		if (strcmp(variable_data[i].name, p->text[0]) == 0) {
			variable_data[i].num = n;
			find_flag = 1;
		}
	}
	if (!find_flag) {
		strcpy(variable_data[variable_num].name, p->text[0]);
		variable_data[variable_num].num = n;
		variable_num++;
	}
	return;
}

int exec_output(char * str) {
	int find_flag = 0;
	for (int i = 0; i < variable_num; i++) {
		if (strcmp(variable_data[i].name, str) == 0) {
			
			return variable_data[i].num;
		}
	}
	if (!find_flag) {
		return Is_Constant(str)?Char_To_Int(str):INT_MIN;
	}
}
bool exec_condition(Link p) {
	char str[30];
	char NAME[30];
	int NUM;
	strcpy(str, p->text[0]);
	int i = 0;
	while (str[i] == ' ')i++;
	int j = 0;
	while (str[i] >= 'A'&&str[i] <= 'z') {
		NAME[j++] = str[i++];
	}
	while (str[i] != '>'&&str[i] != '='&&str[i] != '<') i++;
	switch (str[i]) {
	case '>':
		if (str[i + 1] == '=') {
			return exec_output(NAME) >= atoi(str + i + 2);
		}
		else {
			return exec_output(NAME) > atoi(str + i + 1);
		}
		break;
	case '=':
		if (str[i + 1] == '=') {
			return exec_output(NAME) == atoi(str + i + 2);
		}
		else {
			return exec_output(NAME) == atoi(str + i + 1);
		}
		break;
	case '<':
		if (str[i + 1] == '=') {
			return exec_output(NAME) <= atoi(str + i + 2);
		}
		else {
			return exec_output(NAME) < atoi(str + i + 1);
		}
		break;
	}

}
void TimerEventProcess(int timerID) {
	char* sth;
	char sth_arr[30];
	switch (current_exec->type) {
	case empty:
		if (current_exec->next) current_exec = current_exec->next;
		else {
			cancelTimer(2); current_exec = NULL;
		}
		break;
		
	case input:
		if ((sth = get_last_terminal()) != NULL) {
			exec_input(current_exec, atoi(sth));
			current_exec = current_exec->next;
		}
		break;
	case output:
		add_log(itoa(exec_output(current_exec->text[0]), sth_arr, 10));
		current_exec = current_exec->next;
		break;
	case condition:
		if (exec_condition(current_exec))
			current_exec = current_exec->next;
		else
			current_exec = current_exec->next_extra;
		break;
	case loop:
		if (exec_condition(current_exec))
			current_exec = current_exec->next_extra;
		else
			current_exec = current_exec->next;
		break;
	case loop_start:
		current_exec = current_exec->next;
		break;
	case loop_end:
		current_exec = current_exec->to_loop;

		break;
	}
}
