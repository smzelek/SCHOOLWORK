/************************************************************/
/*
Author     : Steven Zelek
Course     : CSCI 476
Assignment : Lab #5
Description:
*/

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
getUserValues (unsigned& vectorSize, unsigned& cutoff, unsigned& maxDepth);

void
printSummary (double parallelTime, double serialTime, double stdTime, bool parallelOk, bool serialOk);

void
fillRandomVector (vector<int>& v, unsigned numElements);

void
printVector (const vector<int>& vec);

int
generateInRange (int lower, int upper);

void
parallelMergeSort (vector<int>& v, iter first, iter last, unsigned cutoff, unsigned depth, unsigned maxDepth);

void
serialMergeSort (vector<int>& v, iter first, iter last, unsigned cutoff);

void
serialInsertionSort (vector<int>& v, iter first, iter last);

/************************************************************/

int main (int argc, char* argv[])
{
  unsigned vectorSize, cutoff, maxDepth;
  getUserValues (vectorSize, cutoff, maxDepth);

  //randomly generate vectorSize nums, duplicate vector twice
  vector<int> numsSortedByPMS;
  fillRandomVector (numsSortedByPMS, vectorSize);
  vector<int> numsSortedBySMS (numsSortedByPMS);
  vector<int> numsSortedBySTD (numsSortedByPMS);

  double parallelTime, serialTime, stdTime;

  //time each of the three sorts on a copy of the random array

  Timer<> timer;
  parallelMergeSort (numsSortedByPMS, numsSortedByPMS.begin (), numsSortedByPMS.end (), cutoff, 0, maxDepth);
  timer.stop ();
  parallelTime = timer.getElapsedMs ();

  timer.start ();
  serialMergeSort (numsSortedBySMS, numsSortedBySMS.begin (), numsSortedBySMS.end (), cutoff);
  timer.stop ();
  serialTime = timer.getElapsedMs ();

  timer.start ();
  std::sort (numsSortedBySTD.begin (), numsSortedBySTD.end ());
  timer.stop ();
  stdTime = timer.getElapsedMs ();

  //ensure both sort results match STD sort
  bool parallelOk = (numsSortedBySTD == numsSortedByPMS);
  bool serialOk   = (numsSortedBySTD == numsSortedBySMS);

  //output formatted timings and whether sorts are correct
  printSummary (parallelTime, serialTime, stdTime, parallelOk, serialOk);

  return EXIT_SUCCESS;
}

/************************************************************/

void
getUserValues (unsigned& vectorSize, unsigned& cutoff, unsigned& maxDepth)
{
  cout << "N       ==> ";
  cin >> vectorSize;

  cout << "Cutoff  ==> ";
  cin >> cutoff;

  cout << "Depth   ==> ";
  cin >> maxDepth;
  cout << endl;
}

/************************************************************/

void
printSummary (double parallelTime, double serialTime, double stdTime, bool parallelOk, bool serialOk)
{
  printf ("Parallel time:  %.2f ms\n", parallelTime);
  printf ("Serial time:    %.2f ms\n", serialTime);
  printf ("std::sort time: %.2f ms\n", stdTime);

  cout << "\nParallel ok? " << (parallelOk? "true" : "false") << endl;
  cout << "Serial ok?   " << (serialOk? "true" : "false") << endl;
}

/************************************************************/

void
parallelMergeSort (vector<int>& v, iter first, iter last, unsigned cutoff, unsigned depth, unsigned maxDepth)
{
  unsigned numElements = std::distance (first, last);

  if (numElements <= cutoff)
  {
    serialInsertionSort (v, first, last);
  }
  else if (depth < maxDepth)
  {
    iter midpoint = first + numElements / 2;

    std::thread t1 (parallelMergeSort, std::ref (v), first, midpoint, cutoff, depth + 1, maxDepth);
    std::thread t2 (parallelMergeSort, std::ref (v), midpoint, last, cutoff, depth + 1, maxDepth);
    t1.join ();
    t2.join ();

    //https://en.wikipedia.org/wiki/Bitonic_sorter#Example_code
    //TODO: add bitonic parallel merge function

    std::inplace_merge (first, midpoint, last);
  }
  else
  {
    iter midpoint = first + numElements / 2;

    serialMergeSort (v, first, midpoint, cutoff);
    serialMergeSort (v, midpoint, last, cutoff);

    std::inplace_merge (first, midpoint, last);
  }
}

/************************************************************/

void
serialMergeSort (vector<int>& v, iter first, iter last, unsigned cutoff)
{
  unsigned numElements = std::distance (first, last);

  if (numElements <= cutoff)
  {
    serialInsertionSort (v, first, last);
  }
  else
  {
    iter midpoint = first + numElements / 2;

    serialMergeSort (v, first, midpoint, cutoff);
    serialMergeSort (v, midpoint, last, cutoff);

    std::inplace_merge (first, midpoint, last);
  }
}

/************************************************************/

void
serialInsertionSort (vector<int>& v, iter first, iter last)
{
  //slide elements down by 1 after finding place to insert
  for (iter i = first + 1; i != last; ++i)
  {
    int valToSort = *i;

    iter j = i - 1;
    while (valToSort < *j && j != first - 1)
    {
    *(j + 1) = *j;
      --j;
    }
    *(j + 1) = valToSort;
  }
}

/************************************************************/

int
generateInRange (int lower, int upper)
{
  //std::random_device rd;
  static std::mt19937 gen {0};
  return gen() % (upper + 1 - lower) + lower;
}

/************************************************************/

void
fillRandomVector (vector<int>& v, unsigned numElements)
{
  v.reserve (numElements);
  std::generate_n (std::back_inserter (v), numElements,
  []()
  {
    return generateInRange (0, 999);
  });
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
