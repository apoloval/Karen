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

#ifndef KAREN_UTILS_TIMING_H
#define KAREN_UTILS_TIMING_H

#include "utils/exception.h"

namespace karen { namespace utils {

/**
 * Abstract counter class. This class provides an abstract interface
 * for a time counter. 
 */
class AbstractCounter
{
public:

   /**
    * Virtual destructor.
    */
   inline virtual ~AbstractCounter() {}

   /**
    * Start counting time. Throws a InvalidStateException if counter was
    * already started.
    */
   virtual void start() throw (InvalidStateException) = 0;
   
   /**
    * Stop counting time, returning elapsed time. Throws a 
    * InvalidStateException if counter was already stopped. 
    */
   virtual double stop() throw (InvalidStateException) = 0;

   /**
    * Check whether counter is running.
    */
   virtual bool isRunning() const = 0;

};

/**
 * Counter class. This concrete counter class wraps an actual implementation
 * by encapsulating it with an AbstractCounter interface. 
 */
class Counter : public AbstractCounter
{
public:

   /**
    * Create a new counter object by wrapping an concrete implementation. 
    * This constructor provides an implementation according to runtime
    * platform.
    */
   Counter();
   
   /**
    * Start counting time. Throws a InvalidStateException if counter was
    * already started.
    */
   inline virtual void start()
   throw (InvalidStateException)
   { _impl->start(); }
   
   /**
    * Stop counting time, returning elapsed time. Throws a 
    * InvalidStateException if counter was already stopped. 
    */
   inline virtual double stop()
   throw (InvalidStateException)
   { return _impl->stop(); }
   
   /**
    * Check whether counter is running.
    */
   inline virtual bool isRunning() const
   { return _impl->isRunning(); }

private:

   /*
    * Actual implementation for counter.
    */
   AbstractCounter *_impl;
};

/**
 * Sleep thread for given milliseconds.
 */
void sleepMillis(unsigned long millis);

/**
 * Get the number of milliseconds since Karen engine started. 
 */
double getTimeSinceLaunched();

}} /* namespace karen::utils */

#endif
