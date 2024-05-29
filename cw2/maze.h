#ifndef __maze__H_
#define __maze__H_
#define MAX_DIM 100
#define MIN_DIM 5
#define SUCCESS 0
#define ARG_ERROR 1
#define FILE_ERROR 2
#define MAZE_ERROR 3
#define UNSUCCESS 100
#define ERROR_VALUE -1

#define WALL '#'
#define PATH ' '
#define START 'S'
#define EXIT 'E'
#define PLAYER 'X'

typedef struct{
    int width;
    int height;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    char**maze;
} Maze_information;

typedef struct 
{
    int x;
    int y;
} player_information;




#endif
