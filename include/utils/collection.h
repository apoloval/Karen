/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA
 * 
 * ---------------------------------------------------------------------
 */

#ifndef KAREN_UTILS_COLLECTIONS_H
#define KAREN_UTILS_COLLECTIONS_H

#include "exception.h"
#include "pointer.h"
#include "types.h"

namespace karen { namespace utils {

/**
 * Null iterator exception. This exception is raised when a null iterator
 * is moved or deferenced. 
 */
KAREN_DECL_EXCEPTION(NullIteratorException);

/*
 * Iterator direction enum.
 */
enum IteratorDirection
{
   FORWARD_ITERATOR_DIR,
   BACKWARD_ITERATOR_DIR
};

/**
 * Begin at enumeration. This enumeration describes the possible
 * combinations of how a begin operation may start iterating.
 */
enum BeginAt
{
   BEGIN_AT_HEAD,
   BEGIN_AT_BACK,
};

/**
 * Abstract iterator base. This abstract class provides the base for
 * any iterator, including the common functionality of any iterator
 * for moving forwards and backwards and indicate whether it is null.
 */
template <class T>
class AbstractIteratorBase
{
public:

   /** 
    * Virtual destructor.
    */
   virtual ~AbstractIteratorBase() {}

   /**
    * Check whether iterator is null
    */
   virtual bool isNull() const = 0;
   
   /**
    * Move forward iterator to the next element
    */
   virtual void next() throw (NullIteratorException) = 0;
   
   /**
    * Move backward iterator to the previous element
    */
   virtual void prev() throw (NullIteratorException) = 0;
   
   /**
    * Cast operator to boolean type
    */
   inline operator bool() { return !this->isNull(); }
   
};

/**
 * Abstract iterator class. This class provides an abstraction for a 
 * collection iterator that is able to alter the state of pointed element.
 */
template <class T>
class AbstractIterator : public virtual AbstractIteratorBase<T>
{
public:

   /**
    * Obtain collection element pointed by iterator.
    */
   virtual T& get() throw (NullIteratorException) = 0;
   
   /**
    * Dereference operator
    */
   inline T& operator * ()
   throw (NullIteratorException)
   { return get(); }

   /**
    * Field-access operator
    */
   inline T* operator -> ()
   { return &get(); }
   
};

/** 
 * Abstract constant iterator class. This class provides an abstraction 
 * for a collection iterator unable to modify collection elements. 
 */
template <class T>
class AbstractConstIterator : public virtual AbstractIteratorBase<T>
{
public:

   /**
    * Obtain collection element pointed by iterator.
    */
   virtual const T& get() const throw (NullIteratorException) = 0;

   /**
    * Dereference operator.
    */
   inline const T& operator * () const
   throw (NullIteratorException)
   { return get(); }

   /**
    * Field-access operator.
    */
   inline const T* operator -> () const
   { return &get(); }
   
};

/** 
 * Iterator class. This class is used by any collection to wrap an actual
 * iterator implementation. 
 */
template <class T>
class Iterator : public AbstractIterator<T>
{
public:

   /**
    * Create a new null iterator.
    */
   inline Iterator() : _impl(NULL) {}

   /**
    * Create a new iterator by wrapping an actual implementation.
    */
   inline Iterator(AbstractIterator<T>* impl) : _impl(impl) {}

   /**
    * Virtual destructor.
    */
   inline virtual ~Iterator() {}

   /**
    * Check whether iterator is null.
    */
   inline virtual bool isNull() const
   { return _impl.isNull() || _impl->isNull(); }
   
   /**
    * Move forward iterator to the next element.
    */
   inline virtual void next()
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of increment a null iterator");
      return _impl->next();
   }
   
   /**
    * Move backward iterator to the previous element.
    */
   inline virtual void prev()
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of decrement a null iterator");
      return _impl->prev();
   }
   
   /**
    * Obtain collection element pointed by iterator.
    */
   inline virtual T& get()
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of dereference a null iterator");
      return _impl->get();
   }
   
   /**
    * Obtain iterator implementation.
    */
   inline AbstractIterator<T>* impl()
   { return _impl; }

   /**
    * Increment operator.
    */
   inline Iterator& operator ++ ()
   { this->next(); return *this; }
   
   /**
    * Increment operator.
    */
   inline Iterator& operator ++ (int n)
   { this->next(); return *this; }
   
   /**
    * Decrement operator.
    */
   inline Iterator& operator -- ()
   { this->prev(); return *this; }
   
   /**
    * Decrement operator.
    */
   inline Iterator& operator -- (int n)
   { this->prev(); return *this; }
   
private:

   Ptr<AbstractIterator<T>> _impl;

};

/** 
 * Constant iterator class. This class is used by any collection to wrap an 
 * actual constant iterator implementation. 
 */
template <class T>
class ConstIterator : public AbstractConstIterator<T>
{
public:

   /**
    * Create a new null const iterator.
    */
   inline ConstIterator() : _impl(NULL) {}

   /**
    * Create a new const iterator by wrapping an actual implementation.
    */
   inline ConstIterator(AbstractConstIterator<T>* impl) : _impl(impl) {}

   /**
    * Virtual destructor.
    */
   inline virtual ~ConstIterator() {}

   /**
    * Check whether iterator is null.
    */
   inline virtual bool isNull() const
   { return _impl.isNull() || _impl->isNull(); }
   
   /**
    * Move forward iterator to the next element.
    */
   inline virtual void next()
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of increment a null iterator");
      return _impl->next();
   }
   
   /**
    * Move backward iterator to the previous element.
    */
   inline virtual void prev()
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of decrement a null iterator");
      return _impl->prev();
   }
   
   /**
    * Obtain collection element pointed by iterator.
    */
   inline virtual const T& get() const
   throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException,
            "atempt of dereference a null iterator");
      return _impl->get();
   }

   /**
    * Obtain iterator implementation.
    */
   inline AbstractConstIterator<T>* impl()
   { return _impl; }

   /**
    * Increment operator.
    */
   inline ConstIterator& operator ++ (int n)
   { this->next(); return *this; }
   
   /**
    * Increment operator.
    */
   inline ConstIterator& operator ++ ()
   { this->next(); return *this; }
   
   /**
    * Decrement operator.
    */
   inline ConstIterator& operator -- (int n)
   { this->prev(); return *this; }
   
   /**
    * Decrement operator.
    */
   inline ConstIterator& operator -- ()
   { this->prev(); return *this; }
   
private:

   Ptr<AbstractConstIterator<T>> _impl;

};

/** 
 * Abstract collection class. This class provides an abstraction for any
 * collection class in GEO.
 */
template <class T>
class AbstractCollection
{
public:

   /**
    * Virtual destructor.
    */
   inline virtual ~AbstractCollection() {}

};

/**
 * Associative collection class. This class provides an abstraction for
 * a collection which members are inserted in an associative fashion.
 */
template <class T>
class AssociativeCollection : public AbstractCollection<T>
{
public:

   /**
    * Obtain a constant iterator to the begin of collection.
    */
   virtual ConstIterator<T> begin(BeginAt at = BEGIN_AT_HEAD) const = 0;
   
   /**
    * Obtain a constant iterator to the end of collection.
    */
   virtual ConstIterator<T> end() const = 0;

};

/**
 * Sequential collection class. This class provides an abstraction for
 * a collection which members are inserted in an sequential fashion.
 */
template <class T>
class SequentialCollection : public AbstractCollection<T>
{
public:

   /**
    * Obtain an iterator to the begin of collection.
    */
   virtual Iterator<T> begin(BeginAt at = BEGIN_AT_HEAD) = 0;
   
   /**
    * Obtain a constant iterator to the begin of collection.
    */
   virtual ConstIterator<T> begin(BeginAt at = BEGIN_AT_HEAD) const = 0;
   
   /**
    * Obtain an iterator to the end of collection.
    */
   virtual Iterator<T> end() = 0;

   /**
    * Obtain a constant iterator to the end of collection.
    */
   virtual ConstIterator<T> end() const = 0;

};

/**
 * Linked list template class. This is a low-level implementation 
 * of a linked list aimed to be wrapped by other collection types. This
 * template class should not be used directly in user code.
 */
