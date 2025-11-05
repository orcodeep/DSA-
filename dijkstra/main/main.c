#include "main.h"

static void buildGraph(FILE* fp);
static void bubbleUp(int u);
static void heapInsert(int tail, Edge* edge);
static void bubbleDown(int u);
static void relaxEdges(int tail, Edge* head);
static void popHeap(void);
void freeGraph(void);

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

    // print the shortest paths required

    freeGraph();

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
        popHeap();
}

static void popHeap(void)
{
    // we must finalise its distance and relax its edges
    int index = heap[0].vertex - 1; 
    if (dist[index] == -1)
        dist[index] = heap[0].key; 

    Edge* head = graph[index].edge; 
    relaxEdges(heap[0].vertex, head); // tail vertex is just the vertex being popped 

 /* now we delete this vertex and promote the last leaf in the heap to the first
    and then we should bubble down to its correct position to restore heap property.
    heapSize is 0 indexed and always points to the next available space */
    heapindex[heap[0].vertex - 1] = -1;
    heap[0].vertex = heap[heapSize - 1].vertex;
    heap[0].key = heap[heapSize - 1].key;

    heapindex[heap[heapSize - 1].vertex - 1] = 0;
    heap[heapSize - 1].vertex = -1;
    heap[heapSize - 1].key = 1000000;
    heapSize--;

    /* now we bubble down the flase root and then we are done */
    bubbleDown(0);
}

static void relaxEdges(int tail, Edge* head)
{
    Edge* edge = head;

    while(edge != NULL)
    {
        int index = edge->headVertex - 1;
        
        if (dist[index] != -1) // i.e finalised
        {
            edge = edge->next;
            continue;
        }

        if (X[index] == false) // i.e not explored
        {
            heapInsert(tail, edge);
        }
        else                   // i.e explored before
        {
            int DGS = dist[tail - 1] + edge->weight; 
            int hIndex = heapindex[index];
            if (DGS < heap[hIndex].key) 
            {
                    heap[hIndex].key = DGS; 
                    bubbleUp(hIndex);
            }
        }

        edge = edge->next;
    }
}

static void bubbleDown(int u)
{
    int v1; int v2;
    int tvertex; int tkey;
    int smallest;

    while(true)
    {
        v1 = 2*u + 1;
        v2 = 2*u + 2;
        smallest = u;

        if (v1 < heapSize && heap[v1].key < heap[u].key)
            smallest = v1;
        if (v2 < heapSize && heap[v2].key < heap[smallest].key)
            smallest = v2;

        if (smallest == u)
            break;

        tvertex = heap[smallest].vertex;
        tkey = heap[smallest].key;

        heap[smallest].vertex = heap[u].vertex;
        heap[smallest].key = heap[u].key;

        heap[u].vertex = tvertex;
        heap[u].key = tkey;

        // Update heapindex[] after the swap 
        heapindex[heap[u].vertex - 1] = u;
        heapindex[heap[smallest].vertex - 1] = smallest;

        u = smallest;
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
    X[edge->headVertex - 1] = true;
    bubbleUp(hIndex);
    if (heapSize <= 199)
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

void freeGraph(void)
{
    Edge* head; Edge* current;
    for (int i = 0; i < 200; i++)
    {
        head = graph[i].edge;
        current = head;
        Edge* next;
        while(current != NULL)
        {
            next = current->next;
            free(current);
            current = next;
        }
        graph[i].edge = NULL; /* Otherwise graph[i].edge still points to old memory
                                 i.e dangling pointer. Pointing it to NULL is safer */
    }
}
