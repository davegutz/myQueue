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
#define READ_DELAY 				60000UL 		// 1 min code reading period
#define RESET_DELAY 			300000UL 		// 5 min reset period
Queue* Q;

void setup()
{
	Serial.begin(9600);

	// Load fault queue from eeprom to prom
	int front 	= EEPROM.read(FLT_FRONT_LOC);
	int rear  	= EEPROM.read(FLT_REAR_LOC*sizeof(unsigned long));
	int maxSize = EEPROM.read(FLT_MAX_SIZE_LOC*sizeof(unsigned long));
	Serial.printf("NVM front, rear, maxSize:  %d,%d,%d\n", front, rear, maxSize);  delay(2000);
  #ifndef REFRESH_NVM
	#ifndef DISABLE_NVM
		if ( maxSize==MAX_SIZE	&&					\
			front<=MAX_SIZE 	&& front>=-1 &&		 \
			rear<=MAX_SIZE  	&& rear>=-1 )
			{
				Q = new Queue(front, rear, maxSize);
				for ( uint8_t i=0; i<MAX_SIZE; i++ )
				{
					unsigned long tim;
					EEPROM.get((i+1+FLT_MAX_SIZE_LOC)*sizeof(unsigned long), tim);
					unsigned long cod;
					EEPROM.get((i+1+FLT_MAX_SIZE_LOC+MAX_SIZE)*sizeof(unsigned long), cod);
					bool res;
					EEPROM.get((i+1+FLT_MAX_SIZE_LOC+MAX_SIZE+MAX_SIZE)*sizeof(unsigned long), res);
					Serial.printf("%d %d %d\n", tim, cod, res);
					Q->loadRaw(i, FaultCode(tim, cod, res));
				}
				Q->Print();
			}
		else
		{
			Serial.printf("from scratch...\n");
			Q = new Queue();
		}
		Q->Print();
	#else
		Serial.printf("from scratch...\n");
		Q = new Queue();
  #endif
	#else
		Serial.printf("from scratch...\n");
		delay(1000);
		Q = new Queue();
	#endif
	Q->Print();

	Serial.printf("setup ending\n");
	delay(4000);
}

// Add a fault
void newCode(const unsigned long tim, const unsigned long cod)
{
	FaultCode newOne 	= FaultCode(tim, cod, false); // false, by definition new
	FaultCode front 	= Q->Front();
	FaultCode rear 		= Q->Rear();
	Serial.printf("Front is ");  front.Print(); Serial.printf("\n");
	Serial.printf("Rear  is ");  rear.Print();  Serial.printf("\n");
  //	if ( front.isReset || !(newOne.time==front.time && newOne.code==front.code) )
	if ( front.isReset() )
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

void loop()
{
	FaultCode newOne;
	bool 					reading;
	bool 					resetting;
	unsigned long now = Time.now();
	static unsigned long 	lastRead  = -READ_DELAY;  // Last reset time, ms
	static unsigned long 	lastReset = 0UL;  // Last reset time, ms

	reading 	= ((now-lastRead) >= READ_DELAY);
	if ( reading ) lastRead = now;

	resetting = ((now-lastReset) >= RESET_DELAY);
  if ( resetting ) lastReset  = now;

		newCode(now, 2002UL);
  	Q->EnqueueOver(FaultCode(Time.now(), 2004));  Q->Print();
  	Q->EnqueueOver(FaultCode(Time.now(), 2006));  Q->Print();
  	Q->Dequeue();	 Q->Print();
		if ( reading )
		{
  	Q->EnqueueOver(FaultCode(Time.now(), 2008));  Q->Print();
		Q->Dequeue();	 Q->Print();
		Q->Dequeue();	 Q->Print();
		Q->Dequeue();	 Q->Print();
		Q->EnqueueOver(FaultCode(Time.now(), 2009));  Q->Print();
		Q->EnqueueOver(FaultCode(Time.now(), 2010));  Q->Print();
		Q->EnqueueOver(FaultCode(Time.now(), 2011));  Q->Print();
		#ifndef DISABLE_NVM
		EEPROM.write(FLT_FRONT_LOC, Q->front());
		EEPROM.write(FLT_REAR_LOC*sizeof(unsigned long), Q->rear());
		EEPROM.write(FLT_MAX_SIZE_LOC*sizeof(unsigned long), Q->maxSize());
		for ( uint8_t i=0; i<MAX_SIZE; i++ )
 		{
			FaultCode val = Q->getRaw(i);
  		Serial.printf("%d %d\n", val.time, val.code);
 			EEPROM.put((i+1+FLT_MAX_SIZE_LOC)*sizeof(unsigned long), val.time);
			EEPROM.put((i+1+FLT_MAX_SIZE_LOC+MAX_SIZE)*sizeof(unsigned long), val.code);
			EEPROM.put((i+1+FLT_MAX_SIZE_LOC+MAX_SIZE+MAX_SIZE)*sizeof(unsigned long), val.reset);
 		}
		#endif
	}
	if ( resetting )
	{
		Q->resetAll();
	}
	delay(2000);
}
