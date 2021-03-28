#include "draw_figure_extemsion.h"//画图必须include下面这四个，第一个是我写的，后面三个是lib的
#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include <math.h>
#include "genlib.h"

void drawTriangle(double x, double y, double w, double h) {
	MovePen(x, y);
	DrawLine(w, 0);
	DrawLine(-w / 2, h);
	DrawLine(-w / 2, -h);
	return;
}

void drawParallelogram(double x1, double y1, double x2, double y2, int fillflag) {
	MovePen(x1, y1);
	if (fillflag) StartFilledRegion(1);
	DrawLine(x2 - x1 - (y1 - y2)*sqrt(3) / 3, 0);
	DrawLine((y1 - y2)*sqrt(3) / 3, y2 - y1);
	DrawLine(-x2 + x1 + (y1 - y2)*sqrt(3) / 3, 0);
	DrawLine((y2 - y1)*sqrt(3) / 3, y1 - y2);
	if (fillflag) EndFilledRegion();
}

void drawDiamond(double x1, double y1, double x2, double y2, int fillflag) {
	double mid = (x1 + x2) / 2;
	double w = (x2 - x1) / 2;
	double h = (y1 - y2) / 2;
	MovePen(mid, y1);
	if (fillflag) StartFilledRegion(1);
	DrawLine(w, -h);
	DrawLine(-w, -h);
	DrawLine(-w, h);
	DrawLine(w, h);
	if (fillflag)EndFilledRegion();
	return;
}
