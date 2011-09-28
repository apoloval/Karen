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

#ifndef KAREN_UTILS_COLLECTION_H
#define KAREN_UTILS_COLLECTION_H

#include "exception.h"
#include "types.h"
#include "pointer.h"

#include <vector>

namespace karen { namespace utils {

/**
 * Null iterator exception. This exception may be raised in an operation
 * not admitted on a null iterator.
 */
KAREN_DECL_EXCEPTION(NullIteratorException);

/**
 * Iterator fingerprint class. This class provides an abstraction of a
 * iterator fingerprint, i.e. a way of univocaly identify an iterator. This
 * is based upon generic properties present in any iterator for comparison
 * purposes. 
 */
class IteratorFingerprint
{
public:

   /**
    * Create a new iterator fingerprint by generic iterator properties. 
    */
   inline IteratorFingerprint(void* collection, void* member)
    : _collection(collection), _member(member)
   {
   }
   
   /**
    * Create a new iterator providing the collection property. 
    */
   inline IteratorFingerprint(void *collection)
    : IteratorFingerprint(collection, NULL) {}
   
   /**
    * Create a new iterator providing no property. 
    */
   inline IteratorFingerprint()
    : IteratorFingerprint(NULL, NULL) {}
   
   /**
    * Copy constructor. 
    */
   inline IteratorFingerprint(const IteratorFingerprint& ifp)
    : _collection(ifp._collection), _member(ifp._member)
   {
   }
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const IteratorFingerprint& ifp) const
   { return (_collection == ifp._collection) && (_member == ifp._member); }

   /**
    * Not equals to operator.
    */
   inline bool operator != (const IteratorFingerprint& ifp) const
   { return !(*this == ifp); }

private:

   void* _collection;
   void* _member;

};

/**
 * Abstract iterator base. This abstract class provides a virtual abstraction
 * of an iterator, i.e. an artifact with the ability of iterate on a 
 * ordered collection.
 */
template <class T>
class AbstractIteratorBase
{
public:

   /**
    * Indicates whether this iterator is null, i.e. either it is pointing
    * to the end of a collection or it has not been initialized. 
    */
   virtual bool isNull() const = 0;

   /**
    * Move the iterator to the next element. If iterator is pointing to
    * the end of the collection, a OutOfBounds exception is raised. 
    */
   virtual AbstractIteratorBase& next() throw (OutOfBoundsException) = 0;

   /**
    * Move the iterator to the previous element. If iterator is pointing to
    * the end of the collection, a OutOfBounds exception is raised. 
    */
   virtual AbstractIteratorBase& prev() throw (OutOfBoundsException) = 0;
   
   /**
    * Obtain iterator fingerprint.
    */
   inline const IteratorFingerprint& fingerprint() const
   { return _fingerprint; }
   
protected:

   /**
    * Set a new fingerprint for this iterator.
    */
   inline void setFingerprint(const IteratorFingerprint &ifp)
   { _fingerprint = ifp; }

private:

   IteratorFingerprint _fingerprint;

};

/**
 * Abstract read-write iterator class. This class provides the abstraction 
 * of a iterator with the ability of read and write the state of the 
 * collection it iterates on.
 */
template <class T>
class AbstractReadWriteIterator : public virtual AbstractIteratorBase<T>
{
public:

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual T& value() throw (NullIteratorException) = 0;

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual const T& value() const throw (NullIteratorException) = 0;

};

/**
 * Abstract const iterator class. This class provides the abstraction of a
 * iterator with the ability of read but not to write the state of the 
 * collection it iterates on.
 */
template <class T>
class AbstractReadOnlyIterator : public virtual AbstractIteratorBase<T>
{
public:

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual const T& value() const throw (NullIteratorException) = 0;

};

/**
 * Base iterator class. This class implements an iterator base for a 
 * multi-collection iterator. 
 */
template <class T>
class IteratorBase : public virtual AbstractIteratorBase<T>
{
public:

   inline IteratorBase(const Ptr<AbstractIteratorBase<T>>& concreteIterator)
   throw (InvalidInputException)
    : _concreteIterator(concreteIterator)
   {
      if (concreteIterator.isNull())
         KAREN_THROW(InvalidInputException, 
            "cannot create a iterator object with a "
            "null implementation object");
   }

   /**
    * Increment operator
    */
   inline IteratorBase& operator ++ () throw (OutOfBoundsException)
   { return _concreteIterator->next(); }

   /**
    * Decrement operator
    */
   inline IteratorBase& operator -- () throw (OutOfBoundsException)
   { return _concreteIterator->previous(); }

