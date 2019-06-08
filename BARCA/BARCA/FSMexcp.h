#ifndef FMSEXCP_H_INCLUDED
#define FMSEXCP_H_INCLUDED
#include <exception>
class alert_excp : public std::exception {
public:
	alert_excp(int errc) : behavior(errc) {};
	int behavior;
};

class update_excp : public std::exception {
public:
	update_excp(int fb) : cfm_bev(fb) {};
	int cfm_bev;
};

void alert(int errc);
void alert();
void update(int fb);
#endif