template <class Data>
class LinkedList
{
public:

   /**
    * Linked list neighbours tuple. This tuple stores the next and
    * previous nodes for a given linked list.
    */
   KAREN_DECL_TUPLE(Neighbours, LinkedList*, LinkedList*, next, prev);

   /**
    * Value constructor.
    */
   inline LinkedList(const Data& d) : _data(d), _next(NULL), _prev(NULL) {}

   /**
    * Value constructor.
    */
   inline LinkedList(const Data& data, LinkedList* next, LinkedList* prev)
      : _data(data), _next(next), _prev(prev) {}
      
   /**
    * Destructor.
    */
   inline virtual ~LinkedList()
   { clear(); }
   
   /**
    * Obtain the number of elements of the list.
    */
   inline unsigned int size() const
   { return _next ? _next->size() + 1 : 1; }
   
   /**
    * Obtain the next sublist in the chain.
    */
   inline LinkedList* next()
   { return _next; }
   
   /**
    * Obtain the previous sublist in the chain.
    */
   inline LinkedList* prev()
   { return _prev; }
   
   /**
    * Obtain the element stored by this node.
    */
   inline const Data& elem() const
   { return _data;}
   
   /**
    * Obtain the element stored by this node.
    */
   inline Data& elem()
   { return _data;}
   
   /**
    * Obtain the first element of the list
    */
   inline LinkedList* first()
   { return _prev ? _prev->first() : this; }
   
   /**
    * Obtain the last element of the list
    */
   inline LinkedList* last()
   { return _next ? _next->last() : this; }
   
   /**
    * Insert element before first element and return it.
    */
   inline LinkedList* insertFirst(const Data& d)
   {
      return _prev ? _prev->insertFirst(d) : 
                     _prev = new LinkedList(d, this, NULL);
   }
   
   /** 
    * Insert element after last element and return it.
    */
   inline LinkedList* insertLast(const Data& d)
   {
      return _next ? _next->insertLast(d) : 
                     _next = new LinkedList(d, NULL, this);
   }
   
   /**
    * Insert a new element before given node and return it.
    */
   inline LinkedList* insertBefore(const Data& d)
   {
      if (_prev)
      {
         LinkedList* lst = new LinkedList(d, this, _prev);
         _prev->_next = lst;
         _prev = lst;
         return lst;
      }
      else
         return insertFirst(d);
   }

   /**
    * Insert a new element after given node and return it.
    */
   inline LinkedList* insertAfter(const Data& d)
   {
      if (_next)
      {
         LinkedList* lst = new LinkedList(d, _next, this);
         _next->_prev = lst;
         _next = lst;
         return lst;
      }
      else
         return insertLast(d);
   }
   
   /**
    * Remove this node from the linked list (unlink). It returns the
    * just unlinked tail.
    */
   inline Neighbours unlink()
   {
      Neighbours result((const LinkedList*&) _next, (const LinkedList*&) _prev);
      if (_prev)
         _prev->_next = _next;
      if (_next)
         _next->_prev = _prev;
      _next = _prev = NULL;
      return result;
   }

   /**
    * Search for an element from head to tail
    */
   inline LinkedList* search(const Data& d)
   { return (d == _data) ? this : (_next ? _next->search(d) : NULL); }

   /**
    * Clear the list removing and releasing all its elements from head to tail.
    */
   inline void clear()
   {
      if (_next)
      {
         _next->clear();
         delete _next;
         _next = NULL;
      }
   }
   
   /**
    * Remove and release the first element of the linked list. It returns
    * the new first element after removal (may be null).
    */
   inline static LinkedList* removeFirst(LinkedList* lst)
   {
      if (lst->_prev)
         return removeFirst(lst->_prev);
      else
      {
         Neighbours nei = lst->unlink();
         delete lst;
         return nei.next();
      }
   }
   
   /**
    * Remove and release the last element of the linked list. It returns
    * the new last element after removal (may be null).
    */
   inline static LinkedList* removeLast(LinkedList* lst)
   {
      if (lst->_next)
         return removeLast(lst->_next);
      else
      {
         Neighbours nei = lst->unlink();
         delete lst;
         return nei.prev();
      }
   }
   
private:

   LinkedList* _next;
   LinkedList* _prev;
   Data        _data;

};

/**
 * List template class. This template class provides an abstraction for a
 * collection implementing a linked list interface. 
 */
template <class T>
class List : public SequentialCollection<T>
{
public:

   /**
    * Iterator class. This class provides an specific iterator for list
    * collection that is able to alter the state of pointed element.
    */
   typedef karen::utils::Iterator<T> Iterator;

   /**
    * Iterator class. This class provides an specific iterator for list
    * collection that is not able to alter the state of pointed element.
    */
   typedef karen::utils::ConstIterator<T> ConstIterator;

   /**
    * Create a new empty list.
    */
   inline List() : _first(NULL), _last(NULL), _numElems(0) {}
   
   /**
    * Create a clone of list passed as argument.
    */
   inline List(const List& l) : List()
   { copy(l); }
   
   /**
    * Virtual destructor.
    */
   inline ~List()
   { clear(); }
   
   /**
    * Copy operator.
    */
   inline List& operator = (const List& l)
   { copy(l); return *this; }
   
   /**
    * Obtain list size. 
    */
   inline unsigned int size() const
   { return _numElems; }
   
   /**
    * Insert an element at list head.
    */
   inline void toHead(const T& t)
   {
      if (_first)
         _first = _first->insertFirst(t);
      else
         _last = _first = new LinkedList<T>(t, NULL, NULL);
      _numElems++;
   }
   
   /**
    * Insert an element at list tail.
    */
   inline void toTail(const T& t)
   {
      if (_last)
         _last = _last->insertLast(t);
      else
         _first = _last = new LinkedList<T>(t, NULL, NULL);
      _numElems++;
   }
   
   /**
    * Obtain element at list head.
    */
   inline T& head()
   throw (InvalidStateException)
   {
      if (!_first) 
         KAREN_THROW(InvalidStateException, 
                     "atempt to obtain the head of an empty list");
      return _first->elem();
   }
   
   /**
    * Obtain element at list head.
    */
   inline const T& head() const
   throw (InvalidStateException)
   {
      if (!_first) 
         KAREN_THROW(InvalidStateException, 
                     "atempt to obtain the head of an empty list");
      return _first->elem();
   }
   
   /**
    * Obtain element at list tail.
    */
   inline T& tail()
   throw (InvalidStateException)
   {
      if (!_last) 
         KAREN_THROW(InvalidStateException, 
                     "atempt to obtain the tail of an empty list");
      return _last->elem();
   }
   
   /**
    * Obtain element at list tail.
    */
   inline const T& tail() const
   throw (InvalidStateException)
   {
      if (!_last) 
         KAREN_THROW(InvalidStateException, 
                     "atempt to obtain the tail of an empty list");
      return _last->elem();
   }

   /**
    * Obtain an iterator to list head.
    */
   inline virtual Iterator begin(BeginAt at = BEGIN_AT_HEAD)
   { 
      return Iterator(new ListIterator(
         this, (at == BEGIN_AT_HEAD) ? _first : _last));
   }
   
   /**
    * Obtain a constant iterator to list head.
    */
   inline virtual ConstIterator begin(BeginAt at = BEGIN_AT_HEAD) const
   { 
      return ConstIterator(new ConstListIterator(
         this, (at == BEGIN_AT_HEAD) ? _first : _last));
   }
   
   /**
    * Obtain an interator beyond the list tail.
    */
   inline virtual Iterator end()
   { return Iterator(new ListIterator(this)); }
   
   /**
    * Obtain a constant interator beyond the list tail.
    */
   inline virtual ConstIterator end() const
   { return ConstIterator(new ConstListIterator(this)); }
   
   /**
    * Check wheter this list is empty.
    */
   inline bool empty() const
   { return _numElems == 0; }
   
