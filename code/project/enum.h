#pragma once
enum figure {
	circle = 1,
	rectangle,
	triangle,
	parallelogram,
	diamond
};
enum sentence {
	empty = 1,
	input,
	output,
	condition,
	loop,
	loop_start,
	loop_end
};
extern enum sentence type_choose;