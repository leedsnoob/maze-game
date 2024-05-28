#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "function.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <maze_file> <width> <height>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *mazeFile = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    if (width < MIN_DIM || width > MAX_DIM || height < MIN_DIM || height > MAX_DIM) {
        fprintf(stderr, "Maze dimensions must be between %d and %d.\n", MIN_DIM, MAX_DIM);
        return EXIT_FAILURE;
    }

    Maze_information maze;
    initializeMaze(&maze, width, height);

    // 加载迷宫文件
    loadMazeFile(&maze, mazeFile);

    // 检查迷宫
    checkMaze(&maze, width, height);

    // 打印迷宫以验证加载是否正确
    printf("Maze loaded with width %d and height %d\n", maze.width, maze.height);
    for (int i = 0; i < maze.height; i++) {
        for (int j = 0; j < maze.width; j++) {
            printf("%c", maze.maze[i][j]);
        }
        printf("\n");
    }

    // 打印起点和终点位置
    printf("Start position: (%d, %d)\n", maze.start_x, maze.start_y);
    printf("End position: (%d, %d)\n", maze.end_x, maze.end_y);

    // 释放内存
    for (int i = 0; i < maze.height; i++) {
        free(maze.maze[i]);
    }
    free(maze.maze);

    return EXIT_SUCCESS;
}
