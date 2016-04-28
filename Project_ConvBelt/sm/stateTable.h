
#ifndef STATETABLE_H_
#define STATETABLE_H_

#include <string>

#define MAXDIA 9
#define MAXLINES 66

std::string actualState[MAXDIA];

std::string timerNames[MAXDIA];

int lines[MAXDIA];

int diagrams;

class TableEntry {
public:
	TableEntry(	
	std::string _actState,
	std::string _nextState,
	std::string _myEvent,
	int _eventTime,
	void (*_action)(),
	bool (*_condition)()
	);
	std::string actState;
	std::string nextState;
	std::string myEvent;
	int eventTime;
	void (*action)();
	bool (*condition)();
};

TableEntry * tab[MAXDIA][MAXLINES];

#endif // STATETABLE_H_