   /**
    * Insert a new element before given iterator.
    */
   inline void insertBefore(const T& t, Iterator& it)
   throw (InvalidInputException)
   {
      LinkedList<T>* n = iteratorToNode(it);
      LinkedList<T>* nn = n->insertBefore(t);
      if (n == _first)
         _first = nn;
      _numElems++;
   }
   
   /**
    * Insert a new element after given iterator.
    */
   inline void insertAfter(const T& t, Iterator& it)
   throw (InvalidInputException)
   {
      LinkedList<T>* n = iteratorToNode(it);
      LinkedList<T>* nn = n->insertAfter(t);
      if (n == _last)
         _last = nn;
      _numElems++;
   }
   
   /**
    * Check wheter this list contains given element.
    */
   inline bool contains(const T& t) const
   { return _first ? _first->search(t) : false; }
   
   /**
    * Remove all occurrences of given element.
    */
   inline bool remove(const T& t)
   {
      bool result = false;
      if (_first)
      {
         LinkedList<T>* n = _first;
         while (n && (n = n->search(t)))
         {
            typename LinkedList<T>::Neighbours nei = n->unlink();
            if (n == _first)
               _first = nei.next();
            if (n == _last)
               _last = nei.prev();
            delete n;
            n = nei.next();
            _numElems--;
            result = true;
         }
      }
      return result;
   }
   
   /** 
    * Remove element pointed by it from list.
    */
   inline void remove(Iterator& it, 
                      IteratorDirection move = FORWARD_ITERATOR_DIR) 
   throw (InvalidInputException)
   {
      LinkedList<T>* n = iteratorToNode(it);
      typename LinkedList<T>::Neighbours nei = n->unlink();
      if (n == _first)
         _first = nei.next();
      if (n == _last)
         _last = nei.prev();
      delete n;
      _numElems--;
      LinkedList<T>* further = (move == FORWARD_ITERATOR_DIR) ? nei.next() : nei.prev();
      it = Iterator(new ListIterator(this, further));
   }
   
   /**
    * Clear all elements from list.
    */
   inline void clear()
   {
      if (_first)
      {
         _first->clear();
         delete _first;
         _first = _last = NULL;
         _numElems = 0;
      }
   }
   
private:

   /**
    * The first node of the list.
    */
   LinkedList<T>* _first;
   
   /** 
    * The last node of the list.
    */
   LinkedList<T>* _last;
   
   /**
    * The number of elements in the list.
    */
   unsigned int _numElems;

   /**
    * List iterator base. This class provides a base for list iterators.
    */
   class ListIteratorBase : public virtual AbstractIteratorBase<T>
   {
   public:
   
      /**
       * Constructor.
       */
      inline ListIteratorBase(const List* l, LinkedList<T>* n) 
       : _list(l), _node(n)
      {
      }
      
      /**
       * Constructor.
       */
      inline ListIteratorBase(const List* l) : ListIteratorBase(l, NULL) {}

      /**
       * Check whether iterator is null.
       */
      inline virtual bool isNull() const
      { return !_node; }
      
      /**
       * Move forward iterator to the next element.
       */
      inline virtual void next()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _node = _node->next();
      }
      
      /**
       * Move backward iterator to the previous element.
       */
      inline virtual void prev()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _node = _node->prev();
      }
      
      /**
       * Obtain referenced list.
       */
      inline List* list()
      { return _list; }
      
      /**
       * Obtain the referenced node.
       */
      inline LinkedList<T>* node()
      { return _node; }
      
      /**
       * Check whether iterator iterates on given list.
       */
      inline bool iteratesOn(const List* l)
      { return _list == l; }

   protected:
   
      LinkedList<T>* _node;
      const List*    _list;
      
   };

   /**
    * List iterator for lists.
    */
   class ListIterator : public ListIteratorBase,
                        public AbstractIterator<T>
   {
   public:
   
      /**
       * Constructor.
       */
      inline ListIterator(const List* l, LinkedList<T>* n) 
       : ListIteratorBase(l, n)
      {
      }

      /**
       * Constructor.
       */
      inline ListIterator(const List* l) : ListIteratorBase(l) {}

      /**
       * Obtain collection element pointed by iterator.
       */
      virtual T& get()
      throw (NullIteratorException)
      {
         if (!this->_node) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return this->_node->elem();
      }
   };   
   
   /**
    * Non-constant iterator for lists.
    */
   class ConstListIterator : public ListIteratorBase,
                             public AbstractConstIterator<T>
   {
   public:
   
      /**
       * Constructor.
       */
      inline ConstListIterator(const List* l, LinkedList<T>* n)
         : ListIteratorBase(l, n) {}

      /**
       * Constructor.
       */
      inline ConstListIterator(const List* l) : ListIteratorBase(l) {}

      /**
       * Obtain collection element pointed by iterator.
       */
      virtual const T& get() const
      throw (NullIteratorException)
      {
         if (!this->_node) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return this->_node->elem();
      }

   };
   
   /**
    * Obtain the linked list node that given iterator is pointing to. If
    * iterator is null, return a null pointer. 
    */
   inline LinkedList<T>* iteratorToNode(Iterator& it)
   throw (InvalidInputException)
   {
      ListIterator* li = dynamic_cast<ListIterator*> (it.impl());
      if (!li)
         KAREN_THROW(InvalidInputException, 
            "atempt of remove on list with an iterator unconnected to the list");
      if (!li->iteratesOn(this))
         KAREN_THROW(InvalidInputException,
            "atempt of remove on list with an iterator unconnected to the list");
      LinkedList<T>* n = li->node();
      if (!n)
         KAREN_THROW(InvalidInputException,
            "atempt of remove on list with a null iterator");
      return n;
   }
   
   /**
    * Copy the contents of given list to this one.
    */
   inline void copy(const List& l)
   {
      clear();
      for (ConstIterator it = l.begin(); it; it++)
         toTail(*it);
   }
   
};

/**
 * Operator for inserting elements at list tail.
 */
template <class T>
List<T>& operator << (List<T>& l, const T& t)
{ l.toTail(t); return l; }

/**
 * Operator for inserting elements at list head.
 */
template <class T>
List<T>& operator >> (const T& t, List<T>& l)
{ l.toHead(t); return l; }

/**
 * Queue template class.
 */
template <class T>
class Queue : public List<T>
{
public:

   /**
    * Create a new empty queue.
    */
   inline Queue() : List<T>() {}
   
   /**
    * Push a new element to queue tail.
    */
   inline void push(const T& t)
   { List<T>::toTail(t); }

   /**
    * Pop an element at queue head.
    */
   inline T pop()
   { 
      T t = List<T>::head();
      Iterator<T> h(List<T>::begin());
      List<T>::remove(h);
      return t;
   }
   
};

/**
 * Operator for pushing elements in queue.
 */
template <class T>
Queue<T>& operator << (Queue<T>& q, const T& t)
{ q.push(t); return q; }

/**
 * Operator for poping elements from queue.
 */
template <class T>
Queue<T>& operator >> (Queue<T>& q, T& t)
{ t = q.pop(); return q; }

/**
 * Default less than operator.
 */
template <class T1, class T2>
struct DefaultLessThan
{
   bool operator ()(const T1& lhs, const T2& rhs) const { return lhs < rhs; }
};

/**
 * Key value pair type. This struct is used to represent key-value pairs.
 */
template <class Key, class Data>
struct KeyValuePair
{
   Key key;       /**< Key. */
   Data value;    /**< Value. */

   /**
    * Value constructor.
    */
   KeyValuePair(const Key& k, const Data& v) : key(k), value(v) {}
   
};

/**
 * Binary search tree base. This class provides a base for a binary search
 * tree that may be used as support for any collection that requeries that
 * kind of structure.
 */
template <class Extension, class Key, class Data, class Holder, 
          class LessThanOp = DefaultLessThan<Key, Key> >
class BinarySearchTreeBase : public KeyValuePair<Key, Holder>
{
public:

   /**
    * Default constructor.
    */
   inline BinarySearchTreeBase(
         const Key& k,
         const Holder& h,
         const LessThanOp& lt,
         Extension* l, 
         Extension* r, 
         Extension* p)
      : KeyValuePair<Key, Holder>(k, h), _left(l), _right(r), 
        _parent(p), _lessThan(lt)
   {
      if (_left) _left->_parent = (Extension*) this;
      if (_right) _right->_parent = (Extension*) this;
   }

