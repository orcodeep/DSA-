#include "main.h"

Node graph[200];

int main(int argc, char* argv[])
{
    if(argc != 2) {fprintf(stderr, "\nUsage: ./main filename.txt\n"); return 1;}
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {fprintf(stderr, "\nError opening file\n"); return 1;}

    while(true)
    {
        char* line = fgets(buff, buffsize, fp);
        if(line == NULL) break; 

        char* tkn = strtok(line, " \t\n\r");
        int vertex = atoi(tkn);
        int graphIndex = vertex - 1;
        
        graph[graphIndex].vertex = vertex; 
        graph[graphIndex].edge = NULL;
        
        while((tkn = strtok(NULL, " \t\n\r")) != NULL)
        {
            int head = 0;
            int weight = 0;

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
    fclose(fp);

    /*
    TEST:-

    printf("%i\n", graph[0].vertex);
    Edge* e = graph[0].edge;
    while(e != NULL)
    {
        printf("%i, %i\n", e->head, e->weight);
        e = e->next;
    }
    */
}

