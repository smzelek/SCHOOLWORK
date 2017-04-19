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

/************************************************************/
// Local includes

#include "List.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;

/************************************************************/
// Function prototypes/global vars/typedefs

int
execute (List<int> listedInts, size_t numJumps);

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
  List<int> intList;

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
execute (List<int> intList, size_t numJumps)
{
  List<int>::iterator it = intList.begin ();
  
  // N - 1 removals must occur
  while (intList.size () > 1)
  {
    // The iterator jumps k - 1 times, because erase jumps once.
    for (size_t jumpCount = 1; jumpCount < numJumps; ++jumpCount)
    {
      //cout << "Iter is @ " << *it << endl;
      ++it;
      if (it == intList.end ())
      {
        it = intList.begin ();
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
