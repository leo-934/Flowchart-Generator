#include "link.h"
#include "genlib.h"
#include "enum.h"
#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "draw_log.h"
#include <string.h>
#include "genlib.h"
#include "mousecallback.h"
#include <stdlib.h>
#include <stdio.h>
#include "generate_id.h"
#include "file_operate.h"
int insert_flag = 0;//插入标志，1表示插入next、2表示插入next_extra,当用prev和tmp也无法区分的时候用这个区分
int tra_i = 0, tra_j = -1;//to generate different GenUIID in the same line
int figure_num = 0;//链表长度

Link list;

Link figure_to_move = NULL;
Link insert_place;
Link insert_prev;

//Link find_prev(Link head, Link tmp);
void init_list(Link * head) {
	*head = NULL;
	return;
}
void insert(Link *head, Link p) {//头插
	
	p->next = *head;
	*head = p;
	
	return;
}
void insert_in(Link *head, Link figure_to_insert, Link prev, Link p) {//在p和prev之前插入figure_to_insert
	
	
	if (insert_flag == 1 || insert_flag == 0) {
		prev->next = figure_to_insert;

	}
	else if (insert_flag == 2) {
		prev->next_extra = figure_to_insert;
	}
	insert_flag = 0;
	figure_to_insert->next = p;
	if (figure_to_insert->type == condition) {
		figure_to_insert->next_extra = p;
	}
	

}
void delete(Link *head, Link p) {
	if (p->type == empty||p->type==loop_start||p->type==loop_end) {
		return;
	}
	if (*head == NULL) {
		return;
	}
	if ((*head)->next_extra == p) {
		(*head)->next_extra = p->next;
		//free_r(p);
	}
	else {
		delete(&((*head)->next_extra), p);
	}
	if ((*head)->next == p) {
		(*head)->next = p->next;
		//free_r(p);
	}
	else {
		delete(&((*head)->next), p);
	}
	return;

}
void free_r(Link p) {
	if (!p) return;
	if (p->type == loop) {
		free_r(p->next_extra);
		free(p);
	}
	else if (p->type == condition) {
		free(p);//暂时没想好如何避免删掉有用的节点
	}
	else {
		free(p);
	}
}
/*Link find_prev(Link head, Link tmp) {
	if (!head || head->type == nouse) return NULL;
	if (head->next == tmp || head->next_extra == tmp) return head;
	Link p = find_prev(head->next, tmp);
	if (p != NULL) return p;
	else return find_prev(head->next_extra, tmp);
}*/
void traverse(Link prev, Link tmp) {
	if (!tmp) return;//loop内部的线单独画
	//Link prev = NULL;
	//if(tmp!=list) prev= find_prev(list,tmp);
	double x1, y1, x2, y2;



	x1 = tmp->figure1x;
	x2 = tmp->figure2x;
	y1 = tmp->figure1y;
	y2 = tmp->figure2y;
	int text_size0 = strlen(tmp->text[0]);
	int text_size1 = strlen(tmp->text[1]);
	int text_size2 = strlen(tmp->text[2]);

	switch (tmp->type) {
	case empty:

		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;
	case input:

		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 + GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		//drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 - GetFontHeight()) / 2, "=");
		//textbox(GenUIID(tra_i), (x1 + x2 - text_size1 * 0.1) / 2, (y1 + y2 - GetFontHeight() * 3) / 2, text_size1*0.1 + 0.1, GetFontHeight(), tmp->text[1], 30);
		break;
	case output:

		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;
	case condition:

		drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 + GetFontHeight()) / 2, "IF");
		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight() * 3) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;
	case loop:

		SetPenColor("Green");
		drawLabel((x1 + x2) / 2 - 0.05, (y1 + y2 + GetFontHeight()) / 2, "WHILE");
		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight() * 3) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;
	case loop_start:

		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;
	case loop_end:

		textbox(GenUIID(tra_i), (x1 + x2 - text_size0 * 0.1) / 2, (y1 + y2 - GetFontHeight()) / 2, text_size0*0.1 + 0.1, GetFontHeight(), tmp->text[0], 30);
		break;

	}
	if (current_exec == tmp) SetPenSize(3);

	switch (tmp->kind) {
	case circle:
		MovePen(x2, (y1 + y2) / 2);
		DrawEllipticalArc((x2 - x1) / 2, (y1 - y2) / 2, 0, 360);
		break;
	case rectangle:
		drawRectangle(x1, y2, x2 - x1, y1 - y2, 0);
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
	if (current_exec == tmp) SetPenSize(1);
	if (prev) {
		if (prev->type == condition) {
			if (tmp == prev->next) {


				MovePen(prev->figure1x, (prev->figure1y + prev->figure2y) / 2);
				DrawLine(-0.3, 0);
				DrawLine((x1 + x2) / 2 - prev->figure1x + 0.3, y1 - (prev->figure1y + prev->figure2y) / 2);
				if (button(GenUIID(tra_j), ((x1 + x2) / 2 + prev->figure1x - 0.3) / 2 - 0.125, ((prev->figure1y + prev->figure2y) / 2 + y1) / 2, 0.25, 0.25, "+")) {
					insert_place = tmp;
					insert_flag = 1;
					insert_prev = prev;
					is_add_figure = TRUE;
					add_log("请点击左上角图标按钮选择图形");
				}
			}
			if (tmp == prev->next_extra) {

				MovePen(prev->figure2x, (prev->figure1y + prev->figure2y) / 2);
				DrawLine(0.3, 0);
				DrawLine((x1 + x2) / 2 - prev->figure2x - 0.3, y1 - (prev->figure1y + prev->figure2y) / 2);
				if (button(GenUIID(tra_j), ((x1 + x2) / 2 + prev->figure2x + 0.3) / 2 - 0.125, ((prev->figure1y + prev->figure2y) / 2 + y1) / 2, 0.25, 0.25, "+")) {
					insert_place = tmp;
					insert_flag = 2;
					insert_prev = prev;
					is_add_figure = TRUE;
					add_log("请点击左上角图标按钮选择图形");
				}
			}

		}
		/*else if (prev->type == loop) {
			
		}*/
		else {
			if (prev->type == loop) traverse_loop(prev);
			MovePen((prev->figure1x + prev->figure2x) / 2, prev->figure2y);
			DrawLine((x1 + x2) / 2 - (prev->figure1x + prev->figure2x) / 2, y1 - prev->figure2y);
			if (button(GenUIID(tra_j), (x1 + x2 + prev->figure1x + prev->figure2x) / 4 - 0.125, (prev->figure2y + y1) / 2, 0.25, 0.25, "+")) {
				insert_place = tmp;
				is_add_figure = TRUE;
				insert_prev = prev;
				add_log("请点击左上角图标按钮选择图形");
			}
		}
	}
	tra_i++;
	tra_j++;
	traverse(tmp, tmp->next);
	if(tmp->type!=loop)
	traverse(tmp, tmp->next_extra);
}
void traverse_loop(Link p) {
	double p1x = p->figure1x, p2x = p->figure2x, p1y = p->figure1y,p2y = p->figure2y;
	double pn1x = p->next_extra->figure1x, pn2x = p->next_extra->figure2x, pn1y = p->next_extra->figure1y, pn2y = p->next_extra->figure2y;
	
	MovePen(p2x, (p1y + p2y) / 2);
	DrawLine((pn1x + pn2x) / 2 - p2x, pn1y+0.25 - (p1y + p2y) / 2);
	DrawLine(0, -0.25);
	traverse(NULL, p->next_extra);
	p = p->next_extra;
	while (p->type != loop_end) p = p->next;
	pn1x = p->figure1x, pn2x = p->figure2x, pn1y = p->figure1y, pn2y = p->figure2y;
	MovePen(p2x, (p1y + p2y) / 2);
	DrawLine((pn1x + pn2x) / 2 - p2x, pn2y - 0.25 - (p1y + p2y) / 2);
	DrawLine(0, 0.25);
}
void init_graph() {
	init_list(&list);
	Link start = (Link)malloc(sizeof(struct node));
	start->kind = circle;
	start->figure1x = 4.5;
	start->figure1y = 6;
	start->figure2x = 5.5;
	start->figure2y = 5.5;
	start->next = NULL;
	start->next_extra = NULL;
	strcpy(start->text[0], "start");
	start->type = empty;
	Link end = (Link)malloc(sizeof(struct node));
	end->kind = circle;
	end->figure1x = 4.5;
	end->figure1y = 1;
	end->figure2x = 5.5;
	end->figure2y = 0.5;
	end->next = NULL;
	end->next_extra = NULL;
	strcpy(end->text[0], "end");
	end->type = empty;
	end->serial = generate_id();
	start->serial = generate_id();
	insert(&list, end);
	insert(&list, start);
	figure_num += 2;
}

