# The Puzzle Board

The aim of this project is the development of a microcontroller based
board to perfom a quiz or puzzle in public. This board is developed
with NGO's in mind, working with the audience on the streets.

The quiz is based on questions with 4 answers. Only one answer is
correct. There are up to 8 questions possible.

The answer is given by pressing one of four buttons. The result is
shown immediately through a red or green LED. The puzzle is done as soon
as all questions got an answer. As an additional option, a time limit
can be enabled, so that all answers must be given within this time period.

Since we need a larger number of IOs, we will use a
[Arduino Mega 2560](http://arduino.cc/en/Main/arduinoBoardMega2560)
board.

Planned features are:

* stand-alone usage without a PC
* selectable set of *correct answers*
* power-safe mode for running on batteries
* optional observation of the time to answer all questions (timeout)

**Note:** the main audience of this project is the German NGO [BUND](http://www.bund.net),
a environmental (protection) organisation. Because of this, all further documents
are written in German. If there is interest from outside of Germany, I may
add an english translation.
