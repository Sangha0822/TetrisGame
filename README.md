# Arduino Tetris Game

## Project Overview
This project is an Arduino-based Tetris game designed for single-player interaction. The game uses an IR remote controller to start and control the gameplay, where players navigate Tetris blocks on a dot matrix display. Points are scored by completing rows, with audio feedback provided through a passive buzzer.

## Hardware Components
Here's the wiring diagram for setting up the hardware:
![Wiring Diagram](/Users/sangha/Documents/PlatformIO/Projects/EE120 Custom Project/images/Wiring Diagram.png "Wiring Diagram")



## Features
- **Game Control**: Players can start the game, move shapes left and right, and rotate them counterclockwise using the remote controller.
- **Scoring**: Each completed row adds 1 point. A brief sound signifies scoring.
- **Game End Conditions**: The game concludes with a victory tune if the player scores 10 or more points, or a losing sound if the blocks reach the top of the display.
- **Restart Capability**: The game can be restarted at any time using the remote controller's on button.

## Hardware Components
- **MAX7219 Dot Matrix Module x 4**: Used to display the Tetris game.
- **IR Receiver**: For receiving signals from the remote controller.
- **Remote Controller**: Used to control the game.
- **Passive Buzzer**: Provides audio feedback for game events.
- **Button x 3**: For manual game controls.
- **7-segment Display**: Shows the current score.

## Software and Libraries
This project makes extensive use of several libraries and custom functions to control hardware components and game logic:
- **Standard Libraries**: `<stdint.h>`
- **Custom Functions and Libraries**:
  - `MAX7219.h`: For controlling the dot matrix display.
  - `gamesong.h`: For managing sound outputs.
  - `timerISR.h`, `helper.h`, `periph.h`, `irAVR.h`, `serialATmega.h`, `spiAVR.h`, `time.h`: Provide various utility functions and hardware interface abstractions.

## Getting Started
1. **Setup Hardware**: Assemble the hardware components based on the wiring diagram provided.
2. **Flash Firmware**: Load the software onto the Arduino board.
3. **Power On**: Use the remote to turn on the game.
4. **Play Game**: Use the buttons on the remote to control the game blocks.

## Build-Upons
- **Multiple Displays**: Utilizing 4 chained MAX7219 modules to create an extended playing field.
- **Remote Gameplay Control**: Integration with a remote controller to manage game states and control movement.
- **Audio Feedback**: Using a buzzer to enhance gameplay with audio cues for starting, winning, losing, and scoring.

## User Guide
Turn the game on/off with the remote control's on button. Control the shapes using the left button (move left), middle button (move right), and right button (rotate counterclockwise).

## Diagrams
Include any task, wiring, and synchronization state machine diagrams to aid in understanding the system architecture and wiring setup.

---

Enjoy building and playing your Arduino Tetris game!