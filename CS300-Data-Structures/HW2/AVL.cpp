#include <iostream>
using namespace std;


template <class T>
        class AVL;

template <class T>
class AvlNode
{
        T 	element;
        AvlNode   		*left;
        AvlNode   		*right;
		int        		height;

        AvlNode( const T & theElement, 
			        AvlNode *lt, AvlNode *rt, int h = 0 )
            : element( theElement ), left( lt ), right( rt ), height( h ) { }

        friend class AVL<T>;
};


template <class T>
class AVL
{
public:
    AVL();
    //AVL( const AVL & rhs );
	//~AVL( );

	const T & findMin( ) const;
	//const T & findMax( ) const;
	const T & find( const T & x ) const;
	bool isEmpty( ) const;
	void printTree();
	void printItemTree();
	void makeEmpty( );
	void insert( const T & x );
	void remove( const T & x );
	AvlNode<T> *root;
	AvlNode<T> * clone( AvlNode<T> *t ) const;
	const AVL<T> & operator=( const AVL<T> & rhs );
	int getBalance(const AvlNode<T> * N) const;
private:
	void copy(AvlNode<T> * t);
	
    const T ITEM_NOT_FOUND;

    const T & elementAt( AvlNode<T> *t ) const;

    void insert( const T & x, AvlNode<T> * & t ) const;
    void remove(const T & x, AvlNode<T> * &t ) const;

    AvlNode<T> * findMin( AvlNode<T> *t ) const;
    //AvlNode<T> * findMax( AvlNode<T> *t ) const;
    AvlNode<T> * find( const T & x, AvlNode<T> *t ) const;
    void makeEmpty( AvlNode<T> * & t ) const;
    void printTree( AvlNode<T> *t );
	void printItemTree( AvlNode<T> *t);
   

    // Avl manipulations
    int height( AvlNode<T> * t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlNode<T> * & k2 ) const;
    void rotateWithRightChild( AvlNode<T> * & k1 ) const;
    void doubleWithLeftChild( AvlNode<T> * & k3 ) const;
    void doubleWithRightChild( AvlNode<T> * & k1 ) const;

};


template <class T>
AVL<T>::AVL() : root(NULL){}





template <class T>
int AVL<T>::height( AvlNode<T> * t ) const {
        if (t == NULL)
	       return -1;
	
      return t->height;
}

template <class T>
int AVL<T>::max( int lhs, int rhs ) const {
      if (lhs > rhs)
	      return lhs;

      return rhs;            
}

template <class T>
void AVL<T>::insert( const T & x ) {
    insert( x, root );
}

template <class T>
void AVL<T>::insert( const T & x, AvlNode<T> * & t ) const
 {
      if ( t == NULL )
           t = new AvlNode<T>( x, NULL, NULL );

      else if (x < t->element ) {
           // X should be inserted to the left tree!
           insert( x, t->left );
           
    // Check if the left tree is out of balance (left subtree grew in height!)
           if ( height( t->left ) - height( t->right ) == 2 )
                if ( x < t->left->element )  // X was inserted to the left-left subtree!
                     rotateWithLeftChild( t );
                else			     // X was inserted to the left-right subtree!
                     doubleWithLeftChild( t );
       }

	   else if( t->element < x )
			{    // Otherwise X is inserted to the right subtree
					insert( x, t->right );
					if ( height( t->right ) - height( t->left ) == 2 )
			// height of the right subtree increased
						if ( t->right->element < x )
		     			// X was inserted to right-right subtree
							rotateWithRightChild( t );
					else // X was inserted to right-left subtree
							doubleWithRightChild( t );
			}
			else
				;  // Duplicate; do nothing
            
			// update the height the node
			t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template <class T>
void AVL<T>::rotateWithLeftChild( 
			AvlNode<T> * & k2 ) const
{
        AvlNode<T> *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), height(k2) ) + 1;
        k2 = k1;
}

template <class T>
void AVL<T>::rotateWithRightChild (AvlNode<T> * & k1 ) const {
	AvlNode<T> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
	k2->height = max( height( k2->right ), height(k1) ) + 1;
	k1 = k2;
}


template <class T>
void AVL<T>::doubleWithLeftChild ( AvlNode<T> * & k3 ) const {
        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );
}

template <class T>
void AVL<T>::doubleWithRightChild (AvlNode<T> * & k1 ) const {
        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );
}

template <class T>
AvlNode<T> * AVL<T>::clone( AvlNode<T> * t) const {
    if ( t == NULL )
        return NULL;
    else
        return new AvlNode<T> 
			( t->element, 
				clone( t->left ), 
				clone( t->right ) );
}

template <class T>
void AVL<T>::makeEmpty() {
        makeEmpty( root );
}

template <class T>
void AVL<T>::makeEmpty( AvlNode<T> * & t ) const {
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}


