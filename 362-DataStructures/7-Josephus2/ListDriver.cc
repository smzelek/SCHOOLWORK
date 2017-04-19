/*
  Filename   : ListDriver.cc
  Author     : Steve Zelek
  Course     : CSCI 362-01
  Assignment : Lab 7
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
using std::endl;

/************************************************************/
// Function prototypes/global vars/typedefs

/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  List<int> intList;
  /*
  cout << "Added a 1." << endl;

  cout << "Inserted at front: "
       << *(intList.insert (intList.end (), 1)) << endl;

  cout << intList << endl;
  cout << "Size: " << intList.size ()
       << "\nFront: " << intList.front ()
       << "\nBack: " << intList.back () << endl;
  
  cout << "Then inserted: "
       << *(intList.insert (intList.end (), 2)) << endl;
  
  cout << "Inserted at end: ";
  for (size_t i = 1; i < 9; ++i)
    {
       cout << i << ", ";
       intList.push_back(i);
    }
    
  cout << endl << intList << endl;
  cout << "Size: " << intList.size ()
       << "\nFront: " << intList.front ()
       << "\nBack: " << intList.back () << endl;
  
  cout << "\nPopped last element. ";
  intList.pop_back (); 

  cout << endl << intList << endl;
  cout << "Size: " << intList.size ()
       << "\nFront: " << intList.front ()
       << "\nBack: " << intList.back () << endl;
  */

  List<int> A;

  A.push_back (5);
  cout << endl << A << endl;

  A.push_front (3);
  cout << endl << A << endl;

  A.push_back (9);
  cout << endl << A << endl;

  /*
  while (A.size () > 1)
  {
    //A.pop_back ();
    A.pop_front ();
  }
  
  cout << endl << A << endl;
  */

  List<int> B (A);
  List<int> C = A;

  cout << endl << "B: " << B << endl << "C: " << C << endl;


  cout << "Popped front on A." << endl;
  A.pop_front();
  
  cout << endl << A << endl;
  cout << "Size: " << A.size ()
       << "\nFront: " << A.front ()
       << "\nBack: " << A.back () << endl;

  cout << endl << B << endl;
  cout << "Size: " << B.size ()
       << "\nFront: " << B.front ()
       << "\nBack: " << B.back () << endl;

  cout << endl << C << endl;
  cout << "Size: " << C.size ()
       << "\nFront: " << C.front ()
       << "\nBack: " << C.back () << endl;

  cout << "Erased A... and added 1-9." << endl;
  while (!A.empty ())
  {
    A.pop_back ();
  }
  cout << A << endl;

  for (size_t i = 9; i > 0; --i)
    {
      A.push_front (i);
    }

  cout << A << endl;

  auto iter = A.begin ();
  while (*iter != 5)
    {
      // cout << *iter << " ";
      ++iter;
    }

  A.erase(A.begin (), iter);
  cout << "\nErased [1,5) in A. " << endl << A << endl;
  
  List<int> D (4, 5);
  
  cout << "Size 4 list containing 5s: " << endl << D << endl;

  List<int> E {1, 2, 6, 7, 1, 0};

  cout << "Random list. " << endl << E << endl;

  cout << "Are B and C equal at start? " << (B.front()==C.front()) << endl;
  cout << "Are D and E equal at end? " << (D.back()==E.back()) << endl;
  cout << "Are B and C equal at end? " << (B.back()==C.back()) << endl;
  
  
  return EXIT_SUCCESS; 
}

/************************************************************/
/************************************************************/
