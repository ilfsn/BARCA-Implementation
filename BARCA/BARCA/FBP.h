#ifndef FBP_H_INCLUDED
#define FBP_H_INCLUDED
#include "tinyfsm.hpp"

// ----------------------------------------------------------------------------
// Event declarations
//
struct FBPEvent : tinyfsm::Event
{
	int pred;
};

struct Feedback : tinyfsm::Event
{
	int fb;
};

// ----------------------------------------------------------------------------
// Feedfback Provider (FSM base class) declaration
//
class FeedbPro : public tinyfsm::Fsm<FeedbPro> {
public:

	/* default reaction for unhandled events */
	void react(tinyfsm::Event const &) { };

	virtual void react(FBPEvent const &);
	virtual void react(Feedback const &);
	//void         react(Alarm       const &);

	virtual void entry(void) { };  /* entry actions in some states */
	void         exit(void) { };  /* no exit actions at all */

protected:

	static constexpr int initial_value = 0;
	static int Gr;
	static int Tr;
	static int anom;
	static int norm;
	static int count;
	static int z;
	static int t;
};
#endif