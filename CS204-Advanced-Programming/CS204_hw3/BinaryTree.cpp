#include "BinaryTree.h"
#include <iostream>

using namespace std;

//Constructor
BinaryTree::BinaryTree(){
  root = nullptr;
}

//Copy Constructor
BinaryTree::BinaryTree(const BinaryTree& rhs){
  //TODO: Implement deep copy constructor
  //You may make use of Iterator class
}

//Destructor
BinaryTree::~BinaryTree(){
  //TODO: Implement destructor
}

void BinaryTree::insert(int num) {
  //TODO: Implement insert function for BinaryTree
}

//Operators
//Give the implementations of operators in here either as free function or member function


/*
 *  ITERATOR CLASS
 */

//Constructor
Iterator::Iterator()
  :myCurrent(nullptr), stack(nullptr)
{}

void Iterator::Init(const BinaryTree &tree)
{
  myCurrent = tree.root;
  stack = new Stack();
}

//Returns true if tree has more nodes to explore
bool Iterator::hasNext()
{
  //TODO: Implement a function to see if there is more nodes to iterate over.
}

//Next node to explore
TreeNode* Iterator::Next()
{
  //TODO: Implement next function of the iterator
  //Note that it returns the next element
  return node;
}


