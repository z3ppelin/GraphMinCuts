/**
 * Min cuts randomized contraction algorithm for graphs.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.07.26
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/GraphMinCuts
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "GraphMinCuts.h"

/**
 * Main function. Reads graph, reapeats randomized contraction algorithm a
 * few times and prints the result.
 * @param     argc       Command line arguments no.
 * @param     argv       Command line arguments.
 * @return               Success/error code. (0 is a success, anything else is error).
 */
int main(int argc, char** argv) {
	printf("------ Begin Graph Min Cuts ------\n");
	int i, minCutsCount = 1000000, numberOfCuts;
	Graph graph;
	Node *node1, *node2;
	clock_t start, end;
	char error[128];

	/* read graph */
	if (argc == 1) {
		err("Err. The input file must be given as an argument.\n");
	}
	if (!readGraphFromFile(&graph, argv[1], error)) {
		err(error);
	}

	/* print read graph */
	//printf("The read graph is: ");
	//printGraph(graph);
	//printf("\n");
	
	/* compute min cuts */
	start = clock();
	for (i = 0; i < graph.n * 50; i++) {
		numberOfCuts = randomizedContraction(graph);
		if (numberOfCuts < minCutsCount) {
			minCutsCount = numberOfCuts;
		}
	}
	end = clock();

	/* free memory */
	freeGraph(&graph);

	/* print result */
	printf("Min cuts count is: %d\n", minCutsCount);
	printf("Elapsed: %f seconds to compute the min cuts count.\n",
			(double) (end - start) / CLOCKS_PER_SEC);
	printf("------- End Graph Min Cuts -------\n");
	return EXIT_SUCCESS;
}

/**
 * Prints error and exits program.
 * @param    msg   The error to print.
 */
void err(const char* msg) {
	printf(msg);
	printf("------- End Graph Min Cuts -------\n");
	exit(EXIT_FAILURE);
}

/**
 * Reads graph from file.
 * @param      graph     Pointer to the graph.
 * @param      file      The file where to read graph from.
 * @param      err       An error message, if any occcurred during reading.
 * @return               1 if everything went fine, 0 otherwise. 
 */
int readGraphFromFile(Graph* graph, char* file, char* err) {
	FILE *inputFile = fopen(file, "rt");
	int n, m = 0, i, countLines = 0, vertex, vertexLinked, edgesC;
	struct Node** edges;
	int* edgesCount;
	char buffer[10000];
	char *pbuff = NULL;
	Node* node;

	if (inputFile == NULL) {
		strcpy(err, "Err. Could not open file.\n");
		return 0;
	}
	if (NULL == fgets(buffer, sizeof buffer, inputFile)
	    || 1 != sscanf(buffer, "%d", &n)) {
		fclose(inputFile);
		strcpy(err, "Err. Could not read number of vertices.\n");
		return 0;
	}
	edges = (Node**) malloc(sizeof(Node*) * n);
	edgesCount = (int*) malloc(sizeof(int) * n);
	for (i = 0; i < n; i++) {
		edges[i] = NULL;
		edgesCount[i] = 0;
	}
	while (NULL != fgets(buffer, sizeof buffer, inputFile)) {
		pbuff = strtok(buffer, " \t"); //split line after spaces
		i = edgesC = 0;
		while (NULL != pbuff) {
			if (!i) {
				vertex = atoi(pbuff) - 1;
				if (vertex < 0 || vertex >= n) {
					fclose(inputFile);
					strcpy(err, "Err. Invalid vertex.\n");
					return 0;
				}
				i = 1;
			} else {
				vertexLinked = atoi(pbuff) - 1;
				if (vertexLinked < 0 || vertexLinked >= n) {
					break;
				}
				node = (Node*) malloc(sizeof(Node*));
				node->vertex = vertexLinked;
				if (NULL == edges[vertex]) {
					node->next = NULL;
				} else {
					node->next = edges[vertex];
				}
				edges[vertex] = node;

				edgesC++;
			}
			pbuff = strtok(NULL, " \t");
		}
		edgesCount[vertex] = edgesC;
		m += edgesC;
		countLines++;
	}
	fclose(inputFile);
	free(pbuff);
	if (countLines != n) {
		strcpy(err,	"Err. Number of declared vertices does not match with the one found in file.\n");
		return 0;
	}
	graph->n = n;
	graph->m = m;
	graph->edges = edges;
	graph->edgesCount = edgesCount;
	return 1;
}

/**
 * Prints graph.
 * @param   graph   The graph to print.
 */
void printGraph(const Graph graph) {
	printf("Graph has %d vertices and %d edges.\n", graph.n, graph.m);
	Node* node;
	int i;
	for (i = 0; i < graph.n; i++) {
		printf("Vertex %d has edge with: ", i + 1);
		node = graph.edges[i];
		if (NULL == node) {
			printf("nobody");
		} else {
			while (NULL != node) {
				printf("%d ", node->vertex + 1);
				node = node->next;
			}
		}
		printf("\n");
	}
}

