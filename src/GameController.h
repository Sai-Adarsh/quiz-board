/* QuizBoard - GameController.h
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
 * This class implements logic of the game controller. It connects the board
 * with the game itself. It also handles all of the keypresses and timers.
 *
 * We have three states implemented in this class:
 *   * "Idle" is the state right after the powerup. The game is not running.
 *   * "Running" means, the game has been started and the system awaits the
 *     answers.
 *   * "Done" means, the game is done and the system shows the result. This
 *     is a transit state. After a while, the system goes in power-down or
 *     switches to "Idle".
 *
 * ========================================================================
 * TODO:
 *
 * ========================================================================
 */
#ifndef __QB_GAMECONTROLLER_HXX__
#define __QB_GAMECONTROLLER_HXX__

/* These local include files must be opened in a tab. If not, the headers
 * could not be found! Don't aks me why...
 */
#include "config.h"

#include <stdarg.h>
#include <Arduino.h>

#include "SimpleLogging.h"
#include "SimpleTimer.h"
#include "QuizBoard.h"
#include "QuizGame.h"

#ifndef CFG_MAX_ANSWERS
#  error "fatal: CFG_MAX_ANSWERS must be defined in config.h"
#endif
#ifndef CFG_MAX_QUESTIONS
#  error "fatal: CFG_MAX_QUESTIONS must be defined in config.h"
#endif
#ifndef CFG_MAX_SOLUTIONSETS
#  error "fatal: CFG_MAX_SOLUTIONSETS must be defined in config.h"
#endif
#ifndef CFG_GAME_TIMEOUT_WARN
#  error "fatal: CFG_GAME_TIMEOUT_WARN must be defined in config.h"
#endif
#ifndef CFG_GAME_TIMEOUT
#    error "fatal: CFG_GAME_TIMEOUT must be defined in config.h"
#endif



/* The "Game Controller" class
 *
 */
class QuizGameController
{
  /* --------------------------------------------
   * inner data types
   */
  protected:
    enum GAME_STATES { GS_IDLE=0, GS_RUNNING, GS_GAME_DONE };
    enum STATE_PARMS { SP_NONE=0, SP_NEW_GAME, SP_GAME_WON, SP_GAME_LOST, SP_GAME_ABORT };
    enum GAME_TIMERS { EV_INVALID=0, EV_RUNNING, EV_RESULT, EV_TIMEOUT_WARN, EV_GAME_TIMEOUT, EV_RESULT_TIMEOUT };

  /* --------------------------------------------
   * constants declarations
   */
  //private:
  //public:

  /* --------------------------------------------
   * configuration
   */
  protected:
    bool m_WantResultAnimation;
    bool m_WantGameTimeout;

  /* --------------------------------------------
   * local attributes
   */
  protected:
    int m_State;
    bool m_StatusLedState;
    bool m_GameWon;
    SimpleLogging m_dbg;
    SimpleTimer m_Tick;
    QuizBoard m_Board;
    QuizGame m_Game;


/* --------------------------------------------
   * methods
   */
#ifdef DEBUG
  public:
#else
  protected:
#endif
    void handleTimerEvent ( int ev );
    void handleAnswerKey ( int question, int answer );
    void handleKey ( int kcode );

  public:
    QuizGameController();
    bool setup ( void );
    void dispatchEvents ( void );
    void switchState ( int NewState, int StateParm );
    void setAliveState ( bool val );
    void configSetResultAnimation ( bool flag ) { m_WantResultAnimation=flag; };
    void configSetGameTimeout ( bool flag ) { m_WantGameTimeout=flag; };
};


/* If this define is defined, we can allocate our code and static data here.
 * This way we avoid libraries and additional CXX files.
 */
#ifdef __ALLOC_STATICS_HERE__

QuizGameController::QuizGameController()
{
#ifdef DEBUG
    m_dbg.setLevel(SimpleLogging::LVL_DEBUG);
#else
    m_dbg.setLevel(SimpleLogging::LVL_WARNING);
#endif
    m_dbg.log(SimpleLogging::LVL_INFO,"QuizBoard %s",APP_VERSION);
    m_StatusLedState = false;
    m_WantResultAnimation = false;
    m_WantGameTimeout = false;
}

