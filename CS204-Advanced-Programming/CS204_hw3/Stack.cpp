#include <iostream>
#include "Stack.h"
using namespace std;

//Constructor
Stack::Stack()
{
	top=nullptr;
}

StackNode* Stack::GetTopPointer(Stack myStack)
{
	return myStack.top;
}

//Push back elements to the stack
void Stack::push(TreeNode* elmt)
{
	StackNode *newNode;

	newNode = new StackNode;
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
TreeNode* Stack::pop()
{
	StackNode *temp;

	if(!isEmpty())
	{
		TreeNode* elmt = top->value;
		temp = top->next;
		delete top;
		top = temp;
		return elmt;
	}
	return nullptr;
}

//If the stack is empty check function
bool Stack::isEmpty()
{
	return (top==nullptr);
}
