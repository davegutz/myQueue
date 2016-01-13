#ifndef _myQueue_h
#define _myQueue_h

#define MAX_SIZE 101  //maximum size of the array that will store Queue.

class Queue
{
private:
	int A[MAX_SIZE];
	int front, rear;
public:
	Queue(void);
	bool IsEmpty(void);
	bool IsFull(void);
	void Enqueue(int x);
	void Dequeue(void);
	int Front(void);
	void Print(void);
};

#endif
