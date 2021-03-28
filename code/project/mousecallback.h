#pragma once
#include "link.h"
#include "genlib.h"

extern bool is_exec;
extern Link current_exec;
extern bool is_create_circle;//等待创建圆形模式
extern bool is_draw_circle;//是否正在画圆
extern bool is_create_rectangle;
extern bool is_draw_rectangle;
extern bool is_create_triangle;
extern bool is_draw_triangle;
extern bool is_create_parallelogram;
extern bool is_draw_parallelogram;
extern bool is_create_diamond;
extern bool is_draw_diamond;
extern bool is_move_figure;//是否正在移动图形
extern bool is_add_figure;//确定用户画图之前是否点击加号，没点不能画
extern double circle1x, circle1y, circle2x, circle2y;//circle1是用户画圆时候的左上角，circle2是右下角figure
extern double rectangle1x, rectangle1y, rectangle2x, rectangle2y;
extern double triangle1x, triangle1y, triangle2x, triangle2y;
extern double parallelogram1x, parallelogram1y, parallelogram2x, parallelogram2y;
extern double diamond1x, diamond1y, diamond2x, diamond2y;
extern int variable_num;
struct variable {
	char name[30];
	int num;
};
extern struct variable input_data[100];
void read_sentence(Link p);
void MouseEventProcess(int x, int y, int button, int event);
bool is_in_a_figure(Link head, double x, double y);
bool in_box(double x0, double y0, double x1, double x2, double y1, double y2);

void CharEventProcess(char ch);
void KeyboardEventProcess(int key, int event);
void TimerEventProcess(int timerID);