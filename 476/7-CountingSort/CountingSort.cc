/************************************************************/
/*
Author     : Steven Zelek
Course     : CSCI 476
Assignment : Lab #7
Description: Implementation of serial and parallel counting
             sort.
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
#include <future>

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
using std::thread;

/************************************************************/
// Function prototypes/global vars/typedefs

typedef vector<unsigned>::iterator iter;

void
getUserValues (unsigned& vectorSize, unsigned& k, unsigned& p, unsigned& cutoff);

void
printSummary (double parallelTime, double serialTime, double stdTime, bool parallelOk, bool serialOk);

void
fillRandomVector (vector<unsigned>& v, unsigned numElements, unsigned k);

void
countingSort (vector<unsigned>& v, unsigned k);

void
parCountingSort (vector<unsigned>& v, unsigned k, unsigned p, unsigned cutoff);

void
writeParallelDuplicates (vector<unsigned>& v, unsigned numToCopy, unsigned numCopies, unsigned startIndex);

void
sumLocalCountSlice (vector<unsigned>& globalCounts, const vector<vector<unsigned>>& localCounts, unsigned start, unsigned end);

void
getLocalCount (vector<unsigned>& myLocalCount, iter begin, iter end, unsigned k);

template <typename RAI>
void
printVector (RAI begin, RAI end);

int
generateInRange (int lower, int upper);

/************************************************************/

int main (int argc, char* argv[])
{
  unsigned vectorSize, k, p, cutoff;
  getUserValues (vectorSize, k, p, cutoff);

  //randomly generate vectorSize nums, duplicate vector twice
  vector<unsigned> numsParallelCountSort;
  fillRandomVector (numsParallelCountSort, vectorSize, k);

  vector<unsigned> numsSerialCountSort (numsParallelCountSort);
  vector<unsigned> numsSTDSort         (numsParallelCountSort);

  Timer<> timer;
  double parallelTime, serialTime, stdTime;

  timer.start ();
  parCountingSort (numsParallelCountSort, k, p, cutoff);
  timer.stop ();
  parallelTime = timer.getElapsedMs ();


  timer.start ();
  countingSort(numsSerialCountSort, k);
  timer.stop ();
  serialTime = timer.getElapsedMs ();

  timer.start ();
  std::sort (numsSTDSort.begin (), numsSTDSort.end ());
  timer.stop ();
  stdTime = timer.getElapsedMs ();

  //ensure both sort results match STD sort
  bool parallelOk = (numsParallelCountSort == numsSTDSort);
  bool serialOk   = (numsSerialCountSort   == numsSTDSort);

  //output formatted timings and whether sorts are correct
  printSummary (parallelTime, serialTime, stdTime, parallelOk, serialOk);
  return EXIT_SUCCESS;
}

/************************************************************/

void
getUserValues (unsigned& vectorSize, unsigned& k, unsigned& p, unsigned& cutoff)
{
  cout << "N       ==> ";
  cin >> vectorSize;

  cout << "k       ==> ";
  cin >> k;

  cout << "p       ==> ";
  cin >> p;

  cout << "Cutoff  ==> ";
  cin >> cutoff;

  cout << endl;
}

/************************************************************/

void
printSummary (double parallelTime, double serialTime, double stdTime, bool parallelOk, bool serialOk)
{
  printf ("Parallel time:  %8.2f ms\n",   parallelTime);
  printf ("Serial time:    %8.2f ms\n\n", serialTime);

  printf ("std::sort time: %8.2f ms\n\n", stdTime);

  cout << std::boolalpha;
  cout << "Parallel ok? " << parallelOk << endl;
  cout << "Serial ok?   " << serialOk << endl;
}


/************************************************************/

// Serial counting sort of 'v'
void
countingSort (vector<unsigned>& v, unsigned k)
{
  //range goes from [0, k)
  vector<unsigned> counts (k);

  auto i = v.begin ();
  while (i < v.end ())
  {
    counts[*i++]++;
  }

  i = v.begin ();

  //go through each count bucket
  for (unsigned n = 0; n < k; ++n)
  {
    //duplicate n c times
    for (int numCopies = counts[n]; numCopies > 0; --numCopies)
    {
      *i++ = n;
    }
  }
}

/************************************************************/

// Parallel counting sort of 'v'
void
parCountingSort (vector<unsigned>& v, unsigned k, unsigned p, unsigned cutoff)
{
  //simply do a serial counting sort if there are too few elements.
  if (v.size () <= cutoff)
  {
    countingSort (v, k);
    return;
  }

  vector<vector<unsigned>> localCounts;
  vector<thread> threads;
  threads.reserve (p);
  localCounts.resize (p);

  //p rows, k cols in a  2d matrix
  //each row will be filled up by a thread
  for (unsigned i = 0; i < p; ++i)
  {
    auto begin = v.begin () + (v.size () * i) / p;
    auto end = v.begin () + (v.size () * (i + 1)) / p;

    threads.push_back (thread (getLocalCount, std::ref (localCounts[i]), begin, end, k));
  }


  for (auto &t : threads){
    t.join();
  }
  threads.clear();
  
  //now have all local counts
  vector<unsigned> globalCounts (k);

  for (unsigned i = 0; i < p; ++i)
  {
    unsigned begin = (i * k) / p;
    unsigned end = ((i + 1) * k) / p;
    threads.push_back (thread (sumLocalCountSlice, std::ref (globalCounts), std::cref (localCounts), begin, end));
  }

  for (auto &t : threads)
  {
    t.join();
  }
  threads.clear();

  //now have global count...
  //now must put into original vector.
  unsigned startIndex = 0;

  for (unsigned i = 0; i < k; ++i)
  {
    //launch thread given v, i and startidx
    threads.push_back (thread (writeParallelDuplicates, std::ref(v), i, globalCounts[i], startIndex));
    startIndex += globalCounts[i];
  }

  for (auto &t : threads)
  {
    t.join();
  }
}

/************************************************************/

void
writeParallelDuplicates (vector<unsigned>& v, unsigned numToCopy, unsigned numCopies, unsigned startIndex)
{
  auto i = v.begin () + startIndex;
  while (numCopies > 0)
  {
    *i++ = numToCopy;
    numCopies--;
  }
}

/************************************************************/

void
sumLocalCountSlice (vector<unsigned>& globalCounts, const vector<vector<unsigned>>& localCounts, unsigned start, unsigned end)
{
  for (unsigned i = 0; i < localCounts.size(); ++i)
  {
    for (unsigned j = start; j < end; ++j)
    {
      globalCounts[j] += localCounts[i][j];
    }
  }
}

/************************************************************/

void
getLocalCount (vector<unsigned>& myLocalCount, iter begin, iter end, unsigned k)
{
  myLocalCount.resize(k);
  iter i = begin;
  while (i < end)
  {
    myLocalCount[*i++]++;
  }
  
}

/************************************************************/

int
generateInRange (int lower, int upper)
{
  //std::random_device rd;
  static std::mt19937 gen {std::random_device {} ()};
  return gen () % (upper - lower) + lower;
}

/************************************************************/

void
fillRandomVector (vector<unsigned>& v, unsigned numElements, unsigned k)
{
  v.reserve (numElements);
  std::generate_n (std::back_inserter (v), numElements,
  [k]()
  {
    return generateInRange (0, k);
  });
}

/************************************************************/

template <typename RAI>
void
printVector (RAI begin, RAI end)
{
  RAI i = begin;
  cout << "{ " << *i++;
  while (i < end)
  {
    cout << ", " << *i++;
  }
  cout << " }" << endl;
}

/************************************************************/
/************************************************************/
