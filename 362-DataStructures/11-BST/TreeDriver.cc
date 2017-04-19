/*
  Filename   : TreeDriver.cc
  Author     : Steve Zelek
  Course     : CSCI 362-01
  Assignment : Lab 11
  Description: File to test all methods of SearchTree.hpp.
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <set>

/************************************************************/
// Local includes

#include "SearchTree.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::string;

/************************************************************/
// Function prototypes/global vars/typedefs
template<typename T>
void
printInfo (SearchTree<T>& t);


void
treeInsert (SearchTree<unsigned>& t, unsigned min, unsigned max);
/************************************************************/

int      
main (int argc, char* argv[]) 
{
  //need to test all members of searchtree piece of the hpp
  
  //original
  SearchTree<unsigned> tree;
  treeInsert (tree, 2, 25);

  //copy
  SearchTree<unsigned> tree2 (tree);

  //assignment
  SearchTree<unsigned> tree3;
  printInfo(tree3);
  
  tree3 = tree2;
  
  cout << "Tree 1:" << endl;
  printInfo (tree);
  
  cout << "\nTree 2:" << endl;
  printInfo (tree2);

  cout << "\nTree 3:" << endl;
  printInfo (tree3);
  
  return EXIT_SUCCESS;
}

/************************************************************/

//prints in order and level order
template<typename T>
void
printInfo (SearchTree<T>& t)
{
  cout << t << endl;
  auto iter = t.begin ();
  while (iter != t.end () && iter.safe())
    cout << *iter++ << " ";
  cout << endl;
  cout << "This tree is " << ( t.empty() ? "" : "NOT ") << "empty"
       << endl << endl; 
}

/************************************************************/

void
treeInsert (SearchTree<unsigned>& t, unsigned min, unsigned max)
{
  if (max - min < 1)
    return;
  
  unsigned mid = (max + min) / 2;

  t.insert (mid);
  
  treeInsert (t, min, mid);
  treeInsert (t, mid + 1, max);

}
/************************************************************/
/************************************************************/
