# Sokoban
This is a classic puzzle game, Sokoban, incorporating player-controlled movement, teleporters, undo functionality, and dynamic box movement to create an engaging and immersive gaming experience. This project leverages the SFML library for graphic rendering, ensuring a polished and visually appealing interface that enhances user enjoyment and immersion. Sokoban applies object-oriented programming principles to encapsulate game logic, streamline level loading, and implement efficient game state serialization, promoting code organization and maintainability.
Implements OOP, inheritance, encapsulation, file manipulation, 2D vectors, stacks, lambdas, overloaded operators, SFML library graphic rendering.

# How to play
Run the Makefile

Enter the command `./Sokoban <name of level>`

To move use the `WASD` or `ARROW` keys.

To reset the game, press `R`.

To undo a move, press `Z`.

# Features
Dynamic player and box movement.

Teleport stations.

Movement undo.

Timer.

# Files
Sokoban.hpp: Header file containing the Sokoban class, player movement enumeration, and player defined SB namespace.

Sokoban.cpp: Implementation file for the Sokoban class and SB namespace.

main.cpp: Main program file that includes the game and drawing loop.

test.cpp: Test file that holds tests testing for faulty implementations.
