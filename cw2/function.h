#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include "maze.h"

void initializeMaze(Maze_information *maze, int width, int height);

void loadMazeFile(Maze_information *maze, const char *filename);

void checkMaze(const Maze_information *maze, int width, int height);

void initializePlayerPosition(player_information *player, const Maze_information *maze);

void showMap(const Maze_information *maze, const player_information *player);

int isMoveValid(const Maze_information *maze, int newX, int newY);

void updatePlayerPosition(player_information *player, int newX, int newY);

int checkIfExitReached(const player_information *player, const Maze_information *maze);


void freeMaze(Maze_information *maze);

#endif