
#include "systemManager.h"
#include "stateMachine.h"
#include "keyboard.h"
#include "myFunctions.h"


extern "C"{
	#include "../labFiles/hwFunc.h"
}

char textBuffer[88];
int speed,tempspeed,step;	// motorspeed calculation
int zeroSpeed=2048;
bool direction;				// rotation of the motor
bool formerRequest;			// to save request during operation mode

StateMachine * myStateMachine;
Keyboard * myKeyboard;

SystemManager :: SystemManager() {
	// Initialize table for all diagrams, event time in ms (POSIX)
	// The maximum size of the table is defined in stateTable.h:
	// MAXDIA = 9, MAXLINES = 66
	// Should these be exceeded, please correct!
	
	// SetMode - Doku - Abbildung 3.1
	tab[0][0] = new TableEntry ("idle","idleLocalMode","keyAIsPressed",0,myAction00,myConditionTrue);
	tab[0][1] = new TableEntry ("idle","idleChainMode","keyBIsPressed",0,myAction01,myConditionTrue);
	
	// LocalMode - Doku - Abbidlung 3.2
	tab[1][0] = new TableEntry ("idleLocalMode","idleLocalMode","keyLeftIsPressed",0,myAction10,myConditionTrue);
	tab[1][1] = new TableEntry ("idleLocalMode","idleLocalMode","keyRightIsPressed",0,myAction11,myConditionTrue);
	tab[1][2] = new TableEntry ("idleLocalMode","idleLocalMode","keyDecIsPressed",0,myAction12,myConditionTrue);
	tab[1][3] = new TableEntry ("idleLocalMode","idleLocalMode","keyIncIsPressed",0,myAction13,myConditionTrue);
	tab[1][4] = new TableEntry ("idleLocalMode","runLocalProfile","keyStartIsPressed",0,myAction14,myConditionTrue);
	tab[1][5] = new TableEntry ("runLocalProfile","idleLocalMode","Timer1",8000,myAction15,myConditionTrue);
	tab[1][6] = new TableEntry ("idleLocalMode","idleChainMode","keyBIsPressed",0,myAction16,myConditionTrue);
	
	// ChainMode - Doku - Abbidlung 3.3
	tab[2][0] = new TableEntry ("idleChainMode","slowMovement1","receiveRequest",0,myAction20,myConditionTrue);
	tab[2][1] = new TableEntry ("slowMovement1","runChainProfile","Timer2",1000,myAction21,myConditionTrue);
	tab[2][2] = new TableEntry ("runChainProfile","waitForReady","Timer2",8000,myAction22,myConditionTrue);
	tab[2][3] = new TableEntry ("waitForReady","waitForReady","receiveWait",0,myAction23,myConditionTrue);
	tab[2][4] = new TableEntry ("waitForReady","slowMovement2","receiveReady",0,myAction24,myConditionTrue);
	tab[2][5] = new TableEntry ("slowMovement2","checkFormerRequest","receiveRelease",0,myAction25,myConditionTrue);
	tab[2][6] = new TableEntry ("checkFormerRequest","slowMovement1","Timer2",2000,myAction26,myConditionFormerReqTrue);
	tab[2][7] = new TableEntry ("checkFormerRequest","idleChainMode","Timer2",2000,myAction27,myConditionFormerReqFalse);
	tab[2][8] = new TableEntry ("idleChainMode","idleLocalMode","keyAIsPressed",0,myAction28,myConditionTrue);
	// if a request arrives during the chain mode operation (Abbildung 3.3), the request should be saved and a wait msg should
	// be sent
	tab[2][9] = new TableEntry ("slowMovement1","slowMovement1","receiveRequest",0,myAction29,myConditionTrue);
	tab[2][10] = new TableEntry ("waitForReady","waitForReady","receiveRequest",0,myAction29,myConditionTrue);
	tab[2][11] = new TableEntry ("waitForReady","waitForReady","receiveRequest",0,myAction29,myConditionTrue);
	tab[2][12] = new TableEntry ("slowMovement2","slowMovement2","receiveRequest",0,myAction29,myConditionTrue);
	tab[2][13] = new TableEntry ("checkFormerRequest","checkFormerRequest","receiveRequest",0,myAction29,myConditionTrue);
	
	// Ablauf-Steuerung - Doku - Abbildung 3.4
	tab[3][0] = new TableEntry ("idleFlowControl","accelerate","startChainProfile",0,myAction30,myConditionTrue);
	tab[3][1] = new TableEntry ("idleFlowControl","accelerate","startLocalProfile",0,myAction31,myConditionTrue);
	tab[3][2] = new TableEntry ("idleFlowControl","runSlowMovement2","startSlowMovement2",0,myAction32,myConditionTrue);
	tab[3][3] = new TableEntry ("idleFlowControl","runSlowMovement1","startSlowMovement1",0,myAction33,myConditionTrue);
	tab[3][4] = new TableEntry ("runSlowMovement1","idleFlowControl","Timer3",1000,myAction34,myConditionTrue);
	tab[3][5] = new TableEntry ("runSlowMovement2","idleFlowControl","stop",0,myAction35,myConditionTrue);
	
	// runChainProfile - Doku - Abbildung 3.5
	tab[4][0] = new TableEntry ("accelerate","accelerate","Timer4",50,myAction40,myConditionTrue);
	tab[4][1] = new TableEntry ("accelerate","constantVelocity","Timer4",1000,myAction41,myConditionTrue);
	tab[4][2] = new TableEntry ("constantVelocity","decelerate","Timer4",6000,myAction42,myConditionTrue);
	tab[4][3] = new TableEntry ("decelerate","decelerate","Timer4",50,myAction43,myConditionTrue);
	tab[4][4] = new TableEntry ("decelerate","idleFlowControl","Timer4",6000,myAction44,myConditionTrue);

	tab[5][0] = new TableEntry ("checkKey","checkKey","Timer5",100,myAction50,myConditionTrue);
 
	// Init timer names for all diagrams
	timerNames[0]= "Timer1";
	timerNames[1]= "Timer2";
	timerNames[2]= "Timer3";
	timerNames[3]= "Timer4";
	timerNames[4]= "Timer5";

	// Initialize line numbers for all diagrams
	lines[0] = 2;
	lines[1] = 7;
	lines[2] = 14;
	lines[3] = 6;
	lines[4] = 5;
	lines[5] = 1;


	// Initialize first state for all diagrams
	actualState[0] = "idle";
	actualState[3] = "idleFlowControl";
	
	// Set the actual number of diagrams
	diagrams = 6;
	
	// Create instance of my Keyboard
	myKeyboard = new Keyboard;

	// Create instance of state machine
	myStateMachine = new StateMachine;

	// Start timer for each diagram which needs one in the first state!
	myStateMachine->diaTimerTable[0]->startTimer(tab[5][0]->eventTime);
	//myStateMachine->diaTimerTable[1]->startTimer(tab[1][0]->eventTime);
	
	//Display speed
	speed = 100;
	sprintf (textBuffer,"speed:           %i    ", speed); writeToDisplay (5, 20, textBuffer);
	//Display direction
	direction = false;
	sprintf (textBuffer,"direction = false (links)"); writeToDisplay (7, 20, textBuffer);
	//Display receivedMessages
	sprintf (textBuffer,"ReceivedMessages = -"); writeToDisplay (9, 20, textBuffer);
	//Display sendMessages
	sprintf (textBuffer,"sentMessages = -"); writeToDisplay (11, 20, textBuffer);
	//Display state - Förderband
	sprintf (textBuffer,"State: idle"); writeToDisplay (13, 20, textBuffer);
	sprintf (textBuffer,"Substate: -"); writeToDisplay (15, 20, textBuffer);
	//Display state - Ablaufsteuerung
	sprintf (textBuffer,"State-Ablaufsteuerung: idle"); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate-Ablaufsteuerung: -"); writeToDisplay (19, 20, textBuffer);
	
	return;
}

