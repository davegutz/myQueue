#include "application.h"
#include "myQueue.h"
#include "mySubs.h"
static Queue* Q;
extern int verbose;


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
