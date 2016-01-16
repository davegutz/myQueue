#include "application.h"
#include "myQueue.h"
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
		Serial.printf("Enqueuing %d\n", x.code);
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

	// Print
	void Queue::Print()
	{
		//Finding number of elements in queue
		int count = (rear_+maxSize_-front_)%maxSize_ + 1;
		Serial.printf("Queue front, rear, maxSize: %d  %d  %d:", front_, rear_, maxSize_);
		for(int i = 0; i <count; i++)
		{
			int index = (front_+i) % maxSize_; // Index of element while travesing circularly from front_
			Serial.printf("%d ", A_[index].code);
		}
		Serial.printf("\n\n");
	}
