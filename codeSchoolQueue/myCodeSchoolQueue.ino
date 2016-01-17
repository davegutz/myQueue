//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
SYSTEM_THREAD(ENABLED);        // Make sure heat system code always run regardless of network status
#include "myQueue.h"
#include "mySubs.h"
#define READ_DELAY 				10000UL 		// 1 min code reading period
#define RESET_DELAY 			50000UL 		// 5 min reset period
using namespace std;
extern const int verbose = 1;
static Queue* Q;

//#define DISABLE_NVM
//#define REFRESH_NVM


void setup()
{
	Serial.begin(9600);
	loadNVM(true);
	Q->Print();
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

	newCode(faultNow, 2002UL); Q->Print();
	if ( reading )
	{
		newCode(faultNow, 2004UL); Q->Print();
		newCode(faultNow, 2006UL); Q->Print();
		newCode(faultNow, 2008UL); Q->Print();
		Q->Dequeue();	 Q->Print();
		Q->Dequeue();	 Q->Print();
		Q->Dequeue();	 Q->Print();
		newCode(faultNow, 2009UL); Q->Print();
		newCode(faultNow, 2010UL); Q->Print();
		newCode(faultNow, 2011UL); Q->Print();
		#ifndef DISABLE_NVM
		EEPROM.write(FLT_FRONT_LOC, Q->front());
		EEPROM.write(FLT_REAR_LOC*sizeof(unsigned long), Q->rear());
		EEPROM.write(FLT_MAX_SIZE_LOC*sizeof(unsigned long), Q->maxSize());
		for ( uint8_t i=0; i<MAX_SIZE; i++ )
 		{
			FaultCode val = Q->getRaw(i);
  		if ( verbose > 4 ) Serial.printf("%d %d %d\n", val.time, val.code, val.reset);
			EEPROM.put((i+1+FLT_MAX_SIZE_LOC)*sizeof(FaultCode), val);
 		}
		#endif
	}
	if ( resetting )
	{
		Q->resetAll();
	}
	delay(2000);
}
