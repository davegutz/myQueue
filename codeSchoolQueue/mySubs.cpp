#include "application.h"
#include "myQueue.h"
#include "mySubs.h"
static Queue* F;
static Queue* I;
extern int verbose;


// Add a fault
void newCode(const unsigned long tim, const unsigned long cod)
{
	FaultCode newOne 	= FaultCode(tim, cod, false); // false, by definition new
	FaultCode front 	= F->Front();
	FaultCode rear 		= F->Rear();
	if ( verbose > 4 )
	{
		Serial.printf("Front is ");  front.Print(); Serial.printf("\n");
		Serial.printf("Rear  is ");  rear.Print();  Serial.printf("\n");
	}
	// Queue inserts at rear (FIFO)
	if ( rear.isReset() || (newOne.time!=rear.time || newOne.code!=rear.code) )
	{
		F->EnqueueOver(newOne);
		if ( verbose > 4 ) Serial.printf("newCode:      ");
		F->Print();
	}
	else
	{
		Serial.printf("newCode already logged:  ");
		newOne.Print();
		Serial.printf("\n");
	}
}

// Add a fault
void newImpend(const unsigned long tim, const unsigned long cod)
{
	FaultCode newOne 	= FaultCode(tim, cod, false); // false, by definition new
	FaultCode front 	= I->Front();
	FaultCode rear 		= I->Rear();
	if ( verbose > 4 )
	{
		Serial.printf("Imp front is ");  front.Print(); Serial.printf("\n");
		Serial.printf("Imp rear  is ");  rear.Print();  Serial.printf("\n");
	}
	// Queue inserts at rear (FIFO)
	if ( rear.isReset() || (newOne.time!=rear.time || newOne.code!=rear.code) )
	{
		I->EnqueueOver(newOne);
		if ( verbose > 4 ) Serial.printf("newImpend:    ");
		I->Print();
	}
	else
	{
		Serial.printf("Imp already logged:  ");
		newOne.Print();
		Serial.printf("\n");
	}
}



// Load fault queue from eeprom to prom
Queue* loadNVM(const int start, const bool reset, Queue* II)
{
	int p = start;
	int front 	= EEPROM.read(p); p += sizeof(int);
	int rear  	= EEPROM.read(p); p += sizeof(int);
	int maxSize = EEPROM.read(p); p += sizeof(int);
	if ( verbose > 4 ) Serial.printf("NVM front, rear, maxSize:  %d,%d,%d\n", front, rear, maxSize);  delay(2000);
	if ( !reset )
	{
		if ( maxSize==MAX_SIZE	&&					\
		front<=MAX_SIZE 	&& front>=-1 &&		 \
		rear<=MAX_SIZE  	&& rear>=-1 )
		{
			F = new Queue(front, rear, maxSize);
			for ( uint8_t i=0; i<MAX_SIZE; i++ )
			{
				unsigned long tim;
				FaultCode fc;
				EEPROM.get(p, fc); p += sizeof(FaultCode);
				if ( verbose > 4 ) Serial.printf("%d %d %d\n", fc.time, fc.code, fc.reset);
				F->loadRaw(i, fc);
			}
		}
		else
		{
			Serial.printf("NVM test failed.   From scratch...\n");
			F = new Queue();
		}
	}
	else
	{
		Serial.printf("reset.   From scratch...\n");
		delay(1000);
		F = new Queue();
	}
	I = new Queue();
	II = I;
	return F;
}


int storeNVM(const int start)
{
	int p = start;
	EEPROM.write(p, F->front()); 		p += sizeof(int);
	EEPROM.write(p, F->rear() );		p += sizeof(int);
	EEPROM.write(p, F->maxSize());	p += sizeof(int);
	for ( uint8_t i=0; i<MAX_SIZE; i++ )
	{
		FaultCode val = F->getRaw(i);
		if ( verbose > 4 ) Serial.printf("%d %d %d\n", val.time, val.code, val.reset);
		EEPROM.put(p, val); p += sizeof(FaultCode);
	}
	return p;
}
