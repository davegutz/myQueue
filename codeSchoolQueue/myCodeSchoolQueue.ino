//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
#include "myQueue.h"
using namespace std;

//#define DISABLE_NVM

#define FLT_MAX_SIZE_LOC 	1
#define FLT_FRONT_LOC 		2
#define FLT_REAR_LOC			3

Queue* Q;

void setup()
{
	Serial.begin(9600);
	size_t length = EEPROM.length();
	Serial.printf("EElen=%d\n", length);

	// Load fault queue from eeprom to prom
	int maxSize = EEPROM.read(FLT_MAX_SIZE_LOC);
	int front 	= EEPROM.read(FLT_FRONT_LOC);
	int rear  	= EEPROM.read(FLT_REAR_LOC);
	Serial.printf("nvm=%d,%d,%d", maxSize, front, rear);delay(4000);
	#ifndef DISABLE_NVM
		if ( maxSize==MAX_SIZE	&&					\
			front<=MAX_SIZE 	&& front>=-1 &&		 \
			rear<=MAX_SIZE  	&& rear>=-1 )
			{
				Q = new Queue(front, rear, maxSize);
				for ( uint8_t i=0; i<MAX_SIZE; i++ )
				{
					int val = EEPROM.read(i+FLT_REAR_LOC);
					Q->loadRaw(i, val);
				}
			}
		else
		{
			Q = new Queue();
		}
		Q->Print();
	#else
		Q = new Queue();
  #endif

	Serial.printf("setup ending\n");
	delay(4000);
}


void loop()
{
  Q->Enqueue(2);  Q->Print();
  Q->Enqueue(4);  Q->Print();
  Q->Enqueue(6);  Q->Print();
  Q->Dequeue();	 Q->Print();
  Q->Enqueue(8);  Q->Print();
	Q->Dequeue();	 Q->Print();
	Q->Dequeue();	 Q->Print();
	Q->Dequeue();	 Q->Print();
	#ifndef DISABLE_NVM
	EEPROM.write(FLT_MAX_SIZE_LOC, Q->maxSize());
	EEPROM.write(FLT_FRONT_LOC, Q->front());
	EEPROM.write(FLT_REAR_LOC, Q->rear());
	for ( uint8_t i=0; i<MAX_SIZE; i++ )
 	{
 		EEPROM.write(i+FLT_REAR_LOC, Q->getRaw(i));
 	}
	#endif

	delay(2000);
}
