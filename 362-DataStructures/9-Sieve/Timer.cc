/*
  Filename   : Timer.cc
  Author     : Steven Zelek
  Course     : CSCI 362-01
  Assignment : 
  Description: 
*/   

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>

/************************************************************/
// Local includes

#include "Timer.h"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;

/************************************************************/

Timer::Timer ()
  : m_start{0,0} , m_stop{0,0}
{
}

/************************************************************/

void
Timer::reset ()
{
  m_start.tv_sec = 0;
  m_start.tv_nsec = 0;

  m_stop.tv_sec = 0;
  m_stop.tv_nsec = 0;
}

/************************************************************/

void
Timer::start () 
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_start);
}

/************************************************************/

void
Timer::stop () 
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_stop);
}

/************************************************************/
// Return elapsed time in milliseconds
double
Timer::getElapsedMs () const
{
  double seconds = m_stop.tv_sec - m_start.tv_sec; 
  double nseconds = m_stop.tv_nsec - m_start.tv_nsec;

  return (seconds * Timer::MILLISECONDS_PER_SECOND) + (nseconds / Timer::NANOSECONDS_PER_MILLISECOND);
}

/************************************************************/
/************************************************************/