void write_insert(Link prev,Link figure_to_insert,Link p,int insert_flag) {
	char tmp[20];
	fprintf(fp, "1 ");
	
	itoa(prev->serial, tmp,  10);
	fprintf(fp,tmp);
	fprintf(fp, " ");
	itoa(p->serial, tmp,  10);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	itoa(insert_flag, tmp, 10);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	itoa((int)figure_to_insert->kind, tmp,  10);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	itoa((int)figure_to_insert->type, tmp,  10);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	sprintf(tmp, "%.2lf", figure_to_insert->figure1x);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	sprintf(tmp, "%.2lf", figure_to_insert->figure1y);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	sprintf(tmp, "%.2lf", figure_to_insert->figure2x);
	fprintf(fp, tmp);
	fprintf(fp, " ");
	sprintf(tmp, "%.2lf", figure_to_insert->figure2y);
	fprintf(fp, tmp);
	fprintf(fp, "\n");
	fprintf(fp, figure_to_insert->text[0]);
	fprintf(fp, "\n");
	fprintf(fp, figure_to_insert->text[1]);
	fprintf(fp, "\n");
	fprintf(fp, figure_to_insert->text[2]);
	fprintf(fp, "\n");

	
	
}
void write_delete(Link p) {
	char tmp[20];
	fprintf(fp, "2 ");
	itoa(p->serial, tmp, 10);
	fprintf(fp, tmp);
	fprintf(fp, "\n");
}
Link search_serial(Link head,int serial) {
	if (!head) return NULL;
	
	if (head->serial==serial) {
		return head;
	}
	return search_serial(head->next, serial) ? search_serial(head->next, serial) : search_serial(head->next_extra, serial);
	/*
	Link tmp;
	if (tmp=search_serial(head->next, serial)) {
		return tmp;
	}
	else if(tmp=search_serial(head->next_extra, serial)) {
		return tmp;
	}
	else {
		return NULL;
	}*/

}