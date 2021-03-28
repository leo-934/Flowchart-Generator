#pragma once
#include "enum.h" 
int insert_flag;
struct node {
	enum figure kind;
	enum sentence type;
	int serial;
	double figure1x, figure1y, figure2x, figure2y;//用户画框的区域，也就是拖动和点击的触发区域
	char text[3][10];
	struct node* next;
	struct node* next_extra;
	struct node* to_loop;
};
typedef struct node* Link;
extern Link list;

extern Link figure_to_move;
extern Link insert_place;
extern Link insert_prev;
extern int tra_i, tra_j;//to generate different GenUIID in the same line
extern int figure_num;//链表长度
void init_list(Link * head);
void insert(Link *head, Link p);
void insert_in(Link *head, Link figure_to_insert, Link prev, Link p);
void delete(Link *head, Link p);
void traverse(Link prev, Link tmp);
void traverse_loop(Link p);
void free_r(Link p);
void write_insert(Link prev, Link figure_to_insert, Link p, int insert_flag);
void init_graph();
void write_delete(Link p);
Link search_serial(Link head, int serial);