   /**
    * Indicates whether this iterator is null, i.e. either it is pointing
    * to the end of a collection or it has not been initialized. 
    */
   virtual bool isNull() const
   { return _concreteIterator->isNull(); }

   /**
    * Move the iterator to the next element. If iterator is pointing to
    * the end of the collection, a OutOfBounds exception is raised. 
    */
   inline virtual IteratorBase& next() throw (OutOfBoundsException)
   { _concreteIterator->next(); return *this; }

   /**
    * Move the iterator to the previous element. If iterator is pointing to
    * the end of the collection, a OutOfBounds exception is raised. 
    */
   inline virtual IteratorBase& prev() throw (OutOfBoundsException)
   { _concreteIterator->prev(); return *this; }

protected:

   /**
    * Return the implementing concrete iterator. 
    */
   inline Ptr<AbstractIteratorBase<T>>& impl() { return _concreteIterator; }

private:

   Ptr<AbstractIteratorBase<T>> _concreteIterator;

};

/**
 * Iterator class. This class provides a generic iterator with the ability
 * of iterating on any ordered collection.
 */
template <class T>
class Iterator : public IteratorBase<T>, public AbstractReadWriteIterator<T>
{
public:

   /**
    * Create a new iterator from a concrete implementation. If given concrete
    * iterator is null, a InvalidInputException is raised. 
    */
   inline Iterator(const Ptr<AbstractReadWriteIterator<T>>& concreteIterator)
   throw (InvalidInputException)
    : IteratorBase<T>(concreteIterator), _concreteIterator(concreteIterator)
   {
   }
   
   /**
    * Copy constructor. 
    */
   inline Iterator(const Iterator& it) : Iterator(it._concreteIterator) {}
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const Iterator& it) const
   {
      return _concreteIterator->fingerprint() == 
             it._concreteIterator->fingerprint();
   }

   /**
    * Not equals to operator.
    */
   inline bool operator != (const Iterator& it) const
   {
      return _concreteIterator->fingerprint() != 
             it._concreteIterator->fingerprint();
   }
   
   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual T& value() throw (NullIteratorException)
   { return _concreteIterator->value(); }

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual const T& value() const throw (NullIteratorException)
   { return _concreteIterator->value(); }

private:

   Ptr<AbstractReadWriteIterator<T>> _concreteIterator;

};

/**
 * Constant iterator class. This class provides a generic iterator with the 
 * ability of iterating on any ordered collection without altering its 
 * contents. 
 */
template <class T>
class ConstIterator : public IteratorBase<T>, public AbstractReadOnlyIterator<T>
{
public:

   /**
    * Create a new iterator from a concrete implementation. If given concrete
    * iterator is null, a InvalidInputException is raised. 
    */
   inline ConstIterator(const Ptr<AbstractReadOnlyIterator<T>>& concreteIterator)
   throw (InvalidInputException)
    : IteratorBase<T>(concreteIterator), _concreteIterator(concreteIterator)
   {
   }
   
   /**
    * Copy constructor. 
    */
   inline ConstIterator(const ConstIterator& it)
    : ConstIterator(it._concreteIterator)
   {}
   
   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual const T& value() const throw (NullIteratorException)
   { return _concreteIterator->value(); }

private:

   Ptr<AbstractReadOnlyIterator<T>> _concreteIterator;

};

template <class T>
class OrderedCollection
{
public:

   /**
    * Obtain an iterator for this collection. 
    */
   virtual Iterator<T> begin() = 0;

   /**
    * Obtain a constant iterator for this collection. 
    */
   virtual ConstIterator<T> begin() const = 0;   

};

template <class T>
class SequentialCollection : public OrderedCollection<T>
{
public:
};


template <class T, class StlContainer, class StlIterator>
class IteratorBaseImpl : public virtual AbstractIteratorBase<T>
{
public:

   /**
    * Create a new iterator from pointed collection and given iterator
    * base. 
    */
   inline IteratorBaseImpl(const StlContainer& stlContainer, 
                           const StlIterator& stlIterator)
    : _stlContainer(&stlContainer), _stlIterator(stlIterator)
   {
   }
   
   inline virtual bool isNull() const
   { return _stlIterator == _stlContainer->end(); }

   inline virtual AbstractIteratorBase<T>& next() 
   throw (OutOfBoundsException)
   {
      if (isNull())
         KAREN_THROW(OutOfBoundsException, 
                     "invoke on prev operation on a null iterator");
      _stlIterator++;
      return *this;
   }

