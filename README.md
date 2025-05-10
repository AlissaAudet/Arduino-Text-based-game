# Arduino Text-Based Game

A simple text based interactive game built on an Arduino Uno.

Includes a sprite animation at the end.

## Link to simulate it (Tinkercad account needed)

https://www.tinkercad.com/things/fyty8zlRFxC-text-based-game

 ### Tools Used

    Tinkercad for circuit simulation and prototyping
### Design

![image](https://github.com/user-attachments/assets/d8aee41c-027e-4cd3-9529-fe374bd1cc33)   

### Wiring Schematic

![image](https://github.com/user-attachments/assets/3c1e8f6d-f602-4e8d-a081-b3077a4a67c7)

### Circuit Components

    1x Arduino Uno

    1x 16x2 I2C LCD

    2x push-buttons

    2x LEDs (for feedback when buttons are pressed)

    2x 220 Ω resistors (for LEDs)

    Breadboard + jumper wires

## How It Works

The game progresses using a finite state machine (FSM) implemented with a switch statement on the gameState variable.

    The player is presented with a series of story choices displayed on the LCD.

    Two buttons are used to make decisions.

    LEDs blink whenever a button is pressed.

    Depending on the choices made, different endings are displayed.

    A sprite animation plays at the end 

### State Management

Several variables are used to control game flow:

    gameState: determines the current scene or logic branch

    stateJustChanged: prevents repeated LCD redraws

    readyForEndTransition: controls when input is allowed to trigger the ending animation

    waitingForButtonRelease: ensures a button is released before it's counted again
    

## What I learned

    Coding a game on Arduino is fun but also comes with challenges. Despite the game having only 8 states, it required nearly 300 lines of code, largely due to the manual control needed over input and display logic.

Some of the main challenges I encountered:

    Button input handling isn't as simple as checking if a button is pressed : you need to detect state changes (from HIGH to LOW), which adds logic and variables.

    Managing the game's progression requires multiple state variables, like gameState, stateJustChanged, readyForEndTransition, and waitingForButtonRelease, to ensure clean transitions and restarts.

    I had to implement a custom scrolling function to make the text move from right to left, as this isn’t natively handled in a smooth way by the LCD.

    Working with the LCD display itself requires explicitly clearing the screen and manually setting the cursor before every message : otherwise the output overlaps or appears in the wrong place.

While this was a cool project and a great learning experience, I realized that building a more complex game wouldn't be very sustainable with this kind of display or setup. That said, it’s a fun way to explore state machines, low-level UI logic, and how hardware and software interact on microcontrollers.
