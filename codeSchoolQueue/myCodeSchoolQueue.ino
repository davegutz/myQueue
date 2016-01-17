//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
SYSTEM_THREAD(ENABLED);        // Make sure heat system code always run regardless of network status
#include "myQueue.h"
using namespace std;

//#define DISABLE_NVM
//#define REFRESH_NVM

#define FLT_FRONT_LOC 		1
#define FLT_REAR_LOC			2
#define FLT_MAX_SIZE_LOC 	3
#define READ_DELAY 				10000UL 		// 1 min code reading period
#define RESET_DELAY 			50000UL 		// 5 min reset period
Queue* Q;
extern const int verbose = 1;

// Load fault queue from eeprom to prom
void loadNVM(const bool reset)
{
int front 	= EEPROM.read(FLT_FRONT_LOC);
int rear  	= EEPROM.read(FLT_REAR_LOC*sizeof(unsigned long));
int maxSize = EEPROM.read(FLT_MAX_SIZE_LOC*sizeof(unsigned long));
if ( verbose > 4 ) Serial.printf("NVM front, rear, maxSize:  %d,%d,%d\n", front, rear, maxSize);  delay(2000);
if ( !reset )
{
	#ifndef DISABLE_NVM
		if ( maxSize==MAX_SIZE	&&					\
		front<=MAX_SIZE 	&& front>=-1 &&		 \
		rear<=MAX_SIZE  	&& rear>=-1 )
		{
			Q = new Queue(front, rear, maxSize);
			for ( uint8_t i=0; i<MAX_SIZE; i++ )
			{
				unsigned long tim;
				FaultCode fc;
				EEPROM.get((i+1+FLT_MAX_SIZE_LOC)*sizeof(FaultCode), fc);
				if ( verbose > 4 ) Serial.printf("%d %d %d\n", fc.time, fc.code, fc.reset);
				Q->loadRaw(i, fc);
			}
		}
		else
		{
			Serial.printf("from scratch...\n");
			Q = new Queue();
		}
	#else
		Serial.printf("from scratch...\n");
		Q = new Queue();
	#endif
	}
	else
	{
		Serial.printf("from scratch...\n");
		delay(1000);
		Q = new Queue();
	}
}


// Add a fault
void newCode(const unsigned long tim, const unsigned long cod)
{
	FaultCode newOne 	= FaultCode(tim, cod, false); // false, by definition new
	FaultCode front 	= Q->Front();
	FaultCode rear 		= Q->Rear();
	if ( verbose > 4 )
	{
		Serial.printf("Front is ");  front.Print(); Serial.printf("\n");
		Serial.printf("Rear  is ");  rear.Print();  Serial.printf("\n");
	}
	// Queue inserts at rear (FIFO)
  //	if ( rear.isReset || !(newOne.time==rear.time && newOne.code==rear.code) )
	if ( rear.isReset() || (newOne.time!=rear.time || newOne.code!=rear.code) )
	{
		Q->EnqueueOver(newOne);
		Q->Print();
	}
	else
	{
		Serial.printf("Already logged:  ");
		newOne.Print();
		Serial.printf("\n");
	}
}



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
