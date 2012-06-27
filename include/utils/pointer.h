/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2012 Alvaro Polo
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

#ifndef KAREN_UTILS_POINTER_H
#define KAREN_UTILS_POINTER_H

#include "exception.h"

namespace karen { namespace utils {

/**
 * Smart pointer template class. This template class provides a 
 * count-reference smart pointer. It works like any other pointer type, 
 * allowing the use of operators over the pointed object like it was a real 
 * pointer. The main difference is that memory is automatically deallocated 
 * when all references to the object are lost. To create an smart pointer, 
 * just instantiate the template class with the pointed object as argument 
 * and, then, use other smart pointer or a real pointer to construct it.
 */
template <class T>
class Ptr
{
public:

   /**
    * Default constructor. It creates a null-valued pointer. 
    */
   Ptr() : _refc(NULL), _obj(NULL) {}
   
   /**
    * Value constructor. It creates a new smart pointer from given raw
    * pointer. If given pointer is NULL, the smart pointer is null as well
    * and no reference count is allocated to manage it.
    */
   Ptr(T *value) : _refc(value ? new RefCounter(1) : NULL), _obj(value) {}
   
   /**
    * Copy constructor. This copy constructor creates a new smart pointer
    * instance that shares the reference of the original smart pointer
    * object. 
    */
   Ptr(const Ptr &p) { copy(p); }
   
   /**
    * Move constructor. Move the content of pointer passed as argument
    * to a new instance of pointer.
    */
   Ptr(Ptr&& p) { move(p); }
   
   /**
    * Cast and copy constructor. This constructor accepts a smart pointer
    * instance of another type Other and attempts to cast it into type T.
    * If casting fails, the constructed smart pointer is null.
    */
   //! Cast and copy constructor
   template <class Other>
   Ptr(const Ptr<Other> &p) { copy(p); }
   
   /**
    * Destructor. This destructor checks the count reference in order to 
    * determine whether pointed object should be deallocated. If count
    * reference reachs 0 after this smart pointer destruction, the referenced
    * object is deallocated from memory.
    */
   virtual ~Ptr() { release(); }
   
   /**
    * Assign operator. This operator destroys the old association with pointed
    * object (if any) and creates a new association with the object pointed
    * by p argument.
    */
   Ptr& operator = (const Ptr &p)
   { release(); copy(p); return *this; }

   /**
    * Cast and assign operator. This assign operator accepts a smart pointer
    * instantiated for another template type Other. It attempts to cast
    * the object pointed by p to T. If such casting fails, the this pointer
    * is set to null.
    */
   template <class Other>
   Ptr& operator = (const Ptr<Other> &p)
   { release(); copy(p); return *this; }

   /**
    * Cast and move operator. This assign operator accepts a smart pointer
    * intsntiated for another template type Other. It attempts to cast
    * the object pointed by p to T. If such casting fails, the this pointer
    * is set to null.
    */
   template <class Other>
   Ptr& operator = (Ptr<Other>&& p)
   { release(); move(p); return *this; }
   
   /**
    * Compare operator. Two smart pointers are considered equal if they are
    * pointing to the same object. In order to compare the pointed objects,
    * the smart pointers should be dereferenced and the results should be
    * compared. 
    */
   template <class Other>
   bool operator == (const Ptr<Other> &p) const
   { return this->_refc == p._refc; }
   
   /**
    * Cast operator to raw pointer. This operator converts the smart pointer
    * into a raw C-like pointer. This should be used carefully. Any attempt to
    * deallocate the object by using the C-like pointer with 'delete' operator
    * would derive in memory deallocation issues. 
    */
   operator T* () const { return _obj; }
   
   /**
    * Bool casting operator. The smart pointer may be casted into a boolean 
    * value, which indicates whether the pointer is null (false) or not (true). 
    */
   operator bool () const
   { return isNotNull(); }
   
   /**
    * Check whether smart pointer is null. 
    */
   bool isNull() const
   { return _refc == NULL; }
   
   /**
    * Check whether smart pointer is not null. 
    */
   bool isNotNull() const
   { return _refc != NULL; }
   
