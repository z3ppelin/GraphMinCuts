/**
 * Node class used for a simple linked list.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.07.26
 * @version     1.0
 * @link        GitHub  https://github.com/z3ppelin/GraphMinCuts
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */
public class Node {

    public int vertex;
    public Node next;

    /**
     * Constructor.
     * @param vertex    The vertex to store. 
     */
    public Node(int vertex) {
        this.vertex = vertex;
        this.next = null;
    }

    /**
     * Constructor.
     * @param vertex    The vertex to store.
     * @param node      The next node to link with.
     */
    public Node(int vertex, Node node) {
        this(vertex);
        this.next = node;
    }
    
    /**
     * Clones the object.
     * @return  The cloned object 
     */
    @Override
    public Node clone() {
        Node cloned = new Node(this.vertex);
        if (null != this.next) {
            cloned.next = this.next.clone();
        }
        return cloned;
    }
}