   /**
    * Copy constructor.
    */
   inline BinarySearchTreeBase(const BinarySearchTreeBase& t)
      : KeyValuePair<Key, Holder>(t.key, t.value), _left(NULL), _right(NULL), 
        _lessThan(t._lessThan), _parent(NULL)
   {
      if (t._right)
      {
         _right = new Extension(*t._right);
         _right->_parent = (Extension*) this;
      }
      if (t._left)
      {
         _left = new Extension(*t._left);
         _left->_parent = (Extension*) this;
      }
   }
   
   /**
    * Assign operator.
    */
   inline Extension& operator = (const Extension& t)
   { copy(t); }
   
   /**
    * Virtual destructor.
    */
   inline virtual ~BinarySearchTreeBase()
   {
      if (_left) delete _left;
      if (_right) delete _right;
   }

   /**
    * Check amount of elements in set.
    */
   inline unsigned long size() const
   {
      unsigned long r = 1;
      if (_left) r += _left->size();
      if (_right) r += _right->size();
      return r;
   }
   
   /**
    * Obtain the node with minor element recursively.
    */
   inline Extension* minor()
   {
      Extension* n = (Extension*) this;
      while (n->_left)
         n = n->_left;
      return n;
   }
   
   /**
    * Obtain the node with major element recursively.
    */
   inline Extension* major()
   {
      Extension* n = (Extension*) this;
      while (n->_right)
         n = n->_right;
      return n;
   }
   
   /**
    * Return the next subtree following the binary tree order.
    */
   inline Extension* next()
   {
      if (_right)
         return _right->minor();
      else
      {
         Extension* n = (Extension*) this;
         while (n->_parent && n->_parent->_right == n)
            n = n->_parent;
         return n->_parent;
      }      
   }
   
   /**
    * Return the previous subtree following the binary tree order.
    */
   inline Extension* prev()
   {
      if (_left)
         return _left->major();
      else
      {
         Extension* n = (Extension*) this;
         while (n->_parent && n->_parent->_left == n)
            n = n->_parent;
         return n->_parent;
      }      
   }
   
   /**
    * Remove the tree and return that one which substitutes it.
    */
   inline Extension* remove()
   {
      if (_left && _right)
      {
         Extension* t = _right->minor();
         this->value = t->value;
         this->key = t->key;
         t->remove();
         return (Extension*) this;
      }
      else if (_left || _right)
      {
         Extension* rep = _left ? _left : _right;
         replaceInParent(rep);
         _left = _right = _parent = NULL;
         delete this;
         return rep;            
      }
      else
      {
         replaceInParent(NULL);
         _left = _right = _parent = NULL;
         delete this;
         return NULL;
      }
   }
   
   /**
    * Clear the tree by all its children.
    */
   inline void clear()
   {
      if (_left)
      {
         delete _left;
         _left = NULL;
      }
      if (_right)
      {
         delete _right;
         _right = NULL;
      }
      _parent = NULL;
   }
   
   /**
    * Insert an element recursively. Return true if inserted (not dup).
    */
   virtual bool insert(const Key& k, const Data& e) = 0;
      
   /**
    * Search a subtree recursively.
    */
   virtual Extension* search(const Key& k)
   {
      if (!this->_lessThan(k, this->key) && !this->_lessThan(this->key, k))
         return (Extension*) this;
      if (this->_lessThan(k, this->key))
         return this->_left ? this->_left->search(k) : NULL;
      else
         return this->_right ? this->_right->search(k) : NULL;         
   }
   
protected:

   Extension* _left;
   Extension* _right;
   Extension* _parent;
   LessThanOp _lessThan;
   
   /**
    * Copy given tree on this.
    */
   inline void copy(const Extension& t)
   {
      clear();
      if (t._right)
      {
         _right = new Extension(*t._right);
         _right->parent = this;
      }
      if (t.left)
      {
         _left = new Extension(*t._left);
         _left->parent = this;
      }
      this->key   = t.key;
      this->value = t.value;
   }

   /**
    * Replace the given node in parent.
    */
   inline void replaceInParent(Extension* n)
   {
      if (_parent && _parent->_left == this)
         _parent->_left = n;
      else if (_parent && _parent->_right == this)
         _parent->_right = n;
      if (n)
      {
         if (n->_parent)
            n->replaceInParent(NULL);
         n->_parent = _parent;
      }
   }
      
};

/**
 * Binary tree template class.
 */
template <class Key, class Data, class LessThanOp = DefaultLessThan<Key, Key> >
class BinarySearchTree: 
      public BinarySearchTreeBase<BinarySearchTree<Key, Data, LessThanOp>, Key, Data, Data, LessThanOp>
{
public:

   /**
    * Rename for base class. 
    */
   typedef BinarySearchTreeBase<BinarySearchTree<Key, Data, LessThanOp>, 
                                Key, Data, Data, LessThanOp> BaseClass;

   /**
    * Default constructor.
    */
   inline BinarySearchTree(
         const Key& k,
         const Data& v,
         const LessThanOp& lt,
         BinarySearchTree* l, 
         BinarySearchTree* r, 
         BinarySearchTree* p)
    : BaseClass(k, v, lt, l, r, p)
   {
   }

   /**
    * Copy constructor.
    */
   inline BinarySearchTree(const BinarySearchTree& t) : BaseClass(t) {}
   
   /**
    * Insert an element recursively. Return true if inserted (not dup).
    */
   inline virtual bool insert(const Key& k, const Data& e)
   {
      if (this->_lessThan(k, this->key))
      {
         if (this->_left)
            return this->_left->insert(k, e);
         else
         {
            this->_left = new BinarySearchTree(
                  k, e, this->_lessThan, NULL, NULL, this);
            return true;
         }
      }
      else if (this->_lessThan(this->key, k))
      {
         if (this->_right)
            return this->_right->insert(k, e);
         else
         {
            this->_right = new BinarySearchTree(
                  k, e, this->_lessThan, NULL, NULL, this);
            return true;
         }
      }
      return false;
   }
      
};

/**
 * Multi binary search tree template class. This template class provides a
 * low-level implementation of a binary search tree able to store more than
 * one element in each tree node. 
 */
template <class Key, class Data, class LessThanOp = DefaultLessThan<Key, Key> >
class MultiBinarySearchTree : 
      public BinarySearchTreeBase<MultiBinarySearchTree<Key, Data, LessThanOp>, 
                                  Key, Data, LinkedList<Data>*, LessThanOp>
{
public:

   /**
    * Redefinition of MultiBinarySearchTree as base class of
    * MultiBinarySearchTree.
    */
   typedef BinarySearchTreeBase<MultiBinarySearchTree<Key, Data, LessThanOp>, 
                                Key, 
                                Data, 
                                LinkedList<Data>*, 
                                LessThanOp> BaseClass;

   /**
    * Create a new tree with all properties passed as arguments.
    */
   MultiBinarySearchTree(const Key& k,
                         const Data& d,
                         const LessThanOp& lt,
                         MultiBinarySearchTree* l, 
                         MultiBinarySearchTree* r, 
                         MultiBinarySearchTree* p)
      : BaseClass(k, new LinkedList<Data>(d), lt, l, r, p) {}

   /**
    * Copy constructor.
    */
   MultiBinarySearchTree(const MultiBinarySearchTree& t) : BaseClass(t)
   { this->value = t.value->copy(); }
   
   /**
    * Virtual destructor.
    */
   virtual ~MultiBinarySearchTree()
   { if (this->value) delete this->value; }
   
   /**
    * Insert an element recursively. Return true if inserted (not dup).
    */
   virtual bool insert(const Key& k, const Data& e)
   {
      if (this->_lessThan(k, this->key))
      {
         if (this->_left)
            return this->_left->insert(k, e);
         else
            this->_left = new MultiBinarySearchTree(
                  k, e, this->_lessThan, NULL, NULL, this);
      }
      else if (this->_lessThan(this->key, k))
      {
         if (this->_right)
            return this->_right->insert(k, e);
         else
            this->_right = new MultiBinarySearchTree(
                  k, e, this->_lessThan, NULL, NULL, this);
      }
      else
      {
         this->value->insertLast(e);
      }
      return true;
   }

};

