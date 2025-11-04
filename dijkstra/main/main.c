#include "main.h"

Node graph[200];

static int main(int argc, char* argv[])
{
    if(argc != 2) {fprintf(stderr, "\nUsage: ./main filename.txt\n"); return 1;}
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {fprintf(stderr, "\nError opening file\n"); return 1;}

    while(true)
    {
        char* line = fgets(buff, buffsize, fp);
        if(line == NULL) break; 

        char* tkn = strtok(line, " \t\n\r");
        int graphIndex = atoi(tkn);
        int vertex = graphIndex;
        
        graph[graphIndex].vertex = vertex; 
        graph[graphIndex].edge = NULL;
        
        while((tkn = strtok(NULL, " \t\n\r")) != NULL)
        {
            int head;
            int weight;

            char* comma = strchr(tkn, ','); // find the comma
            *comma = '\0';                  // split the string

            head = atoi(tkn);                     // part before comma
            weight = atoi(comma + 1);

            Edge* e = malloc(sizeof(Edge));
            e->head = head;
            e->weight = weight;
            e->next = graph[graphIndex].edge;
            graph[graphIndex].edge = e;

        }
    }
}

