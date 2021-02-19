#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include <iostream>
#include <string>
#include "Stack.h"

using namespace std;

// I have to define it before BinaryTree class
template <class T> class Iterator;

// template T for all int, float, double, string
template <class T>
class BinaryTree
{
 public:
  //Constructor
  BinaryTree();
  //Copy Constructor
  BinaryTree(const BinaryTree&);
  //Destructor
  ~BinaryTree();
  
  // Insertion method
  void insert(T);
  TreeNode<T>* getRoot();
  
  //operators
  const BinaryTree& operator= (const BinaryTree&);
  bool operator== (const BinaryTree&);
  bool operator!= (const BinaryTree&);
  void operator+= (const BinaryTree&);
  BinaryTree operator+ (const BinaryTree&);
  void operator+= (T);
  BinaryTree operator+(T);
  
  //these are friend, not in binary, hence, I have to make template them in here
  template <class T>
  friend BinaryTree<T> operator+(T, const BinaryTree<T>&);
  
  template <class T>
  friend ostream& operator<<(ostream&, const BinaryTree<T>&);

 private:
  //The root of the tree
  TreeNode<T>* root;

  //same as before, iterator class is friend of binarytree class
  friend class Iterator<T>;
};

template <class T>
class Iterator{
 public:
  //Constructor
  Iterator();
  void Init(const BinaryTree<T>& );
  bool hasNext();
  TreeNode<T>* Next(); //Actually the same with InOrderNext()
  TreeNode<T>* InOrderNext(); 
  TreeNode<T>* PreOrderNext(); 
 private:
  TreeNode<T>* myCurrent;
  Stack<T>* stack;
  Stack<T>* stack2;
};


// I have to define it here for fundamental dilemma and I removed both stack.cpp and binarytree.cpp
#include "BinaryTree.cpp"
#endif
