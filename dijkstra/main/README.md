# OUTPUT

The shortest path from the Node 1 to the nodes 
7,37,59,82,99,115,133,165,188,197 in that order
is:

2599,2610,2947,2052,2367,2399,2029,2442,2505,3068 

# Tokenization 

- strtok() returns each (HEAD,WEIGHT) edge 

- in buildGraph() graph[graphIndex].edge is set to NULL which will result in First found edge's next ptr to point to NULL 

- Each new edge token is appended to beginning of list (last given edge = first edge in edge list of a node) 
  
# Arrays Used

dist[200] contains the current shortest distances of all nodes from the Source  

    If a node was unreachable from the source its dist = 1000000  

X[200] tells if a node has been explored or not. Initially all nodes are unexplored 

    1. X[i] == true means the vertex is or previously was in the heap.

    2. I do not use X to tell if the vertex has been finalised.

    3. This will work even though a vertex that was popped from the heap 
    is still marked explored. The condition to compare its DGS with a heap 
    key will never be reached because it will be checked by the finalised
    distance condition before.

heapindex[200] is the auxilary map for the heap which provides O(1) search time for a vertex in the heap 

heap[200] contains HeapNodes which are (vertex, Dijkstra's greedy score for this vertex)


# NOTE 

In bubbleUp():

After the swap now [heap[u].vertex - 1] = [parentVertexID - 1] not [newNodeVertexID - 1] so when we do heapindex[heap[u].vertex - 1] we are doing heap[parent node] and parent node is now at u not v so heap[parent node] = u is setting the hIndex of that parent node correctly now

For bubbleDown():

    1. First ask Do the children exist

    2. Then compare the parent’s key against those that exist.

    3. If only one child exists you only compare with that one.