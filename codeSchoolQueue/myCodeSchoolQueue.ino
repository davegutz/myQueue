//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
SYSTEM_THREAD(ENABLED);        // Make sure heat system code always run regardless of network status
#include "myQueue.h"
#include "mySubs.h"
#define READ_DELAY 				10000UL 		// 10 sec code reading period
#define RESET_DELAY 			80000UL 		// 50 sec reset period
#define DISPLAY_DELAY 		30000UL 		// 10 sec code reading period
using namespace std;
extern int verbose = 1;
static Queue* F;
static Queue* I;
const int faultNVM 			= 1; 					// NVM location
const int GMT 					= -5; 				// Greenwich mean time adjustment, hrs
int impendNVM;  											// NVM locations, calculated
#define REFRESH_NVM   false						// Command to reset NVM on fresh load
#define MAX_SIZE 30  //maximum size of the array that will store Queue.
// Caution:::do not exceed about 30 for two code quees



void setup()
{
	Serial.printf("\n\n\nSetup ...\n");
	Serial.begin(9600);
	F = new Queue(MAX_SIZE, GMT);
	I = new Queue(MAX_SIZE, GMT);
	if ( !REFRESH_NVM )
	{
		impendNVM = F->loadNVM(faultNVM);
		I->loadNVM(impendNVM);
	}
	Serial.printf("setup...\n");
	if ( verbose > 1 ) F->Print();
	if ( verbose > 1 ) I->Print();
	Serial.printf("setup ending\n");
	delay(4000);
}


void loop()
{
	FaultCode newOne;
	bool 									displaying;
	bool 									reading;
	bool 									resetting;
	unsigned long 				now = millis();     // Keep track of time
	static unsigned long 	lastDisplay = 0UL;  // Last display time, ms
	static unsigned long 	lastRead  	= -READ_DELAY;  // Last read time, ms
	static unsigned long 	lastReset 	= 0UL;  // Last reset time, ms
	unsigned long faultNow = Time.now();

	reading 		= ((now-lastRead   ) >= READ_DELAY);
	if ( reading   ) lastRead = now;

	resetting 	= ((now-lastReset  ) >= RESET_DELAY);
  if ( resetting  ) lastReset  = now;

	displaying	= ((now-lastDisplay) >= DISPLAY_DELAY);
	if ( displaying ) lastDisplay = now;

	if ( reading )
	{
		F->newCode(faultNow, 2002UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2012UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->newCode(faultNow, 2004UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2014UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->newCode(faultNow, 2006UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2016UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->newCode(faultNow, 2008UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2018UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->Dequeue();	 								if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		F->Dequeue();	 								if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		F->Dequeue();	 								if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		F->newCode(faultNow, 2009UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2019UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->newCode(faultNow, 2010UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2020UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
		F->newCode(faultNow, 2011UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); F->Print();}
		I->newCode(faultNow, 2021UL); if ( verbose > 4 ) {Serial.printf("mycodeschool: "); I->Print();}
	}

	if ( resetting )
	{
		impendNVM = F->storeNVM(faultNVM);
		if ( verbose > 2 ) Serial.printf("impendNVM=%d\n", impendNVM);
		if ( impendNVM<0 || I->storeNVM(impendNVM)<0 ) Serial.printf("Failed pre-resest storeNVM\n");
		else
		{
			F->resetAll();
			I->resetAll();
		}
		impendNVM = F->storeNVM(faultNVM);
		if ( verbose > 2 ) Serial.printf("impendNVM=%d\n", impendNVM);
		if ( impendNVM<0 || I->storeNVM(impendNVM)<0 ) Serial.printf("Failed post-reset storeNVM\n");
	}

	// Display any non-reset codes and first three reset codes
	if ( displaying )
	{
		Serial.printf("Active Codes:\n");
		if ( F->printActive()>0 );
		else Serial.printf("none\n");
		Serial.printf("Active Impending:\n");
		if ( I->printActive()>0 );
		else Serial.printf("none\n");
	}

	delay(2000);
}
