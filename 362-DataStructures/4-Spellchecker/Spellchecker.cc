/*
  Filename   : Spellchecker.cc
  Author     : Steven Zelek
  Course     : CSCI 362-01
  Assignment : Lab4
  Description: Allows a user to spell check a given file using
  a predetermined dictionary file. Outputs any misspelled words.
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>

/************************************************************/
// Local includes

#include "Timer.h"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::string;
using std::ifstream;

/************************************************************/
// Function prototypes/global vars/typedefs

// Uses shell sort to sort a given vector "v", ascending.
template<typename T>
void
shellSort (vector<T>& v);

// Uses a binary search to determine if "value" is contained
// within vector "v".
template<typename T>
bool
lookup (const vector<T>& v, const T& value);

// Return true if the vector is sorted, and false otherwise.
template<typename T>
bool
isSorted (const vector<T>& v);

// Iterates through "filename" and adds all elements to vector "v".
template<typename T>
void
readToVector (vector<T>& v, string filename);

// Prints each element of vector "v" on its own line.
template<typename T>
void
printVector (const vector<T>& v);

/************************************************************/
/************************************************************/

template<typename T>
void
shellSort (vector<T>& v)
{
  // Finds the initial h value for shellSort.
  size_t h;
  for (h = 1; h <= v.size () / 4; h = (h * 2) + 1)
    /*empty*/;

  while (h > 0)
  {
    // Performs an insertion sort with the given h value.
    for (size_t index = h; index < v.size (); ++index)
    {
      size_t k = index;
      T elem = v[k];
      while (k >= h && elem < v[k - h])
      {
        v[k] = v[k - h];
	k = k - h;
      }
      v[k] = elem;
    }

    // Reduces h for the next sort.
    h = h / 2;
  }
}

/************************************************************/

template<typename T>
bool
lookup (const vector<T>& v, const T& value)
{
  // Binary search uses v's first and last index.
  // Defined here, rather than passed in as args.
  int first = 0;
  int last = v.size ();
  
  while (first < last)
  {										
    int midIndex = (first + last) / 2;
    T midValue = v[midIndex];

    if (value == midValue)
      return true;
    if (value < midValue)
      last = midIndex;
    else
      first = midIndex + 1;
   }
  
  return false;
}

/************************************************************/

template<typename T>
bool
isSorted (const vector<T>& v)
{
  // Iterates through given vector, and returns false if any
  // pair of adjacent values are not sorted.
  for (size_t i = 0; i < v.size () - 1; ++i)
  {
    if (v[i] > v[i + 1])
    {
      return false;
    }
  }

  // Otherwise, returns true.
  return true;
}

/************************************************************/

template<typename T>
void
readToVector (vector<T>& v, string filename)
{

  // Safety check for the given file.
  ifstream fileStream (filename);
  if (!fileStream)
  {
    cout << "File not found. Exiting." << endl;
    exit (-1);
  }

  T elem;
  // Reads all items of type T into the given vector.
  while (fileStream >> elem)
  {
    v.push_back (elem);
  }
}

/************************************************************/

template<typename T>
void
printVector (const vector<T>& v)
{
  for (T elem : v)
  {
    cout << elem << "\n";
  }
}

/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  cout << "File to spell check ==> ";
  string filename;
  cin >> filename;

  // Attempts to load a file with the supplied name or path.
  ifstream fileStream (filename);
  if (!fileStream)
  {
    cout << "File not found. Exiting." << endl;
    exit (-1);
  }

  // Using filepath of Dictionary.txt as supplied by Dr. Zoppetti,
  // stores all given words in a vector.
  vector<string> definedWords;
  readToVector (definedWords, "/home/faculty/zoppetti/Dictionary.txt");

  // Sorts the vector of all dictionary words and times the process.----v
  Timer timer;
  timer.start ();
  shellSort (definedWords);
  timer.stop ();
  
  // Looks to see if shellSort() was successful and outputs the check.
  cout << "\nSorted? ";
  if (isSorted (definedWords))
  {
    cout << "yes" << endl;
  }
  else
  {
    cout << "no" << endl;
    // Should the program terminate here?
    //return EXIT_FAILURE;
  }

  // Outputs the result of timing the Dictionary shellSort().----------^
  cout << "Sort took " << timer.getElapsedMs () << " ms." << endl;

  // Creates a vector and populates it with any words in the user's
  // file that were not also found in the dictionary file.
  vector<string> misspelledWords;
  string word;
  while (fileStream >> word)
  {
    if (!lookup (definedWords, word))
    {
      misspelledWords.push_back (word);
    }
  }

  // Outputs the misspelled words, if there were any. Otherwise,
  // alerts the user that there were no spelling errors found.
  if (misspelledWords.size () > 0)
  {
    cout << "\n\nMisspelled Words"
         <<   "\n================" << endl;
    printVector (misspelledWords);
  }

  else
  {
    cout << "\nNo mispelled words found." << endl;
  }

  return EXIT_SUCCESS; 
}

/************************************************************/
/************************************************************/
