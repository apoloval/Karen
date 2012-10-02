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

#include <KarenCore/events.h>
#include <KarenCore/test.h>

using namespace karen;

static const int A = 7;
static const float B = 3.1415f;
static const String C = "Hello World!";

KAREN_DECL_EVENT(EventTypeA, int a; float b);
KAREN_DECL_EVENT(EventTypeB, String c);

class DummyEventChannel : public LocalEventChannel
{
public:

   inline void triggerEventA()
   {
      EventTypeA event;
      this->makeEventA(event);
      this->sendEvent(event);
   }

   inline void triggerEventB()
   {
      EventTypeB event;
      this->makeEventB(event);
      this->sendEvent(event);
   }
   
   inline void makeEventA(EventTypeA& event)
   {
      event.a = A;
      event.b = B;
   }
   
   inline void makeEventB(EventTypeB& event)
   {
      event.c = C;
   }
};

class DummyEventConsumer
{
public:

   EventTypeA lastReceivedEventA;
   EventTypeB lastReceivedEventB;

   void onEventA(const EventTypeA& event)
   {
      this->lastReceivedEventA = event;
   }

   void onEventB(const EventTypeB& event)
   {
      this->lastReceivedEventB = event;
   }
};

KAREN_BEGIN_UNIT_TEST(EventsTest);
   KAREN_DECL_TEST(shouldReceiveEventByFunctionMember, 
   {
      DummyEventChannel channel;
      DummyEventConsumer consumer;

      channel.subscribe(&consumer, &DummyEventConsumer::onEventA);
      channel.subscribe(&consumer, &DummyEventConsumer::onEventB);

      channel.triggerEventA();
      channel.triggerEventB();
      assertEquals(A, consumer.lastReceivedEventA.a);
      assertEquals(B, consumer.lastReceivedEventA.b);
      assertEquals(C, consumer.lastReceivedEventB.c);
   });

#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
   KAREN_DECL_TEST(shouldReceiveEventByLambda, 
   {
      DummyEventChannel channel;
      DummyEventConsumer consumer;
      int a;
      float b;
      String c;
      
      channel.subscribe<EventTypeA>([&a, &b] (const EventTypeA& event)
            {
               a = event.a;
               b = event.b;
            });
      channel.subscribe<EventTypeB>([&c] (const EventTypeB& event)
            {
               c = event.c;
            });
      channel.triggerEventA();
      channel.triggerEventB();
      assertEquals(A, a);
      assertEquals(B, b);
      assertEquals(C, c);
   });
#endif

KAREN_END_UNIT_TEST(EventTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   EventsTest unitTest;
   unitTest.run(&rep, nullptr, 0);
}
