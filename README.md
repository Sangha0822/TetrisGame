# Arduino Tetris Game

## Project Overview
This project is an Arduino-based Tetris game designed for single-player interaction. The game uses an IR remote controller to start and control the gameplay, where players navigate Tetris blocks on a dot matrix display. Points are scored by completing rows, with audio feedback provided through a passive buzzer.




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

## Wiring Diagram
Here's the wiring diagram for setting up the hardware:
<img width="643" alt="Wiring Diagram" src="https://github.com/user-attachments/assets/823cf452-ba0c-47db-8bcf-7a1be1ae74b8" />

## Playing Tetris
The video demonstrates the gameplay mechanics and user interaction with the Arduino-based Tetris game. Watch as the Tetris pieces fall and are maneuvered using the IR remote control. The gameplay video highlights how players can control the movement and rotation of the Tetris blocks, showcasing the game's responsiveness and the visual feedback provided by the LED matrix display. It's a great way to see the game in action and understand the real-time performance of the hardware setup.

<a href="https://www.youtube.com/watch?v=jDF5iNfeIBw&ab_channel=Jeonsangha">
    <img width="500" alt="TetrisVideo" src="https://github.com/user-attachments/assets/9fba5bb2-59d9-4d57-9eb7-4a1ddd314028" />
</a>

---

Enjoy building and playing your Arduino Tetris game!
