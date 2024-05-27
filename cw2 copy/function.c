#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "function.h"
void initializeMaze(Maze_information *maze, int width, int height) {

    maze->width = width;
    maze->height = height;
    
    // 为迷宫的二维字符数组分配内存
    maze->maze = malloc(width * sizeof(char *));
    if (maze->maze == NULL) {
        fprintf(stderr, "Failed to allocate memory for maze\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < width; i++) {
        maze->maze[i] = malloc(width * sizeof(char));
        if (maze->maze[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for maze row %d\n", i);
            // 如果内存分配失败，释放已分配的内存
            for (int j = 0; j < i; j++) {
                free(maze->maze[j]);
            }
            free(maze->maze);
            exit(EXIT_FAILURE);
        }
    }
}