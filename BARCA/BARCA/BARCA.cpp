// BARCA.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DEADLOCK = 2 - LIVELOCK = 3 - THRASHING = 4 - MEMORY LEAKED = 5

#include <iostream>
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include "FMSList.h"
#include "FSMexcp.h"
#include "BE.h"
#include "BID.h"



int main()
{

	fsm_list::start();
	FBPEvent event;
	Feedback feedback;


	BehEx BE(12, 1, 3);
	two_step BID(0.1, 0.25, 0.2, 0.25, 0.005, 48);
	int pred;
	int vt = 0;
	while (pred = BID.predict(BE.read_metric()))
	{	
		try {
			event.pred = pred;
			send_event(event);
		}
		catch (alert_excp al) {
			std::cout <<std::endl<< "Anomalous behaviors detected: ";
			switch (al.behavior)
			{
			case -2:
				std::cout << " ALERT!" << std::endl;
			case 2:
				std::cout << " DEADLOCK" << std::endl;
			case 3:
				std::cout << " LIVELOCK" << std::endl;
			case 4:
				std::cout << " THRASHING" << std::endl;
			case 5:
				std::cout << " MEMORY LEAKED" << std::endl;			
			}
			std::cout << std::endl << "DEADLOCK = 2 - LIVELOCK = 3 - THRASHING = 4 - MEMORY LEAKED = 5 - WRONG ALERT = -1" << std::endl;
			std::cout << "Feedback provide: ";
			int fb;
			std::cin >> fb;
			try {
				feedback.fb = fb;
				send_event(feedback);
			}
			catch (update_excp upd) {
				BID.update(upd.cfm_bev);
			}
		}
		catch (update_excp upd) {
			BID.update(upd.cfm_bev);
		}
		vt++;
		std::cout << std::endl << vt << "th scan." << std::endl;
	}


	return (0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