SystemManager :: ~SystemManager() {
	return;
}

void SystemManager :: action00(){	// Move from idle to idleLocalMode
	sprintf (textBuffer,"State: localMode"); writeToDisplay (13, 20, textBuffer);
	sprintf (textBuffer,"Substate: idleLocalMode"); writeToDisplay (15, 20, textBuffer);
	
	
	printf(" initLocalChain -> Transition00 -> runLocalChain\n\r"); 
	writeToDisplay (5,20, "State: runLocalChain          ");
	
	int value;
	if(direction == false) value=zeroSpeed-speed*zeroSpeed/2200;
	else value = zeroSpeed+speed*zeroSpeed/2200;
	
	printf("value: %i",value);
	

	writeAnalog (0, value);
	motorOn();
	
	//int rot = getRotationDirection (0);
	//sprintf (textBuffer,"direction: %i    ", rot);
	//writeToDisplay (11,20, textBuffer);
	
	return;
}

void SystemManager :: action01(){ 	// Move from idle to idleChainMode
	sprintf (textBuffer,"State: chainMode"); writeToDisplay (13, 20, textBuffer);
	sprintf (textBuffer,"Substate: idleChainMode"); writeToDisplay (15, 20, textBuffer);
	return;
}

void SystemManager :: action10(){	// LM - change direction to true
	direction = true;
	return;
}

