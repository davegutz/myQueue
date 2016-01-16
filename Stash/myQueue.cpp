#include "application.h"
#include "myQueue.h"

// class Queue

// constructors
Queue::Queue()
	: front_(-1), rear_(-1), maxSize_(MAX_SIZE)
{
//	T_ 				= new unsigned long[maxSize_];
	A_ 				= new int[maxSize_];
	for ( int i=0; i<maxSize_; i++ )
	{
		T_[i] = 0UL;
	//	A_[i] = 0;
	}
}
Queue::Queue(const int front, const int rear, const int maxSize)
: front_(front), rear_(rear), maxSize_(maxSize)
{
//	T_ 				= new unsigned long[maxSize_];
	A_ 				= new int[maxSize_];
	for ( int i=0; i<maxSize_; i++ )
	{
//		T_[i] = 0UL;
		A_[i] = 0;
	}
}
Queue::Queue(const Queue & Q)
{
	front_ 		= Q.front_;
	rear_ 		= Q.rear_;
	maxSize_ 	= Q.maxSize_;
	//T_ 				= new unsigned long[maxSize_];
	A_ 			= new int[maxSize_];
	for ( int i=0; i<maxSize_; i++ )
	{
		//T_[i] = Q.T_[i];
		A_[i] = Q.A_[i];
	}
}
Queue::~Queue(){}

// operators
/*
Queue & operator=(const Queue & Q)
{
	if ( this == &Q ) return *this; // self
	front_ 		= Q.front_;
	rear_ 		= Q.rear_;
	maxSize_ 	= Q.maxSize_;
	T_ 				= new unsigned long[maxSize_];
	A_ 			= new int[maxSize_];
	for ( int i=0; i<maxSize_; i++ )
	{
		T_[i] = Q.T_[i];
		A_[i] = Q.A_[i];
	}
}
*/

// functions

// Print
void Queue::Print()
{
	// Finding number of elements in queue
	int count = (rear_+maxSize_-front_)%maxSize_ + 1;
	Serial.printf("Queue front, rear, maxSize: %d  %d  %d:", front_, rear_, maxSize_);
	for(int i = 0; i <count; i++)
	{
		int index = (front_+i) % maxSize_; // Index of element while travesing circularly from front_
		Serial.printf("%d ", A_[index]);
	}
	Serial.printf("\n\n");
}

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
int Queue::loadRaw(uint8_t i, int x)
{
	if ( i>= maxSize_ )
	{
		Serial.printf("Bad entry %d\n", i);
		return -1;
	}
	A_[i] = x;
	return 0;
}

// Return queue to memory
int Queue::getRaw(uint8_t i)
{
	if ( i>= maxSize_ )
	{
		Serial.printf("Bad request %d\n", i);
		return -1;
	}
	return(A_[i]);
}

// Inserts an element in queue at rear_ end
void Queue::Enqueue(int x)
{
	Serial.printf("Enqueuing %d\n", x);
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
	A_[rear_] = x;
}

// Inserts an element in queue at rear_ end.  Pops one off if full
void Queue::EnqueueOver(const unsigned long t, const int x)
{
	Serial.printf("Enqueuing %d, %d\n", t, x);
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
//	T_[rear_] = t;
	A_[rear_] = x;
}

// Inserts an element in queue at rear_ end.  Pops one off if full
void Queue::EnqueueOver(int x)
{
	Serial.printf("Enqueuing %d\n", x);
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
	Serial.printf("Dequeuing \n");
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
int Queue::Front()
{
	if(front_ == -1)
	{
		Serial.printf("Error: cannot return front_ from empty queue\n");
		return -1;
	}
	return A_[front_];
}

// Returns front_ value.
int Queue::front()
{
	return front_;
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
