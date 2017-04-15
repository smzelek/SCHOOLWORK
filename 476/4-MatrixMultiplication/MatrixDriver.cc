/************************************************************/
/*
Author     : Steven Zelek
Course     : CSCI 476
Assignment : Lab #4
Description: Computes the matrix product of two square matrices
             using different index ordering to exploit caching.
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
#include "Matrix.hpp"

/************************************************************/
// Using declarations
using std::cout;
using std::cin;
using std::endl;

/************************************************************/
// Function prototypes/global vars/typedefs

void
fillMatrix (Matrix<int>& m);

void
printMatrix (const Matrix<int>& m);

int
generateInRange (int lower, int upper);

double
matrixMultiplication (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, std::string version);

double
ijkMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result);

double
jkiMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result);

double
kijMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result);

double
blockMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, unsigned blockSize);

double
parallelMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, unsigned numThreads);

void
computeMatrixBlock (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, unsigned id, unsigned numThreads);

int
checksumMatrix (const Matrix<int>& m);

/************************************************************/

int main (int argc, char* argv[])
{

  unsigned matrixSize;
  std::string version;

  cout << "N       ==> ";
  cin >> matrixSize;

  cout << "Version ==> ";
  cin >> version;

  Matrix<int> A (matrixSize, matrixSize);
  Matrix<int> B (matrixSize, matrixSize);
  Matrix<int> C (matrixSize, matrixSize);

  fillMatrix (A);
  fillMatrix (B);

  double multTime = matrixMultiplication (A, B, C, version);
  int checksum = checksumMatrix (C);

  cout << endl;
  cout << "Sum:  " << checksum << endl;
  printf ("Time: %.3f ms\n", multTime);
  
  
  return EXIT_SUCCESS;
}

/************************************************************/

int
checksumMatrix (const Matrix<int>& m)
{
  unsigned stop = std::min (m.size (), 10000u);
  return std::accumulate (m.begin (), m.begin () + stop, 0);
}

/************************************************************/

double
matrixMultiplication (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, std::string version)
{
  if (version == "ijk")
  {
    return ijkMultiply (m1, m2, result);
  }
  else if (version == "jki")
  {
    return jkiMultiply (m1, m2, result);
  }
  else if (version == "kij")
  {
    return kijMultiply (m1, m2, result);
  }
  else if (version == "block")
  {
    unsigned blockSize;
    cout << "BS      ==> ";
    cin >> blockSize;

    return blockMultiply (m1, m2, result, blockSize);
  }
  else if (version == "parallel")
  {
    unsigned numThreads;
    cout << "Threads ==> ";
    cin >> numThreads;
    
    return parallelMultiply (m1, m2, result, numThreads);
  }
  
  //throw error?
  return EXIT_FAILURE;
}

/************************************************************/

double
parallelMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, unsigned numThreads)
{
  Timer<> timer;
  std::vector<std::thread> myThreads;

  for (unsigned i = 0; i < numThreads; ++i)
  {
    myThreads.push_back(std::thread (computeMatrixBlock, std::cref(m1), std::cref(m2), std::ref(result), i, numThreads)); 
  }
  
  //wait for all threads
  for (auto& t : myThreads)
  {
    t.join ();
  }
  
  timer.stop ();
  return timer.getElapsedMs ();
}

/************************************************************/

void
computeMatrixBlock (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result, unsigned id, unsigned numThreads)
{ 
  unsigned N = result.getNumRows(); 
  unsigned start = (id * N) / numThreads;
  unsigned end = ((id + 1) * N) / numThreads;
  
  for (unsigned i = start; i < end; ++i)
  {
    for (unsigned j = 0; j < N; ++j)
    {
      int sum = 0;
      for (unsigned k = 0; k < N; ++k)
      {
        sum += m1 (i, k) * m2 (k, j);
      }
      result (i, j) = sum;
    }
  }
}

/************************************************************/

double
ijkMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result)
{
  Timer<> timer;
  unsigned N = m1.getNumRows ();
  for (unsigned i = 0; i < N; ++i)
  {
    for (unsigned j = 0; j < N; ++j)
    {
      int sum = 0;
      for (unsigned k = 0; k < N; ++k)
      {
        sum += m1 (i, k) * m2 (k, j);
      }
      result (i, j) = sum;
    }
  }
  timer.stop ();
  return timer.getElapsedMs ();
}

