/* QuizBoard - SimpleTimer.h
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
 * This class implements a simple software bases timer class. It knows two
 * types of timer. The normal timer event occures after the passed time
 * is gone. It occures only once. A "repeated timer" does the same, but
 * after reading the eventwith geEvent(), it is started again. So these
 * "repeated timers" run infinitly.
 *
 * To avoid things like C++ callbacks (functor) or listener classes with
 * virtual methods, the use a simple get-method to query all occured
 * timer events. This is not as nice as the variants mentioned before, but
 * it is much easier to understand and is costs less ressources.
 *
 * The dispatcher function must be calles within loop(). It returns true
 * if a timer events occured. Than we can use getEvent() to query the ID
 * of this timer. If no timer event has occured, the method returs 0. So
 * all timer IDs must be greater than 0.
 *
 * ========================================================================
 * TODO:
 *
 * ========================================================================
 */
#ifndef __QB_SIMPLETIMER_HXX__
#define __QB_SIMPLETIMER_HXX__

/* These local include files must be opened in a tab. If not, the headers
 * could not be found! Don't aks me why...
 */
#include "config.h"

#include <stdarg.h>
#include <Arduino.h>

#include "SimpleLogging.h"


/* If not predefined in config.h, we use our local default for the max. length
 * of all single logging messages.
 */
#ifndef SZ_QB_MAX_TIMERS
#  define SZ_QB_MAX_TIMERS 5
#endif


/* Simple class implementing software timer.
 *
 */
class SimpleTimer
{
  /* --------------------------------------------
   * inner data types
   */
  //public:
  private:
    struct TimerEvent
    {
	int id;
	unsigned long ticks;
	int reload;
	bool occured;
    };

  /* --------------------------------------------
   * constants declarations
   */
  //private:
  //public:

  /* --------------------------------------------
   * local attributes
   */
  protected:
    TimerEvent m_Timer[SZ_QB_MAX_TIMERS];	// our "software timers"
    //unsigned long m_LocalTicker;		// internal ticker counter
    int m_NumTimers;				// number of active timers
    int m_EventsOccured;			// number of stopped timers
    SimpleLogging m_dbg;

/* --------------------------------------------
   * methods
   */
  protected:

    int findTimer ( int id );
    bool isValid ( int id ) { return id>0 ? true:false; };

  public:

    SimpleTimer();

    //void reset ( void )
    //{
    //	m_LocalTicker = millis();
    //};

    bool dispatch ( void );
    int getEvent ( void );

    bool addTimer ( int id, int seconds, bool repeated = false );
    void killTimer ( int id );
    bool isRunning ( int id );
};


/* If this define is defined, we can allocate our code and static data here.
 * This way we avoid libraries and additional CXX files.
 */
#ifdef __ALLOC_STATICS_HERE__

SimpleTimer::SimpleTimer()
{
#ifdef DEBUG
    m_dbg.setLevel(SimpleLogging::LVL_DEBUG);
#else
    m_dbg.setLevel(SimpleLogging::LVL_WARNING);
#endif
    m_dbg.log(SimpleLogging::LVL_DEBUG,"SimpleTimer::SimpleTimer");
    m_NumTimers = 0;
    m_EventsOccured = 0;
    // m_LocalTicker = 0UL;
    for ( int i=0; i<SZ_QB_MAX_TIMERS; i++ )
    {
	m_Timer[i].id = 0;
	m_Timer[i].ticks = 0UL;
	m_Timer[i].reload = 0;
	m_Timer[i].occured = false;
    }
};


bool SimpleTimer::dispatch ( void )
{
    bool one_occured = false;
    unsigned long curr = millis();
    for ( int i=0; i<SZ_QB_MAX_TIMERS; i++ )
    {
	// walk through all defined timers which are not done
	if ( m_Timer[i].id>0 && !m_Timer[i].occured )
	{
	    // if the timer entry is done,
	    if ( curr >= m_Timer[i].ticks )
	    {
		// raise it "occured" flag
		m_Timer[i].occured = true;
		one_occured = true;
	    }
	}
    }
    return one_occured;
}

int SimpleTimer::getEvent ( void )
{
    int rc = 0;
    for ( int i=0; i<SZ_QB_MAX_TIMERS; i++ )
    {
	// a timer is defined and it's done?
	if ( m_Timer[i].id>0 && m_Timer[i].occured )
	{
	    // so remember it's ID
	    rc = m_Timer[i].id;
	    if ( m_Timer[i].reload > 0 )
	    {
		// if needed, restart the timer with the same slot
		m_Timer[i].ticks = millis() + (unsigned long)(m_Timer[i].reload)*1000;
		m_Timer[i].occured = false;
	    }
	    else
	    {
		// or clear it's slot and return it's ID,
		m_Timer[i].id = 0;
		m_Timer[i].ticks = 0UL;
		m_Timer[i].reload = 0;
		m_Timer[i].occured = false;
	    }
	    return rc;
	}
    }
    return rc;
}


bool SimpleTimer::addTimer ( int id, int seconds, bool repeated )
{
    int free_one = SZ_QB_MAX_TIMERS;

    if ( !isValid(id) )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"addTimer: invalid ID");
	return false;
    }
    if ( seconds <= 0 )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"addTimer: invalid time");
	return false;
    }
    for ( int i=0; i<SZ_QB_MAX_TIMERS; i++ )
    {
	if ( id == m_Timer[i].id )
	{
	    // OK, the timer is already running. So we stop searching
	    // simple start it again with the passed values.
	    free_one = i;
	    break;
	}
	else if ( m_Timer[i].id == 0 )
	{
	    // remember the first free slot if we don't find the timer.
	    if ( free_one == SZ_QB_MAX_TIMERS )
		free_one = i;
	}
    }
    if ( free_one == SZ_QB_MAX_TIMERS )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"addTimer: no free slot");
	return false;
    }
    m_dbg.log(SimpleLogging::LVL_DEBUG,"addTimer: use slot %d",free_one);
    m_Timer[free_one].id = id;
    m_Timer[free_one].occured = false;
    m_Timer[free_one].reload = repeated ? seconds : 0;
    m_Timer[free_one].ticks = millis() + (unsigned long)seconds*1000;
}

void SimpleTimer::killTimer ( int id )
{
    int i;
    if ( !isValid(id) )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"killTimer: invalid ID");
	return;
    }
    i = findTimer(id);
    if ( i >= 0 )
    {
	m_Timer[i].id = 0;
	m_Timer[i].ticks = 0UL;
	m_Timer[i].reload = 0;
	m_Timer[i].occured = false;
    }
    else
	m_dbg.log(SimpleLogging::LVL_DEBUG,"killTimer: not found");
}

bool SimpleTimer::isRunning ( int id )
{
    bool rc = false;
    int i;
    if ( !isValid(id) )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"isRunning: invalid ID");
	return false;
    }
    i = findTimer(id);
    if ( i >= 0 )				// OK, we have found it
    {
	if ( !m_Timer[i].occured )		// not done yet?
	    rc = true;				// so it's running
    }
    return rc;
}

int SimpleTimer::findTimer ( int id )
{
    for ( int i=0; i<SZ_QB_MAX_TIMERS; i++ )
    {
	if ( id == m_Timer[i].id )
	    return i;
    }
    return -1;
}

#endif // __ALLOC_STATICS_HERE__
#endif // __QB_SIMPLELOGGING_HXX__
