/** 
 * Header file for min cuts randomized contraction algorithm for graphs.
 * 
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.07.26
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/GraphMinCuts
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

#ifndef _MIN_CUTS_H_
#define _MIN_CUTS_H_

struct Node {
	int vertex;
	struct Node* next;
};
typedef struct Node Node;

struct Graph {
	int n;
	int m;
	struct Node** edges;
	int* edgesCount;
};
typedef struct Graph Graph;

/* function prototypes */
void err(const char*);
int readGraphFromFile(Graph*, char*, char*);
void printGraph(const Graph);
int randomizedContraction(const Graph);
void freeGraph(Graph*);
Graph cloneGraph(const Graph);
Node* cloneNode(Node*);

#endif /* _MIN_CUTS_H_ */
