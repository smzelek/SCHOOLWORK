/************************************************************/
/*
  Author     : Steven Zelek
  Course     : CSCI 476
  Assignment : Lab #2
  Description: Uses forking and pipes to accomplish parallel summation of a vector.
                Times parallel and serial versions for comparision.
*/

/************************************************************/
// System includes

#include <iostream>
#include <cassert>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>
#include <random>
#include <functional>

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
#define READ  0
#define WRITE 1

void
fillVector (std::vector<int>& nums, size_t numElements);

void
computeSerialSum ();

void
computeParallelSum (size_t numElementss);

void
computePartialSum (int mypipe[2], unsigned i, unsigned numProcessors);


void
printVector (std::vector<int> vec);


std::vector<int> g_A;

/************************************************************/

int main (int argc, char* argv[])
{
    size_t numElements = 0;
    size_t numProcessors = 0;

    cout << "p ==> ";
    cin >> numProcessors;
    cout << endl;

    cout << "N ==> ";
    cin >> numElements;
    cout << endl;

    //resizes vector and fill with random elements
    fillVector (g_A, numElements);
    computeParallelSum (numProcessors);
    computeSerialSum ();

    return EXIT_SUCCESS;
}

/************************************************************/

void
computeParallelSum (size_t numProcessors)
{
    //also starts TIMER
    Timer<> timer;
    int mypipe[2];

    assert (pipe (mypipe) != -1);

    for (unsigned i = 0; i < numProcessors; ++i)
    {
        computePartialSum (mypipe, i, numProcessors);
    }

    int totalSum = 0;

    close (mypipe[WRITE]);
    for (unsigned i = 0; i < numProcessors; ++i)
    {
        int partialSum = 0;
        int childStatus;
        waitpid (-1, &childStatus, 0);

        read (mypipe[READ], &partialSum, sizeof (partialSum));
        totalSum += partialSum;
    }

    timer.stop ();

    cout << "Parallel sum:  " << totalSum << endl;
    cout << "Parallel time: " << timer.getElapsedMs () << " ms" << endl << endl;
}

/************************************************************/

void
computePartialSum (int mypipe[2], unsigned i, unsigned numProcessors)
{
    pid_t childPid = fork ();

    //Ensure that the process forked correctly before proceeding.
    assert (childPid != -1);

    //Child processes execute this code, parent skips.
    if (childPid == 0)
    {
        unsigned rangeSize = g_A.size () / numProcessors;

        auto startIdx = g_A.begin() + (i * rangeSize);
        auto endIdx = startIdx + rangeSize;

        //The last processor must finish the full range.
        if (i == numProcessors - 1)
        {
            endIdx = g_A.end();
        }

        int partialSum = std::accumulate (startIdx, endIdx, 0);

        //Store the partial sum in the pipe to be retrieved by the master process.
        close (mypipe[READ]);
        write (mypipe[WRITE], &partialSum, sizeof (partialSum));

        // This will cause child process to terminate
        exit (5);
    }
}

/************************************************************/

void
computeSerialSum ()
{
    Timer<> timer;
    int totalSum = std::accumulate (g_A.begin (), g_A.end (), 0);
    timer.stop ();

    cout << "Serial sum:    " << totalSum << endl;
    cout << "Serial time:   " << timer.getElapsedMs () << " ms" << endl;
}

/************************************************************/

void
fillVector (std::vector<int>& nums, size_t numElements)
{

    nums.resize (numElements);
    std::mt19937 gen (0);
    std::uniform_int_distribution<> dis (0, 4);

    std::generate_n (g_A.begin (), g_A.size (),
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
    for(const auto& e : vec)
    {
        std::cout << sep << e;
        sep = separator;
    }
    std::cout << " }";
}

/************************************************************/
/************************************************************/
