//mycodeschool/Queue_CircularArrayImplementation.cpp
/* Queue - Circular Array implementation in C++*/
#include "application.h"
#include "myQueue.h"
using namespace std;

void setup()
{
	Serial.begin(9600);
}
void loop()
{
	Queue Q; // creating an instance of Queue.
	/*Driver Code to test the implementation
	  Printing the elements in Queue after each Enqueue or Dequeue
	*/
   Q.Enqueue(2);  Q.Print();
   Q.Enqueue(4);  Q.Print();
   Q.Enqueue(6);  Q.Print();
   Q.Dequeue();	  Q.Print();
   Q.Enqueue(8);  Q.Print();
	 delay(2000);
}
