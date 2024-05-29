#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

void loadMazeFromFile(Maze_information *maze, const char *filename) {
    FILE *maze = fopen(filename, "r");
    if (maze == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(FILE_ERROR);
    }

    initializeMaze(maze, maze);
    fclose(maze);
}

void initializeMaze(Maze_information *maze, FILE *file) {
    int row = 0;
    char line[MAX_DIM + 2];  // 一个是换行符一个是”0“

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        if (row == 0) {
            maze->width = len;
        } else if (len != maze->width) {
            fprintf(stderr, "Error: Row length does not match the expected width.\n");
            fclose(file);
            exit(MAZE_ERROR);
        }
        row++;
    }

    maze->height = row;

    if (maze->width < MIN_DIM || maze->width > MAX_DIM || maze->height < MIN_DIM || maze->height > MAX_DIM) {
        fprintf(stderr, "Maze dimensions must be between %d and %d.\n", MIN_DIM, MAX_DIM);
        fclose(file);
        exit(MAZE_ERROR);
    }

    rewind(file);

    maze->maze = (char **)malloc(maze->height * sizeof(char *));
    if (maze->maze == NULL) {
        fprintf(stderr, "Memory allocation failed for maze rows.\n");
        exit(UNSUCCESS);
    }

    for (int i = 0; i < maze->height; i++) {
        maze->maze[i] = (char *)malloc(maze->width * sizeof(char));
        if (maze->maze[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for maze columns.\n");
            exit(UNSUCCESS);
        }
    }

    row = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        for (int col = 0; col < maze->width; col++) {
            maze->maze[row][col] = line[col];
        }
        row++;
    }
}

void checkMaze(Maze_information *maze) {
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
                    maze->start_x = j;
                    maze->start_y = i;
                    break;
                case EXIT:
                    exit_count++;
                    maze->end_x = j;
                    maze->end_y = i;
                    break;
                default:
                    fprintf(stderr, "Error: Invalid character in maze file: %c\n", ch);
                    exit(MAZE_ERROR);
            }
        }
    }

    if (start_count != 1) {
        fprintf(stderr, "Error: Maze must have exactly one start point.\n");
        exit(MAZE_ERROR);
    }

    if (exit_count != 1) {
        fprintf(stderr, "Error: Maze must have exactly one exit point.\n");
        exit(MAZE_ERROR);
    }

    if (maze->start_x == ERROR_VALUE || maze->start_y == ERROR_VALUE) {
        fprintf(stderr, "Error: No start point in the maze.\n");
        exit(MAZE_ERROR);
    }
    if (maze->end_x == ERROR_VALUE || maze->end_y == ERROR_VALUE) {
        fprintf(stderr, "Error: No exit point in the maze.\n");
        exit(MAZE_ERROR);
    }
}
void initializePlayerPosition(player_information *player, const Maze_information *maze) {
    player->x = maze->start_x;
    player->y = maze->start_y;
}




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
   
    if (newX < 0 || newX >= maze->width || newY < 0 || newY >= maze->height) {
        return 0; 
    }

    if (maze->maze[newY][newX] == WALL) {
        return 0; 
    }
    return 1; 
}

void updatePlayerPosition(player_information *player, int newX, int newY) {
    player->x = newX;
    player->y = newY;
    printf("Move successfully\n");
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