/**
 * Min cuts randomized contraction algorithm. It does not always return the
 * correct value, should be repeated. For n^2 * log n the probaility to fail
 * is < 1 / n
 *
 * @param   graph   The graph to calculate min cuts.
 * @return          Min cuts count.
 */
int randomizedContraction(const Graph graph) {
	Graph g = cloneGraph(graph);

	int randomVertex, randomLinkedVertexIndex, randomLinkedVertex,
			graphNoOfNodes = graph.n, k, returnValue = -1;
	Node *node, *node2, *node3, *node3Prev;
	int* randomlyChoosed = (int*) malloc(sizeof(int) * graph.n);
	int i;
	for (i = 0; i < graph.n; i++) {
		randomlyChoosed[i] = 0;
	}

	while (graphNoOfNodes > 2) {
		/* pickup a random edge */
		do {
			srand(time(NULL));
			randomVertex = rand() % graph.n;
		} while (randomlyChoosed[randomVertex]);
		randomlyChoosed[randomVertex] = 1;
		srand(time(NULL));
		randomLinkedVertexIndex = rand() % g.edgesCount[randomVertex];
		node = g.edges[randomVertex];
		k = 0;
		while (NULL != node && k < randomLinkedVertexIndex) {
			node = node->next;
			k++;
		}
		/* contract randomVertex into randomLinkedVertex */
		if (NULL != node) {
			randomLinkedVertex = node->vertex;
			/* modify globally edges to point to the new combined vertex */
			for (i = 0; i < graph.n; i++) {
				if (!randomlyChoosed[i]) {
					node2 = g.edges[i];
					while (NULL != node2) {
						if (node2->vertex == randomVertex) {
							node2->vertex = randomLinkedVertex;
						}
						node2 = node2->next;
					}
				}
			}
			/* add randomVertex 's edges to the randomLinkedVertex 's edges */
			node2 = g.edges[randomVertex];
			if (NULL != node2) {
				node3 = g.edges[randomLinkedVertex];
				if (NULL != node3) {
					while (NULL != node3->next) {
						node3 = node3->next;
					}
					node3->next = cloneNode(node2);
				} else {
					g.edges[randomLinkedVertex] = cloneNode(node2);
				}
			}
			g.edgesCount[randomLinkedVertex] += g.edgesCount[randomVertex];
			/* eliminate self pointing edges of randomLinkedVertex */
			node3 = g.edges[randomLinkedVertex];
			node3Prev = NULL;
			while (NULL != node3) {
				if (node3->vertex == randomLinkedVertex
						|| node3->vertex == randomVertex) {
					if (NULL != node3Prev) {
						node3Prev->next = node3->next;
					} else {
						g.edges[randomLinkedVertex] = node3->next;
					}
					node3 = node3->next;
					g.edgesCount[randomLinkedVertex]--;
				} else {
					node3Prev = node3;
					node3 = node3->next;
				}
			}
		}
		graphNoOfNodes--;
	}
	/* 2 vertices left, return the number of edges of the first one found */
	for (i = 0; i < graph.n; i++) {
		if (!randomlyChoosed[i]) {
			returnValue = g.edgesCount[i];
			break;
		}
	}
	freeGraph(&g);
	free(randomlyChoosed);
	return returnValue;
}

/**
 * Frees memory occupied by pointer members of a graph.
 * @param       graph    The graph to free pointer members.
 */
void freeGraph(Graph* graph) {
	Node *node1, *node2;
	int i;
	for (i = 0; i < graph->n; i++) {
		node1 = graph->edges[i];
		while (NULL != node1) {
			node2 = node1;
			node1 = node1->next;
			free(node2);
		}
	}
	free(graph->edges);
	free(graph->edgesCount);
}

/**
 * Clones a graph.
 * @param       graph    The graph to clone.
 * @return               The cloned graph.
 */
Graph cloneGraph(const Graph graph) {
	Graph clonedGraph;
	Node** edges;
	int* edgesCount;
	int i;

	edges = (Node**) malloc(sizeof(Node*) * graph.n);
	edgesCount = (int*) malloc(sizeof(int) * graph.n);
	for (i = 0; i < graph.n; i++) {
		edgesCount[i] = graph.edgesCount[i];

		edges[i] = NULL;
		if (NULL != graph.edges[i]) {
			edges[i] = cloneNode(graph.edges[i]);
		}
	}

	clonedGraph.n = graph.n;
	clonedGraph.m = graph.m;
	clonedGraph.edges = edges;
	clonedGraph.edgesCount = edgesCount;
	return clonedGraph;
}

/**
 * Clones simple linked list.
 * @param node   Pointer to the start node;
 * @return       Pointer to the start node of the cloned list.
 */
Node* cloneNode(Node* node) {
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->vertex = node->vertex;
	if (NULL != node->next) {
		newNode->next = cloneNode(node->next);
	} else {
		newNode->next = NULL;
	}
	return newNode;
}