template <class T>
const T & AVL<T>::elementAt( AvlNode<T> *t ) const {
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}


template <class T>
const T & AVL<T>::find( const T & x ) const {

        return elementAt( find( x, root ) );
}

template <class T>
AvlNode<T> *AVL<T>::find( const T & x, AvlNode<T> * t ) const
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
const T & AVL<T>::findMin( ) const {
    return elementAt( findMin( root ) );
}

template <class T>
AvlNode<T> * AVL<T>::findMin( AvlNode<T> *t ) const {
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}


template <class T>
void AVL<T>::printTree() 
{
    if( isEmpty() ) {
	}
    else{
        printTree( root );		
	}
}

template <class T>
void AVL<T>::printItemTree() 
{
    if( isEmpty() )
        cout << "";
    else
        printItemTree( root );
}


template <class T>
void AVL<T>::printTree( AvlNode<T> * t ) {
        if ( t != NULL ) {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
}

template<class K>
void AVL<K>::printItemTree(AvlNode<K> *t) {
	 if ( t != NULL ) {
            printItemTree( t->left );
            cout << t->element << endl;
            printItemTree( t->right );
        }
}

template<class T>
const AVL<T> & AVL<T>::operator=(const AVL<T> & tt) {
	makeEmpty();
	AvlNode<T> *x = tt.root;
	copy(x);

	return *this;
}

template <class T>
void AVL<T>::copy(AvlNode<T> * t) {
	
	insert(t->element);

	if (t->left != nullptr) {
		copy(t->left);
	}
	if(t->right != nullptr){
		copy(t->right);
	}
}

template <class T>
bool AVL<T>::isEmpty() const {
	if (root == NULL) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
void AVL<T>::remove ( const T & x ) {
        remove( x, root );
}


template <class T>
void AVL<T>::remove( const T & x, AvlNode<T> * &t ) const{
      
 if (t == NULL) 
        return ; 
  
    // If the key to be deleted is smaller 
    // than the root's key, then it lies
    // in left subtree 
    if ( x < t->element ) 
       remove(x, t->left); 
  
    // If the key to be deleted is greater 
    // than the root's key, then it lies 
    // in right subtree 
    else if(x > t->element ) 
        remove(x, t->right); 
  
    // if key is same as root's key, then 
    // This is the node to be deleted 
    else if (t->left != nullptr && t->right != nullptr) 
    { 
		t->element = findMin(t->right)->element;
		remove(t->element, t->right);
        // node with only one child or no child 
  //      if( (t->left == NULL) ||
  //          (t->right == NULL) ) 
  //      { 
  //          AvlNode<T> *temp =  t->left ? 
  //                       t->left : 
  //                       t->right;
  //
  //          // No child case 
  //          if (temp == NULL) 
  //          { 
  //              temp = t; 
  //              t = NULL; 
  //          } 
  //          else {// One child case 
  //          *t = *temp; // Copy the contents of 
  //                         // the non-empty child 
  //          delete temp; 
		//	}
		//} 
	}
    else
    { 
        // node with two children: Get the inorder 
        // successor (smallest in the right subtree) 
        AvlNode<T>* temp = t;
  
        // Copy the inorder successor's 
        // data to this node 
        t = (t->left != nullptr) ? t->left:t->right;
		delete temp;
    } 
    
  
    // If the tree had only one node
    // then return 
    if (t == NULL) 
		return ; 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 

	if(height(t->left) - height(t->right) > 1)
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
	else 
		if (height(t->right) - height(t->left) > 1)
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
    t->height = max(height(t->left), height(t->right)) + 1;
  
    // STEP 3: GET THE BALANCE FACTOR OF 
    // THIS NODE (to check whether this 
    // node became unbalanced)
	//int balance = getBalance(t);
  
    // If this node becomes unbalanced, 
    // then there are 4 cases 
  
    // Left Left Case 
 //   if (balance > 1 && 
 //       getBalance(t->left) >= 0) 
	//	rotateWithLeftChild(t); 
 // 
 //   // Left Right Case 
 //   if (balance > 1 && 
 //       getBalance(t->left) < 0) 
 //   { 
	//	rotateWithRightChild(t->left); 
	//	rotateWithLeftChild(t); 
 //   } 
 // 
 //   // Right Right Case 
 //   if (balance < -1 && 
 //       getBalance(t->right) <= 0) 
	//	rotateWithRightChild(t); 
 // 
 //   // Right Left Case 
 //   if (balance < -1 && 
 //       getBalance(t->right) > 0) 
 //   { 
	//	rotateWithLeftChild(t->right); 
	//	rotateWithRightChild(t); 
 //   } 
	//return t;

}      

template <class T>
int AVL<T>::getBalance(const AvlNode<T> * N) const
{ 
    if (N == NULL) 
        return 0; 
    return height(N->left) - 
           height(N->right); 
} 













