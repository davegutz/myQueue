#ifndef _myQueue_h
#define _myQueue_h

#define MAX_SIZE 10  //maximum size of the array that will store Queue.

class Queue
{
private:
	int front_, rear_, maxSize_;
	unsigned long *T_;
	int *A_;
public:
	Queue(void);
	Queue(int front, int rear, int maxSize);
	Queue(const Queue & Q);
	Queue & operator=(const Queue & Q);
	~Queue(void);
	bool IsEmpty(void);
	bool IsFull(void);
	void Enqueue(int x);
	void EnqueueOver(int x);
	void EnqueueOver(const unsigned long t, const int x);
	void Dequeue(void);
	int  Front(void);
	int  Rear(void);
	void Print(void);
	int  front(void);
	int  rear(void);
	int  maxSize(void);
	int  loadRaw(uint8_t i, int x);
	int  getRaw(uint8_t i);
};

#endif