bool QuizGameController::setup ( void )
{
    bool rc = true;
    m_State = GS_IDLE;
    m_StatusLedState = false;
    m_GameWon = false;
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::setup");
    rc = m_Board.setup();
    m_Board.disableLEDs();
    m_Board.setLED(QuizBoard::LED_STATUS,HIGH);
    return rc;
}

void QuizGameController::setAliveState ( bool val )
{
    m_Board.setLED(QuizBoard::LED_ALIVE,val);
}

void QuizGameController::dispatchEvents ( void )
{
    int kcode, q, a;
    int ev;

    //~ m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::");
#ifdef DEBUG
    m_Board.dbg_Check();
#endif
    kcode = m_Board.waitKey();
    if ( kcode != QuizBoard::KEY_NONE )
    {
        m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::dispatchEvents key=%d",kcode);
        if ( !m_Board.isRegularKey(kcode) )
	{
	    q = m_Board.isQuestion(kcode);
	    a = m_Board.isAnswer(kcode);
	    m_dbg.log(SimpleLogging::LVL_DEBUG,"Question %d is answered with %d",q,a);
	    handleAnswerKey(q,a);
	}
	else
	{
	    handleKey(kcode);
	}
    }
    if ( m_Tick.dispatch() )
    {
        do
        {
            ev = m_Tick.getEvent();
            if ( ev )
                handleTimerEvent(ev);
        } while ( ev!=0 );
    }
}

void QuizGameController::handleAnswerKey ( int question, int answer )
{
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::handleAnswerKey(%d,%d)",question,answer);
    if ( m_Game.giveAnswer(question,answer) )
    {
	// If the answer was OK, we have to check the result. We must set
	// the LEDs according to this result.
	//
	int res = m_Game.questionAnswered(question);
	m_dbg.log(SimpleLogging::LVL_INFO,"handleAnswerKey: result=%d",res);
	if ( res == QuizGame::RES_CORRECT )
	{
	    m_Board.showAnswer(question,QuizBoard::ANS_CORRECT);
	} else if ( res == QuizGame::RES_WRONG )
	{
	    m_Board.showAnswer(question,QuizBoard::ANS_WRONG);
	}
	else
	    m_dbg.log(SimpleLogging::LVL_INFO,"handleAnswerKey: answer not accepted!");

	// OK, let's see if this was the last answer. If so, we have to check
	// if we won or lost this game.
	//
	if ( m_Game.gameFinished() )
	{
	    switchState(GS_GAME_DONE,m_Game.success()?SP_GAME_WON:SP_GAME_LOST);
	}
    }
}

void QuizGameController::handleTimerEvent ( int ev )
{
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::handleTimerEvent(%d)",ev);
    switch ( ev )
    {
        case EV_RUNNING:
//            if ( m_Game.gameRunning() )
//            {
		m_StatusLedState = !m_StatusLedState;
		m_Board.setLED(QuizBoard::LED_STATUS,m_StatusLedState);
//	    }
            break;

        case EV_RESULT:
            m_StatusLedState = !m_StatusLedState;
	    if ( m_GameWon )
	    {
		// Or should we use m_Game.questionAnswered(i)==RES_CORRECT to
		// only let the correct LEDs flash?
		m_Board.setLED(QuizBoard::LED_GREEN1,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN2,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN3,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN4,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN5,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN6,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN7,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_GREEN8,m_StatusLedState);
	    }
	    else
	    {
		// Or should we use m_Game.questionAnswered(i)==RES_WRONG to
		// only let the wrong LEDs flash?
		m_Board.setLED(QuizBoard::LED_RED1,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED2,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED3,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED4,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED5,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED6,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED7,m_StatusLedState);
		m_Board.setLED(QuizBoard::LED_RED8,m_StatusLedState);
	    }
	    m_Board.setLED(QuizBoard::LED_STATUS,!m_StatusLedState);
            break;

        case EV_TIMEOUT_WARN:
            m_dbg.log(SimpleLogging::LVL_DEBUG,"EV_TIMEOUT_WARN: some left!");
            m_Tick.addTimer(EV_RUNNING,1,true);            // make blinking faster
            break;

        case EV_GAME_TIMEOUT:
	    // abort the game.
            m_dbg.log(SimpleLogging::LVL_DEBUG,"EV_GAME_TIMEOUT: abort game");
	    switchState(GS_GAME_DONE,SP_GAME_ABORT);
            break;

	case EV_RESULT_TIMEOUT:
	    switchState(GS_IDLE,SP_NONE);
	    break;

        default:
            m_dbg.log(SimpleLogging::LVL_INFO,"ev=%d",ev);
    }
}

