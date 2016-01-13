#include "application.h"
#include "myQueue.h"
// class Queue
// constructors
	Queue::Queue()
	{
		front = -1;
		rear = -1;
	}

// operators
// functions
	// To check wheter Queue is empty or not
	bool Queue::IsEmpty()
	{
		return (front == -1 && rear == -1);
	}

	// To check whether Queue is full or not
	bool Queue::IsFull()
	{
		return (rear+1)%MAX_SIZE == front ? true : false;
	}

	// Inserts an element in queue at rear end
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
			front = rear = 0;
		}
		else
		{
		    rear = (rear+1)%MAX_SIZE;
		}
		A[rear] = x;
	}

	// Removes an element in Queue from front end.
	void Queue::Dequeue()
	{
		Serial.printf("Dequeuing \n");
		if(IsEmpty())
		{
			Serial.printf("Error: Queue is Empty\n");
			return;
		}
		else if(front == rear )
		{
			rear = front = -1;
		}
		else
		{
			front = (front+1)%MAX_SIZE;
		}
	}
	// Returns element at front of queue.
	int Queue::Front()
	{
		if(front == -1)
		{
			Serial.printf("Error: cannot return front from empty queue\n");
			return -1;
		}
		return A[front];
	}
	/*
	   Printing the elements in queue from front to rear.
	   This function is only to test the code.
	   This is not a standard function for Queue implementation.
	*/
	void Queue::Print()
	{
		// Finding number of elements in queue
		int count = (rear+MAX_SIZE-front)%MAX_SIZE + 1;
		Serial.printf("Queue       : ");
		for(int i = 0; i <count; i++)
		{
			int index = (front+i) % MAX_SIZE; // Index of element while travesing circularly from front
			Serial.printf("%d ", A[index]);
		}
		Serial.printf("\n\n");
	}
