
#include "stateTable.h"

TableEntry :: TableEntry (
	std::string _actState,
	std::string _nextState,
	std::string _myEvent,
	int _eventTime,
	void (*_action)(),
	bool (*_condition)()
) {
	actState = _actState;
	nextState = _nextState;
	actState = _actState;
	myEvent = _myEvent;
	eventTime = _eventTime;
	action = _action;
	condition = _condition;
}
