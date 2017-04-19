/*
  Filename   : Timer.h
  Author     : Steven Zelek
  Course     : CSCI 362-01
  Assignment : 
  Description: 
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef TIMER_H         
#define TIMER_H

/************************************************************/
// System includes

#include <ctime>
#include <cstdlib>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

/************************************************************/

class Timer
{
public:
  Timer ();

  //Resets elapsed time to zero
  void
  reset ();

  void
  start ();

  void
  stop ();

  //Return elapsed time in milliseconds
  double
  getElapsedMs () const;
  
private:
  
  static const int MILLISECONDS_PER_SECOND = 1000;
  static const int NANOSECONDS_PER_MILLISECOND = 1000000;
  timespec m_start;
  timespec m_stop;
};

// Place all free function prototypes after the class declaration
//   Include only functions that act on the class

/************************************************************/

#endif

/************************************************************/
