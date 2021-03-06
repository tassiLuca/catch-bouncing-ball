# Catch the Bouncing Led Ball
This is the first assignment for the Embedded System and IoT course of the Computer Science and Engineering Bachelor Degree @ UniBO.

## Project overview
The game board is based on 4 green leds L1, L2, L3, L4 and red led LS, four tactile buttons T1, T2, T3, T4 and a potentiometer Pot.
The green leds represent four different positions in which a virtual ball (the led ball) is positioned. A led on means that the ball is 
in that position. During the game, the ball moves repeatedly in from L1 to L4 and back, with a speed S for some random time T1. After T1, 
the ball stops in some position P and the user playing the game has T2 time to press the button in position P. If the player presses the 
button within time T2, it gets one point augmenting the score, and the game goes on – augmenting the speed S and reducing the time T2 of 
a factor F. If the player does not press the button on time, the game is over.

## More infos
[Here](https://vimeo.com/693300376) you can find a short video demonstrating the system.

[Here](https://www.tinkercad.com/things/0fTYDu71vPB) the breadboard circuit schema.

In `doc` folders you can find the system requirements and the breadboard circuit schemas.

