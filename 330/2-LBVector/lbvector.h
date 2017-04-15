#ifndef _LBVECTOR_H
#define _LBVECTOR_H

#include <cstdlib>
#include <cassert>
#include <iostream>

// templated vector class, partially based on Budd,
//                         Classic Data Structures in C++
// written 11/5/93, modified 3/23/94
// changed on 3/9/95 to make all methods inline (defined in class decl)
// changed on 10/14/99 to clean up for vector lab in CS 330
// changed on 10/14/04 to update for C++ changes
//////////////////////////////////////////////////////////////////////////
// updated 11/3/16 by Steve Zelek to allow lower and upper bounds
//////////////////////////////////////////////////////////////////////////
//
// for a vector of Items use lbvector<Item>,
//                   e.g., lbvector <int> intvector;
//                   note: Item must have a default constructor
//
// constructors:
//   lbvector( )             -- default, vector of capcity 0 (no entries)
//   lbvector(int lower)     -- vector with (upper-lower+1) entries
//            int upper)
//   lbvector(int lower)     -- vector with (upper-lower+1) entries
//            int upper,         all == fillValue
//            Item fillValue)
//   lbvector(const lbvector & vec) -- copy constructor
//    
//   int capacity( )         -- returns capacity of vector
//   int lower()             -- returns lower bound of vector
//   int upper()             -- returns upper bound of vector
//
//   void resize(int newLower, int newUpper) 
//                           -- change bounds (and therefore size) dynamically
//                             overlapping indices will have their elements copied
//
//   void fill(Item fillValue)  -- set all entries equal to fillValue
//
//   operator =              -- assignment operator works properly
//   operator []             -- indexes both const and non-const vectors
//    
//
//  class invariant:
//   myList points to the beginning of the allocated space 
//   myZero points to where the element with index zero would be if there were one 
//   myLower is the lowest legal index 
//   myUpper is the highest legal index 
//   myCapacity is the number of elements allocated for
//   myZero[myLower] is the same memory location as myList[0]

template <class Item> class lbvector
{
public:
   // default constructor 0 elements
   // postcondition: vector of zero items constructed
   lbvector( )
   {
      myCapacity = 0;
      myList = 0;
      myZero = 0;
      myLower = 0;
      myUpper = -1;
   }
    
   // specify upper and lower bounds of vector
   // precondition: Item must have a default constructor
   //               upper must be greater than or equal to lower
   // postcondition: vector of (upper-lower+1) items constructed
   lbvector(int lower, int upper) 
   {
      assert(upper >= lower);
      myLower = lower;
      myUpper = upper;
      myCapacity = upper - lower + 1;
      myList = new Item [myCapacity];
      myZero = myList - lower;
      
      assert(myList != 0);
   }
    
   // specify lower, upper bounds and fill value
   // precondition: Item must have a default constructor
   //               upper must be greater than or equal to lower
   // postcondition: constructed vector of (upper-lower+1) items,
   //                each item initialized to fillValue
   lbvector(int lower, int upper, Item fillValue) 
   {
      assert(upper >= lower);
      myLower = lower;
      myUpper = upper;
      myCapacity = upper - lower + 1;
      myList = new Item [myCapacity];
      myZero = myList - lower;
      assert(myList != 0);
      fill(fillValue);
   }

   // copy constructor
   // used to create a new and complete copy of the object (vec)
   //    as when passing the object by value or returning object 
   //    as a result - need a deep copy when using dynamic memory
   // precondition: Item supports assignment
   // postcondition: constructed vector is an exact duplicate of vec        
   lbvector(const lbvector<Item> & vec) 
   {
      // allocate storage
      myCapacity = vec.myCapacity;
      myLower = vec.myLower;
      myUpper = vec.myUpper;
      myList = new Item [myCapacity];
      myZero = myList - myLower;
      assert(myList != 0);
        
      // copy elements from vec
      for (int k = 0; k < vec.myCapacity; k++) {
         myList[k] = vec.myList[k];
      }       
   }
    
   // free new'd storage
   // postcondition: dynamically allocated storage freed
   ~lbvector ( ) 
   {
      delete [] myList;
   }
   
   // capacity of vector
   int capacity() const 
   {
      return myCapacity;
   }
   
   // lower bound of vector
   int lower() const
   {
      return myLower;
   }
   
   // upper bound of vector
   int upper() const
   {
       return myUpper;
   }
  
   // change bounds (and therefore size) dynamically
   // precondition: vector has room for myCapacity entries     
   //                  newUpper is greater than or equal newLower
   // postcondition: vector has room for (newUpper-newLower+1) entries
   //          any overlapping indices will have their elements copied
   //          other indices have the default value of item
   void resize(int newLower, int newUpper) 
   {
      assert(newUpper >= newLower);
      int newSize = newUpper - newLower + 1;
      int lowerCopy = myLower > newLower ? myLower : newLower;
      int upperCopy = myUpper < newUpper ? myUpper : newUpper;
    
      // allocate new storage
      Item * newList = new Item[newSize];
      assert(newList != 0);

      Item * newZero = newList - newLower;

      // copy retained values from old to new        
      for (int k= lowerCopy; k <= upperCopy; k++) {
         newZero[k] = myZero[k];
      }

      // return space no longer needed        
      delete [] myList;

      // update instance variables to reflect changes
      myCapacity = newSize;
      myList = newList;
      myLower = newLower;
      myUpper = newUpper;
      myZero = myList - myLower;
   }

   // postcondition: all entries are equal to fillValue
   void fill(Item fillValue) 
   {
      for (int k=0; k < myCapacity; k++) {
         myList[k] = fillValue;
      }
   }
    
   // assignment
   //    need a deep copy when using dynamic memory
   // precondition: Item supports assignment     
   // postcondition: self is assigned vec
   lbvector & operator = (const lbvector<Item> & vec) 
   {
      // don't assign to self!
      if (this != &vec) {
         // out with old list, in with new
         delete [] myList;
         myCapacity = vec.myCapacity;
	       myLower = vec.myLower;
	       myUpper = vec.myUpper;
         myList = new Item [myCapacity];
	       myZero = myList - myLower;
         assert(myList != 0);
            
         // copy elements from vec
         for (int k=0; k < vec.myCapacity; k++) {
            myList[k] = vec.myList[k];
         }
      }
      return *this;           
   }

   // safe indexing, returning reference
   // precondition: myLower <= index <= myUpper
   // postcondition: return index-th item relative to myZero
   // exception: aborts if index is out-of-bounds
   Item & operator [] (int index) 
   {
      checkIndex(index);
      return myZero[index];     
   }
    
   // const index 
   // safe indexing, returning const reference to avoid modification
   // precondition: myLower <= index <= myUpper
   // postcondition: return index-th item relative to myZero
   // exception: aborts if index is out-of-bounds
   const Item & operator [] (int index) const 
   {
      checkIndex(index);
      return myZero[index]; 
   }
   
private:
   Item* myList;  // the array of Items
   Item* myZero; // the virtual origin of the array
   int myLower; // the lowest legal index
   int myUpper; // the highest legal index
   int myCapacity; // number of elements in vector

   // aborts with appropriate message if index is not in appropriate range
   // use std:: to access names in std namespace
   void checkIndex (int index) const 
   {
      if (index < myLower || index > myUpper) {
         std::cerr << "Illegal lbvector index: " << index
                   << " valid bounds are [" << myLower << ", " << myUpper << "]" 
                   << std::endl;
         assert(index >= myLower);
         assert(index <= myUpper);
      }
   }
};


#endif                // _LBVECTOR_H not defined
