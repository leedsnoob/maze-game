#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "function.h"
void initializeMaze(Maze_information *maze, int width, int height) {
    maze->width = width;
    maze->height = height;

    // Allocate memory for the maze
    maze->maze = malloc(height * sizeof(char *));
    if (maze->maze == NULL) {
        fprintf(stderr, "Failed to allocate memory for maze\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < height; i++) {
        maze->maze[i] = malloc(width * sizeof(char));
        if (maze->maze[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for maze row %d\n", i);
            // Free already allocated memory
            for (int j = 0; j < i; j++) {
                free(maze->maze[j]);
            }
            free(maze->maze);
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the maze with paths
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze->maze[i][j] = PATH;  // Initialize each cell as a path
        }
    }
}



void loadMazeFile(Maze_information *maze, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FILE_ERROR);
    }

    int i = 0, j = 0;
    while (i < maze->height && !feof(file)) {
        char ch = fgetc(file);
        if (ch == '\n') {
            continue;  // Skip newlines
        }
        maze->maze[i][j] = ch;
        if (ch == START) {
            maze->start_x = j;
            maze->start_y = i;
        } else if (ch == EXIT) {
            maze->end_x = j;
            maze->end_y = i;
        }
        j++;
        if (j >= maze->width) {
            j = 0;
            i++;
        }
    }

    fclose(file);
}