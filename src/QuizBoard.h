/* QuizBoard -- QuizBoard.h
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
 * The class QuizBoard describes the hardware of the project. In this case
 * this means the keypad and the LEDs. The class distiguishs between regular
 * keys and answer keys. The differtiation is made by an specially formed
 * *key code*. This code is above QuizBoard::OffsetRegularKeys for regular
 * keys. For answer keys, the code is below this value. In addition to that,
 * the code of the answer keys is built by the number of the answer and
 * the number of the question the answer belongs to.
 *
 * ========================================================================
 */
#ifndef __QB_QUIZBOARD_HXX__
#define __QB_QUIZBOARD_HXX__


/* These local include files must be opened in a tab. If not, the headers
 * could not be found! Don't aks me why...
 */
#include "config.h"
#include <Arduino.h>

#include "SimpleLogging.h"

#ifndef CFG_MAX_ANSWERS
#  error "fatal: CFG_MAX_ANSWERS must be defined in config.h"
#endif
#if CFG_MAX_ANSWERS > 4
#  error "todo: change code to allow CFG_MAX_ANSWERS larger than 4"
#endif

#ifndef CFG_MAX_QUESTIONS
#  error "fatal: CFG_MAX_QUESTIONS must be defined in config.h"
#endif


/*
 *
 */
class QuizBoard
{
  /* --------------------------------------------
   * inner data types
   */
  public:
    enum LED {LED_RED1=0, LED_RED2, LED_RED3, LED_RED4, LED_RED5, LED_RED6, LED_RED7, LED_RED8,
              LED_GREEN1, LED_GREEN2, LED_GREEN3, LED_GREEN4, LED_GREEN5, LED_GREEN6, LED_GREEN7, LED_GREEN8,
              LED_STATUS, LED_ALIVE };
    enum ANSWER_STATES {ANS_NONE=0, ANS_WRONG, ANS_CORRECT, MAX_ANSWER_STATES};
    enum KEY {KEY_REGULAR_KEYS=100, KEY_START };
    struct KeyDefinition { int pin; int keycode; };


  /* --------------------------------------------
   * constants declarations
   */
  private:
    const int DebounceTime = 50;                     // [ms] for debouncing keys
    const int OffsetRegularKeys = KEY_REGULAR_KEYS;  // key code were regular keys starts

  public:
    static const int KEY_NONE = -1;
    static const int MAX_ANSWERS = CFG_MAX_ANSWERS;
    static const int MAX_QUESTIONS = CFG_MAX_QUESTIONS;


  /* --------------------------------------------
   * local attributes
   */
  protected:
    int NumOfKeys;
    static KeyDefinition ListOfKeys[];
    int NumOfLEDs;
    static int ListOfLEDs[];
    int CurrentKeyPressed;
    int CurrentKeyChecked;
    long LastReadTime;
    SimpleLogging m_dbg;



  /* --------------------------------------------
   * methods
   */
  public:

    QuizBoard()
    {
	NumOfKeys = 0;                            // not detected
	NumOfLEDs = 0;                            // not detected
	CurrentKeyPressed = KEY_NONE;             // no key pressed
	CurrentKeyChecked = KEY_NONE;             // no key checked
	LastReadTime = 0;
    };

    bool dbg_Check(void)
    {
        if ( CurrentKeyPressed!=KEY_NONE && CurrentKeyChecked!=KEY_NONE )
        {
            m_dbg.log(SimpleLogging::LVL_CRITICAL,"illegal transit state!");
        }
    };

    // to which question belongs the key (0=none)
    int isQuestion ( int KeyNr )
    {
        int i;
        if ( KeyNr < 0 || KeyNr >= OffsetRegularKeys )
	    return 0;
        i = (KeyNr>>2)+1;
        m_dbg.log(SimpleLogging::LVL_DEBUG,"k=%d q=%d",KeyNr,i);
        return i>MAX_QUESTIONS ? 0 : i;
    };

    // to which answer of the question belongs the key (0=none)
    int isAnswer ( int KeyNr )
    {
	int i;
	if ( KeyNr < 0 || KeyNr >= OffsetRegularKeys )
	    return 0;
	i = (KeyNr&0x0003)+1;
	m_dbg.log(SimpleLogging::LVL_DEBUG,"k=%d a=%d",KeyNr,i);
	return i>MAX_ANSWERS ? 0 : i;
    };

    bool isRegularKey ( int KeyNr )
    {
	return ( KeyNr >= OffsetRegularKeys ) ? true : false;
    }

    bool setup ( void );

    void disableLEDs ( void );

    void setLED ( int nr, bool new_state );

    void showAnswer ( int question, int AnswerState );

    int waitKey ( void );
};


/* If this define is defined, we can allocate our code and static data here.
 * This way we avoid libraries and additional CXX files.
 */
