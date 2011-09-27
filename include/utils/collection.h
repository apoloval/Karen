/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
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
 * Abstract iterator class. This class provides the abstraction of a
 * iterator with the ability of modify the state of the collection it iterates
 * on.
 */
template <class T>
class AbstractIterator : public AbstractIteratorBase<T>
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
 * iterator withOUT the ability of modify the state of the collection it 
 * iterates on.
 */
template <class T>
class AbstractConstIterator : public AbstractIteratorBase<T>
{
public:

   /**
    * Obtain the value of the element pointed by this iterator.
    */
   virtual const T& value() const throw (NullIteratorException) = 0;

};

/**
 * Iterator class. This class provides a generic iterator with the ability
 * of iterating on any ordered collection.
 */
template <class T>
class Iterator : public AbstractIterator<T>
{
public:

   /**
    * Create a new iterator from a concrete implementation. If given concrete
    * iterator is null, a InvalidInputException is raised. 
    */
   inline Iterator(const Ptr<AbstractIterator<T>>& concreteIterator)
         throw (InvalidInputException)
    : _concreteIterator(concreteIterator)
   {
      if (concreteIterator.isNull())
         KAREN_THROW(InvalidInputException, 
            "cannot create an instance of Iterator class with a "
            "null concrete iterator");
   }
   
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
    * Increment operator
    */
   inline Iterator& operator ++ () throw (OutOfBoundsException)
   { return _concreteIterator->next(); }

   /**
    * Decrement operator
    */
   inline Iterator& operator -- () throw (OutOfBoundsException)
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
   inline virtual Iterator& next() throw (OutOfBoundsException)
   { _concreteIterator->next(); return *this; }

   /**
    * Move the iterator to the previous element. If iterator is pointing to
    * the end of the collection, a OutOfBounds exception is raised. 
    */
   inline virtual Iterator& prev() throw (OutOfBoundsException)
   { _concreteIterator->prev(); return *this; }

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

   Ptr<AbstractIterator<T>> _concreteIterator;

};

template <class T>
class OrderedCollection
{
public:

   

};

template <class T>
class SequentialCollection : public OrderedCollection<T>
{
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
