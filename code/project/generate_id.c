#include "generate_id.h"
int genid = 0;
int generate_id() {
	genid++;
	return genid;
}