/************************************************************/
/*
Author     : Steven Zelek
Course     : CSCI 476
Assignment : Lab #8
Description: Implementation of the game of Life simulation.

 N           1                 2                     4                    8
====    ===========    ==================    =================    =================
1000    26964.10 ms    14425.50 ms (1.8X)    7193.65 ms (3.7X)    5822.25 ms (4.6X)
750     15728.57 ms    8333.69 ms (1.9X)     5971.02 ms (2.6X)    3154.81 ms (5.0X)
500     6835.98 ms     3444.19 ms (2.0X)     2713.19 ms (2.5X)    1491.85 ms (4.6X)

Discussion:
  Speedup rate seems to be consistent with number of threads, regardless of
  the input size N (at least for the small sample size displayed above).
  The speedup does appear to drop off as more threads are added,
  so there is likely an asymptotic limit to the parallelization of this
  particular problem.

  I'm not sure how my runtimes stack up with my fellow classmates since I
  haven't had the chance to ask them. The algorithm I used makes the most
  sense to me, but it does require making a copy of the original bitset.
  It's possible that Amdahl's law is applying itself to limit my speedup
  due to the serial copy I use - so there is room for further analysis
  and improvement there.
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
#include <fstream>

#include <boost/thread/barrier.hpp>

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
void
initializeGame (unsigned& N, unsigned& p, unsigned& generations, std::vector<vector<bool>>& grid);

void
parallelProcessGenerations (unsigned N, unsigned p, unsigned generations, std::vector<vector<bool>>& grid);

void
outputResultAsBinaryFile (unsigned& N, std::vector<vector<bool>>& grid);

void
fillGridFromRandomSeed (unsigned& N, std::vector<vector<bool>>& grid);

void
fillGridFromFile (unsigned& N, std::string infile, std::vector<vector<bool>>& grid);

unsigned
getNFromFile (std::string infile);

bool
generateBoolFromSeed (unsigned seed);

void
callback (int x);

void
processGridBlock (boost::barrier& b, std::vector<vector<bool>>& grid, std::vector<vector<bool>>& gridCopy,
                  unsigned begin, unsigned end, unsigned generations);


void
printGrid (std::vector<vector<bool>> grid);

/************************************************************/

int main (int argc, char* argv[])
{
  unsigned N, p, generations;
  std::vector<vector<bool>> grid;

  initializeGame(N, p, generations, grid);

  //assume grid was created
  Timer<> timer;
  parallelProcessGenerations (N, p, generations, grid);
  timer.stop();

  outputResultAsBinaryFile (N, grid);

  cout << endl;
  cout << std::fixed << std::setprecision(2);
  cout << "Parallel time:  " << timer.getElapsedMs() << " ms" << endl;
  cout << "Output file  :  GameOfLifeOutput.bin" << endl;

  return EXIT_SUCCESS;
}

/************************************************************/

void
initializeGame (unsigned& N, unsigned& p, unsigned& generations, std::vector<vector<bool>>& grid)
{
  cout << "Input file  ==> ";
  std::string infile;
  cin >> infile;

  if (infile == "n")
  {
    fillGridFromRandomSeed (N, grid);
  }
  else {
    fillGridFromFile (N, infile, grid);
  }

  cout << "p           ==> ";
  cin >> p;

  cout << "Generations ==> ";
  cin >> generations;
}

/************************************************************/

void
processGridBlock (boost::barrier& b, std::vector<vector<bool>>& grid, std::vector<vector<bool>>& gridCopy,
                  unsigned begin, unsigned end, unsigned generations)
{
  unsigned N = grid.size() - 2;

  if (begin == 0)
  {
    begin = 1;
  }
  if (end > N)
  {
    end = N + 1;
  }

  for (unsigned g = 0; g < generations; ++g)
  {
    //UPDATE PIECE OF GRID COPY BASED ON GRID
    for (unsigned i = begin; i < end; ++i)
    {
      for (unsigned j = 1; j <= N; ++j)
      {
        unsigned neighborCount = grid[i-1][j-1]
                                + grid[i-1][j]
                                + grid[i-1][j+1]
                                + grid[i][j-1]
                                + grid[i][j+1]
                                + grid[i+1][j-1]
                                + grid[i+1][j]
                                + grid[i+1][j+1];
        
        if (grid[i][j])
        {
          gridCopy[i][j] = !(neighborCount <= 1 || neighborCount >= 4);
        }
        else {
          gridCopy[i][j] = (neighborCount == 3);
        }
      }
    }
    b.count_down_and_wait();

    //update the real grid
    for (unsigned i = begin; i < end; ++i)
    {
      std::swap(grid[i], gridCopy[i]);
    }
    b.count_down_and_wait();
  }
}

