
/**
 * Min cuts randomized contraction algorithm for graphs.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.07.26
 * @version     1.0
 * @link        GitHub  https://github.com/z3ppelin/GraphMinCuts
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */
import java.io.FileInputStream;
import java.util.*;

public class GraphMinCuts {

    /**
     * Main function. Reads graph, reapeats randomized contraction algorithm a
     * few times and prints the result.
     *
     * @param   args    Command line arguments.
     */
    public static void main(String[] args) {
        System.out.println("------ Begin Graph Min Cuts ------");
        Graph graph = null;
        long start, end;
        int minCutsCount = 10000000, numberOfCuts;

        /* read vector */
        try {
            if (args.length == 0) {
                throw new Exception("The input file must be given as an argument.");
            }
            graph = readGraphFromFile(args[0]);
        } catch (Exception ex) {
            System.out.println("ERR. " + ex.getMessage());
            System.out.println("------- End Graph Min Cuts -------");
            System.exit(-1);
        }

        /* print read graph */
        //System.out.println("The read graph is:");
        //printGraph(graph);
        //System.out.println();

        /* compute min cuts */
        start = System.currentTimeMillis();
        for (int i = 0; i < graph.n * 50; i++) {
            numberOfCuts = randomizedContraction(graph);
            if (numberOfCuts < minCutsCount) {
                minCutsCount = numberOfCuts;
            }
        }
        end = System.currentTimeMillis();

        /* print result */
        System.out.println("Min cuts count is: " + minCutsCount);
        System.out.println("Elapsed: " + ((double) (end - start) / 100) + " seconds to compute the min cuts count.");
        System.out.println("------- End Graph Min Cuts -------\n");
    }

    /**
     * Prints graph.
     * 
     * @param   graph   The graph to print.
     */
    public static void printGraph(Graph graph) {
        System.out.println("Graph has " + graph.n + " vertices and " + graph.m + " edges.");
        Node node;
        for (int i = 0; i < graph.n; i++) {
            System.out.print("Vertex " + (i + 1) + " has edge with: ");
            node = graph.edges[i];
            if (null == node) {
                System.out.print("nobody");
            } else {
                while (null != node) {
                    System.out.print((node.vertex + 1) + " ");
                    node = node.next;
                }
            }
            System.out.println();
        }
    }

    /**
     * Reads graph from file.
     *
     * @param   file        The file where to read array from.
     * @return              The read graph.
     * @throws  Exception
     */
    public static Graph readGraphFromFile(String file) throws Exception {
        Scanner sc, sc2;
        FileInputStream fis = null;
        int n, m = 0, vertex, neighbor, edgesC;
        Node[] edges = null;
        int[] edgesCount = null;
        String line;
        try {
            fis = new FileInputStream(file);
            sc = new Scanner(fis);
            n = sc.nextInt();
            sc.nextLine();
            edges = new Node[n];
            edgesCount = new int[n];
            for (int i = 0; i < n; i++) {
                if (sc.hasNextLine()) {
                    line = sc.nextLine();
                    sc2 = new Scanner(line);
                    vertex = sc2.nextInt() - 1;
                    edgesC = 0;
                    /* read nodes that have edge with vertex */
                    while (sc2.hasNextInt()) {
                        neighbor = sc2.nextInt() - 1;
                        if (null == edges[vertex]) {
                            edges[vertex] = new Node(neighbor);
                        } else {
                            edges[vertex] = new Node(neighbor, edges[vertex]);
                        }
                        edgesC++;
                    }
                    edgesCount[vertex] = edgesC;
                    m += edgesC;
                } else {
                    throw new Exception("Number of declared elements does not match with the one found in file.");
                }
            }
            fis.close();
        } catch (Exception ex) {
            if (fis != null) {
                try {
                    fis.close();
                } catch (Exception e) {
                }
            }
            throw ex;
        }
        return new Graph(n, m, edges, edgesCount);
    }

    /**
     * Min cuts randomized contraction algorithm. It does not always return the
     * correct value, should be repeated. For n^2 * log n the probaility to fail
     * is < 1 / n
     *
     * @param   graph   The graph to calculate min cuts.
     * @return          Min cuts count.
     */
    public static int randomizedContraction(Graph graph) {
        Graph g = graph.clone();
        Random randomGenerator = new Random();
        int randomVertex, randomLinkedVertexIndex, randomLinkedVertex, graphNoOfNodes = graph.n, k;
        Node node, node2, node3, node3Prev;
        boolean[] randomlyChoosed = new boolean[g.n];
        Arrays.fill(randomlyChoosed, false);

        while (graphNoOfNodes > 2) {
            /* pickup a random edge */
            do {
                randomVertex = randomGenerator.nextInt(graph.n);
            } while (randomlyChoosed[randomVertex]);
            randomlyChoosed[randomVertex] = true;
            randomLinkedVertexIndex = randomGenerator.nextInt(g.edgesCount[randomVertex]);
            node = g.edges[randomVertex];
            k = 0;
            while (null != node && k < randomLinkedVertexIndex) {
                node = node.next;
                k++;
            }
            /* contract randomVertex into randomLinkedVertex */
            if (null != node) {
                randomLinkedVertex = node.vertex;
                /* modify globally edges to point to the new combined vertex */
                for (int i = 0; i < graph.n; i++) {
                    if (!randomlyChoosed[i]) {
                        node2 = g.edges[i];
                        while (null != node2) {
                            if (node2.vertex == randomVertex) {
                                node2.vertex = randomLinkedVertex;
                            }
                            node2 = node2.next;
                        }
                    }
                }
                /* add randomVertex 's edges to the randomLinkedVertex 's edges */
                node2 = g.edges[randomVertex];
                if (null != node2) {
                    node3 = g.edges[randomLinkedVertex];
                    if (null != node3) {
                        while (null != node3.next) {
                            node3 = node3.next;
                        }
                        node3.next = node2.clone();
                    } else {
                        g.edges[randomLinkedVertex] = node2.clone();
                    }
                }
                g.edgesCount[randomLinkedVertex] += g.edgesCount[randomVertex];
                /* eliminate self pointing edges of randomLinkedVertex */
                node3 = g.edges[randomLinkedVertex];
                node3Prev = null;
                while (null != node3) {
                    if (node3.vertex == randomLinkedVertex || node3.vertex == randomVertex) {
                        if (null != node3Prev) {
                            node3Prev.next = node3.next;
                        } else {
                            g.edges[randomLinkedVertex] = node3.next;
                        }
                        node3 = node3.next;
                        g.edgesCount[randomLinkedVertex]--;
                    } else {
                        node3Prev = node3;
                        node3 = node3.next;
                    }
                }
            }
            graphNoOfNodes--;
        }
        /* 2 vertices left, return the number of edges of the first one found */
        for (int i = 0; i < graph.n; i++) {
            if (!randomlyChoosed[i]) {
                return g.edgesCount[i];
            }
        }
        return -1;
    }
}
