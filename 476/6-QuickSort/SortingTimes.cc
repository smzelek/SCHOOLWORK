/************************************************************/
/*
Author     : Steven Zelek
Course     : CSCI 476
Assignment : Lab #6
Description: Implementation of parallel/serial versions of both
			 quicksort and mergesort, timed and compared to
			 each other and std::sort.
*/

/************************************************************/
// System includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <thread>
#include <iomanip>

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

void
getUserValues (unsigned& vectorSize, unsigned& cutoff, unsigned& maxDepth);

void
printSummary (double parallelMSTime, double serialMSTime, double parallelQSTime, double serialQSTime, double stdTime, bool parallelOk, bool serialOk);

void
fillRandomVector (vector<int>& v, unsigned numElements);

template <typename RAI>
void
printVector (RAI begin, RAI end);

int
generateInRange (int lower, int upper);

template <typename RAI>
void
parallelMergeSort (RAI first, RAI last, unsigned cutoff, unsigned depth);

template <typename RAI>
void
serialMergeSort (RAI first, RAI last, unsigned cutoff);

template <typename RAI>
void
parallelQuickSort (RAI first, RAI last, unsigned cutoff, unsigned depth);

template <typename RAI>
void
serialQuickSort (RAI first, RAI last, unsigned cutoff);

template <typename RAI>
RAI
partition (RAI first, RAI last);

template <typename RAI>
RAI
medianOf3 (RAI first, RAI last);

template <typename RAI>
void
serialInsertionSort (RAI first, RAI last);

/************************************************************/

