# Dijkstra's Algorithm

To start put the source vertex in the heap with key value 0.

Now as long as the heap is not empty pop the min.

When you pop a vertex:

    1. Its distance is finalised 

    2. Its edges are relaxed 

    3. Then the vertex is removed from the heap

    4. Smallest leaf of the heap takes its place(and heapSize--;) 
    and then bubbles down as required to restore the Heap Property.


Relaxing edges marks new vertices as Explored (if it wasnt already Explored or finalised) and the heapSize is incremented by 1 for each new vertex Explored. 

The heapSize tells us the next available place in the heap array. 

    - Put newly explored vertex in that position and then swap as required to restore heap condition.