void SystemManager :: action11(){	// LM - change direction to false
	direction = false;
	return;
}

void SystemManager :: action12(){	// LM - decrease speed
	if(speed>100)speed = speed-100;
	else speed=100;
	sprintf (textBuffer,"speed:           %i    ", speed); writeToDisplay (5, 20, textBuffer);
	return;
}

void SystemManager :: action13(){	// LM - increase speed
	if(speed<2200) speed = speed+100;
	else speed=2200;
	sprintf (textBuffer,"speed:           %i    ", speed); writeToDisplay (5, 20, textBuffer);
	return;
}

void SystemManager :: action14(){	// LM - start localProfile
	sprintf (textBuffer,"Substate: runLocalProfile"); writeToDisplay (15, 20, textBuffer);
	myStateMachine->sendEvent("startLocalProfile");
	return;
}

void SystemManager :: action15(){	// LM - from runLocalProfile to idleLocalMode
	sprintf (textBuffer,"Substate: idleChainMode"); writeToDisplay (15, 20, textBuffer);
	return;
}

void SystemManager :: action16(){	// LM - from idleLocalMode to idleChainMode
	sprintf (textBuffer,"State: chainMode                 "); writeToDisplay (13, 20, textBuffer);
	sprintf (textBuffer,"Substate: idleChainMode          "); writeToDisplay (15, 20, textBuffer);
	return;
}

void SystemManager :: action20(){	// CM - from idleChainMode to slowMovement1
	sprintf (textBuffer,"ReceivedMessages = Request       "); writeToDisplay (9, 20, textBuffer);
	sprintf (textBuffer,"SentMessages = Ready             "); writeToDisplay (11, 20, textBuffer);
	sprintf (textBuffer,"Substate: slowMovement1          "); writeToDisplay (15, 20, textBuffer);
	sprintf (textBuffer,"Substate: runSlowMovement1       "); writeToDisplay (19, 20, textBuffer);
	
	myStateMachine->sendEvent("startSlowMovement1");
	return;
}

void SystemManager :: action21(){	// CM - from slowMovement1 to runChainProfile
	sprintf (textBuffer,"Substate: runChainProfile        "); writeToDisplay (15, 20, textBuffer);
	sprintf (textBuffer,"Substate: runChainProfile        "); writeToDisplay (19, 20, textBuffer);
	
	myStateMachine->sendEvent("startChainProfile");
	return;
}

