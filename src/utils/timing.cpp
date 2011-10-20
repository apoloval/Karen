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

#include "utils/exception.h"
#include "utils/platform.h"
#include "utils/timing.h"

#define KAREN_TIMING_POSIX    1
#define KAREN_TIMING_SDL      2

/*
 * If no timing backend is specified, let's choose a defaulted one 
 * depending on runtime platform.
 */
#ifndef KAREN_TIMING
#  if KAREN_PLATFORM == KAREN_PLATFORM_LINUX
#     define KAREN_TIMING KAREN_TIMING_POSIX
#  elif KAREN_PLATFORM == KAREN_PLATFORM_OSX
#     define KAREN_TIMING KAREN_TIMING_POSIX
#  else
#     error Unknown runtime platform
#  endif
#endif

#if KAREN_TIMING == KAREN_TIMING_POSIX
#  include <sys/time.h>      
#  include <unistd.h>
#elif KAREN_TIMING == KAREN_TIMING_SDL
#  include <SDL/SDL.h>
#endif


namespace karen { namespace utils {

#if KAREN_TIMING == KAREN_TIMING_SDL
class SDLCounter : public AbstractCounter
{
public:

   inline SDLCounter() : _startTime(0) {}

   inline void start() throw (AlreadyStarted)
   {
      if (isRunning())
         throw AlreadyStarted();
      _startTime = SDL_GetTicks();
   }
   
   inline virtual float stop() throw (NotStarted)
   {
      if (!isRunning())
         throw NotStarted();
      Uint32 before = _startTime;
      Uint32 now = SDL_GetTicks();
      _startTime = 0;
      return now - before;
   }
   
   inline virtual bool isRunning() const
   {
      return (_startTime);
   }

private:

   Uint32 _startTime;
   
};
#endif

#if KAREN_TIMING == KAREN_TIMING_POSIX
class PosixCounter : public AbstractCounter
{
public:

   inline PosixCounter()
   { _startTime.tv_sec = _startTime.tv_usec = 0; }
   
   inline void start()
   throw (InvalidStateException)
   {
      if (isRunning())
         KAREN_THROW(InvalidStateException, 
                     "cannot start counter: already started");
      gettimeofday(&_startTime, NULL);
   }
   
   inline virtual double stop()
   throw (InvalidStateException)
   {
      struct timeval now;
      double result;

      if (!isRunning())
         KAREN_THROW(InvalidStateException, 
                     "cannot stop counter: already stopped");

      gettimeofday(&now, NULL);
      
      result = (now.tv_sec - _startTime.tv_sec) * 1000.0 +
               (now.tv_usec - _startTime.tv_usec) / 1000.0;
               
      _startTime.tv_sec = _startTime.tv_usec = 0; 
      
      return result;
   }

   inline virtual bool isRunning() const
   {
      return (_startTime.tv_sec && _startTime.tv_usec);
   }
   
private:

   struct timeval _startTime;
   
};
#endif

Counter::Counter() : _impl(NULL)
{
#if KAREN_TIMING == KAREN_TIMING_SDL
   _impl = new SDLCounter();
#elif KAREN_TIMING == KAREN_TIMING_POSIX
   _impl = new PosixCounter();
#endif
}

void 
sleepMillis(unsigned long millis)
{
#if KAREN_TIMING == KAREN_TIMING_SDL
   SDL_Delay((Uint32) millis);
#elif KAREN_TIMING == KAREN_TIMING_POSIX
   usleep(millis * 1000l);
#endif
}

#if KAREN_TIMING == KAREN_TIMING_POSIX
timeval initKarenStartTime();

timeval initKarenStartTime()
{
   timeval r;
   gettimeofday(&r, NULL);
   return r;
}
#endif

double
getTimeSinceLaunched()
{
#if KAREN_TIMING == KAREN_TIMING_SDL
   return SDL_GetTicks();
#elif KAREN_TIMING == KAREN_TIMING_POSIX
   static timeval startTime = initKarenStartTime();
   timeval now;
   gettimeofday(&now, NULL);
      
   return (now.tv_sec - startTime.tv_sec) * 1000.0 +
          (now.tv_usec - startTime.tv_usec) / 1000.0;
#endif
}

}} /* namespace karen::utils */
