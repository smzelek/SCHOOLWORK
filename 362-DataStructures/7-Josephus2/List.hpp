/*
  Filename   : List.hpp
  Author     : Steve Zelek
  Course     : CSCI 362-01
  Assignment : Lab 7
  Description: 
*/   

/************************************************************/
// Macro guard

#ifndef LIST_HPP
#define LIST_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <initializer_list>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::ostream_iterator;
using std::initializer_list;

/************************************************************/

template <typename T>
struct ListNode 
{
  // Make a circular node
  ListNode ()
    : data (),
      next (this),
      prev (this)
  { }
  
  // Initialize the data members
  ListNode (const T& d, ListNode* n = nullptr,
	                ListNode* p = nullptr)
    : data (d),
      next (n),
      prev (p)
  { }
    
  T          data;
  ListNode*  next;
  ListNode*  prev;
};

/************************************************************/

template<typename T>
struct ListIterator
{
  typedef ListIterator<T>                  Self;
  typedef ListNode<T>                      Node;

  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef T                                value_type;
  typedef T*                               pointer;
  typedef T&                               reference;

  ListIterator ()
    : m_nodePtr ()
  { }

  explicit
  ListIterator (Node* n)
    : m_nodePtr (n) 
  { }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  // return address of node's data member
  pointer
  operator-> () const
  { 
    return &m_nodePtr->data;
  }
  
  // pre
  Self&
  operator++ ()
  {
    // Make the iterator point to the next node
    //   and return the updated iterator
    m_nodePtr = m_nodePtr->next;
    return *this;
  }
  
  // post
  Self
  operator++ (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->next;
    return temp;
  }

  Self&
  operator-- ()
  {
    // Make the iterator point to the prev node
    //   and return the updated iterator
    m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  Self
  operator-- (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->prev;
    return temp;
  }
  
  bool
  operator== (const Self& i) const
  {
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  {
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};

/************************************************************/

template<typename T>
struct ListConstIterator
{
  typedef ListConstIterator<T>     Self;
  typedef const ListNode<T>        Node;
  typedef ListIterator<T>          iterator;
  
  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef       T                   value_type;
  typedef const T*                  pointer;
  typedef const T&                  reference;
  
  ListConstIterator ()
    : m_nodePtr ()
  { }
  
  explicit
  ListConstIterator (Node* n)
    : m_nodePtr (n)
  { }
  
  ListConstIterator (const iterator& i)
    : m_nodePtr (i.m_nodePtr)
  { }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  pointer
  operator-> () const
  { 
    return &m_nodePtr->data;
  }

  Self&
  operator++ ()
  {
    // Make the iterator point to the next node
    //   and return the updated iterator
    m_nodePtr = m_nodePtr->next;
    return *this;
  }
  
  Self
  operator++ (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->next;
    return temp;
  }
  
  Self&
  operator-- ()
  {
    // Make the iterator point to the prev node
    //   and return the updated iterator
    m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  Self
  operator-- (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->prev;
    return temp;
  }
  
  bool
  operator== (const Self& i) const
  { 
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  { 
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};

template<typename T>
inline bool
operator== (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{ 
  return i1.m_nodePtr == i2.m_nodePtr;
}

template<typename T>
inline bool
operator!= (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{ 
  return i1.m_nodePtr != i2.m_nodePtr;
}

/************************************************************/

template<typename T>
class List 
{
  typedef ListNode<T>            Node;

public:

  typedef ListIterator<T>        iterator;
  typedef ListConstIterator<T>   const_iterator;
  typedef T                      value_type;
  typedef T*                     pointer;
  typedef T&                     reference;
  typedef const T&               const_reference;

  // Initialize data members
  List ()
    : m_header (),
      m_size (0)
  { }

  List (initializer_list<T> values)
    : m_header (),
      m_size (0)
  {
    // Add all values to the list
    for (auto e: values)
      {
	push_back (e);
      }
  }  

  explicit List (size_t n, const T& defValue = T ())
    : m_header (),
      m_size (0)
  {
    for (size_t i = 0; i < n; ++i)
      {
	push_back (defValue);
      }
  }
  
  List (const pointer first, const pointer last)
    : m_header (),
      m_size (0)
  {
    while (first != last)
    {
      push_back (*first);
      ++first;
    }
  }
      
  List (const List& otherList)
    : m_header (),
      m_size (0)
  {
    auto iter = otherList.begin ();
    while (iter != otherList.end ())
    {
      push_back (*iter);
      ++iter;
    }
  }

  ~List ()
  {
    while (!empty ())
    {
      pop_back ();
    }
    // Deallocate all nodes
  }

  List&
  operator= (const List rhs)
  {
    return *this;
  }
  
  bool    
  empty () const
  {
    return (m_size == 0);
  }
  
  size_t
  size ()  const
  {
    return m_size;
  }

  reference
  front ()
  {
    return *(begin ());
  }

  const_reference
  front () const
  {
    return *(begin ());
  }

  reference
  back ()
  {
    return *(--end ());
  }

  const_reference
  back () const
  {
    return *(--end ());
  }

  void 
  push_front (const_reference item)
  {
    insert (begin (), item);
  }

  void
  pop_front  ()
  {
    erase (begin ());
  }

  void 
  push_back  (const_reference item)
  {
    insert (end (), item);
    //an insert before end signifies an insert just prior to header
  }

  void 
  pop_back   ()
  {
    erase (--end ());
    //
  }

  iterator       
  begin ()
  {
    return iterator (m_header.next);
  }

  const_iterator 
  begin () const
  {
    return iterator (m_header.next);
  }

  iterator       
  end   ()
  {
    return iterator (m_header.prev->next);
  }

  const_iterator 
  end   () const
  {
    return iterator (m_header.prev->next);
  }  

  // insert before "i" and return iterator to new
  iterator 
  insert (iterator i, const T& item)
  {
    //could use auto but this is clearer
    Node* succ = i.m_nodePtr;
    Node* pred = i.m_nodePtr->prev;

    Node* n = new Node (item, succ, pred);

    succ->prev = n;
  
    pred->next = n;
    ++m_size;

    return iterator (n);
  }

  // erase *i, return next pos as iterator
  iterator 
  erase  (iterator i)
  {
    Node* succ = i.m_nodePtr->next;
    Node* pred = i.m_nodePtr->prev;

    pred->next = succ;
    succ->prev = pred;
    
    delete i.m_nodePtr;
    --m_size;
    
    return iterator (succ);
  } 

  // erase [i1, i2)
  iterator 
  erase  (iterator i1, iterator i2)
  {
    while (i1 != i2)
    {
      i1 = erase (i1);
    }

    return i1;
  }
  
private:

  // Dummy header
  Node   m_header;
  size_t m_size;

};

/************************************************************/

// output "list" as ( e1 e2 e3 ... en )
template<typename T>
ostream&
operator<< (ostream& out, const List<T>& list)
{
  out << "( ";
  std::copy (list.begin (), list.end (),
	     ostream_iterator <T> (out, " "));
  out << ')';
  return out;
}

/************************************************************/

#endif

/************************************************************/
