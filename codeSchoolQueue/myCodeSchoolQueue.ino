//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
SYSTEM_THREAD(ENABLED);        // Make sure heat system code always run regardless of network status
#include "myQueue.h"
#include "mySubs.h"
#define READ_DELAY 				10000UL 		// 10 sec code reading period
#define RESET_DELAY 			50000UL 		// 50 sec reset period
using namespace std;
extern int verbose = 5;
static Queue* F;
static Queue* I;
const int faultNVM 			= 1; 					// NVM location
int impendNVM;  											// NVM locations, calculated
#define REFRESH_NVM   false						// Command to reset NVM on fresh load


void setup()
{
	Serial.printf("\n\n\nSetup ...\n");
	Serial.begin(9600);
	F = new Queue();
	I = new Queue();
	if ( !REFRESH_NVM )
	{
		impendNVM = F->loadNVM(faultNVM);
		I->loadNVM(impendNVM);
	}
	F->Print();
	I->Print();
	Serial.printf("setup ending\n");
	delay(4000);
}


void loop()
{
	FaultCode newOne;
	bool 					reading;
	bool 					resetting;
	unsigned long now = millis();     // Keep track of time
	static unsigned long 	lastRead  = -READ_DELAY;  // Last reset time, ms
	static unsigned long 	lastReset = 0UL;  // Last reset time, ms
	unsigned long faultNow = Time.now();

	reading 	= ((now-lastRead) >= READ_DELAY);
	if ( reading ) lastRead = now;

	resetting = ((now-lastReset) >= RESET_DELAY);
  if ( resetting ) lastReset  = now;

	F->newCode(faultNow, 2002UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
	I->newCode(faultNow, 2012UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
	if ( reading )
	{
		F->newCode(faultNow, 2004UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2014UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		F->newCode(faultNow, 2006UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2016UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		F->newCode(faultNow, 2008UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2018UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		F->Dequeue();	 if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		F->Dequeue();	 if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		F->Dequeue();	 if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		F->newCode(faultNow, 2009UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2019UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		F->newCode(faultNow, 2010UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2020UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		F->newCode(faultNow, 2011UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); F->Print();
		I->newCode(faultNow, 2021UL); if ( verbose > 4 ) Serial.printf("mycodeschool: "); I->Print();
		//
		impendNVM = F->storeNVM(faultNVM);
		Serial.printf("impendNVM=%d\n", impendNVM);
		if ( impendNVM<0 || I->storeNVM(impendNVM)<0 ) Serial.printf("Failed storeNVM\n");
	}

	if ( resetting )
	{
		F->resetAll();
		I->resetAll();
	}

	delay(2000);
}