#ifdef __ALLOC_STATICS_HERE__

bool QuizBoard::setup ( void )
{
    int i;

#ifdef DEBUG
    m_dbg.setLevel(SimpleLogging::LVL_DEBUG);
#else
    m_dbg.setLevel(SimpleLogging::LVL_WARNING);
#endif
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizBoard::setup");

    /* count the number of keys.
     */
    for ( i=0; ListOfKeys[i].pin>=0; i++ )
    {
	if ( i>=60 )
	{
	    i = 0;
	    break;
	}
    }
    if ( i <= 0 )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"ERROR: QuizBoard: illegal number of keys in list!");
	return false;
    }
    else
	NumOfKeys = i;
    m_dbg.log(SimpleLogging::LVL_DEBUG,"Found %d key definitions!",NumOfKeys);

    /* count the number of LEDs.
     */
    for ( i=0; ListOfLEDs[i]>=0; i++ )
    {
	if ( i>=60 )
	{
	    i = 0;
	    break;
	}
    }
    if ( i <= 0 )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"ERROR: QuizBoard: illegal number of LEDs in list!");
	return false;
    }
    else
	NumOfLEDs = i;
    m_dbg.log(SimpleLogging::LVL_DEBUG,"Found %d LED definitions!",NumOfLEDs);

    // loop through the list and set INPUTs nad OUTPUTs
    for ( i=0; i<NumOfKeys; i++ )
    {
	pinMode(ListOfKeys[i].pin,INPUT_PULLUP);  // input with internal pullup enabled
    }
    for ( i=0; i<NumOfLEDs; i++ )
    {
	pinMode(ListOfLEDs[i],OUTPUT);            // output for LEDs; low means on
	digitalWrite(ListOfLEDs[i],HIGH);         // disable LED
    }

    return true;
}

void QuizBoard::disableLEDs ( void )
{
    for ( int i=0; i<NumOfLEDs; i++ )
    {
	digitalWrite(ListOfLEDs[i],HIGH);         // disable LED
    }
}

/* Simple set one of the LEDs.
 *
 */
void QuizBoard::setLED ( int nr, bool new_state )
{
    if ( nr>=0 && nr<=NumOfLEDs )
    {
	digitalWrite(ListOfLEDs[nr],new_state?LOW:HIGH);
    }
    else
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"ERROR: QuizBoard::setLED: bad parm\n");
    }
}

/* Set the LEDs of the passed question to the passed state.
 */