void SystemManager :: action22(){	// CM - from runChainProfile to waitForReady
	sprintf (textBuffer,"Substate: waitForReady         "); writeToDisplay (15, 20, textBuffer);
	sprintf (textBuffer,"SentMessages = Request         "); writeToDisplay (11, 20, textBuffer);
	return;
}

void SystemManager :: action23(){	// CM - from WaitForReady to WaitForReady
	sprintf (textBuffer,"ReceivedMessages = Wait"); writeToDisplay (9, 20, textBuffer);
	return;
}

void SystemManager :: action24(){	// CM -from WaitForReady to slowMovement2
	sprintf (textBuffer,"Substate: slowMovement2       "); writeToDisplay (15, 20, textBuffer);
	sprintf (textBuffer,"ReceivedMessages = Ready      "); writeToDisplay (9, 20, textBuffer);
	myStateMachine->sendEvent("startSlowMovement2");
	return;
}

void SystemManager :: action25(){	// CM - from slowMovement2 to checkFormerRequest
	sprintf (textBuffer,"ReceivedMessages = Release    "); writeToDisplay (9, 20, textBuffer);
	sprintf (textBuffer,"Substate: checkFormerRequest  "); writeToDisplay (15, 20, textBuffer);
	myStateMachine->sendEvent("stop");
	return;
}

void SystemManager :: action26(){	// CM - checkFormerRequest to slowMovement1
	sprintf (textBuffer,"Substate: slowMovement1      "); writeToDisplay (15, 20, textBuffer);
	formerRequest = false;
	myStateMachine->sendEvent("startSlowMovement1");
	return;
}

void SystemManager :: action27(){	// CM - checkFormerRequest to idleChainMode
	sprintf (textBuffer,"Substate: idleChainMode      "); writeToDisplay (15, 20, textBuffer);
	formerRequest = false;
	return;
}

void SystemManager :: action28(){	// CM - from idleChainMode to idle LocalMode
	sprintf (textBuffer,"State: localMode                 "); writeToDisplay (13, 20, textBuffer);
	sprintf (textBuffer,"Substate: idlelocalMode          "); writeToDisplay (15, 20, textBuffer);
	return;
}

void SystemManager :: action29(){	// CM - Answer Requsts during oparation
	sprintf (textBuffer,"ReceivedMessages = Request       "); writeToDisplay (9, 20, textBuffer);
	sprintf (textBuffer,"SentMessages = Wait!!!!!!!!!!!!!!"); writeToDisplay (11, 20, textBuffer);
	formerRequest = true;
	return;
}

