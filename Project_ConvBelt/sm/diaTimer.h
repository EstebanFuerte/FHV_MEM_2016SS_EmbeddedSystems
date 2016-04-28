

#ifndef DIATIMER_H_
#define DIATIMER_H_

#include <string>
#include <wdLib.h>

class StateMachine;

class DiaTimer {
public:
	DiaTimer(StateMachine * stateMachine);
	~DiaTimer();
	void startTimer(int eventTime);
	void timerCallback (); 
	static void timerCallbackFUNC (const timer_t timerid, const int addressParam);
	std::string timerName;

private:
	StateMachine * myStateMachine;
#ifdef POSIX
	timer_t myTimer;
#else
	WDOG_ID	timerId;
#endif
	struct itimerspec myTimerSpec;
};

#endif // DIATIMER_H_
