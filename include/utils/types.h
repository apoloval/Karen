/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#ifndef KAREN_UTILS_TYPES_H
#define KAREN_UTILS_TYPES_H

namespace karen { namespace utils {

/**
 * Nullable class. This class allows a generic type to be wrapped in a way
 * that it accepts the null state. The wrapped object may be used as it
 * wasn't wrapped by either using the pointer-accessor operator or using
 * the cast operator. 
 */
template <class T>
class Nullable
{
public:

   /**
    * Create a new nullable object without any wrapped object. This
    * nullable is set to null. 
    */
   Nullable() : _null(true) {}
   
   /**
    * Create a new nullable object with a wrapped object. This nullable
    * object is not null. 
    */
   Nullable(const T &val) : _value(val), _null(false) {}
   
   /**
    * Cast to T operator. 
    */
   operator const T &() const { return _value; }
   
   /**
    * Cast to T operator. 
    */
   operator T &() { return _value; }
   
   /**    
    * Check whether nullable object is null. 
    */
   bool isNull() const { return _null; }
   
   /**
    * Pointer-accessor operator. 
    */
   T *operator ->() { return &_value; }

private:

   T     _value;
   bool  _null;

};

/**
 * Tuple template class. This template class provides a wrapper for a tuple
 * of values. This may be extended by using the DECL_TUPLE macro.
 */
template <class T1, class T2>
class Tuple
{
public:

   /**
    * Value constructor.
    */
   Tuple(const T1 &t1, const T2 &t2) : mFirst(t1), mSecond(t2) {}
   
   /**
    * Obtain the first element of the tuple.
    */
   const T1 &first() const
   { return mFirst; }
   
   /**
    * Obtain the first element of the tuple.
    */
   T1 &first()
   { return mFirst; }
   
   /**
    * Obtain the second element of the tuple.
    */
   const T2 &second() const
   { return mSecond; }
   
   /**
    * Obtain the second element of the tuple.
    */
   T2 &second()
   { return mSecond; }
   
private:

   T1 mFirst;  //!< The first element of the tuple
   T2 mSecond; //!< The second element of the tuple

};

/**
 * DECL_TUPLE macro. This macro function may be used to extend the
 * Tuple template class to provide custom getters for the first
 * and second elements of the tuple. 
 */
#define DECL_TUPLE(classname, type1, type2, getter1, getter2) \
   class classname : public ::karen::utils::Tuple<type1, type2> \
   { \
   public:\
      classname(const type1 &t1, const type2 &t2) \
         : ::geo::utils::Tuple<type1, type2>((type1&) t1, (type2&) t2) {} \
      const type1 &getter1() const { return this->first(); } \
      type1 &getter1() { return this->first(); } \
      const type2 &getter2() const { return this->second(); } \
      type2 &getter2() { return this->second(); } \
   };


}}; // namespace karen::utils

#endif
