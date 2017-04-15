/************************************************************/
/*
  Author     : Steven Zelek
  Course     : CSCI 476
  Assignment : Lab #3
  Description: Sums a randomly generated vector using both
               a threaded parallel approach and serial version.

  *For all, assume p = 4, N = 1000000

  Timing measurements*:

                AVG.       Run 1       Run 2       Run 3       Run 4       Run 5
               ------      ------      ------       -----       -----       -----
  process      2.12ms      1.98ms      2.21ms      1.96ms      2.25ms      2.20ms

  threads      6.23ms      5.89ms      6.38ms      6.38ms      6.29ms      6.20ms

  Reference: http://stackoverflow.com/questions/807506/threads-vs-processes-in-linux

  On Linux, threads are created with shared memory and processes are created with
  their own address spaces, and only copy on write. When we're only reading from shared memory,
  processes can do so cheaply. Because threads share memory, they must keep memory synchronized.
  In short, processes are better suited to this task because we only need to communicate twice for
  each process. Using IPC is cheaper if we're only using it a few times instead of ensuring that
  shared memory is synchronized across several threads.

  -SZ
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

/************************************************************/
// Function prototypes/global vars/typedefs

std::vector<int> partialSums;

void
computeSerialSum (const std::vector<int>& nums);

void
computeParallelSum (const std::vector<int>& nums, unsigned numThreads);

void
computePartialSum (const std::vector<int>& nums, unsigned id, unsigned numThreads);

void
fillVector (std::vector<int>& nums, size_t numElements);

void
printVector (std::vector<int> vec);

/************************************************************/

int main (int argc, char* argv[])
{
    size_t numElements = 0;
    size_t numThreads = 0;

    cout << "p ==> ";
    cin >> numThreads;
    cout << endl;

    cout << "N ==> ";
    cin >> numElements;
    cout << endl;

    //original vector is not const, must be modified by fill function
    std::vector<int> A;

    //fills with RANDOM elements [0,4] inclusive
    fillVector (A, numElements);

    //now use const references for summation functions
    computeParallelSum (std::cref (A), numThreads);
    computeSerialSum (std::cref (A));

    return EXIT_SUCCESS;
}

/************************************************************/

void
computeParallelSum (const std::vector<int>& nums, unsigned numThreads)
{
    Timer<> timer;

    //create the threads!
    std::vector<std::thread> myThreads;
    for (unsigned id = 0; id < numThreads; ++id)
    {
        myThreads.push_back(std::thread (computePartialSum, std::cref(nums), id, numThreads));
    }

    //wait for all threads
    for (auto& t : myThreads)
    {
        t.join ();
    }

    //add the partial sums up
    int finalSum = std::accumulate (&partialSums[0], &partialSums[numThreads], 0);

    timer.stop ();

    cout << "Parallel sum:  " << finalSum << endl;
    cout << "Parallel time: " << timer.getElapsedMs () << " ms" << endl << endl;
}

/************************************************************/

void
computePartialSum (const std::vector<int>& nums, unsigned id, unsigned numThreads)
{
    size_t begin = (id * nums.size()) / numThreads;
    size_t end = ((id + 1) * nums.size()) / numThreads;

    partialSums.push_back(std::accumulate (&nums[begin], &nums[end], 0));
}

/************************************************************/

void
computeSerialSum (const std::vector<int>& nums)
{
    Timer<> timer;
    int totalSum = std::accumulate (&nums[0], &nums[nums.size()], 0);
    timer.stop ();

    cout << "Serial sum:    " << totalSum << endl;
    cout << "Serial time:   " << timer.getElapsedMs () << " ms" << endl;
}

/************************************************************/

void
fillVector (std::vector<int>& nums, size_t numElements)
{
    std::mt19937 gen (0);
    std::uniform_int_distribution<> dis (0, 4);

    std::generate_n (std::back_inserter(nums), numElements,
                    [&]()
                    {
                        return dis (gen);
                    }
    );

    return;
}

/************************************************************/

void
printVector (std::vector<int> vec)
{
    const std::string separator = ", ";
    std::string sep = "";
    std::cout << "{ ";
    for (const auto& e : vec)
    {
        std::cout << sep << e;
        sep = separator;
    }
    std::cout << " }";
}

/************************************************************/
/************************************************************/
