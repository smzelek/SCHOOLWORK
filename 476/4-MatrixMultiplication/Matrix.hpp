/************************************************************/
/*
  Author     : Steven Zelek
  Course     : CSCI 476
  Assignment : Lab #4
  Description:
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef MATRIX_H
#define MATRIX_H

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

template <typename T> class Matrix
{
public:

    Matrix (unsigned p_numRows, unsigned p_numCols)
    : m_numRows (p_numRows), m_numCols (p_numCols), m_data (new T[p_numCols * p_numRows])
    {
        // empty
    }

    //Destructor
    ~Matrix ()
    {
        delete[] m_data;
    }

    // Copy constructor
    Matrix (const Matrix& m)
    : m_numRows (m.getNumRows ()), m_numCols (m.getNumCols ()), m_data (new T[m.getNumRows () * m.getNumCols ()])
    {
        std::copy (m.begin (), m.end (), begin ());
    }

    unsigned
    size () const
    {
        return m_numCols * m_numRows;
    }

    T*
    begin () const
    {
        return &m_data[0];
    }

    T*
    end() const
    {
        return begin() + size();
    }

    const unsigned
    getNumRows () const
    {
        return m_numRows;
    }

    const unsigned
    getNumCols () const
    {
        return m_numCols;
    }

    // Copy assignment
    Matrix&
    operator= (const Matrix& m)
    {
        if (&m == this)
        {
            return *this;
        }
        
        delete[] m_data;
        m_data = new T[m.getNumRows () * m.getNumCols ()];
        m_numRows = m.getNumRows ();
        m_numCols = m.getNumCols ();
        
        std::copy (m.begin(), m.end(), begin());
        return *this;
    }

    // Subscript operator (non-const)
    // Do NOT do bounds checking
    T&
    operator () (unsigned row, unsigned col)
    {
        return std::ref(m_data[row * m_numCols + col]);
    }

    // Subscript operator (const)
    // Do NOT do bounds checking
    const T&
    operator () (unsigned row, unsigned col) const
    {
        return std::cref(m_data[row * m_numCols + col]);
    }



private:

    unsigned m_numRows;
    unsigned m_numCols;
    T* m_data;
};

/************************************************************/

#endif

/************************************************************/