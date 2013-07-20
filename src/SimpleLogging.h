/* QuizBoard - SimpleLogging.h
 * ========================================================================
 * Copyright 2013 Joerg Desch <github@jdesch.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ========================================================================
 *
 * This class implements a simple logging class. All messages must have a
 * so called logging level. The filter level of the object controls,
 * which messages are realy printed and which are ignored.
 *
 * The levels should be used for:
 *
 * CRIT		Critical conditions: the program stops right after this.
 *		Only use this if the program is exited from the current
 *		function.
 * ERROR	Error conditions: serious problem, program continues.
 *		Use just before you return -1 from a function.
 * WARN		Warning conditions: request user to fix this problem.
 *		Ex: What a queer port did you select.
 * NOTICE	Normal but significant condition:
 *		Ex: What options have been set.
 * INFO		Informational
 *		Ex: What functions have been called.
 * DEBUG	Debug-level messages: further debug messages
 *		Ex: what are we going to do in the next few lines of code.
 *
 * ========================================================================
 * TODO:
 * (1) handling of PSTR format strings. I have to see how Arduino is doing this.
 *
 * ========================================================================
 */
#ifndef __QB_SIMPLELOGGING_HXX__
#define __QB_SIMPLELOGGING_HXX__

/* These local include files must be opened in a tab. If not, the headers
 * could not be found! Don't aks me why...
 */
#include "config.h"

#include <stdarg.h>
#include <Arduino.h>


/* If not predefined in config.h, we use our local default for the max. length
 * of all single logging messages.
 */
#ifndef SZ_QB_SIMPLELOG_BUFFER
#  define SZ_QB_SIMPLELOG_BUFFER 50
#endif


/* Simple class implementing logging messages.
 *
 */
class SimpleLogging
{
  /* --------------------------------------------
   * inner data types
   */
  public:
    enum LOGLEVEL {LVL_CRITICAL=0, LVL_ERROR, LVL_WARN, LVL_NOTICE, LVL_INFO, LVL_DEBUG};

  /* --------------------------------------------
   * constants declarations
   */
  //private:
  //public:

  /* --------------------------------------------
   * local attributes
   */
  protected:
    static char m_Buffer[SZ_QB_SIMPLELOG_BUFFER+1];	// the line buffer
    int m_CurrLevel;					// the current level

/* --------------------------------------------
   * methods
   */
  public:
    SimpleLogging()
    {
	m_CurrLevel = LVL_ERROR;
    };

    void setLevel ( int NewLevel )
    {
	if ( NewLevel>=LVL_CRITICAL && NewLevel<=LVL_DEBUG )
	  m_CurrLevel = NewLevel;
    };

    void log ( int lvl, const char* fmt, ...) PRINTF_STYLE_CXX(2,3);

};


/* If this define is defined, we can allocate our code and static data here.
 * This way we avoid libraries and additional CXX files.
 */
#ifdef __ALLOC_STATICS_HERE__


void SimpleLogging::log ( int lvl, const char* fmt, ... )
{
    va_list args;

    if ( lvl > m_CurrLevel )
	return;
    if ( !fmt )
	return;
    va_start(args,fmt);
    vsnprintf(m_Buffer,SZ_QB_SIMPLELOG_BUFFER,fmt,args);
    va_end(args);
    Serial.println(m_Buffer);
}


/* The one and only. Even if we have more instances of this clas, we only
 * need one buffer. This is because we format and print the whole line
 * within one call.
 */
char SimpleLogging::m_Buffer[];

#endif // __ALLOC_STATICS_HERE__
#endif // __QB_SIMPLELOGGING_HXX__
