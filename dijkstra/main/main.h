#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Edge {
    int headVertex;          // destination vertex
    int weight;
    struct Edge* next; // next edge in adjacency list
} Edge;

typedef struct Node {
    int vertex;   // vertex number
    Edge* edge;   // pointer to beginning of list of edges
} Node;

typedef struct {
    int vertex;
    int key; // Dijkstra's greedy score
} HeapNode;

#define buffsize 310       // *must be changed if diff file used. 
                           // I didnt want to grow the line buffer for this project
static char buff[buffsize];

void runDijkstra(Node source);