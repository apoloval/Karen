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

#ifndef KAREN_APP_H
#define KAREN_APP_H

#include "utils/collection.h"
#include "utils/platform.h"
#include "utils/string.h"

namespace karen {

/**
 * Application class. This class provides the entry point for an application
 * in Karen. It may be used to initilize the different engines that comprise
 * an application and execute actions to deploy the logic.
 */
class KAREN_EXPORT Application
{
public:

   /**
    * Application properties type. This dictionary type is used to indicate
    * the properties of a Karen application.
    */
   typedef Dictionary<String, String> Properties;
   
   /**
    * The name of the UI engine property.
    */
   static const String UI_ENGINE_PROPERTY;
   
   /**
    * The screen width property.
    */
   static const String SCREEN_WIDTH_PROPERTY;
   
   /**
    * The screen height property.
    */
   static const String SCREEN_HEIGHT_PROPERTY;
   
   /**
    * The fullscreen property.
    */
   static const String FULLSCREEN_PROPERTY;
   
   /**
    * The double buffer property.
    */
   static const String DOUBLE_BUFFER_PROPERTY;
   
   /**
    * Initialize an application object from given properties. If any property 
    * cannot be understanded or applied, a InvalidInputException is raised. If
    * application is arelady initialize, a InvalidStateException is raised.
    */
   static Application& init(const Properties& props) 
         throw (utils::InvalidInputException, utils::InvalidStateException);
         
   /**
    * Run this application.
    */
   virtual void run() = 0;
   
};

}; /* Namespace karen */

#endif
