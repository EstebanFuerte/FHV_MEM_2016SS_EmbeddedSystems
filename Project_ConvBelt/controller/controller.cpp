#include "controller.h"
#include "taskLib.h"
#include "../sm/SystemManager.h"

extern "C"{
	#include "ert_main.h"
	#include "../labFiles/hwFunc.h"
}

double error;
int prevDir = 0;
int cntDir = 0;


SystemManager * mySystemManager;


Controller :: Controller(){			// Konstruktor zum Speicher reservieren
	return;
}

Controller :: ~Controller(){		// Dekonstruktor zum Speicher freigeben
	return;
}

void Controller :: init(){
	this->wsoll = 0;
	motorOn();					// default Motor-Staus = Ein; wenn ein 
									// Stillstand gewünscht ist, dann wird die 
									// Geschwindigkeit auf 0 geregelt.
	getEncoderPulsesZeroCorrected();
	
	printf("wsoll: %f\n\r",this->wsoll);
		
	//ert_main task spawnen
	taskSpawn("ctr",105,0,0x1000, (FUNCPTR) Subsystem_main,110,0,0,0,0,0,0,0,0,0);
	
}

void Controller :: setSpeed(double speed, bool direction){
	if(direction) {
		this->wsoll = speed*(-1.0);
		//printf("direction==true");
	}
	else this->wsoll = speed;
	
	//print only if wsoll new value
	/*
	if (this->wsoll != this->wsoll_old){
		printf("----ws:%.1f\n",this->wsoll);
	}*/
	
	return;
}

double Controller :: getRefSpeed(){
	return this->wsoll;
}

double Controller :: getError(){
	double pulsesPerRound = 64.0;
	int dir = getRotationDirection(0);
	
	if((prevDir!=0) && (dir != prevDir) && (cntDir < 1))
	{
		cntDir++;
		prevDir = dir;
		dir = dir * (-1);
	}
	else
	{
		cntDir = 0;
		prevDir = dir;
	}
	
	double pulses = getEncoderPulsesZeroCorrected();
	//printf("pul=%i\n",pulses);
	
	double rounds = pulses;		// [U/sec]		
	double wist = dir*rounds*60.0;						// [U/min]
	//if (wist != 0) printf("wi=%.1f\n",wist);

	double error;
	error= this->wsoll-wist;
	//if (error !=0)printf("e=%.3f\n",error);
	
	return error;
}


double getErrorC(){
	double myX;
	myX = mySystemManager->getError();
	//printf("f=%.1f\n\r",myX);
	return myX;
}


