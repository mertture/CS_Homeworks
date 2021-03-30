#include <iostream>
using namespace std;

template <typename T>
class Stack{
public:
 Stack();
 void push(T);
 void pop();
 bool isEmpty() const;
 T topAndpop();
 const T & top() const;

private:
	struct ListNode {
		T object;
		ListNode *next;

		ListNode( const T & value, ListNode * n = NULL )
                              : object(value), next(n) {}
		};
		ListNode *topStack;
};

template <typename T>
Stack<T>::Stack(){
  topStack = NULL;
 }

template <typename T>
void Stack<T>::push(T x){
  
  topStack = new ListNode(x, topStack);
}

template <typename T>
void Stack<T>::pop(){
 if (isEmpty()){
  cout << "Stack is Empty" << endl;
 }

 else{
  ListNode* oldTop = topStack;
  topStack = topStack->next;
  delete oldTop;
 }
}

template<class T>
const T & Stack<T>::top() const {
	
	return topStack->object;
}

template <typename T>
T Stack<T>::topAndpop() {
	T topItem = top();
	pop();
	return topItem;
}


template <typename T>
bool Stack<T>::isEmpty() const{
 return topStack == NULL;
}
