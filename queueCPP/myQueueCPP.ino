// rajabishek/queue.cpp
#include "application.h"
using namespace std;

template <class datatype>
class QueueInterface{
public:
	virtual void enqueue(datatype data) = 0;
	virtual void dequeue() = 0;
	virtual bool isEmpty() = 0;
	virtual bool isFull(){
		return true;
	};
	virtual void display() = 0;
	virtual datatype getFront() = 0;
};

template <class datatype>
class QueueAsArray:public QueueInterface<datatype>{
	int front;
	int rear;
	int size;
	datatype* array;
public:
	QueueAsArray(int size){
		front = rear = -1;
		this->size = size;
		array = new datatype[size];
	}

	void enqueue(datatype data){
		if(isEmpty()){
			front = 0;
		}

		if(!isFull()){
			++rear;
			array[rear] = data;
		}
		else
			Serial.printf("The array is full, cannot add %f\n", data);
	}

	datatype getFront(){
		if(!isEmpty())
			return array[front];
		else
			Serial.printf("The queue is empty...\n");
		return -1;
	}

	void dequeue(){
		++front;
		if(front > rear)
			rear = front = -1;
	}

	void display(){
		if(!isEmpty()){
			for(int i=front;i<=rear;++i){
				if(i!=rear)
					Serial.printf("%f | ", array[i]);
				else
					Serial.printf("%f\n", array[i]);
			}
		}
		else
			Serial.printf("The queue is empty...\n");
	}

	bool isFull(){
		if(rear == size - 1)
			return true;
		return false;
	}

	bool isEmpty(){
		if(rear == -1 && front == -1)
			return true;
		return false;
	}

};

template <class datatype>
class QueueAsLinkedlist:public QueueInterface<datatype>{
	datatype data;
	QueueAsLinkedlist* head;
	QueueAsLinkedlist* tail;
	QueueAsLinkedlist* next;
public:
	QueueAsLinkedlist(){
		head = NULL;
	}

	void enqueue(datatype data){
		QueueAsLinkedlist<datatype>* newNode = new QueueAsLinkedlist<datatype>();
		newNode->data = data;
		newNode->next = NULL;
		if(isEmpty()){
			head = newNode;
			tail = newNode;
		}
		else{
			tail->next = newNode;
			tail = newNode;
		}
	}

	datatype getFront(){
		if(!isEmpty()){
			return head->data;
		}
		else
			Serial.printf("The queue is empty...\n");
		return -1;
	}

	void dequeue(){
		if(!isEmpty()){
			QueueAsLinkedlist* temp = head;
			head = head->next;
			delete temp;
		}
		else
			Serial.printf("The queue is empty...\n");
	}

	void display(){
		if(!isEmpty()){
			QueueAsLinkedlist* temp = head;
			while(temp != NULL){
				if(temp->next != NULL)
					Serial.printf("%f | ", temp->data);
				else
					Serial.printf("%f\n", temp->data);
				temp = temp->next;
			}
		}
		else
			Serial.printf("The queue is empty...\n");
	}

	bool isEmpty(){
		if(head == NULL)
			return true;
		return false;
	}

};

template <class datatype>
class QueueFacade{
public:
	static QueueInterface<datatype>* getInstance(string type = "linkedlist",int size = 4){
		if(type == "array")
			return new QueueAsArray<datatype>(size);
		else
			return new QueueAsLinkedlist<datatype>();
	}
};
void setup()
{
  Serial.begin(9600);
}

void loop()
{
	QueueInterface<int>* queue = QueueFacade<int>::getInstance();
	Serial.printf("Enqueing 3...\n");queue->enqueue(3);
	Serial.printf("Enqueing 1...\n");queue->enqueue(1);
	Serial.printf("Enqueing 2...\n");queue->enqueue(2);
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Enqueing 5...\n");queue->enqueue(5);
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Enqueing 9...\n");queue->enqueue(9);
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Dequeing...\n");queue->dequeue();
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Dequeing...\n");queue->dequeue();
	Serial.printf("Dequeing...\n");queue->dequeue();
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Dequeing...\n");queue->dequeue();
	Serial.printf("Contents: \n");queue->display();
	Serial.printf("Dequeing...\n");queue->dequeue();
	Serial.printf("Contents: \n");queue->display();
  delay(2000);
}
