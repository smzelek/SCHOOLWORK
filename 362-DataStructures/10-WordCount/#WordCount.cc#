/*
  Filename   : WordCount.cc
  Author     : Steven Zelek
  Course     : CSCI 362-01
  Assignment : Lab 10
  Description: 

  Time Trial Results
                        1          2          3          4          5         AVG

  Unordered Map     1859.38ms  1812.50ms  1796.88ms  2046.88ms  1812.50ms  1865.63ms

  Map               2343.75ms  2312.50ms  2328.12ms  2421.88ms  2375.00ms  2356.25ms

*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <fstream>
#include <iomanip>

/************************************************************/
// Local includes

#include "Timer.cc"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::map;

/************************************************************/
// Function prototypes/global vars/typedefs

typedef map<string, size_t> MapType;

MapType
readToMap (string filename);

void
printMap (const MapType& m);

void
formatText (string& text);

/************************************************************/

int      
main (int argc, char* argv[]) 
{  
  if (argc != 2)
  {
      printf("Error. Call in form: %s <filename.txt>\n", argv[0]);
      exit (-1);
  }
  string file (argv[1]);

  Timer timer;

  timer.start ();
  MapType wordList = readToMap (file);
  timer.stop ();

  printMap (wordList);
  cout << "\nTime: " << timer.getElapsedMs() << " ms" << endl;
  return EXIT_SUCCESS; 
}

/************************************************************/

MapType
readToMap (string filename)
{
  // Safety check for the given file.
  std::ifstream fileStream (filename);
  if (!fileStream)
  {
    cout << "File not found. Exiting." << endl;
    exit (-1);
  }
  
  string text;
  MapType m;
  
  // Reads all strings into a map
  while (fileStream >> text)
  {
    formatText (text);
    if (text != "")
    {
      auto it = m.insert (make_pair (text, 0)).first;
      it->second++;
    }
  }

  return m;
}

/************************************************************/

void
printMap (const MapType& m)
{
  const size_t WIDTH = 30;
  
  cout << std::setw (WIDTH) << std::left << "Word" << std::right << "Count" << endl;
  cout << std::setw (WIDTH) << std::left << "====" << std::right << "=====" << endl;
  
  for (auto& kv : m)
  {
    cout << std::setw (WIDTH) << std::left << kv.first << std::right << kv.second << endl;
  }
}

/************************************************************/

void
formatText (string& text)
{
    size_t j = 0;
    for (size_t i = 0; i < text.length (); ++i)
    {
      if (!ispunct (text[i]))
      {
	text[j] = tolower (text[i]);
	++j;
      }
    }
    text.resize (j);
}

/************************************************************/
/************************************************************/
