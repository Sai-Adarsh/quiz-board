% Design Decisions for "Quiz-Board"
% Joerg Desch (https://github.com/joede/quiz-board)
% Revision R.1

# About this document

This text documents some of the design decisions I've made. Remember that
one of the main goals of the project is the
[KISS](http://en.wikipedia.org/wiki/KISS_principle) principle!



## Hardware decisions

Our "Quiz Board" contains 8 questions with 4 possible answers. The result is
shown with 2 LEDs. We need a button to start the game and a LED to visualize the
state.

**Note:** all pin numbers mentioned in this document are Arduino pin numbers! See
the Arduino Pin Mapping table for details.

### Keypad Inputs

Each button is realized as a *closing switch*. If pressed, this switch is closed
and pulls down the input level to GND. If not pressed, the internal pull-up
resistor is used to hold the level up to Vcc.

The "Start" button is a special input. Since this button must be able to wake up
the device, the input must be capable of the "pin change interrupt" feature of
the AVR2560.

The 32 answer buttons (8x4) are connected to the pins 22 to 53. The "Start"
button is connected to pin 12. This pin is "PCINT6" of the MCU.

### LED Outputs

The anode pins of all the LEDs are connected to Vcc. The cathode pins are connected
to an 200 ohm resistor. The other end is routed to an output pin of the MCU.
A low level at this output lets the LED glow.

We don't use the analog inputs. So we connect the 16 (2x8) LEDs to the analog
inputs `A0` to `A15`. The pins are used as GPIO instead.

![wiring](https://raw.github.com/joede/quiz-board/master/docs/images/Wiring-sketch.png)


## Software decisions

### Usage of the IDE

The Arduino IDE comes with several limitations. Since this is my first
Arduino project, I possibly have not found the best solutions to these
limitations. So this section may change some time. ;-)

The source code of C++ is split into a *header* and a *code file*. The
header includes declarations and the code file the implementation.
To be included into the project, those files must be opened as a tab in the
corresponding sketch file. To avoid a cluttered IDE, we put both, the
declaration and the implementation into the header file.

In the end, we need one place where all the implementations are included in
*code files*. The solution is to declare a C `define` in the main file (`.ino`).

The only other solution would be a *library* which must be stored in the Arduino
toolchain path. In my opinion, this solution should only be chosen for
independent code.
