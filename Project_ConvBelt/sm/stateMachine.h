

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <queue>
#include <semLib.h>

#include "diaTimer.h"
#include "stateTable.h"

class StateMachine {
public:
	StateMachine();
	~StateMachine();
	void sendEvent(std::string myEvent);
	void runToCompletion();
	std::string myEvent;
	DiaTimer * diaTimerTable[MAXDIA];
	char rightServerIP[20];

private:
	SEM_ID volatile semQueue, semEvent;
	void putEvent(std::string myEvent);
	std::string getEvent();
	std::queue<std::string> queue;
};

#endif // STATEMACHINE_H_