   inline virtual AbstractIteratorBase<T>& prev() 
   throw (OutOfBoundsException)
   {
      if (isNull())
         KAREN_THROW(OutOfBoundsException, 
                     "invoke on prev operation on a null iterator");
      _stlIterator--;
      return *this;
   }
   
protected:

   const StlContainer*  _stlContainer;
   StlIterator          _stlIterator;

};

/**
 * Iterator implementation for array collections.
 */
template <class T, class StlContainer, class StlIterator>
class IteratorImpl : public IteratorBaseImpl<T, StlContainer, StlIterator>,
                     public AbstractReadWriteIterator<T>
{
public:

   /**
    * Create a new iterator from pointed collection and given iterator
    * base. 
    */
   inline IteratorImpl(const StlContainer& stlContainer, 
                       const StlIterator& stlIterator)
    : IteratorBaseImpl<T, StlContainer, StlIterator>(stlContainer, stlIterator)
   {
   }

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   inline virtual T& value() throw (NullIteratorException)
   {
      if (this->isNull())
         KAREN_THROW(NullIteratorException, 
                     "cannot dereference a null iterator");
      return *this->_stlIterator;
   }
   
   /**
    * Obtain the value of the element pointed by this iterator.
    */
   inline virtual const T& value() const throw (NullIteratorException)
   {
      if (this->isNull())
         KAREN_THROW(NullIteratorException, 
                     "cannot dereference a null iterator");
      return *this->_stlIterator;
   }
   
};

/**
 * Iterator implementation for array collections.
 */
template <class T, class StlContainer, class StlIterator>
class ConstIteratorImpl : public IteratorBaseImpl<T, StlContainer, StlIterator>,
                          public AbstractReadOnlyIterator<T>
{
public:

   /**
    * Create a new iterator from pointed collection and given iterator
    * base. 
    */
   inline ConstIteratorImpl(const StlContainer& stlContainer, 
                            const StlIterator& stlIterator)
    : IteratorBaseImpl<T, StlContainer, StlIterator>(stlContainer, stlIterator)
   {
   }

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   inline virtual const T& value() const throw (NullIteratorException)
   {
      if (this->isNull())
         KAREN_THROW(NullIteratorException, 
                     "cannot dereference a null iterator");
      return *this->_stlIterator;
   }
   
};

/**
 * Array template class. This template class provides an abstraction for
 * an array-like collection.
 */
template <class T>
class Array : public SequentialCollection<T>
{
public:

   /**
    * Position data type. This data type provides an abstraction for the
    * position of a vector. 
    */
   typedef unsigned long Position;

   /**
    * Length data type. This data type provides an abstraction for an array
    * length;
    */
   typedef unsigned long Length;

   /**
    * Create an empty array.
    */
   inline Array() : _base() {}
   
   /**
    * Return array length
    */
   inline Length length() const { return _base.size(); }
   
   /**
    * Obtain the element at given array position.
    */
   inline const T& get(Position pos) const throw (OutOfBoundsException)
   { return ((Array*) this)->get(pos); }
   
   /**
    * Obtain the element at given array position.
    */
   inline T& get(Position pos) throw (OutOfBoundsException)
   {
      Length len = length();
      if (pos < len) return _base.at(pos); 
      else 
         KAREN_THROW(OutOfBoundsException, 
            "attempt to get element at position %d in array of %d elements",
            pos, len);
   }
   
   /**
    * Push a new element at the end of the array.
    */
   inline void pushBack(const T& elem)
   { _base.push_back(elem); }
   
   /**
    * Obtain an iterator for this collection. 
    */
   inline virtual Iterator<T> begin()
   {
      return Iterator<T>(
         new IteratorImpl<T, ArrayBase, typename ArrayBase::iterator>(
            _base, _base.begin()));
   }

   /**
    * Obtain a constant iterator for this collection. 
    */
   inline virtual ConstIterator<T> begin() const
   {
      return ConstIterator<T>(
         new ConstIteratorImpl<T, ArrayBase, typename ArrayBase::const_iterator>(
            _base, _base.begin()));
   }

   /**
    * Position-access operator.
    */
   inline const T& operator [] (Position pos) const throw (OutOfBoundsException)
   { return get(pos); }
   
   /**
    * Position-access operator.
    */
   inline T& operator [] (Position pos) throw (OutOfBoundsException)
   { return get(pos); }
   

private:

   typedef std::vector<T> ArrayBase;
   
   ArrayBase _base;
   
};

}} // namespace karen::utils

#endif
