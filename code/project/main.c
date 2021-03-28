/******************************************/
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>

#include "imgui.h"
#include "inputbox.h"
#include "file_operate.h"
/******************************************/


#define PI 3.1415926

//declaration of functions
void display(void);
#include "draw_button.h"
#include "draw_menu.h"
#include "display.h"
#include "draw_button.h"
#include "link.h"
#include "draw_figure_extemsion.h"
#include "draw_log.h"
#include "main.h"
#include "enum.h"
#include "mousecallback.h"
double window_width, window_height;
double menu_height;
double icon_width = 0.3;
double icon_height = 0.3;

void Main() {//entrance
	SetWindowTitle("Simple algorithm flowchart generator");
	//the default size of the graphic window is(10,7);
	SetWindowSize(13, 7);
	InitGraphics();
	//init the graphic window
	window_width = GetWindowWidth();
	window_height = GetWindowHeight();
	//get the window size
	registerCharEvent(CharEventProcess);        // ×Ö·û
	registerKeyboardEvent(KeyboardEventProcess);// ¼üÅÌ
	registerMouseEvent(MouseEventProcess);      // Êó±ê
	registerTimerEvent(TimerEventProcess);
	//register the callback funtions
	//init_graph();
	//fp = fopen("abc.txt", "w");
	
	SetPenSize(1);
	WinExec("..\\project\\tip.exe", SW_HIDE);
	
}


