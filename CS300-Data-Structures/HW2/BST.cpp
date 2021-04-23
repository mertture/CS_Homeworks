#include <iostream>
using namespace std;

template <class T>
class BST;

// nodes
template<class T>
class BinaryNode {
	T element;
	BinaryNode *left;
	BinaryNode *right;

	BinaryNode( const T & theElement, 
		       BinaryNode *lt, BinaryNode *rt )
			   : element( theElement ), left( lt ), right( rt ) {}


	friend class BST<T>;
};

// bst class
template <class T>
class BST {
	

public:
	BST();
	//BST(const BST & rhs);
	//~BST();

	const T & findMin() const;
	//const T & findMax() const;
	const T & find (const T & x) const;
	
	//bool isEmpty() const;
	void printTree();
	void printItemTree();
	bool isEmpty() const;
	void makeEmpty();
	void insert (const T & x);
	void remove (const T & x);
	BinaryNode<T> *root;
	BinaryNode<T> * clone( BinaryNode<T> *t ) const;
	BST<T>& operator=( const BST & tt );
private:
	
    const T ITEM_NOT_FOUND;

    const T & elementAt( BinaryNode<T> *t ) const;
	void copy(BinaryNode<T> * t);
    void insert( const T & x, BinaryNode<T> * & t ) const;
    void remove( const T & x, BinaryNode<T> * & t ) const;
    BinaryNode<T> * findMin( BinaryNode<T> *t ) const;
    //BinaryNode<T> * findMax( BinaryNode<T> *t ) const;
    BinaryNode<T> * find( const T & x, BinaryNode<T> *t ) const;
    void makeEmpty( BinaryNode<T> * & t ) const;
    void printTree( BinaryNode<T> *t );
	void printItemTree( BinaryNode<T> *t);
    //void printItemInfoTree(BinaryNode<T> *t)

};


template <class T>
BST<T>::BST() : root( NULL ) {}



template <class T>
const T & BST<T>::
elementAt( BinaryNode<T> *t ) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class T>
const T & BST<T>::find( const T & x ) const {

        return elementAt( find( x, root ) );
}

template <class T>
BinaryNode<T> *BST<T>::find( const T & x, BinaryNode<T> * t ) const
{
        if ( t == NULL )
            return NULL;
        else if( x < t->element )
            return find( x, t->left );
        else if( t->element < x )
            return find( x, t->right );
        else
            return t; // Match

		
}


template <class T>
void BST<T>::insert( const T & x ) {
    insert( x, root );
}

template <class T>
void BST<T>::insert( const T & x, BinaryNode<T> * & t ) const{
 //   if( t == NULL ) { //  create a new node at the right place
 //       t = new BinaryNode<T>( x, NULL, NULL );
	//}
 //   else if( x < t->element ) {
 //       insert( x, t->left );
	//} // insert at the left or 
 //   else if( t->element < x ) {
 //       insert( x, t->right ); 
	//} // right subtree
 //   else {
	//}  // Duplicate; do nothing
	//BinaryNode<T>* newnode = new BinaryNode<T>(x, NULL, NULL);
 //
 //   // Pointer to start traversing from root and
 //   // traverses downward path to search
 //   // where the new node to be inserted
 //   BinaryNode<T>* a = t;
 //
 //   // Pointer y maintains the trailing
 //   // pointer of x
 //   BinaryNode<T>* y = NULL;
 //
 //   while (a != NULL) {
 //       y = a;
 //       if (x < a->element)
 //           a = a->left;
 //       else
 //           a = a->right;
 //   }
 //
 //   // If the root is NULL i.e the tree is empty
 //   // The new node is the root node
 //   if (y == NULL) {
 //       y = newnode;
	//	t = newnode;
	//}
 //   // If the new key is less then the leaf node key
 //   // Assign the new node to be its left child
 //   else if (x < y->element) {
 //       y->left = newnode;
	//}
 //   // else assign the new node its right child
 //   else {
 //       y->right = newnode;
	//}
 //   // Returns the pointer where the
 //   // new node is inserted
 //  

	BinaryNode<T> *ptr;
    BinaryNode<T> *ptr_parent;

   if(t == NULL)
   {//The BST is Empty...
      BinaryNode<T> *newNode = new BinaryNode<T>(x, NULL, NULL);

      t = newNode;
      ptr = t;
   } else { //traversing the tree to find the insertion point
      ptr = t;
      while(ptr != NULL)
      {
         if(x < (ptr -> element))
         {
            ptr_parent = ptr;
            ptr = ptr -> left;
         } else {
            ptr_parent = ptr;
            ptr = ptr -> right;
         }
      }
      BinaryNode<T> *newNode = new BinaryNode<T>(x, NULL, NULL);

      //checking for parent value to determine if
      //the Node is a left or right child  
      if(x < (ptr_parent -> element))
         ptr_parent -> left = newNode;
      else
         ptr_parent -> right = newNode;
   
   }
  

}

template <class T>
void BST<T>::remove ( const T & x ) {
        remove( x, root );
}

template <class T>
void BST<T>::remove( const T & x, BinaryNode<T> * & t ) const{
    if( t == NULL )
        return;   // Item not found; do nothing
    if( x < t->element )
        remove( x, t->left );
    else if( t->element < x )
        remove( x, t->right );
	else if( t->left != NULL && t->right != NULL ) {// Two children
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else // one or no children
    {
        BinaryNode<T> *oldNode = t;
        t = ( t->left != NULL ) ? t->left : t->right;
        delete oldNode;
    }
}      

template <class T>
const T & BST<T>::findMin( ) const {
    return elementAt( findMin( root ) );
}

template <class T>
BinaryNode<T> * BST<T>::findMin( BinaryNode<T> *t ) const {
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}


template <class T>
void BST<T>::printTree() 
{
    if( isEmpty() ) {
	}
    else{
        printTree( root );		
	}
}

template <class T>
void BST<T>::printItemTree() 
{
    if( isEmpty() )
        cout << "";
    else
        printItemTree( root );
}


template <class T>
void BST<T>::printTree( BinaryNode<T> * t ) {
        if ( t != NULL ) {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
}

template<class K>
void BST<K>::printItemTree(BinaryNode<K> *t) {
	 if ( t != NULL ) {
            printItemTree( t->left );
            cout << t->element << endl;
            printItemTree( t->right );
        }
}

//template<class K>
//void BST<K>::printItemInfoTree(BinaryNode<K> *t) {
//	 if ( t != NULL ) {
//            printItemInfoTree( t->left );
//            cout << t->element.info << endl;
//            printItemInfoTree( t->right );
//        }
//}

template <class T>
void BST<T>::makeEmpty() {
        makeEmpty( root );
}

template <class T>
void BST<T>::makeEmpty( BinaryNode<T> * & t ) const {
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}

//template <class T>
//BST<T>::~BST() {
//    makeEmpty();
//}

template <class T>
bool BST<T>::isEmpty() const {
	if (root == NULL) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
BinaryNode<T> * BST<T>::clone( BinaryNode<T> * t) const {
    if ( t == NULL )
        return NULL;
    else
        return new BinaryNode<T> 
			( t->element, 
				clone( t->left ), 
				clone( t->right ) );
}

template<class T>
BST<T> & BST<T>::operator=(const BST<T> & tt) {
	makeEmpty();
	BinaryNode<T> *x = tt.root;
	copy(x);

	return *this;
}

template <class T>
void BST<T>::copy(BinaryNode<T> * t) {
	
	insert(t->element);

	if (t->left != nullptr) {
		copy(t->left);
	}
	if(t->right != nullptr){
		copy(t->right);
	}
}