int main (int argc, char* argv[])
{
  unsigned vectorSize, cutoff, maxDepth;
  getUserValues (vectorSize, cutoff, maxDepth);

  //randomly generate vectorSize nums, duplicate vector twice
  vector<int> numsSortedByPMS;// = {3, 1, 2};
  fillRandomVector (numsSortedByPMS, vectorSize);

  vector<int> numsSortedBySMS (numsSortedByPMS);
  vector<int> numsSortedByPQS (numsSortedByPMS);
  vector<int> numsSortedBySQS (numsSortedByPMS);
  vector<int> numsSortedBySTD (numsSortedByPMS);


  Timer<> timer;
  double parallelMSTime, serialMSTime, parallelQSTime, serialQSTime, stdTime;
  

  timer.start ();
  parallelMergeSort (numsSortedByPMS.begin (), numsSortedByPMS.end (), cutoff, maxDepth);
  timer.stop ();
  parallelMSTime = timer.getElapsedMs ();
  
  timer.start ();
  serialMergeSort (numsSortedBySMS.begin (), numsSortedBySMS.end (), cutoff);
  timer.stop ();
  serialMSTime = timer.getElapsedMs ();


  timer.start ();
  parallelQuickSort (numsSortedByPQS.begin (), numsSortedByPQS.end (), cutoff, maxDepth);
  timer.stop ();
  parallelQSTime = timer.getElapsedMs ();

  timer.start ();
  serialQuickSort (numsSortedBySQS.begin (), numsSortedBySQS.end (), cutoff);
  timer.stop ();
  serialQSTime = timer.getElapsedMs ();


  timer.start ();
  std::sort (numsSortedBySTD.begin (), numsSortedBySTD.end ());
  timer.stop ();
  stdTime = timer.getElapsedMs ();

  //ensure both sort results match STD sort
  bool parallelOk = (numsSortedBySTD == numsSortedByPQS && numsSortedBySTD == numsSortedByPMS);
  bool serialOk   = (numsSortedBySTD == numsSortedBySQS && numsSortedBySTD == numsSortedBySMS);

  //output formatted timings and whether sorts are correct
  printSummary (parallelMSTime, serialMSTime, parallelQSTime, serialQSTime, stdTime, parallelOk, serialOk);
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
printSummary (double parallelMSTime, double serialMSTime, double parallelQSTime, double serialQSTime, double stdTime, bool parallelOk, bool serialOk)
{
  printf ("Parallel MS time:  %.3f ms\n", parallelMSTime);
  printf ("Serial MS time:    %.3f ms\n\n", serialMSTime);

  printf ("Parallel QS time:  %.3f ms\n", parallelQSTime);
  printf ("Serial QS time:    %.3f ms\n\n", serialQSTime);

  printf ("std::sort time:    %.3f ms\n\n", stdTime);

  cout << std::boolalpha;
  cout << "Parallel ok? " << parallelOk << endl;
  cout << "Serial ok?   " << serialOk << endl;
}


/************************************************************/

template <typename RAI>
void
parallelMergeSort (RAI first, RAI last, unsigned cutoff, unsigned depth)
{
  unsigned numElements = std::distance (first, last);

  if (numElements <= cutoff)
  {
    serialInsertionSort (first, last);
  }
  else if (depth > 0)
  {
    RAI midpoint = first + numElements / 2;
  
    std::thread t (&parallelMergeSort<RAI>, first, midpoint, cutoff, depth - 1);
    parallelMergeSort (midpoint, last, cutoff, depth - 1);
    t.join ();
    
    std::inplace_merge (first, midpoint, last);
  }
  else 
  {      
    RAI midpoint = first + numElements / 2;
  
    serialMergeSort (first, midpoint, cutoff);
    serialMergeSort (midpoint, last, cutoff);
    
    std::inplace_merge (first, midpoint, last);
  }
}

/************************************************************/

template <typename RAI>
void
serialMergeSort (RAI first, RAI last, unsigned cutoff)
{
  unsigned numElements = std::distance (first, last);

  if (numElements <= cutoff)
  {
    serialInsertionSort (first, last);
  }
  else
  {
    RAI midpoint = first + numElements / 2;
    
    serialMergeSort (first, midpoint, cutoff);
    serialMergeSort (midpoint, last, cutoff);

    std::inplace_merge (first, midpoint, last);
  }
}


/************************************************************/

template <typename RAI>
void
parallelQuickSort (RAI first, RAI last, unsigned cutoff, unsigned depth)
{
  if (std::distance (first, last) <= cutoff)
  {
    serialInsertionSort (first, last);
  }
  else if (depth > 0)
  {
    RAI middle = partition (first, last);

    std::thread t (&parallelQuickSort<RAI>, first, middle, cutoff, depth - 1);
    parallelQuickSort (middle + 1, last, cutoff, depth - 1);
    
    t.join ();
  }
  else
  {
  	RAI middle = partition (first, last);

  	serialQuickSort (first, middle, cutoff);
  	serialQuickSort (middle + 1, last, cutoff);
  }

}

/************************************************************/

template <typename RAI>
void
serialQuickSort (RAI first, RAI last, unsigned cutoff)
{
  if (std::distance (first, last) <= cutoff)
  {
    serialInsertionSort (first, last);
  }
  else
  {
    RAI middle = partition (first, last);

    serialQuickSort (first, middle, cutoff);
    serialQuickSort (middle + 1, last, cutoff);
  }

}

/************************************************************/

template <typename RAI>
RAI
partition (RAI first, RAI last)
{
  //obtain pivot element (MO3)
  RAI pivot = medianOf3 (first, last);

  //partition based on pivot (MO3)

  //range to sort = [first+1, median)
  RAI begin = first + 1;
  RAI end = pivot - 1;

  //degenerate case where there are no elements to be partitioned
  if (pivot == first)
  {
	return pivot;
  }
  
  //while loop exits when iters meet or cross.
  while (true)
  {
    while (*begin < *pivot)
    {
      begin++;
    }
    while (*end > *pivot)
    {
      end--;
    }
    if (begin < end)
    {
      iter_swap (begin, end);
      begin++;
      end--;
    }
    else 
    {
	  break;
    }
  }

  iter_swap (begin, pivot);
  //partitioning complete
  //return where pivot ended up....
  return begin;
}


/************************************************************/

// 3-sort the first, middle, and last - 1 elements
//   Swap the median to last - 2
template <typename RAI>
RAI
medianOf3 (RAI first, RAI last)
{
  last = last - 1;
  RAI middle = first + std::distance (first, last) / 2;


  //returns median
  if (*first > *middle)
  {
    std::iter_swap (first, middle);
  }
  if (*first > *last)
  {
    std::iter_swap (first, last);
  }
  if (*middle > *last)
  {
    std::iter_swap (middle, last);
  }

  //middle now holds median
  //put median in original last-2
  std::iter_swap (middle, last - 1);

  //return location of the median
  return last - 1;
}

/************************************************************/

template <typename RAI>
void
serialInsertionSort (RAI first, RAI last)
{
  //slide elements down by 1 after finding place to insert
  for (RAI i = first + 1; i < last; ++i)
  {
    int valToSort = *i;

    RAI j = i - 1;
    while (valToSort < *j && j >= first)
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
  static std::mt19937 gen {std::random_device {} ()};
  return gen () % (upper + 1 - lower) + lower;
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

template <typename RAI>
void
printVector (RAI begin, RAI end)
{
  const std::string separator = ", ";
  std::string sep = "";
  cout << "{ ";
  RAI i = begin;
  while (i < end)
  {
    cout << sep << *i++;
    sep = separator;
  }
  cout << " }" << endl;
}

/************************************************************/
/************************************************************/
