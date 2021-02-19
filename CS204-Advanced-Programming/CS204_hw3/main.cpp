#include "BinaryTree.h"
#include <bits/stdc++.h>

int main(){  

  BinaryTree tree;
  for (int i = 0; i < 10; i++)
  tree.insert(rand() % 100 + 1 );

  cout << "//tree: in-order print with Iterator class"<< endl;
  Iterator i;
  i.Init(tree);
  while (i.hasNext())
    cout << i.Next()->value << " ";
  
  cout << "\n\n//tree: in-order print with << operator"<< endl;
  cout << tree;

  BinaryTree tree_2(tree);
  cout << "\n//tree_2 (copied from tree): \n" << tree_2 << endl;
  
  tree_2 += 124;
  cout << "//tree_2 += 124" << endl;
  cout << "tree_2: " << tree_2 << endl;
  
  BinaryTree tree_3 = tree_2 + 245 + 987 + 457;
  cout << "//tree_3 = tree_2 + 245 + 987 + 457" << endl;
  cout << "tree_3: " << tree_3 << endl;
  
  cout << "//Random number generation for tree_4 and tree_5" << endl;
  cout << "//tree_4 += (rand() % 100 + 1)" << endl;
  cout << "//tree_5 = (rand() % 100 + 50) + tree_5" << endl;
  BinaryTree tree_4, tree_5;
  for (int i = 0; i < 5; i++){
    tree_4 += (rand() % 100 + 1);
    tree_5 = (rand() % 100 + 50) + tree_5 ;
    cout << "("<< i+1 << ". iteration) tree_4: " << tree_4;
    cout << "("<< i+1 << ". iteration) tree_5: " << tree_5;
  }
  
  BinaryTree tree_6;
  tree_6 += 25;
  cout << "\n//tree_6 += 25" << endl;
  cout << "tree_6 (before): " << tree_6 << endl;
  if(tree_4 == tree_5){
    cout << "Content of tree_4 and tree_5 are the same\nrand() isn't that random at the end...\n";
    cout << "//tree_6 += tree_4" << endl;
    tree_6 += tree_4;
  }
  else{
    cout << "Content of tree_4 and tree_5 are not the same\nrand() works like a charm\n";
    cout << "//tree_6 = tree_4 + tree_5" << endl;
    tree_6 = tree_4 + tree_5;
  }
  cout << "\ntree_6 (after) : " << tree_6 << endl;
  
  cin.get();
  cin.ignore();
  return 0;
}
