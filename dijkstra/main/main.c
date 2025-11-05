#include "main.h"

static void buildGraph(FILE* fp);

Node graph[200]; // the edges in the nodes must be freed

/* Compile-time initializaton.
   Uses GCC/Clang range-designator extension (not part of the official ISO C standard) */
int dist[200] = { [0 ... 199] = 1000000 }; 
bool X[200] = { [0 ... 199] = false }; 
int heapindex[200] = { [0 ... 199] = -1 }; 
HeapNode heap[200] = { [0 ... 199] = { .vertex = -1, .key = 1000000 } };
int heapSize = 0;

int main(int argc, char* argv[])
{
    if(argc != 2) {fprintf(stderr, "\nUsage: ./main filename.txt\n"); return 1;}
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {fprintf(stderr, "\nError opening file\n"); return 1;}

    buildGraph(fp);

    Node source = graph[0]; // Taking Node 1 as src

    runDijkstra(source);


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

void runDijkstra(Node source)
{
    // initialise arrays
    int index = source.vertex - 1;
    X[index] = true;
    heap[heapSize].vertex = source.vertex;
    heap[heapSize].key = 0;
    heapindex[index] = heapSize;
    heapSize++; 

    while(heapSize != 0)
    {
        popHeap();
    }

}

static void popHeap(void)
{
    // we must finalise its distance and relax its edges
    int index = heap[0].vertex - 1; 
    if (!(heap[0].key > dist[index]))
        dist[index] = heap[0].key; 

    Edge* head = graph[index].edge; 
    relaxEdges(heap[0].vertex, head); // tail vertex is just the vertex being popped 

    // now we delete this vertex and promote the last leaf in the heap to the first
    // and bubble down to its correct position to restore heap property 
    
}

static void relaxEdges(int tail, Edge* head)
{
    Edge* edge = head;

    while(edge != NULL)
    {
        int index = edge->headVertex - 1;
        int hIndex = heapindex[index];
        if (dist[index] != -1) // i.e finalised
        {
            edge = edge->next;
            continue;
        }

        if (X[index] == false) // i.e not explored
        {
            heapInsert(tail, edge);
        }
        else // i.e explored before
        {
            int DGS = dist[tail - 1] + edge->weight; 
            if (DGS < heap[hIndex].key) 
            {
                    heap[hIndex].key = DGS; 
                    bubbleUp(hIndex);
            }
        }

        edge = edge->next;
    }
}


static void heapInsert(int tail, Edge* edge)
{
    // now its index in the heap = heapSize for now because we havent yet checked
    // if heap property is violated by it
    int hIndex = heapSize; // heapIndex
    heap[hIndex].vertex = edge->headVertex;

    /* now for the key we have to calculate its DGS 
       if edge between: (v*, w*),
       DGS = dist[v*index] + edge->weight;  */
    int DGS = dist[tail - 1] + edge->weight; 
    heap[hIndex].key = DGS;
    heapindex[edge->headVertex - 1] = hIndex;
    bubbleUp(hIndex);
    heapSize++;
}

static void bubbleUp(int u)
{
    // u is the hIndex of the newly inserted vertex in the heap array
    // lets call the edge (v, u). v is the index of parent of u
    int v = (u - 1) / 2; // since our heapSize is starts from 0 
    int tvertex; int tkey;
    while (u != 0 && heap[u].key < heap[v].key) 
    // u!=0 check must be 1st so that the next condition doesnt give undefined behaviour
    {
        tvertex = heap[v].vertex;
        tkey = heap[v].key;

        heap[v].vertex = heap[u].vertex;
        heap[v].key = heap[u].key;

        heap[u].vertex = tvertex;
        heap[u].key = tkey; 

        // Update heapindex[] after the swap
        heapindex[heap[u].vertex - 1] = u;
        heapindex[heap[v].vertex - 1] = v;

        u = v;
        v = (u - 1) / 2;
    }
}

static void buildGraph(FILE* fp)
{
    while(true)
    {
        char* line = fgets(buff, buffsize, fp);
        if(line == NULL) break; 

        char* tkn = strtok(line, " \t\n\r");
        int vertex = atoi(tkn);
        int graphIndex = vertex - 1; // *IMP
        
        graph[graphIndex].vertex = vertex; 
        graph[graphIndex].edge = NULL;
        
        while((tkn = strtok(NULL, " \t\n\r")) != NULL)
        {
            int headVertex = 0;
            int weight = 0;

            char* comma = strchr(tkn, ','); // find the comma
            *comma = '\0';                  // split the string

            headVertex = atoi(tkn);               // part before comma
            weight = atoi(comma + 1);

            Edge* e = malloc(sizeof(Edge));
            e->headVertex = headVertex;
            e->weight = weight;
            e->next = graph[graphIndex].edge;
            graph[graphIndex].edge = e;

        }
    }
    fclose(fp);
    return;  
}

