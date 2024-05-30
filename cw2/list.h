#ifndef LIST_H
#define LIST_H

typedef struct {
    int x, y;
} Cell;

typedef struct Node {
    Cell cell;
    struct Node *next;
} Node;

void add_to_list(Node **head, int x, int y);
void remove_from_list(Node **head, int x, int y);
int list_length(Node *head);
Cell get_random_cell(Node *head);
Cell* get_neighbors(int x, int y, int width, int height, int *count);

#endif 
