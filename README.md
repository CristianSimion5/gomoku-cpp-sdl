# Gomoku in C++
[Gomoku](https://en.wikipedia.org/wiki/Gomoku) is a 2-player board game where players take alternating turns placing stones on empty intersections with the goal to form a line of 5 vertically, diagonally or horizontally.

This variation uses a non-standard 20x23 board (most boards are either 15x15 or 19x19).  
It also has two available [opening rules](http://gomokuworld.com/gomoku/2) which help maintain equal odds for both players: Swap and Swap2.

The implementation was done in C++. The GUI rendering was accomplished using the [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/) Library.