void SystemManager :: action30(){	// Ablauf - from idleFlowControl to accelerate (runChainProfile)
	sprintf (textBuffer,"State: runChainProfile           "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: accelerate             "); writeToDisplay (19, 20, textBuffer);
	
	tempSpeed=0;
	step=90;
	motorOff();
	direction = false;
	
	return;
}

void SystemManager :: action31(){	// Ablauf - from idleFlowControl to accelerate (runlocalProfile)
	sprintf (textBuffer,"State: runlocalProfile           "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: accelerate             "); writeToDisplay (19, 20, textBuffer);
	
	tempSpeed = 0;
	step = speed/20;
	motorOff();
	return;
}

void SystemManager :: action32(){	// Ablauf - from idleFlowControl to runSlowMovement2
	sprintf (textBuffer,"State: runSlowMovement2          "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: -                      "); writeToDisplay (19, 20, textBuffer);
	
	int value = zeroSpeed+speed*zeroSpeed/2200;
	writeAnalog (0, value);
	motorOn();
	
	return;
}

void SystemManager :: action33(){	// Ablauf - from idleFlowControl to runSlowMovement1
	sprintf (textBuffer,"State: runSlowMovement1          "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: -                      "); writeToDisplay (19, 20, textBuffer);
	
	int value = zeroSpeed+speed*zeroSpeed/2200;
	writeAnalog (0, value);
	motorOn();
	
	return;
}

void SystemManager :: action34(){	// Ablauf - from runSlowMovement1 to idleFlowControl
	sprintf (textBuffer,"State: idleFlowControl           "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: -                      "); writeToDisplay (19, 20, textBuffer);
	
	motorOff();
	
	return;
}

void SystemManager :: action35(){	// Ablauf - from runSlowMovement2 to idleFlowControl
	sprintf (textBuffer,"State: idleFlowControl           "); writeToDisplay (17, 20, textBuffer);
	sprintf (textBuffer,"Substate: -                      "); writeToDisplay (19, 20, textBuffer);
	
	motorOff();
	
	return;
}

void SystemManager :: action40(){	// Ablauf - accelerate to accelerate
	sprintf (textBuffer,"Substate-Ablauf: accelerate      "); writeToDisplay (19, 20, textBuffer);
	
	tempspeed = tempspeed+step;
	int value;
	if(direction == false) value=zeroSpeed-tempspeed*zeroSpeed/2200;
	else value = zeroSpeed+tempspeed*zeroSpeed/2200;
	motorOn();
	
	return;
}

void SystemManager :: action41(){	// Ablauf - accelerate to constantVelocity
	sprintf (textBuffer,"Substate-Ablauf: constantVelocity"); writeToDisplay (19, 20, textBuffer);
	return;
}

void SystemManager :: action42(){	// Ablauf - from constantVelocity to decelerate
	sprintf (textBuffer,"Substate-Ablauf: decelerate      "); writeToDisplay (19, 20, textBuffer);
	return;
}

void SystemManager :: action43(){	// Ablauf - from constantVelocity to decelerate
	sprintf (textBuffer,"Substate-Ablauf: deccelerate     "); writeToDisplay (19, 20, textBuffer);
	
	tempspeed = tempspeed-step;
	int value;
	if(direction == false) value=zeroSpeed-tempspeed*zeroSpeed/2200;
	else value = zeroSpeed+tempspeed*zeroSpeed/2200;
	motorOn();
	
	return;
}

void SystemManager :: action44(){	// Ablauf - from decelerate to idleFlowControl
	sprintf (textBuffer,"Substate-Ablauf: idleFlowControl "); writeToDisplay (19, 20, textBuffer);
	motorOff();
	return;
}

void SystemManager :: action50(){
	char myKey;
	myKey = myKeyboard->getPressedKey();

	if (myKey == '5') {
		myStateMachine->sendEvent("keyStartIsPressed");
	}
	if (myKey == '1'){
		myStateMachine->sendEvent("keyIncIsPressed");

	}
	if (myKey == '2'){
		myStateMachine->sendEvent("keyDecIsPressed");

	}
	if (myKey == '3'){
		myStateMachine->sendEvent("keyLeftIsPressed");
	}
	if (myKey == '4'){
		myStateMachine->sendEvent("keyRightIsPressed");
	}
	if (myKey == 'B'){
		myStateMachine->sendEvent("keyBIsPressed");
	}
	if (myKey == 'A'){
		myStateMachine->sendEvent("keyAIsPressed");
	}
	
	//for debuging only
	if (myKey == '6')	myStateMachine->sendEvent("receiveRequest");
	if (myKey == '7')	myStateMachine->sendEvent("receiveWait");
	if (myKey == '8')	myStateMachine->sendEvent("receiveReady");
	if (myKey == '9')	myStateMachine->sendEvent("receiveRelease");
	
	return;
}

bool SystemManager :: conditionTrue(){
	return TRUE;
}

bool SystemManager :: conditionFormerReqTrue(){
	if (formerRequest==true){
		return true;
	}
	else return false;
}

bool SystemManager :: conditionFormerReqFalse(){
	if (formerRequest==false){
		return true;
	}
	else return false;
}