/**
 * Set template class. This template class provides an implementation of
 * a set supported by a binary tree.
 */
template <class Data, class LessThanOp = DefaultLessThan<Data, Data> >
class Set : public AssociativeCollection<Data>
{
public:   

   /**
    * Set constant iterator.
    */
   typedef karen::utils::ConstIterator<Data> ConstIterator;

   /**
    * Create a new empty set with default less than operator.
    */
   inline Set() : _tree(NULL), _lessThan(LessThanOp()) {}
   
   /**
    * Create a new set by copying the contents of given one.
    */
   inline Set(const Set& s)
      : _tree(
         s._tree ? new BinarySearchTree<Data, Data, LessThanOp>(*s._tree)
            : NULL),
        _lessThan(s._lessThan) {}
   
   /**
    * Create a new set by specifying the less than operator.
    */
   inline Set(const LessThanOp& lt) : _tree(NULL), _lessThan(lt) {}
   
   /**
    * Virtual destructor.
    */
   inline virtual ~Set()
   { if (_tree) delete _tree; }

   /**
    * Check whether this set is empty.
    */
   inline bool empty() const
   { return size() == 0; }
   
   /**
    * Obtain the number of elements in this set.
    */
   inline unsigned int size() const
   { return _tree ? _tree->size() : 0; }
   
   /**
    * Obtain a constant iterator to the begin of collection.
    */
   inline virtual ConstIterator begin(BeginAt at = BEGIN_AT_HEAD) const
   {
      switch (at)
      {
         case BEGIN_AT_HEAD:
            return ConstIterator(new SetConstIterator(
                  this, 
                  this->_tree ? (this->_tree->minor()) : NULL));
         case BEGIN_AT_BACK:
            return ConstIterator(new SetConstIterator(
                  this, 
                  this->_tree ? (this->_tree->major()) : NULL));
      }
   }
   
   /**
    * Obtain a constant iterator to the end of collection.
    */
   inline virtual ConstIterator end() const
   { return ConstIterator(new SetConstIterator(this, NULL)); }
   
   /**
    * Insert a new element in set.
    */
   inline void insert(const Data& elem)
   {
      if (this->_tree)
         this->_tree->insert(elem, elem);
      else
         this->_tree = new BinarySearchTree<Data, Data, LessThanOp>(
               elem, elem, _lessThan, NULL, NULL, NULL);
   }
   
   /**
    * Remove an element from set. If not found, nothing happens.
    */
   inline void remove(const Data& elem)
   {
      if (this->_tree)
      {
         BinarySearchTree<Data, Data, LessThanOp>* t = 
               this->_tree->search(elem);
         if (t)
         {
            if (t == this->_tree)
               this->_tree = t->remove();
            else
               t->remove();
         }
      }
   }
   
   /**
    * Remove an element from set.
    */
   inline void remove(ConstIterator& it,
                      IteratorDirection move = FORWARD_ITERATOR_DIR) 
   throw (InvalidInputException)
   {
      BinarySearchTree<Data, Data, LessThanOp>* t = this->iteratorToTree(it);
      bool isRoot = (t == this->_tree);
      BinarySearchTree<Data, Data, LessThanOp>* subst = t->remove();
      
      if (isRoot)
         _tree = t;
      it = ConstIterator(new SetConstIterator(
            this, 
            (move == FORWARD_ITERATOR_DIR) ? subst->next() : subst->prev()));
   }
   
   /**
    * Check whether given element is included in set.
    */
   inline bool contains(const Data& elem) const
   { return this->_tree ? this->_tree->search(elem) : false; }
   
   /**
    * Clear the set removing all its elements (if any).
    */
   inline void clear()
   { delete _tree; _tree = NULL; }
   
private:


   /**
    * Set iterator base. This class provides a base for set iterators.
    */
   class SetIteratorBase : public virtual AbstractIteratorBase<Data>
   {
   public:
   
      /**
       * Create a new iterator by specifying the set and element.
       */
      inline SetIteratorBase(
            const Set* s, 
            BinarySearchTree<Data, Data, LessThanOp>* t)
         : _set(s), _tree(t) {}

      /**
       * Check whether iterator is null.
       */
      inline virtual bool isNull() const
      { return !_tree; }
      
      /**
       * Move forward iterator to the next element.
       */
      inline virtual void next()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _tree = _tree->next();
      }
      
      /**
       * Move backward iterator to the previous element.
       */
      inline virtual void prev()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _tree = _tree->prev();
      }
      
      /**
       * Obtain referenced set.
       */
      inline Set* set()
      { return _set; }
      
      /**
       * Obtain the referenced node.
       */
      inline BinarySearchTree<Data, Data, LessThanOp>* tree()
      { return _tree; }
      
      /**
       * Check whether iterator iterates on given set.
       */
      inline bool iteratesOn(const Set* s)
      { return _set == s; }

   protected:
   
      BinarySearchTree<Data, Data, LessThanOp>* _tree;
      const Set* _set;
   };

   /**
    * Non-constant iterator for lists.
    */
   class SetConstIterator : public SetIteratorBase,
                            public AbstractConstIterator<Data>
   {
   public:
   
      /**
       * Create a new iterator by given set and element.
       */
      inline SetConstIterator(const Set* s, 
                       BinarySearchTree<Data, Data, LessThanOp>* t)
       : SetIteratorBase(s, t) {}

      /**
       * Obtain collection element pointed by iterator.
       */
      inline virtual const Data& get() const
      throw (NullIteratorException)
      {
         if (!this->_tree) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return this->_tree->value;
      }

   };
   
   /**
    * Obtain the tree pointed by given iterator.
    */
   inline BinarySearchTree<Data, Data, LessThanOp>*
   iteratorToTree(ConstIterator& it)
   throw (InvalidInputException)
   {
      SetConstIterator* si = dynamic_cast<SetConstIterator*> (it.impl());
      if (!si)
         KAREN_THROW(InvalidInputException, 
            "atempt of remove on set with an iterator unconnected to the set");
      if (!si->iteratesOn(this))
         KAREN_THROW(InvalidInputException,
            "atempt of remove on set with an iterator unconnected to the set");
      BinarySearchTree<Data, Data, LessThanOp>* t = si->tree();
      if (!t)
         KAREN_THROW(InvalidInputException,
            "atempt of remove on set with a null iterator");
      return t;
   }

   BinarySearchTree<Data, Data, LessThanOp>*    _tree;
   LessThanOp                                   _lessThan;

};

/**
 * Operator for inserting elements in a set.
 */
template <class T>
Set<T>& operator << (Set<T>& s, const T& t)
{ s.insert(t); return s; }

/**
 * Dictionary template class. This template class provides an implementation
 * of a collection that stores key-value pairs supported by a binary search
 * tree data structure.
 */
template <class Key, class Data, class LessThanOp = DefaultLessThan<Key, Key>>
class Dictionary : public AssociativeCollection<KeyValuePair<Key, Data>>
{
public:

   /**
    * Constant iterator for dictionary collections.
    */
   typedef karen::utils::ConstIterator<KeyValuePair<Key, Data> > ConstIterator;
   
   /**
    * Create a new empty dictionary with default less than operator.
    */
   inline Dictionary() : _tree(NULL), _lessThan(LessThanOp()) {}
   
   /**
    * Create a new dictionary by copying the contents of given C-like array
    * of value-pair structures. 
    */
   inline Dictionary(const KeyValuePair<Key, Data>* values, 
                     unsigned int nvalues)
     : _tree(NULL), _lessThan(LessThanOp())
   { for (int i = 0; i < nvalues; i++) insert(values[i]); }
   
   /**
    * Virtual destructor.
    */
   inline virtual ~Dictionary()
   { clear(); }
   
   /**
    * Index operator.
    */
   inline const Data& operator [] (const Key& k) const 
   throw (NotFoundException)
   { return find(k); }
   
