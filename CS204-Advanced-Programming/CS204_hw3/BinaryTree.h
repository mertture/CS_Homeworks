#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include <iostream>
#include "Stack.h"

using namespace std;

class BinaryTree
{
 public:
  //Constructor
  BinaryTree();
  //Copy Constructor - TODO in .cpp file
  BinaryTree(const BinaryTree&);
  //Destructor
  ~BinaryTree();
  
  // Insertion method
  void insert(int);
  
private:
  //The root of the tree
  TreeNode* root;

  friend class Iterator;
};

class Iterator{
public:
  //Constructor
  Iterator();
  
  void Init(const BinaryTree& );
  bool hasNext();
  TreeNode* Next();
  
private:
  TreeNode* myCurrent;
  Stack* stack;
};

#endif
