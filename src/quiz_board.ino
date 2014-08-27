/* Quiz-Board
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
 */

#define __ALLOC_STATICS_HERE__ 1

/* These local include files must be opened in a tab of the Arduino IDE!
 * If not, the headers could not be found! Don't ask me why...
 */
#include "config.h"
#include "GameController.h"


QuizGameController TheGame;


/* ============================================
 * THE APP
 */

void handleCommand ( char cmd )
{
    int i;

    switch ( cmd )
    {
	case '?':
#ifdef DEBUG
	    Serial.print("QuizBoard DEBUG V"); Serial.println(APP_VERSION);
#else
	    Serial.print("QuizBoard V"); Serial.println(APP_VERSION);
#endif
	    break;
	case 'a':
	    TheGame.configSetResultAnimation(false);
	    Serial.println("ResultAnimation=false");
	    break;
	case 'A':
	    TheGame.configSetResultAnimation(true);
	    Serial.println("ResultAnimation=true");
	    break;
	case 'g':
	    TheGame.configSetGameTimeout(false);
	    Serial.println("GameTimeout=false");
	    break;
	case 'G':
	    TheGame.configSetGameTimeout(true);
	    Serial.println("GameTimeout=true");
	    break;
        case 'd':
            TheGame.setLogLevel(SimpleLogging::LVL_DEBUG);
	    Serial.println("LogLevel=debug");
            break;
    }
}

void setup (void)
{
    Serial.begin(9600);
#ifdef DEBUG
    Serial.print("QuizBoard DEBUG V"); Serial.println(APP_VERSION);
#endif
    TheGame.setup();
    // we could read a setup out of the EEPROM here...
    TheGame.configSetResultAnimation(false);
    TheGame.configSetGameTimeout(false);
    TheGame.configSolutionSet(0);
}

void loop (void)
{
    /* LET THE GAME HANDLE THE EVENTS
     */
    TheGame.dispatchEvents();

    /* HANDLE THE SERIAL PORT
     */
    if ( Serial.available() )
    {
	char inChar = (char)Serial.read();
	handleCommand(inChar);
    }
}
