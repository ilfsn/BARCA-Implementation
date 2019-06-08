#include "tinyfsm.hpp"
#include "FMSList.h"
#include "FBP.h"
#include "FSMexcp.h"
#include <iostream>

class NORMAL;

class WARNING;

class COOLING;

class CLEAN : public FeedbPro {
	void entry() override {
		count = 0;
	}
	void react(FBPEvent const & e) override {
		count++;
		if (count >= z) transit<NORMAL>();
	}
};

class NORMAL : public FeedbPro {

	void react(FBPEvent const & e) override {
		if (e.pred < 0);
		else transit<WARNING>();		
	}
};

class TRAIN : public FeedbPro {
	void react(FBPEvent const & e) override {
		t++;
		if (t >= Tr) transit<NORMAL>();
		else update(-1);
	}	
};

class ALERT : public FeedbPro {
	
	void entry() override {
		alert();
	}
	
	void react(Feedback const & e) override {
		if (e.fb > 0) {
			update(e.fb);
		}
		else if (e.fb < 0) {
			transit<CLEAN>();
		}
		else if (e.fb == 0) {
			// ?
		}
	}

	void react(FBPEvent const & e) override {
		if (e.pred < 0) {
			norm = 1;
			transit<COOLING>();
		}
	}
};


class COOLING : public FeedbPro {

	void entry() override {
		norm = 0;
	}
	
	void react(FBPEvent const & e) override {
		if (e.pred < 0) {
			norm++;
			if (norm >= Gr) transit<NORMAL>();
			}
		else if (e.pred > 0) {
			norm = 0;
			transit<ALERT>();
		}
	}
};

class WARNING : public FeedbPro {

	void entry() override {
		anom = 1;
		}

	void react(FBPEvent const & e) override {
		if (e.pred < 0) {
			transit<NORMAL>();
		}
		else {
			anom++;
			if (anom >= Gr) alert(e.pred);
		}
	}

	void react(Feedback const & e) override {
		if (e.fb < 0) {
			update(e.fb);
			transit<NORMAL>();
		}
		else if (e.fb > 0) {
			update(e.fb);
			transit<ALERT>();
		}
	}
};

void FeedbPro::react(FBPEvent const &)
{
	std::cout << "Call event ignored" << std::endl;
}

void FeedbPro::react(Feedback const &)
{
	std::cout << "Call event ignored" << std::endl;
}


int FeedbPro::Gr = 30;
int FeedbPro::Tr = 256;
int FeedbPro::anom = FeedbPro::initial_value;
int FeedbPro::norm = FeedbPro::initial_value;
int FeedbPro::t = FeedbPro::initial_value;
int FeedbPro::count = FeedbPro::initial_value;
int FeedbPro::z = FeedbPro::initial_value;
// ----------------------------------------------------------------------------
// Initial state definition
//
FSM_INITIAL_STATE(FeedbPro, TRAIN)