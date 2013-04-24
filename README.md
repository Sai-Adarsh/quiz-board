# The Puzzle Board

The aim of this project is the development of a microcontroller based
board to perfom a quiz or puzzle in the public.

The quiz is based on questions with 4 answers. Only one answer is
correct. There are up to 8 questions possible.

The answer is given by pressing one of four buttons. The result is
shown immediately through a red or green LED.

Since we need a larger number of IOs, we will use a
[Arduino Mega 2560](http://arduino.cc/en/Main/arduinoBoardMega2560)
board.

Planned features are:

* stand-alone usage without a PC
* selectable set of *correct answers*
* power-safe mode for running with batteries
* optional observation of the time to answer all questions (timeout)
