#include "BinaryTree.h"
#include "Stack.h"
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
//#include "Stack.cpp"
//#include "BinaryTree.cpp"
using namespace std;

template <class T>
void templated_trees(T arr[]){

  BinaryTree<T> tree;
  for (int i = 0; i < 10; i++){
    tree.insert(arr[(rand()+1) %100] );
  }
  
  cout << "//tree: in-order print with Iterator class"<< endl;
  int count = 0;
  Iterator<T> i;
  i.Init(tree);

  while (i.hasNext()){
    cout << i.Next()->value << " ";
  }
  
  cout << "\n\n//tree: in-order print with << operator"<< endl;
  cout << tree;

  BinaryTree<T> tree_2(tree);
  cout << "\n//tree_2 (copied from tree): \n" << tree_2 << endl;
  
  tree_2 += arr[24];
  cout << "//tree_2 += arr[24]" << endl;
  cout << "tree_2: " << tree_2 << endl;
  
  BinaryTree<T> tree_3 = tree_2 + arr[45] + arr[87] + arr[57];
  cout << "//tree_3 = tree_2 + arr[45] + arr[87] + arr[57]" << endl;
  cout << "tree_3: " << tree_3 << endl;
  
  cout << "//Random access for tree_4 and tree_5" << endl;
  cout << "//tree_4 += (arr[(rand()+1)%100])" << endl;
  cout << "//tree_5 = (arr[(rand()+50)%100]) + tree_5" << endl;
  BinaryTree<T> tree_4, tree_5;
  for (int i = 0; i < 5; i++){
    tree_4 += arr[(rand()+1)%100];
    tree_5 = arr[(rand()+50)%100] + tree_5 ;
    cout << "("<< i+1 << ". iteration) tree_4: " << tree_4;
    cout << "("<< i+1 << ". iteration) tree_5: " << tree_5;
  }
  
  BinaryTree<T> tree_6;
  tree_6 += arr[25];
  cout << "\n//tree_6 += arr[25]" << endl;
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
  
  }

int main(){  

  int intarr[100];
  float floatarr[100];
  double doublearr[100];
  char chararr[100];
  string stringarr[100];

  float X0 = 100;
  double X1 = 100;
  

  for(int i = 0; i < 100; i++){
    intarr[i] = rand() % 200;
    floatarr[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/X0);
    doublearr[i] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX/X1);
  }

  for(int i = 34; i < 125; i++){
    chararr[i-34] = static_cast<char>(i);
  }
  for(int i = 91; i < 100; i++){
    chararr[i] = static_cast<char>(i);
  }

  fstream file;
  string word;
  int count = 0;
  
  string filename = "strings.txt";
  file.open(filename.c_str());

  while(file >> word && count < 100){
    stringarr[count] = word;
    count++;
  }
  
  cout << "//--------------------------------------//" << endl;
  cout << "int tree: " << endl;
  cout << "//--------------------------------------//" << endl;
  cout << endl;
  templated_trees(intarr);

  cout << "//--------------------------------------//" << endl;
  cout << "float tree: " << endl;
  cout << "//--------------------------------------//" << endl;
  cout << endl;
  templated_trees(floatarr);

  cout << "//--------------------------------------//" << endl;
  cout << "double tree" << endl;
  cout << "//--------------------------------------//" << endl;
  cout << endl;
  templated_trees(doublearr);

  cout << "//--------------------------------------//" << endl;
  cout << "char tree" << endl;
  cout << "//--------------------------------------//" << endl;
  cout << endl;
  templated_trees(chararr);

  cout << "//--------------------------------------//" << endl;
  cout << "string tree" << endl;
  cout << "//--------------------------------------//" << endl;
  cout << endl;
  templated_trees(stringarr);

  cin.get();
  cin.ignore();
  
}