   /**
    * Index operator.
    */
   inline Data& operator [] (const Key& k)
   {
      try { return find(k); }
      catch (NotFoundException& e)
      {
         insert(k, Data());
         return find(k);
      }
   }
   
   /**
    * Check whether dictionary is empty.
    */
   inline bool empty() const
   { return size() == 0; }
   
   /**
    * Return dictionary size (the number of declared entries).
    */
   inline unsigned int size() const
   { return _tree ? _tree->size() : 0; }
   
   /**
    * Insert a key-value pair.
    */
   inline void insert(const KeyValuePair<Key, Data>& kvp)
   {
      if (this->_tree)
         this->_tree->insert(kvp.key, kvp.value);
      else
         this->_tree = new BinarySearchTree<Key, Data, LessThanOp>(
               kvp.key, kvp.value, _lessThan, NULL, NULL, NULL);
   }
   
   /**
    * Insert given value with given key.
    */
   inline void insert(const Key& k, const Data& v)
   { insert(KeyValuePair<Key, Data>(k, v)); }

   /**
    * Find value by given key.
    */
   inline const Data& find(const Key& k) const 
   throw (NotFoundException)
   {
      if (this->_tree)
      {
         BinarySearchTree<Key, Data, LessThanOp>* t = this->_tree->search(k);
         if (t)
            return t->value;
      }
      KAREN_THROW(NotFoundException, "cannot find given key in dictionary");
   }
   
   /**
    * Find value by given key.
    */
   inline Data& find(const Key& k) 
   throw (NotFoundException)
   {
      if (this->_tree)
      {
         BinarySearchTree<Key, Data, LessThanOp>* t = this->_tree->search(k);
         if (t)
            return t->value;
      }
      KAREN_THROW(NotFoundException, "cannot find given key in dictionary");
   }
   
   /**
    * Check whether given key exists.
    */
   inline bool defined(const Key& k) const
   {
      try
      { 
         find(k); 
         return true;
      } 
      catch (const NotFoundException&)
      { return false; }
   }
   
   /**
    * Remove an entry from dictionary. If not found, nothing happens.
    */
   inline void remove(const Key& k)
   {
      if (this->_tree)
      {
         BinarySearchTree<Key, Data, LessThanOp>* t = this->_tree->search(k);
         if (t)
         {
            if (t == this->_tree)
               this->_tree = t->remove();
            else
               t->remove();
         }
      }
   }

   /**
    * Obtain an iterator to the first key-value pair.
    */
   inline ConstIterator begin(BeginAt at = BEGIN_AT_HEAD) const
   {
      switch (at)
      {
         case BEGIN_AT_HEAD:
            return ConstIterator(new DictConstIterator(
                  this, this->_tree ? this->_tree->minor() : this->_tree));
         case BEGIN_AT_BACK:
            return ConstIterator(new DictConstIterator(
                  this, this->_tree ? this->_tree->major() : this->_tree));
      }
   }   
   
   /**
    * Obtain a null iterator to the end of dictionary.
    */
   inline ConstIterator end() const   
   {
      return ConstIterator(new DictConstIterator(this));
   }   

   /**
    * Clear the dictionary removing all its entries (if any).
    */
   inline void clear()
   { delete _tree; _tree = NULL; }
   

private:

   /**
    * Dict iterator base. This class provides a base for dictionary iterators.
    */
   class DictIteratorBase
      : public virtual AbstractIteratorBase<KeyValuePair<Key, Data>>
   {
   public:
   
      /**
       * Create a new iterator by referenced dictionary and tree.
       */
      inline DictIteratorBase(
            const Dictionary* d, 
            BinarySearchTree<Key, Data, LessThanOp>* t)
       : _dict(d), _tree(t)
      {
      }
      
      /**
       * Create a new null iterator by referenced dictionary.
       */
      inline DictIteratorBase(const Dictionary* d) : DictIteratorBase(d, NULL)
      {
      }

      /**
       * Check whether this iterator is null.
       */
      inline virtual bool isNull() const
      { return !_tree; }
      
      /**
       * Move forward iterator to the next element.
       */
      inline virtual void next()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _tree = _tree->next();
      }
      
      /**
       * Move backward iterator to the previous element.
       */
      inline virtual void prev()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         _tree = _tree->prev();
      }
      
      /**
       * Obtain referenced dictionary.
       */
      inline Dictionary* dict()
      { return _dict; }
      
      /**
       * Obtain the referenced tree.
       */
      inline BinarySearchTree<Key, Data, LessThanOp>* tree()
      { return _tree; }
      
      /**
       * Check whether iterator iterates on given set.
       */
      inline bool iteratesOn(const Dictionary* d)
      { return _dict == d; }

   protected:
   
      BinarySearchTree<Key, Data, LessThanOp>*  _tree;
      const Dictionary*                         _dict;
   };

   /**
    * Non-constant iterator for dictionaries.
    */
   class DictConstIterator
      : public DictIteratorBase,
        public AbstractConstIterator<KeyValuePair<Key, Data> >
   {
   public:
   
      /**
       * Create a new iterator from referenced dictionary and element.
       */
      inline DictConstIterator(
            const Dictionary* d, 
            BinarySearchTree<Key, Data, LessThanOp>* t)
       : DictIteratorBase(d, t)
      {
      }
      
      /**
       * Create a new null iterator from referenced dictionary.
       */
      inline DictConstIterator(const Dictionary* d) : DictIteratorBase(d)
      {
      }

      /**
       * Obtain collection element pointed by iterator.
       */
      inline virtual const KeyValuePair<Key, Data>& get() const 
      throw (NullIteratorException)
      {
         if (!this->_tree) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return *(this->_tree);
      }

   };
   
   /**
    * Obtain the tree pointed by given iterator.
    */
   inline BinarySearchTree<Key, Data, LessThanOp>*
   iteratorToTree(ConstIterator& it)
   throw (InvalidInputException)
   {
      DictConstIterator* si = dynamic_cast<DictConstIterator*> (it.impl());
      if (!si)
         KAREN_THROW(InvalidInputException, 
            "atempt of remove on dict with an iterator unconnected to the dict");
      if (!si->iteratesOn(this))
         KAREN_THROW(InvalidInputException,
            "atempt of remove on dict with an iterator unconnected to the dict");
      BinarySearchTree<Key, Data, LessThanOp>* t = si->node();
      if (!t)
         KAREN_THROW(InvalidInputException,
            "atempt of remove on dict with a null iterator");
      return t;
   }

private:

   BinarySearchTree<Key, Data, LessThanOp>*  _tree;
   LessThanOp                                _lessThan;

};

/**
 * Array template class. This template class provides an implementation for
 * a dynamic array.
 */
template <class T>
class Array : public SequentialCollection<T>
{
public:

   /**
    * The size of an array chunk. Internally, array class allocated chunks
    * when it needs to be resized. This constant determines the size of that
    * chunks. 
    */
   #define ARRAY_CHUNK_SIZE 16;
   
   /**
    * Iterator for array collections.
    */
   typedef Iterator<T> Iterator;
   
   /**
    * Constant iterator for array collections.
    */
   typedef ConstIterator<T> ConstIterator;
   
   /**
    * Create a new empty array.
    */
   inline Array() 
      : _data(NULL), _dataSize(0), _dataAlloc(0) {}
   
   /**
    * Create a new array initialized with given amount of given element.
    *
    * @param size The amount of elements to include in the new array
    * @param value The value of initialized elements.
    */
   inline Array(unsigned int size, 
                const T& value = T())
      : _data(NULL), _dataSize(0), _dataAlloc(0)
   {
      realloc(size);
      for (int i = 0; i < size; i++) _data[i] = new T(value);
      _dataSize = size;
   }
   
   /**
    * Create a new array and initialize it with given C-like vector.
    *
    * @param v C-like array of elements
    * @param s Number of elements of s
    */
   inline Array(T v[], unsigned int s)
      : _data(NULL), _dataSize(0), _dataAlloc(0)
   {
      realloc(s);
      for (int i = 0; i < s; i++) _data[i] = new T(v[i]);
      _dataSize = s;
   }
   