void QuizBoard::showAnswer ( int question, int AnswerState )
{
    if ( AnswerState<ANS_NONE || AnswerState>=MAX_ANSWER_STATES )
    {
	m_dbg.log(SimpleLogging::LVL_ERROR,"ERROR: QuizBoard::showAnswer: bad parm\n");
	return;
    }
    switch (question)
    {
	case 1:
	    setLED(LED_RED1,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN1,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 2:
	    setLED(LED_RED2,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN2,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 3:
	    setLED(LED_RED3,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN3,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 4:
	    setLED(LED_RED4,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN4,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 5:
	    setLED(LED_RED5,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN5,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 6:
	    setLED(LED_RED6,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN6,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 7:
	    setLED(LED_RED7,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN7,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	case 8:
	    setLED(LED_RED8,(AnswerState==ANS_WRONG)?true:false);
	    setLED(LED_GREEN8,(AnswerState==ANS_CORRECT)?true:false);
	    break;
	default:
	    m_dbg.log(SimpleLogging::LVL_ERROR,"ERROR: QuizBoard::showAnswer: bad parm\n");
    }
}


/* Wait for a key to be pressed. The newly pressed key is returned. If no key
 * is pressed or if the previous key is hold pressed, the function returns
 * KEY_NONE (-1).
 *
 * Restrictions: only one key can be pressed at the same time! The priority is
 * ------------- the order inside the list ListOfKeys[].
 */
int QuizBoard::waitKey (void)
{
    int kstate;
    int i;

    // is the previous key still pressed? Wait for it's release
    if ( CurrentKeyPressed != KEY_NONE )
    {
        kstate = digitalRead(ListOfKeys[CurrentKeyPressed].pin);
        //m_dbg.log(SimpleLogging::LVL_DEBUG,"**rel?=%d",CurrentKeyPressed);
        if ( kstate == HIGH )
        {
            //m_dbg.log(SimpleLogging::LVL_DEBUG,"**release=#%d",CurrentKeyPressed);
	    CurrentKeyPressed = KEY_NONE;
            CurrentKeyChecked = KEY_NONE;
        }
        return KEY_NONE;                             // still pressed, so there's no change.
    }
    else
    {
        // is no key pressed?
        if ( CurrentKeyChecked == KEY_NONE )
        {
            // Look for a pressed one.
            for ( i=0; ListOfKeys[i].pin>=0; i++ )
            {
                kstate = digitalRead(ListOfKeys[i].pin);
                LastReadTime = millis();
                if ( kstate == LOW )
                {
                    // stop searching, we have found a candidate.
                    //m_dbg.log(SimpleLogging::LVL_DEBUG,"**new?=%d",i);
                    CurrentKeyChecked = i;
                    return KEY_NONE;                       // still not "officially" pressed..
                }
            }
        }
        else
        {
            // or check the debounce for this key.
            kstate = digitalRead(ListOfKeys[CurrentKeyChecked].pin);
            if ( kstate==LOW && ((millis()-LastReadTime) > DebounceTime) )
            {
                CurrentKeyPressed = CurrentKeyChecked;
                CurrentKeyChecked = KEY_NONE;
            }
            else if ( kstate==HIGH )
            {
                // the key isn't pressed anymore
                CurrentKeyChecked = KEY_NONE;
                //m_dbg.log(SimpleLogging::LVL_DEBUG,"**lost=%d",i);
            }
        }
    }
    //if ( CurrentKeyPressed != KEY_NONE )
    //    m_dbg.log(SimpleLogging::LVL_DEBUG,"**pressed=#%d",CurrentKeyPressed);
    return ListOfKeys[CurrentKeyPressed].keycode;
}

/* generate a keycode out of the question and answer numbers.
 * Both parameters are counting from 1 but stored counting from 0!
 */
#define QB_GENKCODE(question,answer) (((((question)-1)&0x0007)<<2)|(((answer)-1)&0x0003))

/* List of pin numbers with all the keys. The order must match the key
 * code of the enum QuizBoard::KEY!
 */
QuizBoard::KeyDefinition QuizBoard::ListOfKeys[] =
{
    // answer keys for question  #8
    { pin:22, keycode:QB_GENKCODE(8,4) },
    { pin:23, keycode:QB_GENKCODE(8,3) },
    { pin:24, keycode:QB_GENKCODE(8,2) },
    { pin:25, keycode:QB_GENKCODE(8,1) },

    // answer keys for question  #7
    { pin:26, keycode:QB_GENKCODE(7,4) },
    { pin:27, keycode:QB_GENKCODE(7,3) },
    { pin:28, keycode:QB_GENKCODE(7,2) },
    { pin:29, keycode:QB_GENKCODE(7,1) },

    // answer keys for question  #6
    { pin:30, keycode:QB_GENKCODE(6,4) },
    { pin:31, keycode:QB_GENKCODE(6,3) },
    { pin:32, keycode:QB_GENKCODE(6,2) },
    { pin:33, keycode:QB_GENKCODE(6,1) },

    // answer keys for question  #5
    { pin:34, keycode:QB_GENKCODE(5,4) },
    { pin:35, keycode:QB_GENKCODE(5,3) },
    { pin:36, keycode:QB_GENKCODE(5,2) },
    { pin:37, keycode:QB_GENKCODE(5,1) },

    // answer keys for question  #4
    { pin:38, keycode:QB_GENKCODE(4,4) },
    { pin:39, keycode:QB_GENKCODE(4,3) },
    { pin:40, keycode:QB_GENKCODE(4,2) },
    { pin:41, keycode:QB_GENKCODE(4,1) },

    // answer keys for question  #3
    { pin:42, keycode:QB_GENKCODE(3,4) },
    { pin:43, keycode:QB_GENKCODE(3,3) },
    { pin:44, keycode:QB_GENKCODE(3,2) },
    { pin:45, keycode:QB_GENKCODE(3,1) },

    // answer keys for question  #2
    { pin:46, keycode:QB_GENKCODE(2,4) },
    { pin:47, keycode:QB_GENKCODE(2,3) },
    { pin:48, keycode:QB_GENKCODE(2,2) },
    { pin:49, keycode:QB_GENKCODE(2,1) },

    // answer keys for question  #1
    { pin:50, keycode:QB_GENKCODE(1,4) },
    { pin:51, keycode:QB_GENKCODE(1,3) },
    { pin:52, keycode:QB_GENKCODE(1,2) },
    { pin:53, keycode:QB_GENKCODE(1,1) },

    // other (regular) keys
    { pin:8, keycode:KEY_START },

    { pin:-1, keycode:-1 }
};


/* List of pin numbers with all the LEDs. The order must match the LED
 * number of the enum QuizBoard::LED!
 */
int QuizBoard::ListOfLEDs [] =
{
    // "red" LEDs for wrong answers
    A0, A1, A2, A3, A4, A5, A6, A7,
    // "green" LEDs for correct answers
    A8, A9, A10, A11, A12, A13, A14, A15,
    // other LEDs
    11, 13,					// LED_STATUS, LED_ALIVE
    -1
};

#endif // __ALLOC_STATICS_HERE__
#endif // __QUIZBOARD_HXX__
