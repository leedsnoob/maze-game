#include <stdio.h>
#include <stdlib.h>
#include "maze_generator.h"
#include "list.h"
//思路来自https://blog.csdn.net/weixin_45218342/article/details/134369330
//https://blog.csdn.net/weixin_56154577/article/details/136388934
//算法部分来自
//https://blog.csdn.net/weixin_56154577/article/details/136388934
//https://blog.csdn.net/qq_36327203/article/details/84978337
char** initialize_maze(int width, int height) {
    char **maze = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        maze[i] = (char *)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            maze[i][j] = WALL;
        }
    }
    return maze;
}

void print_maze(char **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putchar(maze[i][j]);
        }
        putchar('\n');
    }
}

void save_maze(char **maze, int width, int height, const char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fputc(maze[i][j], file);
        }
        fputc('\n', file);
    }
    fclose(file);
}


void prim_maze(int width, int height, const char *filename) {
    char **maze = initialize_maze(width, height);
    int start_x = (rand() % (width / 2)) * 2;
    int start_y = (rand() % (height / 2)) * 2;
    maze[start_y][start_x] = START;
    // 创建一个链表来存储待处理的墙壁单元格
    Node *walls = NULL;
    add_to_list(&walls, start_x, start_y);
 // 处理链表中的每个节点
    while (walls != NULL) {
        Cell current = get_random_cell(walls);
        int count;
        // 获取当前节点的相邻节点
        Cell *neighbors = get_neighbors(current.x, current.y, width, height, &count);
        Cell next = {-1, -1};
//检查相邻节点是否为墙壁
        for (int i = 0; i < count; i++) {
            if (maze[neighbors[i].y][neighbors[i].x] == WALL) {
                next = neighbors[i];
                break;
            }
        }

        if (next.x != -1 && next.y != -1) {
            int wall_x = (current.x + next.x) / 2;
            int wall_y = (current.y + next.y) / 2;
            maze[wall_y][wall_x] = PATH;
            maze[next.y][next.x] = PATH;
            add_to_list(&walls, next.x, next.y); //如果找到
        } else {
            remove_from_list(&walls, current.x, current.y); // 没找到移除
        }

        free(neighbors);
    }

    // 采用曼哈顿距离最远
    int max_distance = 0;
    int end_x = start_x, end_y = start_y;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == PATH) {
                int distance = abs(start_x - j) + abs(start_y - i);
                if (distance > max_distance) {
                    max_distance = distance;
                    end_x = j;
                    end_y = i;
                }
            }
        }
    }

    maze[end_y][end_x] = END;

    save_maze(maze, width, height, filename);

    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);
}
