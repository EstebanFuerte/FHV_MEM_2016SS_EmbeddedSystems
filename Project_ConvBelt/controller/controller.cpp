#include "controller.h"
#include "taskLib.h"
#include "../sm/SystemManager.h"

extern "C"{
	#include "ert_main.h"
	#include "../labFiles/hwFunc.h"
}

double error;

SystemManager * mySystemManager;


Controller :: Controller(){			// Konstruktor zum Speicher reservieren
	return;
}

Controller :: ~Controller(){		// Dekonstruktor zum Speicher freigeben
	return;
}

void Controller :: init(){
	motorOn();					// default Motor-Staus = Ein; wenn ein 
									// Stillstand gewünscht ist, dann wird die 
									// Geschwindigkeit auf 0 geregelt.
	getEncoderPulsesZeroCorrected();
	this->wsoll = 0;
	printf("wsoll: %f\n\r",this->wsoll);
		
	//ert_main task spawnen
	taskSpawn("ctr",105,0,0x1000, (FUNCPTR) Subsystem_main,110,0,0,0,0,0,0,0,0,0);
	
}

void Controller :: setSpeed(double speed){
	this->wsoll = speed;
	return;
}

double Controller :: getRefSpeed(){
	return this->wsoll;
}

double Controller :: getError(){
	double pulsesPerRound = 64.0;
	int dir = getRotationDirection(0);
	
	// ToDo: Richtungsabhängigkeit bezüglich e = wsoll - wist
	// wist kann negativ sein und somit wird es addiert anstatt subtrahiert??
	
	double pulses = getEncoderPulsesZeroCorrected();
	double rounds = pulses/pulsesPerRound/0.01565;		// [U/sec]		
	double wist = dir*rounds*60.0;						// [U/min]
	printf("wi=%f\n\r",wist);
	printf("ws=%f\n\r",this->wsoll);
	double error = this->wsoll-wist;
	printf("e=%.1f\n\r",error);
	
	return error;
}


double getErrorC(){
	double myX;
	myX = mySystemManager->getError();
	//printf("f=%.1f\n\r",myX);
	return myX;
}


