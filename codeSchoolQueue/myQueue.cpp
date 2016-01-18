#include "application.h"
#include "myQueue.h"
extern int verbose;

// class Queue
// constructors
	Queue::Queue()
	: front_(-1), rear_(-1), maxSize_(MAX_SIZE)
	{
		A_ 				= new FaultCode[maxSize_];

	}
	Queue::Queue(const int front, const int rear, const int maxSize)
	: front_(front), rear_(rear), maxSize_(maxSize)
	{
		A_ 				= new FaultCode[maxSize_];
	}

// operators

// functions
	// To check wheter Queue is empty or not
	bool Queue::IsEmpty()
	{
		return (front_ == -1 && rear_ == -1);
	}

	// To check whether Queue is full or not
	bool Queue::IsFull()
	{
		return (rear_+1)%maxSize_ == front_ ? true : false;
	}

	// Load queue from memory
	int Queue::loadRaw(const uint8_t i, const FaultCode x)
	{
		if ( i>= maxSize_ )
		{
			Serial.printf("Bad entry %d\n", i);
			return -1;
		}
		A_[i] = x;
		return 0;
	}

// Load fault queue from eeprom to prom
int Queue::loadNVM(const int start)
{
	int p = start;
	int front 	= EEPROM.read(p); p += sizeof(int);
	int rear  	= EEPROM.read(p); p += sizeof(int);
	int maxSize = EEPROM.read(p); p += sizeof(int);
	if ( verbose > 4 ) Serial.printf("Queue::loadNVM:  front, rear, maxSize:  %d,%d,%d\n", front, rear, maxSize);  delay(2000);
	if ( maxSize==MAX_SIZE	&&					\
	front<=MAX_SIZE 	&& front>=-1 &&		 \
	rear<=MAX_SIZE  	&& rear>=-1 )
	{
		for ( uint8_t i=0; i<MAX_SIZE; i++ )
		{
			unsigned long tim;
			FaultCode fc;
			EEPROM.get(p, fc); p += sizeof(FaultCode);
			if ( verbose > 4 ) Serial.printf("%d %d %d\n", fc.time, fc.code, fc.reset);
			loadRaw(i, fc);
		}
	}
	else
	{
		Serial.printf("NVM test failed.\n");
	}
	return p;
}


	// Return queue to memory
	FaultCode Queue::getRaw(const uint8_t i)
	{
		if ( i>= maxSize_ )
		{
			Serial.printf("Bad request %d\n", i);
			return FaultCode(0UL, 0UL);
		}
		return(A_[i]);
	}

	// Inserts an element in queue at rear_ end
	void Queue::Enqueue(const FaultCode x)
	{
		Serial.printf("Enqueuing %d\n", x.code);
		if(IsFull())
		{
			Serial.printf("Error: Queue is Full\n");
			return;
		}
		if (IsEmpty())
		{
			front_ = rear_ = 0;
		}
		else
		{
		    rear_ = (rear_+1)%maxSize_;
		}
		A_[rear_] = FaultCode(x);
	}

	// Inserts an element in queue at rear_ end.  Pops one off if full
	void Queue::EnqueueOver(const FaultCode x)
	{
		if ( verbose > 4 ) Serial.printf("Enqueuing %d\n", x.code);
		if(IsFull())
		{
			Queue::Dequeue();
		}
		if (IsEmpty())
		{
			front_ = rear_ = 0;
		}
		else
		{
		    rear_ = (rear_+1)%maxSize_;
		}
		A_[rear_] = x;
	}

	// Removes an element in Queue from front_ end.
	void Queue::Dequeue()
	{
		if ( verbose > 4 ) Serial.printf("Dequeuing \n");
		if(IsEmpty())
		{
			Serial.printf("Error: Queue is Empty\n");
			return;
		}
		else if(front_ == rear_ )
		{
			rear_ = front_ = -1;
		}
		else
		{
			front_ = (front_+1)%maxSize_;
		}
	}

	// Returns element at front_ of queue.
	FaultCode Queue::Front()
	{
		if(front_ == -1)
		{
			Serial.printf("Error: cannot return front_ from empty queue\n");
			return FaultCode(0UL, 0UL);
		}
		return A_[front_];
	}

	// Returns front_ value.
	int Queue::front()
	{
		return front_;
	}

	// Returns element at front_ of queue.
	FaultCode Queue::Rear()
	{
		if(rear_ == -1)
		{
			Serial.printf("Error: cannot return rear_ from empty queue\n");
			return FaultCode(0UL, 0UL);
		}
		return A_[rear_];
	}

	// Returns front_ value.
	int Queue::rear()
	{
		return rear_;
	}

	// Returns front_ value.
	int Queue::maxSize()
	{
		if(front_ == -1)
		{
			return 0;
		}
		return maxSize_;
	}


// Add a fault
void Queue::newCode(const unsigned long tim, const unsigned long cod)
{
	FaultCode newOne 	= FaultCode(tim, cod, false); // false, by definition new
	FaultCode front 	= Front();
	FaultCode rear 		= Rear();
	if ( verbose > 4 )
	{
		Serial.printf("Front is ");  front.Print(); Serial.printf("\n");
		Serial.printf("Rear  is ");  rear.Print();  Serial.printf("\n");
	}
	// Queue inserts at rear (FIFO)
	if ( rear.isReset() || (newOne.time!=rear.time || newOne.code!=rear.code) )
	{
		EnqueueOver(newOne);
		if ( verbose > 4 ) Serial.printf("newCode:      ");
		Print();
	}
	else
	{
		Serial.printf("newCode already logged:  ");
		newOne.Print();
		Serial.printf("\n");
	}
}


// Store in NVM
int Queue::storeNVM(const int start)
{
	int p = start;
	EEPROM.write(p, front_); 		p += sizeof(int);
	EEPROM.write(p, rear_ );		p += sizeof(int);
	EEPROM.write(p, maxSize_);	p += sizeof(int);
	for ( uint8_t i=0; i<MAX_SIZE; i++ )
	{
		FaultCode val = getRaw(i);
		if ( verbose > 4 ) Serial.printf("%d %d %d\n", val.time, val.code, val.reset);
		EEPROM.put(p, val); p += sizeof(FaultCode);
	}
	return p;
}



	// Print
	void Queue::Print()
	{
		//Finding number of elements in queue
		int count = (rear_+maxSize_-front_)%maxSize_ + 1;
		if ( verbose > 4 ) Serial.printf("Queue front, rear, maxSize: %d  %d  %d:", front_, rear_, maxSize_);
		for(int i = 0; i <count; i++)
		{
			int index = (front_+i) % maxSize_; // Index of element while travesing circularly from front_
			Serial.printf("| %d %d %d ", A_[index].time, A_[index].code, A_[index].reset);
		}
		Serial.printf("\n");
	}

// Reset all fault codes
int Queue::resetAll()
{
	int count = (rear_+maxSize_-front_)%maxSize_ + 1;
	for ( int i=0; i<count; i++ )
	{
		int index = (front_+i) % maxSize_; // Index of element while travesing circularly from front_
		A_[index].reset = true;
	}
}
