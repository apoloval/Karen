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

#ifndef KAREN_UTILS_ITERATOR_H
#define KAREN_UTILS_ITERATOR_H

#include "utils/exception.h"
#include "utils/pointer.h"

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
   inline virtual void next() throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException, 
            "cannot move iterator to the next element: null iterator");
      nextAfterNullCheck();
   }
   
   /**
    * Move backward iterator to the previous element
    */
   inline virtual void prev() throw (NullIteratorException)
   {
      if (isNull())
         KAREN_THROW(NullIteratorException, 
            "cannot move iterator to the previous element: null iterator");
      prevAfterNullCheck();
   }
   
   /**
    * Cast operator to boolean type
    */
   inline operator bool() { return !this->isNull(); }
   
private:

   /**
    * Move forward iterator after check that it is not null.
    */
   virtual void nextAfterNullCheck() = 0;
   
   /**
    * Move backward iterator after check that it is not null.
    */
   virtual void prevAfterNullCheck() = 0;
   
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
   inline virtual const T& get() const throw (NullIteratorException)
   {
      if (this->isNull())
         KAREN_THROW(NullIteratorException, 
            "cannot get element from iterator: null iterator");
      return getAfterNullCheck();
   }

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
   
   /**
    * Obtain the element pointed by iterator after a null check.
    */
   virtual const T& getAfterNullCheck() const = 0;

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
   inline virtual T& get() throw (NullIteratorException)
   {
      if (this->isNull())
         KAREN_THROW(NullIteratorException, 
            "cannot get element from iterator: null iterator");
      return getAfterNullCheck();
   }
   
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
   
   virtual Ptr<AbstractConstIterator<T> > toConstIterator() = 0;
   
private:

   /**
    * Obtain the element pointed by iterator after a null check.
    */
   virtual T& getAfterNullCheck() = 0;
   
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
   inline ConstIterator(Ptr<AbstractConstIterator<T> >& impl) : _impl(impl) {}

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
    * Obtain iterator implementation.
    */
   template <class Impl = AbstractConstIterator<T> >
   inline Impl* impl()
   { return dynamic_cast<Impl>((AbstractConstIterator<T>*) _impl); }

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

   /**
    * Move forward iterator to the next element.
    */
   inline virtual void nextAfterNullCheck()
   {
      return _impl->next();
   }
   
   /**
    * Move backward iterator to the previous element.
    */
   inline virtual void prevAfterNullCheck()
   {
      return _impl->prev();
   }
   
   /**
    * Obtain collection element pointed by iterator.
    */
   inline virtual const T& getAfterNullCheck() const
   {
      return _impl->get();
   }

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
   inline Iterator(Ptr<AbstractIterator<T> >& impl) : _impl(impl) {}

   /**
    * Virtual destructor.
    */
   inline virtual ~Iterator() {}
   
   inline operator ConstIterator<T> ()
   { 
      Ptr<AbstractConstIterator<T> > it = _impl->toConstIterator();
      return ConstIterator<T>(it);
   }

   /**
    * Check whether iterator is null.
    */
   inline virtual bool isNull() const
   { return _impl.isNull() || _impl->isNull(); }
   
   /**
    * Obtain iterator implementation.
    */
   template <class Impl = AbstractIterator<T> >
   inline Impl* impl()
   { return dynamic_cast<Impl*>((AbstractIterator<T>*) _impl); }

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
   
   inline virtual Ptr<AbstractConstIterator<T> > toConstIterator()
   { 
      Ptr<AbstractConstIterator<T> > it = _impl->toConstIterator();
      return new ConstIterator<T>(it);
   }
   
private:

   Ptr<AbstractIterator<T>> _impl;

   /**
    * Move forward iterator to the next element.
    */
   inline virtual void nextAfterNullCheck()
   {
      return _impl->next();
   }
   
   /**
    * Move backward iterator to the previous element.
    */
   inline virtual void prevAfterNullCheck()
   {
      return _impl->prev();
   }
   
   /**
    * Obtain collection element pointed by iterator.
    */
   inline virtual T& getAfterNullCheck()
   {
      return _impl->get();
   }
   
};

}} /* namespace karen::utils */

#endif
