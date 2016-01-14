#include "application.h"
#include "myQueue.h"
// class Queue
// constructors
	Queue::Queue()
	{
		front_ 		= -1;
		rear_ 		= -1;
		maxSize_ 	= MAX_SIZE;
		A_ 				= new int[maxSize_];
	}
	Queue::Queue(int front, int rear, int maxSize)
	{
		front_ 		= -1;
		rear_ 		= -1;
		maxSize_ 	= maxSize;
		A_ 				= new int[maxSize_];
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

	/*
	   Printing the elements in queue from front_ to rear_.
	   This function is only to test the code.
	   This is not a standard function for Queue implementation.
	*/
	void Queue::Print()
	{
		// Finding number of elements in queue
		int count = (rear_+maxSize_-front_)%maxSize_ + 1;
		Serial.printf("Queue       : ");
		for(int i = 0; i <count; i++)
		{
			int index = (front_+i) % maxSize_; // Index of element while travesing circularly from front_
			Serial.printf("%d ", A_[index]);
		}
		Serial.printf("\n\n");
	}
