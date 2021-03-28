#include "file_operate.h"
#include "genlib.h"
#include "link.h"
#include "generate_id.h"
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "draw_log.h"
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>
bool to_open_file = FALSE;
bool to_new_file = FALSE;
bool file_open = FALSE;
FILE* fp=NULL;
void open_a_file(char *str) {
	int operate_way, prev_serial, after_serial,flag, enum_kind, enum_type;
	double x1, y1, x2, y2;
	char tex[3][30];
	file_open = TRUE;
	fp = fopen(str, "a+");
	if (fp == NULL) {
		add_log("打开文件失败，文件名可能非法");
	}
	else {
		add_log("打开文件成功，若不存在则创建新文件");
	}
	genid = 0;
	init_graph();
	
	while (fscanf(fp, "%d",&operate_way) != EOF) {
		switch (operate_way) {
		case 1:
			fscanf(fp, "%d", &prev_serial);
			fscanf(fp, "%d", &after_serial);
			fscanf(fp, "%d", &insert_flag);
			fscanf(fp, "%d", &enum_kind);
			fscanf(fp, "%d", &enum_type);
			fscanf(fp, "%lf", &x1);
			fscanf(fp, "%lf",&y1);
			fscanf(fp, "%lf", &x2);
			fscanf(fp, "%lf", &y2);
			fgetc(fp);
			fgets(tex[0],30,fp);
			
			fgets(tex[1], 30, fp);
			
			fgets(tex[2], 30, fp);
			/*InitConsole();
			printf("%d", operate_way);
			printf("%d", prev_serial);
			printf("%d", after_serial);
			printf("%d", insert_flag);
			printf("%d", enum_kind);
			printf("%d", enum_type);
			printf("%lf", x1);
			printf("%lf", y1);
			printf("%lf", x2);
			printf("%lf", y2);
			printf("%d", tex[0][0]);
			printf("%s", tex[0]);
			printf("%s", tex[1]);
			printf("%s" ,tex[2]);*/
			Link tmp = (Link)malloc(sizeof(struct node));
			
			tmp->kind = enum_kind;
			tmp->figure1x = x1;
			tmp->figure1y = y1;
			tmp->figure2x = x2;
			tmp->figure2y = y2;
			//for (int i = 0; i < 3; i++) strcpy(tmp->text[i], "\0");
			tmp->type = enum_type;
			tmp->next_extra = NULL;
			switch (tmp->type) {
			case input:
				strcpy(tmp->text[0], tex[0]);
				strcpy(tmp->text[1], tex[1]);

				break;
			case output:
				strcpy(tmp->text[0], tex[0]);

				break;
			case condition:
				strcpy(tmp->text[0], tex[0]);
				break;
			case loop:
				strcpy(tmp->text[0], tex[0]);
				tmp->next_extra = (Link)malloc(sizeof(struct node));
				tmp->next_extra->kind = circle;
				tmp->next_extra->figure1x = tmp->figure2x + 1;
				tmp->next_extra->figure2x = tmp->figure2x + 2;
				tmp->next_extra->figure1y = tmp->figure1y + 0.5;
				tmp->next_extra->figure2y = tmp->figure1y;
				tmp->next_extra->type = loop_start;
				strcpy(tmp->next_extra->text[0], "loop_start");
				tmp->next_extra->next = (Link)malloc(sizeof(struct node));
				tmp->next_extra->next->kind = circle;
				tmp->next_extra->serial = generate_id();
				tmp->next_extra->next->serial = generate_id();

				tmp->next_extra->next->figure1x = tmp->figure2x + 1;
				tmp->next_extra->next->figure2x = tmp->figure2x + 2;
				tmp->next_extra->next->figure1y = tmp->figure2y;
				tmp->next_extra->next->figure2y = tmp->figure2y - 0.5;
				tmp->next_extra->next_extra = NULL;
				tmp->next_extra->next->type = loop_end;
				tmp->next_extra->next->to_loop = tmp;

				strcpy(tmp->next_extra->next->text[0], "loop_end");
				tmp->next_extra->next->next = NULL;
				tmp->next_extra->next->next_extra = NULL;

			}

			tmp->next = NULL;
			figure_num++;
			tmp->serial = generate_id();
			insert_in(&list, tmp, search_serial(list,prev_serial), search_serial(list,after_serial));
			//insert_in(&list, tmp, search_serial(list,2), search_serial(list, 1));
			break;
		case 2:

			break;
		}
	}
}

void new_a_file(char *str) {
	fp = fopen(str, "w");
	fclose(fp);
	open_a_file(str);
}

void close_a_file() {
	fclose(fp);
	init_list(&list);
}
