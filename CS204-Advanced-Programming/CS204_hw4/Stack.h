#ifndef _STACK_H
#define _STACK_H

#include <iostream>
using namespace std;

//both treenode and stacknode structs are maden templates
template <class T>
struct TreeNode
{
  T value;
  TreeNode *left;
  TreeNode *right;

  TreeNode(){
    left = nullptr;
    right = nullptr;
  }
};
template <class T>
struct StackNode
{
  TreeNode<T>* value;
  StackNode *next;
};

// Also, stack class is maden template too
template <class T>
class Stack
{
private:
  StackNode<T> *top;
  StackNode<T> *GetTopPointer(Stack myStack);
  
public:
  Stack(void);
  void push(TreeNode<T>*);
  TreeNode<T>* pop();
  bool isEmpty();
};

// I have to define it here for Fundemental Dilemma and I removed both binarytree.cpp and stack.cpp
#include "Stack.cpp"
#endif
