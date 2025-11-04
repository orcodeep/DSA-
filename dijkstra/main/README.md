# Tokenization 

- strtok() returns each (HEAD,WEIGHT) edge 

- graph[graphIndex].edge is set to NULL which will result in First found edge's next ptr to point to NULL 

- Each new edge token is appended to beginning of list (last given edge = first edge in edge list of a node) 
  
# Arrays Used

- dist[200] contains the current shortest distances of all nodes from the Source  
    - If a node was unreachable from the source its dist = 1000000  

- X[200] tells if a node has been explored or not. Initially all nodes are unexplored 

- heapindex[200] is the auxilary map for the heap which provides O(1) search time for a vertex in the heap 

- heap[200] contains HeapNodes which are (vertex, Dijkstra's greedy score for this vertex)
