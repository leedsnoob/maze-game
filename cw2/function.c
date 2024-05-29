#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

void initializeMaze(Maze_information *maze, int width, int height) {
    if (width < MIN_DIM || width > MAX_DIM || height < MIN_DIM || height > MAX_DIM) {
        fprintf(stderr, "Maze dimensions must be between %d and %d.\n", MIN_DIM, MAX_DIM);
        exit(ARG_ERROR);
    }

    maze->width = width;
    maze->height = height;

    maze->maze = (char **)malloc(height * sizeof(char *));
    if (maze->maze == NULL) {
        fprintf(stderr, "Memory allocation failed for maze rows.\n");
        exit(UNSUCCESS);
    }
    int i=0;

    for (i = 0; i < height; i++) {
        maze->maze[i] = (char *)malloc(width * sizeof(char));
        if (maze->maze[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for maze columns.\n");
            exit(UNSUCCESS);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze->maze[i][j] = ' ';
        }
    }

    maze->start_x = ERROR_VALUE;
    maze->start_y = ERROR_VALUE;
    maze->end_x = ERROR_VALUE;
    maze->end_y = ERROR_VALUE;
}

void loadMazeFile(Maze_information *maze, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(FILE_ERROR);
    }

    int row = 0;
    char line[MAX_DIM + 2]; // +2 for newline and null terminator
    while (fgets(line, sizeof(line), file)) {
        if (row >= maze->height) {
            fprintf(stderr, "Error: More rows in the file than expected.\n");
            fclose(file);
            exit(EXIT_MAZE_ERROR);
        }
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        if (len != maze->width) {
            fprintf(stderr, "Error: Row length does not match the expected width.\n");
            fclose(file);
            exit(EXIT_MAZE_ERROR);
        }
        for (int col = 0; col < maze->width; col++) {
            char ch = line[col];
            switch (ch) {
                case WALL:
                case PATH:
                    maze->maze[row][col] = ch;
                    break;
                case START:
                    if (maze->start_x != -1 || maze->start_y != -1) {
                        fprintf(stderr, "Error: Multiple start points in the maze.\n");
                        fclose(file);
                        exit(EXIT_MAZE_ERROR);
                    }
                    maze->maze[row][col] = ch;
                    maze->start_x = col;
                    maze->start_y = row;
                    break;
                case EXIT:
                    if (maze->end_x != -1 || maze->end_y != -1) {
                        fprintf(stderr, "Error: Multiple exit points in the maze.\n");
                        fclose(file);
                        exit(EXIT_MAZE_ERROR);
                    }
                    maze->maze[row][col] = ch;
                    maze->end_x = col;
                    maze->end_y = row;
                    break;
                default:
                    fprintf(stderr, "Error: Invalid character in maze file: %c\n", ch);
                    fclose(file);
                    exit(EXIT_MAZE_ERROR);
            }
        }
        row++;
    }

    if (maze->start_x == -1 || maze->start_y == -1) {
        fprintf(stderr, "Error: No start point in the maze.\n");
        fclose(file);
        exit(EXIT_MAZE_ERROR);
    }
    if (maze->end_x == -1 || maze->end_y == -1) {
        fprintf(stderr, "Error: No exit point in the maze.\n");
        fclose(file);
        exit(EXIT_MAZE_ERROR);
    }

    fclose(file);
}

void checkMaze(const Maze_information *maze, int width, int height) {
    if (maze->width != width || maze->height != height) {
        fprintf(stderr, "Error: Maze dimensions do not match the expected dimensions.\n");
        exit(EXIT_MAZE_ERROR);
    }

    int start_count = 0;
    int exit_count = 0;
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            char ch = maze->maze[i][j];
            switch (ch) {
                case WALL:
                case PATH:
                    break;
                case START:
                    start_count++;
                    break;
                case EXIT:
                    exit_count++;
                    break;
                default:
                    fprintf(stderr, "Error: Invalid character in maze file: %c\n", ch);
                    exit(EXIT_MAZE_ERROR);
            }
        }
    }

    if (start_count != 1) {
        fprintf(stderr, "Error: Maze must have exactly one start point.\n");
        exit(EXIT_MAZE_ERROR);
    }

    if (exit_count != 1) {
        fprintf(stderr, "Error: Maze must have exactly one exit point.\n");
        exit(EXIT_MAZE_ERROR);
    }
}


void initializePlayerPosition(player_information *player, const Maze_information *maze) {
    player->x = maze->start_x;
    player->y = maze->start_y;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"



void showMap(const Maze_information *maze, const player_information *player) {
    printf("\n");
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (i == player->y && j == player->x) {
                printf("X");
            } else {
                printf("%c", maze->maze[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int isMoveValid(const Maze_information *maze, int newX, int newY) {
    // Check if the new position is within the bounds of the maze
    if (newX < 0 || newX >= maze->width || newY < 0 || newY >= maze->height) {
        return 0; // Invalid move: out of bounds
    }
    // Check if the new position is a wall
    if (maze->maze[newY][newX] == WALL) {
        return 0; // Invalid move: hitting a wall
    }
    return 1; // Valid move
}

void updatePlayerPosition(player_information *player, int newX, int newY) {
    player->x = newX;
    player->y = newY;
}

int checkIfExitReached(const player_information *player, const Maze_information *maze) {
    return (player->x == maze->end_x && player->y == maze->end_y);
}


void freeMaze(Maze_information *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->maze[i]);
    }
    free(maze->maze);
}