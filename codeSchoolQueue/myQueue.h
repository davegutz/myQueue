#ifndef _myQueue_h
#define _myQueue_h

#define MAX_SIZE 10  //maximum size of the array that will store Queue.

class FaultCode
{
public:
	unsigned long time;
	unsigned long code;
	FaultCode(void)
	{
		time = 0UL;
		code = 0UL;
	}
	FaultCode(unsigned long tim, unsigned long cod)
	{
		time = tim;
		code = cod;
	}
	FaultCode (const FaultCode & FC)
	{
		time = FC.time;
		code = FC.code;
	}
	FaultCode & operator=(const FaultCode & FC)
	{
		if ( this  == &FC ) return *this; // self-assignment
		time = FC.time;
		code = FC.code;
	}
	~FaultCode(){}
};


class Queue
{
private:
	int front_, rear_, maxSize_;
	FaultCode *A_;
public:
	Queue(void);
	Queue(const int front, const int rear, const int maxSize);
	bool IsEmpty(void);
	bool IsFull(void);
	void Enqueue(const FaultCode x);
	void EnqueueOver(const FaultCode x);
	void Dequeue(void);
	FaultCode  Front(void);
	FaultCode  Rear(void);
	void Print(void);
	int  front(void);
	int  rear(void);
	int  maxSize(void);
	int  loadRaw(const uint8_t i, const FaultCode x);
	FaultCode  getRaw(const uint8_t i);
};

#endif
