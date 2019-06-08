#ifndef FMSLIST_H_INCLUDED
#define FMSLIST_H_INCLUDED
#include "tinyfsm.hpp"
#include "FBP.h"

using fsm_list = tinyfsm::FsmList<FeedbPro>;

/** dispatch event to "FeedbPro" */
template<typename E>
void send_event(E const & event)
{
	fsm_list::template dispatch<E>(event);
}
#endif