   /**
    * Create a new array by copying the given one.
    */
   inline Array(const Array& a)
      : _data(NULL), _dataSize(0), _dataAlloc(0)
   { copy(a); }
   
   /**
    * Virtual destructor.
    */
   inline virtual ~Array()
   { release(); }
   
   /**
    * Copy operator.
    */
   inline Array& operator = (const Array& a)
   { copy(a); return *this; }
   
   /**
    * Obtain an iterator to the begin of collection.
    */
   inline virtual Iterator begin(BeginAt at = BEGIN_AT_HEAD)
   {
      return Iterator(new ArrayIterator(
         this, &_data, &_dataSize, (at == BEGIN_AT_HEAD) ? 0 : _dataSize - 1));
   }
   
   /**
    * Obtain a constant iterator to the begin of collection.
    */
   inline virtual ConstIterator begin(BeginAt at = BEGIN_AT_HEAD) const
   {
      return ConstIterator(new ArrayConstIterator(
         this, &_data, &_dataSize, (at == BEGIN_AT_HEAD) ? 0 : _dataSize - 1));
   }
   
   /**
    * Obtain an iterator to the end of collection.
    */
   inline virtual Iterator end()
   { return Iterator(new ArrayIterator(this)); }

   /**
    * Obtain a constant iterator to the end of collection.
    */
   inline virtual ConstIterator end() const
   { return ConstIterator(new ArrayConstIterator(this)); }

   /**
    * Indexer operator.
    */
   inline T& operator [] (unsigned int i)
   throw (OutOfBoundsException)
   { 
      if (i >= _dataSize)
         KAREN_THROW(OutOfBoundsException,
            "accesing array with index out of bounds");
      return (*_data[i]);
   }
   
   /**
    * Indexer operator.
    */
   inline const T& operator [] (unsigned int i) const 
   throw (OutOfBoundsException)
   { 
      if (i >= _dataSize)
         KAREN_THROW(OutOfBoundsException,
            "accesing array with index out of bounds");
      return *(_data[i]);
   }

   /**
    * Operator for inserting an element at the end of array.
    */
   inline Array& operator += (T t)
   { pushBack(t); return *this; }
   
   /**
    * Obtain the number of elements in the array.
    */
   inline unsigned int size() const
   { return _dataSize; }
   
   /**
    * Check if array is empty (zero-elements).
    */
   inline bool empty() const
   { return size() == 0; }
   
   /**
    * Push an element at the end of array.
    */
   inline void pushBack(const T& t)
   {
      realloc(_dataSize + 1);
      _data[_dataSize++] = new T(t);
   }
   
   /**
    * Clear the array removing all its elements.
    */
   inline void clear()
   { _dataSize = 0; }

private:

   T**            _data;
   unsigned int   _dataSize;
   unsigned int   _dataAlloc;

   /**
    * Array iterator base. This class provides a base for array iterators.
    */
   class ArrayIteratorBase : public virtual AbstractIteratorBase<T>
   {
   public:
   
      /**
       * Create a new iterator from all its properties.
       */
      inline ArrayIteratorBase(const Array* a, 
                               T*** d, 
                               unsigned int* s, 
                               unsigned int i)
       : _array(a), _data(d), _dataSize(s), _index(i) 
      {
      }
         
      /**
       * Check whether iterator is null.
       */
      inline virtual bool isNull() const
      { return !_data; }
      
      /**
       * Move forward iterator to the next element.
       */
      inline virtual void next()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         if (++_index >= *_dataSize)
            _data = NULL;
      }
      
