
#include "stateMachine.h"

StateMachine :: StateMachine() {
	// Create the semaphores needed
	semQueue = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	semEvent = semCCreate(SEM_Q_PRIORITY, 0);
	int diagramNo;
	for (diagramNo = 0; diagramNo < diagrams; diagramNo++) {
		diaTimerTable[diagramNo] = new DiaTimer(this); // Create one timer object for each diagram
		diaTimerTable[diagramNo]->timerName = timerNames[diagramNo]; // Assign numbers to timer objects
		}
	return;
}

StateMachine :: ~StateMachine() {
	int diagramNo;
	for (diagramNo = 0; diagramNo < diagrams; diagramNo++) {
		delete diaTimerTable[diagramNo]; // Delete the existing timer objects
		}
	return;
}

void StateMachine :: sendEvent(std::string myEvent) {
	putEvent (myEvent); // Set the event in the queue of this object
	semGive (semEvent); // Signal the arrival of the event through counting semaphore
	return;
}

void StateMachine :: putEvent(std::string myEvent) {
	semTake(semQueue,WAIT_FOREVER);
	queue.push(myEvent);
	semGive(semQueue);
	return;
}

std::string StateMachine :: getEvent() {
	std::string myString;
	semTake(semQueue,WAIT_FOREVER);
	if (queue.empty() == false) {
		myString = queue.front();
		queue.pop();
	}
	semGive(semQueue);
	return myString;
}

void StateMachine :: runToCompletion() {
	std::string actualEvent;
	int d,i,j;
	while (TRUE) {
		semTake (semEvent, WAIT_FOREVER);
		actualEvent = getEvent(); // Get the event from the queue
//		printf(actualEvent.c_str()); // For debug purposes only
		for (d = 0; d < diagrams; d++) {
			for (i = 0; i < lines[d]; i++) {
				if ((actualState[d] == tab[d][i]->actState) && 
						(actualEvent == tab[d][i]->myEvent) && 
						((*tab[d][i]->condition)() == TRUE)) { // Call the condition function which returns bool
					actualState[d] = tab[d][i]->nextState;
					(*tab[d][i]->action)(); //Call the funtion defined by pointer action
					for (j = 0; j < lines[d]; j++) {
						if ((actualState[d] == tab[d][j]->actState) && 
								(tab[d][j]->myEvent == (diaTimerTable[d]->timerName))) {
							diaTimerTable[d]->startTimer(tab[d][j]->eventTime); // Look for timers to start next
							break;
						}
					}
					break;
				}
			}
		}
	}
}