/************************************************************/

double
jkiMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result)
{
  Timer<> timer;
  unsigned N = m1.getNumRows ();
  for (unsigned j = 0; j < N; ++j)
  {
    for (unsigned k = 0; k < N; ++k)
    {
      int r = m2 (k, j);
      for (unsigned i = 0; i < N; ++i)
      {
        result (i, j) += m1 (i, k) * r;
      }
    }
  }
  timer.stop ();
  return timer.getElapsedMs ();
}

/************************************************************/

double
kijMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>& result)
{
  Timer<> timer;
  unsigned N = m1.getNumRows ();
  for (unsigned k = 0; k < N; ++k)
  {
    for (unsigned i = 0; i < N; ++i)
    {
      int r = m1 (i, k);
      for (unsigned j = 0; j < N; ++j)
      {
        result (i, j) += r * m2 (k, j);
      }
    }
  }
  timer.stop ();
  return timer.getElapsedMs ();
}

/************************************************************/

double
blockMultiply (const Matrix<int>& m1, const Matrix<int>& m2, Matrix<int>&result, unsigned blockSize)
{
  Timer<> timer;
  unsigned N = m1.getNumRows ();
  for (unsigned i = 0; i < N; i += blockSize)
  {
    for (unsigned j = 0; j < N; j += blockSize)
    {
      for (unsigned k = 0; k < N; k += blockSize)
      {
        /*B x B mini matrix multiplications*/
        for (unsigned i1 = i; i1 < i + blockSize; ++i1)
        {
          for (unsigned j1 = j; j1 < j + blockSize; ++j1)
          {
            for (unsigned k1 = k; k1 < k + blockSize; ++k1)
            {
              result (i1, j1) += m1 (i1, k1) * m2 (k1, j1);
            }
          }
        }
      }
    }
  }
  timer.stop ();
  return timer.getElapsedMs ();
}

/************************************************************/

int
generateInRange (int lower, int upper)
{
  static std::mt19937 gen {0};
  return std::uniform_int_distribution<> {lower, upper} (gen);
}

/************************************************************/

void
fillMatrix (Matrix<int>& m)
{
  std::generate_n (m.begin (), m.size (),
  []()
  {
    return generateInRange (0, 4);
  });
}

/************************************************************/

void
printMatrix (const Matrix<int>& m)
{
  cout << endl;
  for (unsigned i = 0; i < m.getNumRows (); ++i)
  {
    for (unsigned j = 0; j < m.getNumCols (); ++j)
    {
      cout << m (i, j) << " ";
    }
    cout << endl;
  }
  cout << endl;
}

/************************************************************/
/************************************************************/
/*
  N      ijk      jki       kij        block (64)  parallel (8 th.)
  ====   ======   =======   ========   =========   ================
  1024   1485.68  10211.67  1335.38    2678.01     995.31
  1408   4274.75  12913.48  3257.82    5043.87     1637.52
  1792   13666.8  27859.02  6674.43    11399.82    2886.64

  Discussion
  ==========
  For 1024-block matrices, (ijk) and (kij) remained close together in timing.
  This may be due to the cache being large enough to mitigate the losses incurred
  by iterating down a column in the (ijk) version when the matrix was sized at 1024.
  Even at 1024, the (jki) version that iterates down two columns takes nearly a factor
  of 10 in time in comparison. (block) has no real gains, but it still beats (jki).
  (parallel) is the fastest, but its benefits are not obvious until bigger matrices.
  By the 1792 size case, the winners are clearer. (block) now begins to outperform 
  (ijk), probably because the rows are simply becoming too long. (jki) is incredibly slow
  due to its anti-optimization w.r.t. the cache. (kij) still outperforms the other 3, now 
  more obviously by a factor of 2 in comparison to the next fastest (block). But (parallel)
  even outperforms (kij) still by a further factor of 2. This is interesting because the 
  parallel version is forced to use (ijk) multiplication in order to avoid synchronizing adds
  to (i,j) locations in the resultant matrix. The parallel version thus misses out on the 
  best possible ordering for cache optimization obtained in (kij). However, parallelizing so 
  many adds and multiplies is worth the loss in cache efficiency.
*/


