#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include <ctype.h>
#include "function.h"

/* This is the entry point of the program, which first checks if any of the three command line arguments were passed in. These parameters are the maze file, the width and height of the maze.
The program then initializes a maze structure and a player structure.
Next, the program calls several functions to load the maze file, check that the maze is the right size, and initialize the player's position.
Enter a loop that waits for the player to enter a move command, updates the player position based on the input, and exits when the end point is reached.*/

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <maze_file> <width> <height>\n", argv[0]); //传入迷宫
        return EXIT_FAILURE;
    }
    
    Maze_information maze;
    player_information player;
    initializeMaze(&maze, atoi(argv[2]), atoi(argv[3]));//Initialize the maze structure, setting its width and height
    loadMazeFile(&maze, argv[1]);  //This involves reading the walls, paths, start and end points of the maze.
    checkMaze(&maze,atoi(argv[2]), atoi(argv[3])); // The width and height of the checking maze are not the same as the width and height of the loading maze
    initializePlayerPosition(&player, &maze); // Initialize player placement, placing the player at the beginning of the maze.
 int inputChar;
    do {
        printf("Enter your move (WASD to move, Q to quit,M to show map): ");
        inputChar = getchar();
        while ((getchar()) != '\n'); 
        inputChar = toupper(inputChar);
        int newX = player.x, newY = player.y; 
        switch (inputChar) {
            case 'W':
                newY -= 1;
                break;
            case 'A':
                newX -= 1;
                break;
            case 'S':
                newY += 1;
                break;
            case 'D':
                newX += 1;
                break;
            case 'M':
                showMap(&maze, &player); //Shows the maze map and the player's current location
                break;
            case 'Q':
                printf("Exiting game.\n");
                return 0;
            default:
                printf("Invalid input!\n");
                continue;
        }
        if (isMoveValid(&maze, newX, newY)) {  //Check that the player is trying to make a legal move, such as moving outside a wall or maze.
            updatePlayerPosition(&player, newX, newY); //If the move is legal, update the player's position
            printf("Move successfully\n");
            if (checkIfExitReached(&player, &maze)) {  //Check if the player has reached the exit of the maze, and if so, return a marker indicating the game is over.
                printf("Congratulations, you've reached the exit!\n");
                break;
            }
        } else {
            printf("Move is not valid!\n");
        }
    } while (1);
freeMaze(&maze);
    return 0;
}

