#include "BinaryTree.h"
#include <iostream>
#include <string>
using namespace std;


//Before, it was only for integers, I changed integers to T
// Also, added T to functions which needed templates
// The functions were from Iterator, BinaryTree and Stack class
//Constructor
template <class T>
BinaryTree<T>::BinaryTree(){
  root = nullptr;
}

//Copy Constructor
template <class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& rhs){
  
  root = nullptr;
  
  Iterator<T> iter;
  iter.Init(rhs);
    
  while(iter.hasNext()){
    this->insert(iter.InOrderNext()->value);
  }
}

//Destructor
template <class T>
BinaryTree<T>::~BinaryTree(){
  Iterator<T> iter;
  iter.Init(*this);
    
  while(iter.hasNext()){
    delete iter.InOrderNext();
  }
}
template <class T>
TreeNode<T>* BinaryTree<T>::getRoot(){
  return this->root;
}

//First create the new node with the given value
template <class T>
void BinaryTree<T>::insert(T num) {
  TreeNode<T> *newNode,	// Pointer to a new node
    *nodePtr;	// Pointer to traverse the tree
  
  // Create a new node
  newNode = new TreeNode<T>;
  newNode->value = num;
  newNode->left = newNode->right = nullptr;
  
  if (!root)	// Is the tree empty?
    root = newNode;
  else 
    {
      //we have a root, need to find the location for the next item
      nodePtr = root; 
      while (nodePtr) {
	if (num < nodePtr->value) {
	  if (nodePtr->left)
	    nodePtr = nodePtr->left;
	  else {
	    nodePtr->left = newNode;
	    break;
	  }
	}
	else if (num > nodePtr->value) {
	  if (nodePtr->right)
	    nodePtr = nodePtr->right;
	  else {
	    nodePtr->right = newNode;
	    break;
	  }
	}
	else { //num is not greater or smaller. So it is equal
	  cout << "Duplicate value found in tree.\n";
	  break;
	}
      }		
    }
}

//Operators
template <class T>
const BinaryTree<T>& BinaryTree<T>::operator= (const BinaryTree<T>& rhs){
  if(this!=&rhs)
    {
      this->~BinaryTree();
      this->root = nullptr;
      
      Iterator<T> iter;
      iter.Init(rhs);
      while (iter.hasNext()) {
	this->insert(iter.Next()->value);
      }
    }
  return *this;
}
template <class T>
bool BinaryTree<T>::operator== (const BinaryTree<T>& rhs){
  if(this != &rhs){
    Iterator<T> iter_lhs, iter_rhs;
    iter_lhs.Init(*this);
    iter_rhs.Init(rhs);
    while (iter_lhs.hasNext() && iter_rhs.hasNext()) {
      if(iter_lhs.Next()->value != iter_rhs.Next()->value){
	return false;
      }
    }
    if(iter_lhs.hasNext() || iter_rhs.hasNext()){
      return false;
    }
  }
  return true;
}
template <class T>
bool BinaryTree<T>::operator!= (const BinaryTree<T>& rhs){
  return !operator==(rhs);
}
template <class T>
void BinaryTree<T>::operator+=(const BinaryTree<T>& rhs){
  Iterator<T> iter;
  iter.Init(rhs);
  while (iter.hasNext()) {
    this->insert(iter.Next()->value);
  }
}
template <class T>
BinaryTree<T> BinaryTree<T>::operator+(const BinaryTree<T>& rhs){
  BinaryTree<T> temp(*this);
  temp += rhs;
  return temp;
}

template <class T>
void BinaryTree<T>::operator+=(T number){
  this->insert(number);
}
template <class T>
BinaryTree<T> BinaryTree<T>::operator+(T number){
  BinaryTree<T> temp(*this);
  temp.insert(number);
  return temp;
}
template <class T>
BinaryTree<T> operator+(T number, const BinaryTree<T>& tree){
  BinaryTree<T> temp(tree);
  temp.insert(number);
  return temp;
}
template <class T>
ostream & operator<< (ostream & os, const BinaryTree<T> & rhs)
{
  Iterator<T> iter;
  iter.Init(rhs);
  while (iter.hasNext()) {
    os << iter.Next()->value << " ";
  }
  os << endl;
  return os;
}


/*
 *  ITERATOR CLASS
 */

//Constructor
template <class T>
Iterator<T>::Iterator()
  :myCurrent(nullptr), stack(nullptr), stack2(nullptr)
{}
template <class T>
void Iterator<T>::Init(const BinaryTree<T>& tree){
  myCurrent = tree.root;
  stack = new Stack<T>();
}

template <class T>
bool Iterator<T>::hasNext()
{
  return (!stack->isEmpty() || myCurrent != nullptr);
}
template <class T>
TreeNode<T>* Iterator<T>::Next()
{
  while (myCurrent != nullptr) {
    stack->push(myCurrent);
    myCurrent = myCurrent->left;
  }
  
  myCurrent = stack->pop();
  TreeNode<T>* node = myCurrent;
  myCurrent = myCurrent->right;
  
  return node;
}
template <class T>
TreeNode<T>* Iterator<T>::InOrderNext()
{
  while (myCurrent != nullptr) {
    stack->push(myCurrent);
    myCurrent = myCurrent->left;
  }
  
  myCurrent = stack->pop();
  TreeNode<T>* node = myCurrent;
  myCurrent = myCurrent->right;
  
  return node;
}


//This is just to show. Iterative post-order is much more complex.
template <class T>
TreeNode<T>* Iterator<T>::PreOrderNext()
{
  if(myCurrent->left != nullptr || myCurrent->right != nullptr){
    stack->push(myCurrent);
  }
  
  TreeNode<T>* node = myCurrent;

  if(myCurrent->left == nullptr && myCurrent->right == nullptr){
    if(!stack->isEmpty()){
      myCurrent = stack->pop();
    }
    else{
      myCurrent = nullptr;
      return node;
    }
    if(myCurrent->right){
      myCurrent = myCurrent->right;
    }
    else{
      while(!myCurrent->right){
	myCurrent = stack->pop();
	myCurrent = myCurrent->right;
      }
    }   
  }
  else if(myCurrent->left){
    myCurrent = myCurrent -> left;
  }
  else if(myCurrent->right){
    myCurrent = stack->pop();
    myCurrent = myCurrent -> right;
  }

  return node;
}


  
