
#define POSIX

#include "stateMachine.h"
#include <sysLib.h>

DiaTimer :: DiaTimer(StateMachine * stateMachine) {
	myStateMachine = stateMachine; // Assign pointer of parent state machnine class
#ifdef POSIX
	timer_create (CLOCK_REALTIME, NULL, &myTimer);
#else
	timerId = wdCreate(); // Create the VxWorks Watchdog timer
#endif
	return;
}

DiaTimer :: ~DiaTimer() {
#ifdef POSIX
	timer_delete (myTimer);
#else
	wdDelete(timerId); // Delete the VxWorks Watchdog timer
#endif
	return;
}

void DiaTimer :: startTimer(int eventTime) {
#ifdef POSIX
	int addressParam = (int)this;
	VOIDFUNCPTR callback = (VOIDFUNCPTR) timerCallbackFUNC;
	eventTime = eventTime - 1000/sysClkRateGet();
	long nSeconds = eventTime/1000;
	long nNanoSeconds = (eventTime % 1000)*1000000;
	myTimerSpec.it_interval.tv_sec = 0;
	myTimerSpec.it_interval.tv_nsec = 0;
	myTimerSpec.it_value.tv_sec = nSeconds;
	myTimerSpec.it_value.tv_nsec = nNanoSeconds;
	timer_connect(myTimer, (TIMER_CONNECT_FUNC) callback, addressParam);
	// added the new type-cast (TIMER_CONNECT_FUNC) due to error message in WB33!
	timer_settime(myTimer, CLOCK_REALTIME, &myTimerSpec, NULL);
#else
	wdStart(timerId, eventTime, (FUNCPTR) timerCallbackFUNC, addressParam); // Start the VxWorks Watchdog timer
#endif
	return;
}

void DiaTimer :: timerCallback () {
	myStateMachine->sendEvent(timerName); // Send event to parent state machine class
	return;
}

void DiaTimer :: timerCallbackFUNC (const timer_t timerid, const int addressParam) {
	((DiaTimer*)addressParam)->timerCallback(); // Conversion needed for C callback function :-(
	return;
}