/************************************************************/

void
parallelProcessGenerations (unsigned N, unsigned p, unsigned generations, std::vector<vector<bool>>& grid)
{
  boost::barrier b (p);
  std::vector<vector<bool>> gridCopy (grid);
  
  vector<thread> threads;
  for (unsigned i = 0; i < p; ++i)
  {
    unsigned begin = (grid.size () * i) / p;
    unsigned end = (grid.size () * (i + 1)) / p;
    threads.push_back(thread (processGridBlock, std::ref(b), std::ref(grid), std::ref(gridCopy), begin, end, generations));
  }

  for (auto& t : threads)
  {
    t.join();
  }
}

/************************************************************/

void
outputResultAsBinaryFile (unsigned& N, std::vector<vector<bool>>& grid)
{
  // Write the vector to a file in binary format.
  std::ofstream bitFile ("GameOfLifeOutput.bin", std::ios::out | std::ios::binary);

  std::vector<bool> bits;
  for (auto& bitvec : grid)
  {
    for (auto bit : bitvec)
    {
      bits.push_back(bit);
    }
  }

  //turn into one vector
  // Create a vector of bytes to hold the bits in the set.
  // We can write this vector to a file, but we cannot
  //   write the buffer of the bitset directly to a file. 
  vector<unsigned char> bytes ((bits.size () + 7) / 8);

  // Fill the bytes vector with the bits from the bitset
  char bitCount = 0;
  size_t byteIndex = 0;
  for (auto bit : bits)
  {
    bytes[byteIndex] = (bytes[byteIndex] << 1) | bit;
    ++bitCount;
    if (bitCount == 8)
    {
      // Move to the next byte
      ++byteIndex;
      bitCount = 0;
    }
  }

  // Shift the remaining bits, if any, to the left
  if (bitCount != 0)
    bytes[byteIndex] <<= (8 - bitCount);

  // Lastly, write the sequence of bytes to the stream
  bitFile.write (reinterpret_cast<char*> (bytes.data ()),
		   bytes.size ());
  bitFile.close ();
}

/************************************************************/

void
fillGridFromRandomSeed (unsigned& N, std::vector<vector<bool>>& grid)
{
  cout << "N           ==> ";
  cin >> N;

  cout << "Seed        ==> ";
  unsigned seed;
  cin >> seed;

  grid.resize (N+2);
  grid[0].resize(N+2);
  grid[N+1].resize(N+2);
  for (unsigned i = 1; i <= N; ++i)
  {
    grid[i].resize(N+2);
    for (unsigned j = 1; j <= N; ++j)
    {
      grid[i][j] = generateBoolFromSeed(seed);
    }
  }
}

/************************************************************/

void
fillGridFromFile (unsigned& N, std::string infile, std::vector<vector<bool>>& grid)
{
  N = getNFromFile(infile);
  cout << N << endl;
  grid.resize (N+2);
  grid[0].resize(N+2);
  grid[N+1].resize(N+2);

  std::ifstream fin (infile);
  for (unsigned i = 1; i <= N; ++i)
  {
    grid[i].resize(N+2);
    for (unsigned j = 1; j <= N; ++j)
    {
      char c;
      fin >> c;
      if (c == '#')
        grid[i][j] = true;
    }
  }
}

/************************************************************/

unsigned
getNFromFile (std::string infile)
{
  std::ifstream fin (infile);
  std::string s;
  std::getline(fin, s);
  return (s.size() + 1) / 2;
}

/************************************************************/

bool
generateBoolFromSeed (unsigned seed)
{
  static std::minstd_rand gen {seed};
  return gen () % 2;
}

void
printGrid (std::vector<vector<bool>> grid)
{
  for (unsigned i = 0; i < grid.size(); ++i)
  {
    for (unsigned j = 0; j < grid[i].size(); ++j)
    {
      if (grid[i][j])
      {
        cout << "# ";
      }
      else {
        cout << "- ";
      }
    }
    cout << endl;
  }
}

/************************************************************/
/*********************************************TOOTTOOT*******/
