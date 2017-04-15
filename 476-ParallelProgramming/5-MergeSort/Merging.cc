/************************************************************/
// System includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <thread>

/************************************************************/
// Local includes
#include "Timer.hpp"

/************************************************************/
// Using declarations
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::iterator;

/************************************************************/
// Function prototypes/global vars/typedefs

typedef vector<int>::iterator iter;

void 
printVector (const vector<int>& vec);

/************************************************************/

int main (int argc, char* argv[])
{  
  vector<int> sortedHalvesArray = {1, 2, 7, 5, 6, 8};
  iter sortSpot = sortedHalvesArray.begin ();
  iter compSpot1 = sortedHalvesArray.begin ();
  iter compSpot2 = sortedHalvesArray.begin () + 3;
  
  while (sortSpot != sortedHalvesArray.end())
  {
    if (*compSpot1 < *compSpot2)
    {
      std::iter_swap(sortSpot, compSpot1);
      compSpot1++;
    }
    else 
    {
      std::iter_swap(sortSpot, compSpot2);
      compSpot2++;
    }
    sortSpot++;
  }
  
  printVector(sortedHalvesArray);
  
  
  return EXIT_SUCCESS;
}

/************************************************************/

void 
printVector (const vector<int>& vec)
{
  const std::string separator = ", ";
  std::string sep = "";
  cout << "{ ";
  for (const auto& e : vec) 
  {
    cout << sep << e;
    sep = separator;
  }
  cout << " }" << endl;
}

/************************************************************/
/************************************************************/