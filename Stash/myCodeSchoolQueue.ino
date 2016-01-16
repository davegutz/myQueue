//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
#include "myQueue.h"
#include "SparkTime.h"
using namespace std;

//#define DISABLE_NVM
#define REFRESH_NVM

#define FLT_MAX_SIZE_LOC 	1
#define FLT_FRONT_LOC 		2
#define FLT_REAR_LOC			3

// Global variables
Queue* Q;
unsigned long currentTime;
SparkTime           rtc;                    // Time value
UDP                 UDPClient;              // Time structure

void setup()
{
	Serial.begin(9600);
	size_t length = EEPROM.length();
	Serial.printf("EElen=%d\n", length);

	// Load fault queue from eeprom to prom
	int front 	= EEPROM.read(FLT_FRONT_LOC);
	int rear  	= EEPROM.read(FLT_REAR_LOC);
	int maxSize = EEPROM.read(FLT_MAX_SIZE_LOC);
	Serial.printf("nvm front, rear, maxSize:  %d,%d,%d\n", front, rear, maxSize);  delay(4000);
	#ifndef REFRESH_NVM
	#ifndef DISABLE_NVM
		if ( maxSize==MAX_SIZE	&&					\
			front<=MAX_SIZE 	&& front>=-1 &&		 \
			rear<=MAX_SIZE  	&& rear>=-1 )
			{
				Q = new Queue(front, rear, maxSize);
				for ( uint8_t i=0; i<MAX_SIZE; i++ )
				{
					int val = EEPROM.read(i+1+FLT_REAR_LOC);
					Serial.printf("%d ", val);
					Q->loadRaw(i, val);
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

	// Time schedule convert and check
  rtc.begin(&UDPClient, "pool.ntp.org");  // Workaround - see particle.io
  rtc.setTimeZone(-5); // gmt offset

	delay(4000);
}


void loop()
{
//  Q->EnqueueOver(rtc.now(), 2002);  Q->Print();
	unsigned long currentTime = rtc.now();
  Q->EnqueueOver(currentTime, 2002);  Q->Print();
/*
  Q->EnqueueOver(4);  Q->Print();
  Q->EnqueueOver(6);  Q->Print();
  Q->Dequeue();	 Q->Print();
  Q->EnqueueOver(8);  Q->Print();
	Q->Dequeue();	 Q->Print();
	Q->Dequeue();	 Q->Print();
	Q->Dequeue();	 Q->Print();
	Q->EnqueueOver(9);  Q->Print();
	Q->EnqueueOver(10);  Q->Print();
	Q->EnqueueOver(12);  Q->Print();
	Q->EnqueueOver(13);  Q->Print();
	Q->EnqueueOver(14);  Q->Print();
	Q->EnqueueOver(15);  Q->Print();
	*/
	#ifndef DISABLE_NVM
	EEPROM.write(FLT_FRONT_LOC, Q->front());
	EEPROM.write(FLT_REAR_LOC, Q->rear());
	EEPROM.write(FLT_MAX_SIZE_LOC, Q->maxSize());
	for ( uint8_t i=0; i<MAX_SIZE; i++ )
 	{
 		EEPROM.write(i+1+FLT_REAR_LOC, Q->getRaw(i));
 	}
	#endif

	delay(2000);
}