void QuizGameController::handleKey ( int kcode )
{
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::handleKey(%d)",kcode);
    switch ( kcode)
    {
	case QuizBoard::KEY_START:
            m_Tick.killTimer(EV_RESULT);
	    if ( m_State == GS_RUNNING )
	    {
		// abort running game and switch do GS_GAME_DONE
		m_Game.stopGame();
		switchState(GS_GAME_DONE,SP_GAME_ABORT);
	    }
            else
	    {
		// start a new game
		switchState(GS_RUNNING,SP_NEW_GAME);
	    }
	    break;
    }
}

void QuizGameController::switchState ( int NewState, int StateParm )
{
    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState");
    switch ( NewState )
    {
	case GS_IDLE:
	    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState: idle now");
	    m_State = NewState;
	    m_Board.disableLEDs();
	    m_Board.setLED(QuizBoard::LED_STATUS,HIGH);
	    // may go into power-safe here?
	    break;

	case GS_RUNNING:
	    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState: new game");
	    if ( StateParm == SP_NEW_GAME )
	    {
		m_Tick.addTimer(EV_RUNNING,2,true);
		if ( m_WantGameTimeout )
		{
		    m_Tick.addTimer(EV_TIMEOUT_WARN,CFG_GAME_TIMEOUT_WARN);
		    m_Tick.addTimer(EV_GAME_TIMEOUT,CFG_GAME_TIMEOUT);
		}
	    }
	    m_Tick.killTimer(EV_RESULT_TIMEOUT);
	    m_Board.setLED(QuizBoard::LED_STATUS,HIGH);
	    m_Game.startGame(0);	// for now only set #0
	    m_State = NewState;
	    break;

	case GS_GAME_DONE:
	    m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState: game done");
	    if ( m_WantGameTimeout )
	    {
		// kill the timer of the runtime observation.
		m_Tick.killTimer(EV_TIMEOUT_WARN);
		m_Tick.killTimer(EV_GAME_TIMEOUT);
	    }
	    if ( StateParm == SP_GAME_ABORT )
	    {
		m_Tick.addTimer(EV_RUNNING,5,true);	// totally slow blinking
	    }
	    else if ( StateParm == SP_GAME_WON )
	    {
		m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState: WON");
		// visualize WON
		m_GameWon = true;
		if ( m_WantResultAnimation )
		{
		    m_Board.disableLEDs();
		    m_Tick.killTimer(EV_RUNNING);
		    m_Tick.addTimer(EV_RESULT,1,true);
		}
		else
		    m_Board.setLED(QuizBoard::LED_STATUS,LOW);
	    }
	    else if ( StateParm == SP_GAME_LOST )
	    {
		m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::switchState: LOST");
		// visualize LOST
		m_GameWon = false;
		if ( m_WantResultAnimation )
		{
		    m_Board.disableLEDs();
		    m_Tick.killTimer(EV_RUNNING);
		    m_Tick.addTimer(EV_RESULT,1,true);
		}
		else
		    m_Board.setLED(QuizBoard::LED_STATUS,LOW);
	    }
	    m_Game.stopGame();
	    m_State = NewState;
	    m_Tick.addTimer(EV_RESULT_TIMEOUT,CFG_RESULT_TIMEOUT);
	    break;
#ifdef DEBUG
	default:
	    m_dbg.log(SimpleLogging::LVL_WARNING,"QuizGameController::switchState: Opps %d",NewState);
	    break;
#endif
    }
}

//~ void QuizGameController:: (  )
//~ {
    //~ m_dbg.log(SimpleLogging::LVL_DEBUG,"QuizGameController::");
//~ }

#endif // __ALLOC_STATICS_HERE__
#endif // __QB_QUIZGAME_HXX__
