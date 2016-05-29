#include "controller.h"
#include "taskLib.h"
#include "TaskManager.h"

extern "C"{
#include "ert_main.h"
#include "../labFiles/hwFunc.h"
}

Controller :: Controller(){			// Konstruktor zum Speicher reservieren
	return;
}

Controller :: ~Controller(){		// Dekonstruktor zum Speicher freigeben
	return;
}

void Controller :: init(){
	motorOn();						// default Motor-Staus = Ein; wenn ein 
									// Stillstand gewünscht ist, dann wird die 
									// Geschwindigkeit auf 0 geregelt.
	getEncoderPulsesZeroCorrected();
	//ert_main task spawnen
	taskSpawn("ctr",105,0,0x1000, (FUNCPTR) Subsystem_main,110,0,0,0,0,0,0,0,0,0);
	this->wsoll = 0;
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
	//double intervalTime = 1/16.0;
	int dir = getRotationDirection();
	
	// ToDo: Richtungsabhängigkeit bezüglich e = wsoll - wist
	// wist kann negativ sein und somit wird es addiert anstatt subtrahiert??
	
	double pulses = getEncoderPulsesZeroCorrected();
	double rounds = pulses/pulsesPerRound;
	//double wist = dir*rounds/intervalTime*60.0;			// [U/min]
	double wist = dir*rounds*60.0;			// [U/min]
	
	return (this->wsoll - actSpeed);
}

extern "C"
{
	double getErrorC(){
		return mySystemManger->myController->getError();
	}
}

