/*
  Filename   : PQSort.cc
  Author     : Steve Zelek
  Course     : CSCI 362-01
  Assignment : Lab 8
  Description: Allows a user to compare sort times of a sort
  using priority queue, defined below as pqSort, and std::sort.

  Sorting Time Results

  N      1000     10000      50000      75000      100000

  STD    .09ms    9.02ms    41.39ms     65.69ms     94.74ms
 
  PQ     7.3ms   441.5ms    11657ms   28578.4ms   53496.8ms

  The complexity of PQ appears to be quadratic.
  STD appears to behave at N (linear) complexity.
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>

/************************************************************/
// Local includes

#include "Timer.cc"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

/************************************************************/
// Function prototypes/global vars/typedefs

// Iterates through "filename" and adds all elements to vector "v".
template<typename T>
void
readToVector (vector<T>& v, size_t numElem, string filename);

//performs desired sort based on command line args
template<typename T>
double
sortVector (vector<T>& v, string sortType);

//performs a priority queue sort on the given vector
template<typename T>
void
pqSort (vector<T>& v);

// Prints each element of vector "v" on its own line.
template<typename T>
void
printVector (const vector<T>& v);
/************************************************************/

int      
main (int argc, char* argv[]) 
{
  /*
  if (argc != 3)
  {
    cout << "Incorrect inputs." << endl;
    return EXIT_FAILURE;
  }
  */
  
  string sortType (argv[1]);
  long size = strtol (argv[2], 0, 10);
  vector<string> v;

  readToVector (v, size, "/home/faculty/zoppetti/Dictionary.txt");

  double sortTime = sortVector (v, sortType);
  
  cout << "sorted? "
       << (std::is_sorted (v.begin (), v.end ()) ? "yes": "no")
       << endl;

  cout << sortType << " sort took " << sortTime << " ms." << endl;
  
  return EXIT_SUCCESS; 
}

/************************************************************/

template<typename T>
void
readToVector (vector<T>& v, size_t numElem, string filename)
{
  // Safety check for the given file.
  std::ifstream fileStream (filename);
  if (!fileStream)
  {
    cout << "File not found. Exiting." << endl;
    exit (-1);
  }
  
  T elem;
  size_t count = 0;
  
  // Reads all items of type T into the given vector.
  while (fileStream >> elem && count < numElem)
  {
    v.push_back (elem);
    ++count;
  }
}

/************************************************************/

template<typename T>
double
sortVector (vector<T>& v, string sortType)
{
  Timer timer;
  if (sortType == "std")
  {
    timer.start ();
    std::sort (v.begin (), v.end ());
    timer.stop ();
    return timer.getElapsedMs ();
  }

  if (sortType == "pq")
  {
    timer.start ();
    pqSort (v);
    timer.stop ();
    return timer.getElapsedMs ();
  }

  cout << "Invalid inputs" << endl;
  return -1;
}

/************************************************************/

template<typename T>
void
pqSort (vector<T>& v)
{
  std::priority_queue<T> pq (v.begin (), v.end ());
  v.clear ();
  while (!pq.empty ())
  {
    v.push_back (pq.top ());
    pq.pop ();
  }
  std::reverse (v.begin (), v.end ());
}

/************************************************************/

template<typename T>
void
printVector (const vector<T>& v)
{
  for (T elem : v)
  {
    cout << elem << endl;
  }
}

/************************************************************/
/************************************************************/
