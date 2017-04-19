/*
  Filename   : Josephus.cc
  Author     : Steve Zelek
  Course     : CSCI 362-01
  Assignment : Lab 6
  Description: 
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
#include <iterator>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::list;
using std::endl;

/************************************************************/
// Function prototypes/global vars/typedefs

int
execute (list<int> listedInts, size_t numJumps);

/************************************************************/

int      
main (int argc, char* argv[]) 
{
  cout << "Please enter N: ";
  size_t listLength;
  cin >> listLength;

  cout << "Please enter K: ";
  size_t numJumps;
  cin >> numJumps;
  
  //create list
  list<int> intList;

  for (size_t i = 1; i <= listLength; ++i)
  {
    intList.push_back(i);
    //cout << i << endl;
  }

  //return remaining elem
  cout << "\nThe surviving element is: "
       << execute (intList, numJumps)
       << endl;
  
  return EXIT_SUCCESS; 
}

/************************************************************/

// The complexity of execute ():
// The number of traversals can be given as a function of T.
// T(N,K) = (N - 1) * (k)
// Therefore T(N,k) = kN - k.
// So, execute is complexity O(N) as long as k << N.

int
execute (list<int> intList, size_t numJumps)
{
  list<int>::iterator it = intList.begin ();
  
  // N - 1 removals must occur
  for (size_t numItems = intList.size (); numItems > 1; --numItems)
  {
    // The iterator jumps k - 1 times, because erase jumps once.
    for (size_t jumpCount = 1; jumpCount < numJumps; ++jumpCount)
    {
      //cout << "Iter is @ " << *it << endl;
      if (it == prev (intList.end ()))
      {
        it = intList.begin ();
      }
      else
      {
        ++it;
      }
    }    
    //cout << "Erased " << *it << endl;
    it = intList.erase (it);
    
    if (it == intList.end ())
      it = intList.begin ();
  }
  return intList.front ();
}
/************************************************************/
/************************************************************/