   /**
    * Obtain the value of the count reference for this smart pointer. If no
    * object is being reference (the pointer is null), it returns 0. Otherwise,
    * it returns the number of smart pointers that are referencing the same
    * object as this. 
    */
   unsigned int count() const
   { return _refc ? *_refc : 0; }

   /**
    * Pointer-accessor operator. This operator may be used to access the
    * reference object using the same syntax available for raw C-like
    * pointers (e.g., myObj->myMember). If pointer is null, a 
    * NullPointerException is raised. 
    */
   T *operator -> () throw (NullPointerException)
   { return safeObject(); }
   
   /**
    * Pointer-accessor operator. This operator may be used to access the
    * reference object using the same syntax available for raw C-like
    * pointers (e.g., myObj->myMember). If pointer is null, a 
    * NullPointerException is raised. 
    */
   const T *operator -> () const throw (NullPointerException)
   { return safeObject(); }
   
   /**
    * Deference operator. This operator returns a reference to the referenced
    * object. If pointer is null, a NullPointerException is raised. 
    */
   T &operator * () throw (NullPointerException)
   { return *safeObject(); }
   
   /**
    * Deference operator. This operator returns a reference to the referenced
    * object. If pointer is null, a NullPointerException is raised. 
    */
   const T &operator * () const throw (NullPointerException)
   { return *safeObject(); }

   /**
    * Check whether reference object may be casted into given type Other. If 
    * pointer is null, a NullPointerException is raised. 
    */
   template <class Other>
   bool isOfClass() const throw (NullPointerException)
   { return dynamic_cast<Other*>(safeObject()) != NULL; }
   
   /**
    * This function member performs a dynamic casting of the pointer. It
    * returns another smart pointer for Other objects, which object is shared
    * given this smart pointer. If the casting fails, the resulting pointer
    * is null. If pointer is null, a NullPointerException is raised. 
    */
   template <class Other>
   Ptr<Other> dynCasting() throw (NullPointerException)
   {
      Other *cast = dynamic_cast<Other*>(safeObject());
      Ptr<Other> result;
      if (cast)
         result.uncheckedCopy(*this, cast);
      return result;
   }

   /**
    * This function member performs a dynamic casting of the pointer. It
    * returns another smart pointer for Other objects, which object is shared
    * given this smart pointer. If the casting fails, the resulting pointer
    * is null. If pointer is null, a NullPointerException is raised. 
    */
   template <class Other>
   Ptr<const Other> dynCasting() const throw (NullPointerException)
   {
      const Other *cast = dynamic_cast<const Other*>(safeObject());
      Ptr<const Other> result;
      if (cast)
         result.uncheckedCopy(*this, cast);
      return result;
   }

   /* 
    * This friendship is established with the template class Ptr itself
    * and used to allow castings.
    */
   template <class Other> friend class Ptr;   
   
private:

   typedef unsigned int RefCounter;

   RefCounter *_refc;
   T *_obj;
   
   void release()
   {
      if (_refc)
      {
         (*_refc)--;
         if ((*_refc) == 0)
         {  
            if (_obj)
               delete _obj;
            delete _refc;
         }
      }
      _refc = NULL;
      _obj = NULL;
   }
   
   template <class Other>
   void copy(const Ptr<Other> &p)
   {
      this->_refc = p._refc;
      this->_obj  = p._obj;
      if (this->_refc)
         (*this->_refc)++;
   }
   
   template <class Other>
   void move(Ptr<Other>& p)
   {
      this->_refc  = p._refc;
      this->_obj   = p._obj;
      p._refc      = NULL;
      p._obj       = NULL;
   }

   template <class Other, class Obj>
   void uncheckedCopy(const Ptr<Other> &p, Obj *obj)
   {
      this->_refc = p._refc;
      this->_obj  = obj;
      if (this->_refc)
         (*this->_refc)++;
   }

   T *safeObject() throw (NullPointerException)
   {
      if (!_obj) 
         KAREN_THROW(NullPointerException, 
                     "attempt to dereference a null pointer");
      return _obj;
   }   

   const T *safeObject() const throw (NullPointerException)
   { return ((Ptr*) this)->safeObject(); }

};

}}; // namespace karen::utils

using karen::utils::Ptr;

#endif
