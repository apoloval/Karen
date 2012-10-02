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

#ifndef KAREN_CORE_FIRST_CLASS_H
#define KAREN_CORE_FIRST_CLASS_H

namespace karen {

template <typename Left, typename Right>
struct BinaryPredicate
{
   inline virtual ~BinaryPredicate() {}
   
   inline bool operator() (const Left& lhs, const Right& rhs) const
   { return this->evaluate(lhs, rhs); }
   
   inline operator const char* () const
   { return this->str(); }
   
   virtual bool evaluate(const Left& lhs, const Right& rhs) const = 0;
   
   virtual const char* str() const = 0;
};

template <typename Left, typename Right>
struct Equals : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs == rhs; }
   
   inline const char* str() const
   { return "is equals to"; }
};

template <typename Left, typename Right>
struct NotEquals : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs != rhs; }

   inline const char* str() const
   { return "is not equals to"; }
};

template <typename Left, typename Right>
struct LessThan : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs < rhs; }

   inline const char* str() const
   { return "is less than"; }
};

template <typename Left, typename Right>
struct LessThanOrEquals : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs <= rhs; }

   inline const char* str() const
   { return "is less than or equals to"; }
};

template <typename Left, typename Right>
struct GreaterThan : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs > rhs; }

   inline const char* str() const
   { return "is greater than"; }
};

template <typename Left, typename Right>
struct GreaterThanOrEquals : BinaryPredicate<Left, Right>
{
   inline virtual bool evaluate(const Left& lhs, const Right& rhs) const
   { return lhs >= rhs; }

   inline const char* str() const
   { return "is greater than or equals to"; }
};

};

#endif