      /**
       * Move backward iterator to the previous element.
       */
      inline virtual void prev()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         if (_index)
            _index--;
         else
            _data = NULL;
      }
      
      /**
       * Obtain referenced array.
       */
      inline Array* array()
      { return _array; }
      
      /**
       * Check whether iterator iterates on given array.
       */
      inline bool iteratesOn(const Array* a)
      { return _array == a; }

   protected:
   
      const Array*   _array;
      T***           _data;
      unsigned int*  _dataSize;
      unsigned int   _index;
      
   };

   /**
    * Iterator implementation for arrays.
    */
   class ArrayIterator : public ArrayIteratorBase,
                         public AbstractIterator<T>
   {
   public:
   
      /**
       * Create a new iterator from all of its properties.
       */
      inline ArrayIterator(const Array* a, 
                           T*** d, 
                           unsigned int* s, 
                           unsigned int i)
       : ArrayIteratorBase(a, d, s, i)
      {
      }

      /**
       * Create a new null iterator from referenced array.
       */
      inline ArrayIterator(const Array* a) : ArrayIterator(a, NULL, NULL, 0) 
      {
      }

      /**
       * Obtain collection element pointed by iterator.
       */
      inline virtual T& get()
      throw (NullIteratorException)
      {
         if (this->isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return *((*this->_data)[this->_index]);
      }
   };   
   
   /**
    * Non-constant iterator for lists.
    */
   class ArrayConstIterator : public ArrayIteratorBase,
                              public AbstractConstIterator<T>
   {
   public:
   
      /**
       * Create a new iterator from all of its properties.
       */
      inline ArrayConstIterator(
            const Array* a, 
            T** const * d, 
            const unsigned int* s,
            unsigned int i)
       : ArrayIteratorBase(a, (T***) d, (unsigned int*) s, i) 
      {
      }

      /**
       * Create a new null iterator from referenced array.
       */
      inline ArrayConstIterator(const Array* a)
       : ArrayConstIterator(a, NULL, NULL, 0) 
      {
      }

      /**
       * Obtain collection element pointed by iterator.
       */
      inline virtual const T& get() const
      throw (NullIteratorException)
      {
         if (this->isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return (*this->_array)[this->_index];
      }

   };
   
   /**
    * Reallocate array storage for given capacity. If current storage is
    * big enough, nothing is done.
    */
   inline bool realloc(unsigned long size)
   {
      bool proceed = false;
      
      while (size >= _dataAlloc)
      {
         proceed = true;
         _dataAlloc += ARRAY_CHUNK_SIZE;
      }
      if (proceed)
      {
         T** former = _data, ** later = new T*[_dataAlloc];
         if (former)
         {
            memcpy(later, former, _dataSize * sizeof(T*));
            delete former;
         }         
         for (int i = _dataSize; i < _dataAlloc; i++)
            later[i] = NULL;
         _data = later;
      }
      return proceed;
   }
   
   /**
    * Copy the contents of array passed as argument to this one.
    */
   inline void copy(const Array& a)
   {
      release();
      if (a._data)
      {
         realloc(a._dataSize);
         for (int i = 0; i < a._dataSize; i++)
            _data[i] = new T(*(a._data[i]));
         _dataSize = a._dataSize;
      }
   }
   
   /**
    * Release allocated memory for this array.
    */
   inline void release()
   {
      if (_data)
      {
         for (int i = 0; i < _dataSize; i++)
            delete _data[i];
         delete _data;
         _data = NULL;
      }
      _dataSize = _dataAlloc = 0;
   }
   
};

/** 
 * Priority queue template class. This template class provides a queue-like
 * collection which order is designated by element priority.
 */
template <class Data, class LessThanOp = DefaultLessThan<Data, Data> >
class PriorityQueue : public AssociativeCollection<Data>
{
public:

   /**
    * Iterator for priority queue.
    */
   typedef karen::utils::ConstIterator<Data> ConstIterator;

   /**
    * Create a new priority queue from given less than operator.
    */
   inline PriorityQueue(LessThanOp lt = LessThanOp()) 
    : _tree(NULL), _numElems(0), _lessThan(lt)
   {
   }
   
   /**
    * Create a new priority queue by copying the contents of given one.
    */
   inline PriorityQueue(const PriorityQueue& q)
    : _tree(NULL), _numElems(0), _lessThan(q._lessThan)
   { copy(q); }
   
   /**
    * Virtual destructor.
    */
   inline virtual ~PriorityQueue()
   { clear(); }
   
   /**
    * Assignment operator.
    */
   inline PriorityQueue& operator = (const PriorityQueue& q)
   { copy(q); }
   
   /**
    * Push a new element in the queue.
    */
   inline void push(const Data& d)
   {
      if (_tree)
      {
         if (_tree->insert(d, d))
            _numElems++;
      }
      else
      {
         _tree = new MultiBinarySearchTree<Data, Data, LessThanOp>(
            d, d, _lessThan, NULL, NULL, NULL);
         _numElems = 1;
      }
   }
   
   /**
    * Pull an element from queue.
    */
   inline Data pull()
   throw (InvalidStateException)
   {
      if (!_tree)
         KAREN_THROW(InvalidStateException, "cannot pull form an empty queue");
      MultiBinarySearchTree<Data, Data, LessThanOp>* t = _tree->major();
      Data d = t->value->first()->elem();
      t->value = LinkedList<Data>::removeFirst(t->value);
      if (!t->value)
      {
         if (t == _tree)
            _tree = t->remove();
         else
            t->remove();
      }
      _numElems--;
      return d;
   }
   
   /**
    * Obtain the next element to be pulled.
    */
   inline const Data& next() const
   throw (InvalidStateException)
   {
      if (!_tree)
         KAREN_THROW(InvalidStateException, "cannot pull form an empty queue");
      return _tree->major()->value->first()->elem();
   }

   /**
    * Get list size.
    */
   inline unsigned long size() const
   { return _numElems; }
   
   /**
    * Check whether the queue is empty.
    */
   inline bool empty() const
   { return _numElems == 0; }
   
   /**
    * Check whether an element is queued.
    */
   inline bool contains(const Data& d) const
   { return _tree ? _tree->search(d) : false; }
   
   /**
    * Find al element in queue.
    */
   inline const Data& find(const Data& d) const
   throw (NotFoundException)
   {
      if (_tree)
      {
         MultiBinarySearchTree<Data, Data, LessThanOp>* t;
         if ((t = _tree->search(d)))
         {
            LinkedList<Data>* lst = t->value->search(d);
            if (lst)
               return lst->elem();
         }
      }
      KAREN_THROW(NotFoundException, "element not found in queue");
   }
   
   /**
    * Remove element from queue.
    */
   inline bool remove(const Data& d)
   {
      MultiBinarySearchTree<Data, Data, LessThanOp>* t;
      if (_tree && (t = _tree->search(d)))
      {
         if (t == _tree)
            _tree = t->remove();
         else
            t->remove();
         _numElems--;
         return true;
      }
      else
         return false;
   }
   
   /**
    * Remove an element from priority queue.
    */
   inline void remove(ConstIterator& it,
                      IteratorDirection move = FORWARD_ITERATOR_DIR) 
   throw (InvalidInputException)
   {
      BinarySearchTree<Data, Data, LessThanOp>* t = this->iteratorToTree(it);
      bool isRoot = (t == this->_tree);
      BinarySearchTree<Data, Data, LessThanOp>* subst = t->remove();
      
      if (isRoot)
         _tree = t;
      it = ConstIterator(new PriorityQueueConstIterator(
            this, 
            (move == FORWARD_ITERATOR_DIR) ? subst->next() : subst->prev()));
   }
   
   /**
    * Clear the queue removing all its elements.
    */
   inline void clear()
   {
      if (_tree)
      {
         delete _tree;
         _tree = NULL;
      }
      _numElems = 0;
   }

   /**
    * Obtain a constant iterator to the begin of collection.
    */
   inline virtual ConstIterator begin(BeginAt at = BEGIN_AT_HEAD) const
   {
      switch (at)
      {
         case BEGIN_AT_HEAD:
            return ConstIterator(new PriorityQueueConstIterator(
                  this, this->_tree ? this->_tree->minor() : NULL));
         case BEGIN_AT_BACK:
            return ConstIterator(new PriorityQueueConstIterator(
                  this, this->_tree ? this->_tree->major() : NULL));
      }
   }
   
   /**
    * Obtain a constant iterator to the end of collection.
    */
   inline virtual ConstIterator end() const
   {
      return ConstIterator(new PriorityQueueConstIterator(
            this, this->_tree ? this->_tree->major() : NULL));
   }

private:

   MultiBinarySearchTree<Data, Data, LessThanOp>*  _tree;
   unsigned long                                   _numElems;
   LessThanOp                                      _lessThan;
   
   /**
    * Priority queue iterator base.
    */
   class PriorityQueueIteratorBase : public virtual AbstractIteratorBase<Data>
   {
   public:
   
      /**
       * Create a new iterator from referenced queue and element.
       */
      inline PriorityQueueIteratorBase(
            const PriorityQueue* q, 
            MultiBinarySearchTree<Data, Data, LessThanOp>* t)
         : _queue(q), _tree(t), _holder(t->value) {}

      /**
       * Check whether iterator is null.
       */
      inline virtual bool isNull() const
      { return !_tree; }
      
      /**
       * Move forward iterator to the next element.
       */
      inline virtual void next()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         if (_holder->next())
            _holder = _holder->next();
         else
         {
            _tree = _tree->next();
            _holder = _tree ? _tree->value->first() : NULL;
         }
      }
      
      /**
       * Move backward iterator to the previous element.
       */
      inline virtual void prev()
      throw (NullIteratorException)
      {
         if (isNull()) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain next element on null iterator");
         if (_holder->prev())
            _holder = _holder->prev();
         else
         {
            _tree = _tree->prev();
            _holder = _tree ? _tree->value->last() : NULL;
         }
      }
      
      /**
       * Obtain referenced queue.
       */
      inline PriorityQueue* queue()
      { return _queue; }
      
      /**
       * Obtain the referenced node.
       */
      inline BinarySearchTree<Data, Data, LessThanOp>* tree()
      { return _tree; }
      
      /**
       * Check whether iterator iterates on given set.
       */
      inline bool iteratesOn(const PriorityQueue* q)
      { return _queue == q; }

   protected:
   
      MultiBinarySearchTree<Data, Data, LessThanOp>*  _tree;
      LinkedList<Data>*                               _holder;
      const PriorityQueue*                            _queue;
   };

   /**
    * Non-constant iterator for priority queue.
    */
   class PriorityQueueConstIterator : public PriorityQueueIteratorBase,
                                      public AbstractConstIterator<Data>
   {
   public:
   
      /**
       * Create a new iterator from referenced queue and element.
       */
      inline PriorityQueueConstIterator(
            const PriorityQueue* q, 
            MultiBinarySearchTree<Data, Data, LessThanOp>* t)
       : PriorityQueueIteratorBase(q, t)
      {
      }

      /**
       * Obtain collection element pointed by iterator.
       */
      virtual const Data& get() const
      throw (NullIteratorException)
      {
         if (!this->_tree) 
            KAREN_THROW(NullIteratorException,
               "atempt of obtain element on null iterator");
         return this->_holder->elem();
      }

   };
   
   /**
    * Obtain the tree pointed by given iterator.
    */
   inline MultiBinarySearchTree<Data, Data, LessThanOp>*
   iteratorToTree(ConstIterator& it)
   throw (InvalidInputException)
   {
      PriorityQueueConstIterator* si = 
            dynamic_cast<PriorityQueueConstIterator*> (it.impl());
      if (!si)
         KAREN_THROW(InvalidInputException, 
            "atempt of remove on priority queue with an unconnected iterator");
      if (!si->iteratesOn(this))
         KAREN_THROW(InvalidInputException,
            "atempt of remove on priority queue with an unconnected iterator");
      MultiBinarySearchTree<Data, Data, LessThanOp>* t = si->tree();
      if (!t)
         KAREN_THROW(InvalidInputException,
            "atempt of remove on priority queue with an unconnected iterator");
      return t;
   }

   /**
    * Copy the contents of given queue to this object.
    */
   inline void copy(const PriorityQueue& q)
   {
      clear();
      _tree = q._tree ? new BinarySearchTree<Data, Data, LessThanOp>(q._tree) 
                      : NULL;
      _numElems = q._numElems;
      _lessThan = q._lessThan;
   }
   
};

}} /* namespace karen::utils */

using karen::utils::List;
using karen::utils::Queue;
using karen::utils::Set;
using karen::utils::PriorityQueue;
using karen::utils::Array;
using karen::utils::Dictionary;

#endif
