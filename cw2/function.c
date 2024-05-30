#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

void loadMazeFromFile(Maze_information *maze, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(FILE_ERROR);
    }

    if (!initializeMaze(maze, file)) {
        fclose(file);
        exit(MAZE_ERROR);
    }

    fclose(file);

    if (!checkMaze(maze)) {
        freeMaze(maze);
        exit(MAZE_ERROR);
    }
}

int initializeMaze(Maze_information *maze, FILE *file) {
    int row = 0;
    char line[MAX_DIM + 2];  // 包含换行符和终止符
    // https://blog.csdn.net/zcc1229936385/article/details/120982704 以为部分代码参考这篇文章
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
            return 0;
        }
        row++;
    }
    maze->height = row;

    if (maze->width < MIN_DIM || maze->width > MAX_DIM || maze->height < MIN_DIM || maze->height > MAX_DIM) {
        fprintf(stderr, "Maze dimensions must be between %d and %d.\n", MIN_DIM, MAX_DIM);
        return 0;
    }

    // 动态分配内存
    maze->maze = (char **)malloc(maze->height * sizeof(char *));
    if (maze->maze == NULL) {
        fprintf(stderr, "Memory allocation failed for maze rows.\n");
        return 0;
    }
    for (int i = 0; i < maze->height; i++) {
        maze->maze[i] = (char *)malloc(maze->width * sizeof(char));
        if (maze->maze[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for maze columns.\n");
            for (int j = 0; j < i; j++) {
                free(maze->maze[j]);
            }
            free(maze->maze);
            return 0;
        }
    }

    // 将数据存储到内存
    rewind(file);
    row = 0;
    while (fgets(line, sizeof(line), file) && row < maze->height) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        strncpy(maze->maze[row], line, maze->width);
        row++;
    }
    return 1;
}


int checkMaze(Maze_information *maze) {
    int start_count = 0;
    int exit_count = 0;
    maze->start_x = ERROR_VALUE;
    maze->start_y = ERROR_VALUE;
    maze->end_x = ERROR_VALUE;
    maze->end_y = ERROR_VALUE;
    //检查
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
                    return 0;
            }
        }
    }

    // 检查是否唯一的起点和终点
    if (start_count > 1) {
        fprintf(stderr, "Error: Maze must have exactly one start point 'S'.\n");
        return 0;
    }

    if (exit_count > 1) {
        fprintf(stderr, "Error: Maze must have exactly one exit point 'E'.\n");
        return 0;
    }
    //没有起点和终点
    if (maze->start_x == ERROR_VALUE || maze->start_y == ERROR_VALUE) {
        fprintf(stderr, "Error: Maze must have a valid start point 'S'.\n");
        return 0;
    }

    if (maze->end_x == ERROR_VALUE || maze->end_y == ERROR_VALUE) {
        fprintf(stderr, "Error: Maze must have a valid exit point 'E'.\n");
        return 0;
    }

    return 1;
}
void initializePlayerPosition(player_information *player, const Maze_information *maze) {
    if (maze->start_x == ERROR_VALUE || maze->start_y == ERROR_VALUE) {
        fprintf(stderr, "Error: Invalid start position for player.\n");
        exit(MAZE_ERROR);
    }//二次检查
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
