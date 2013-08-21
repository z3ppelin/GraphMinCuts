Karger 's  min cuts randomized contraction algorithm.
===================

About
------------
Karger 's algorithm is a randomized algorithm to compute a minimum cut of a connected graph.
It was invented by David Karger and first published in 1993.
The idea of the algorithm is based on the concept of contraction of an edge (u, v) in a graph G = (V, E).
Informally speaking, the contraction of an edge merges the nodes u and v into one, 
reducing the total number of nodes of the graph by one. 
All other edges connecting either u or v are "reattached" to the merged node, 
effectively producing a multigraph. 
Karger's basic algorithm iteratively contracts randomly chosen edges until only two nodes remain; 
those nodes represent a cut in the original graph. 
By iterating this basic algorithm a sufficient number of times, a minimum cut can be found with high probability.

Running code examples
------------
**C** implementation:

You should compile the source file first.

    cd C/
    gcc -o GraphMinCuts GraphMinCuts.c (Linux)
    ./GraphMinCuts ../in/inputMedium.txt (Linux)
    
    GraphMinCuts.exe ../in/inputMedium.txt (Windows)

**Java** implementation:

Used java 1.6.0_33 to compile source files.

    cd Java/
    java GraphMinCuts ../in/inputMedium.txt (Windows & Linux)

To compile yourself the source files:

    cd Java/
    javac Node.java Graph.java GraphMinCuts.java (Windows & Linux)
