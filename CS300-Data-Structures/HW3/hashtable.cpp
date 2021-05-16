//#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <algorithm>
//#include "linkedlist.h"



//template <class T>
//class HashTable
//{
//public:
//	explicit HashTable( const T & notFound, 
//						int size = 101 );
//	HashTable( const HashTable & rhs )
//			: ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ), 
//								theLists( rhs.theLists ) { }
//
//	const T & find( const T & x ) const;
//
//	void makeEmpty( );
//	bool insert( const T & x );
//	void remove( const T & x );
//	//bool contains( const T & x ) const;
//	const HashTable & operator=( const HashTable & rhs);
//private:
//      
//	int currentSize;
//    const T ITEM_NOT_FOUND;
//	vector<list<T>> theLists; 
//};
//
//// int hash( const vector<string> & key, int tableSize );
////int hash( int key, int tableSize );
//
//
//inline int hash( int key, int tableSize )
//{
//    if( key < 0 ) key = -key;
//    return key % tableSize;
//}
//
//inline int hash (const vector<string> & key, int tableSize)
//{ 
//    int hashVal = 0;
//	string ans = "";
//	for (int i = 0; i < key.size(); i++) {
//		ans += key[i] + ",";
//	}
//
//	ans = ans.substr(0, ans.length()-1);
//
//    for (int i = 0; i < ans.length();i++) 
//	  hashVal = 37 * hashVal + ans[ i ];
//
//    hashVal = hashVal % tableSize;
//
//    if (hashVal < 0) 
//	  hashVal = hashVal + tableSize;
//
//   return(hashVal);
//}
//
//inline bool isPrime( int n ) {
//    if ( n == 2 || n == 3 )
//        return true;
//
//    if ( n == 1 || n % 2 == 0 )
//        return false;
//
//    for ( int i = 3; i * i <= n; i += 2 )
//            if ( n % i == 0 )
//                return false;
//
//    return true;
//}
//
//
//inline int nextPrime( int n ) {
//        if ( n % 2 == 0 )
//            n++;
//
//        for ( ; ! isPrime( n ); n += 2 )
//            ;
//
//        return n;
//}
//
//
//template <class T>
//HashTable<T>::HashTable( 
//								const T & notFound, int size )
//            : ITEM_NOT_FOUND( notFound ), theLists( nextPrime( size ) ) {}
//
//
//template <class T>
//void HashTable<T>::makeEmpty( ) {
//    for( int i = 0; i < theLists.size( ); i++ )
//        theLists[ i ].makeEmpty( );  
//			// destroy the lists but not the vector!
//}
//
//template <class T>
//bool HashTable<T>::insert( const T & x )
//{
//    // hash the given object and locate the list it should be on 
//    auto & whichList = theLists[ hash( x, theLists.size( ) ) ];
//    // locate the object in the list (using List’s find)
//    if( find( begin( whichList ), end( whichList ), x ) != end( whichList ) )
//		 return false;
//	whichList.push_back( x );
//    // insert the new item at the head of the list if not found!
//    return true;
//}
//
//
//template <class T>
//void HashTable<T>::remove( const T & x )
//{
//    // remove from the appropriate list
//	 auto & whichList = theLists[ myhash( x ) ];
//	 auto itr = find( begin( whichList ), end( whichList ), x );
//
//	 if( itr == end( whichList ) )
//	 return false;
//
//	 whichList.erase( itr );
//	 //--currentSize;
//	 return true;
//}
//
//
//template <class T>
//const T & HashTable<T>::
//                  		find( const T & x ) const
//{
//        list<T> & whichList = theLists[hash(x, theLists.size())];
//        // locate the approriate list and search there
//		auto itr = find(whichList.begin(), whichList.end(), x);
//        // retrieve from the located position
//		if ( itr == whichList.end())
//	  		return ITEM_NOT_FOUND;
//	
//	return itr.retrieve( );
//}
//
//
//template <class T>
//const HashTable<T> & HashTable<T>::operator=( const HashTable<T> & rhs )
//{
//    if( this != &rhs )
//        theLists = rhs.theLists;
//    return *this;
//}


        #include "hashtable.h"
        #include <iostream>


        
        //Internal method to test if a positive number is prime.
        //Not an efficient algorithm.         
        bool isPrime( int n )
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        }

        
         //Internal method to return a prime number at least as large as n.        
        int nextPrime( int n )
        {
            if( n % 2 == 0 )
                n++;

            for( ; !isPrime( n ); n += 2 )
                ;

            return n;
        }

        
        //Construct the hash table.         
        template <class HashedObj>
        HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size )
          : ITEM_NOT_FOUND( notFound ), theLists( nextPrime( size ) )
        {
        }

      
         //Insert item x into the hash table. If the item is
         //already present, then do nothing.
        template <class HashedObj>
        void HashTable<HashedObj>::insert( const HashedObj & x )
        {
            List<HashedObj> & whichList = theLists[ hash1( x, theLists.size( ) ) ];
            ListItr<HashedObj> itr = whichList.find( x );

            if( itr.isPastEnd( ) )
                whichList.insert( x, whichList.zeroth( ) );
        }

        
        //Remove item x from the hash table.
        template <class HashedObj>
        void HashTable<HashedObj>::remove( const HashedObj & x )
        {
           theLists[ hash1( x, theLists.size( ) ) ].remove( x );
        }

        
         //Find item x in the hash table.
         //Return the matching item or ITEM_NOT_FOUND if not found
        template <class HashedObj>
        const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
        {
            ListItr<HashedObj> itr;
            itr = theLists[ hash1( x, theLists.size( ) ) ].find( x );
            if( itr.isPastEnd( ) )
                return ITEM_NOT_FOUND;
            else
                return itr.retrieve( );
        }

        
        // Make the hash table logically empty.        
        template <class HashedObj>
        void HashTable<HashedObj>::makeEmpty( )
        {
            for( int i = 0; i < theLists.size( ); i++ )
                theLists[ i ].makeEmpty( );
        }

        
        //Deep copy.
        template <class HashedObj>
        const HashTable<HashedObj> &
        HashTable<HashedObj>::operator=( const HashTable<HashedObj> & rhs )
        {
            if( this != &rhs )
                theLists = rhs.theLists;
            return *this;
        }

        
       
         // A hash routine for string objects.
        int hash1( const vector<string> & key, int tableSize )
        {
			string ans = "";
				for (int i = 0; i < key.size(); i++) {
					ans += key[i] + ",";
				}
			
				ans = ans.substr(0, ans.length()-1);


            int hashVal = 0;

            for( int i = 0; i < ans.length( ); i++ )
                hashVal = 37 * hashVal + ans[ i ];

            hashVal %= tableSize;
            if( hashVal < 0 )
                hashVal += tableSize;

            return hashVal;
        }


        
         //A hash routine for ints.
        int hash1( int key, int tableSize )
        {
            if( key < 0 ) key = -key;
            return key % tableSize;
        }
