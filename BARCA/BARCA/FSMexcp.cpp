#include "FSMexcp.h"
void alert(int errc) {
	throw alert_excp(errc);
};
void alert() {
	throw alert_excp(-2);
};
void update(int fb) {
	throw update_excp(fb);
};