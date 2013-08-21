/**
 * Graph class.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.07.26
 * @version     1.0
 * @link        GitHub  https://github.com/z3ppelin/GraphMinCuts
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */
public class Graph {

    public int n;
    public int m;
    public Node[] edges;
    public int[] edgesCount;

    /**
     * Constructor.
     * @param   n             Number of vertices the graph has.
     * @param   m             Number of edges the graph has.
     * @param   edges         The edges, key is the vertex and the value is a simple linked list of vertices the vertex has edges with.
     * @param   edgesCount    Stores for each vertex how many edges has.
     */
    public Graph(int n, int m, Node[] edges, int[] edgesCount) {
        this.n = n;
        this.m = m;
        this.edges = edges;
        this.edgesCount = edgesCount;
    }
    
    /**
     * Retreive a cloned object.
     * @return  The cloned object.
     */
    @Override
    public Graph clone() {
        Node[] edges = new Node[this.n];
        for (int i = 0; i < n; i++) {
            edges[i] = new Node(this.edges[i].vertex, this.edges[i].next.clone());
        }
        return new Graph(this.n, this.m, edges, this.edgesCount.clone());
    }
}
