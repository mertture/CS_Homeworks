#include <iostream>
#include "Stack.h"
using namespace std;

//Constructor

// template type T for stacks
template <class T>
Stack<T>::Stack()
{
	top=nullptr;
}
template <class T>
StackNode<T>* Stack<T>::GetTopPointer(Stack<T> myStack)
{
	return myStack.top;
}

//Push back elements to the stack
template <class T>
void Stack<T>::push(TreeNode<T>* elmt)
{
	StackNode<T> *newNode;

	newNode = new StackNode<T>;
	newNode->value = elmt;

	if(isEmpty())
	{
		top = newNode;
		newNode->next = nullptr;
	}
	else
	{
		newNode->next = top;
		top = newNode;
	}
}
//Pop up elements from the stack
template <class T>
TreeNode<T>* Stack<T>::pop()
{
	StackNode<T> *temp;

	if(!isEmpty())
	{
		TreeNode<T>* elmt = top->value;
		temp = top->next;
		delete top;
		top = temp;
		return elmt;
	}
	return nullptr;
}

//If the stack is empty check function
template <class T>
bool Stack<T>::isEmpty()
{
	return (top==nullptr);
}
