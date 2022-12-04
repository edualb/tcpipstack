#include "graph.h"

/*
* FirstTopology representation:
*
*                         +------------+
*                     0/4 |            |0/0
*        +----------------+   R0_re    +---------------------------+
*        |     40.1.1.1/24| 122.1.1.0  |20.1.1.1/24                |
*        |                +------------+                           |
*        |                                                         |
*        |                                                         |
*        |                                                         |
*        |40.1.1.2/24                                              |20.1.1.2/24
*        |0/5                                                      |0/1
*    +---+---+                                                +----+-----+
*    |       |0/3                                          0/2|          |
*    | R2_re +------------------------------------------------+   R1_re  |
*    |       |30.1.1.2/24                          30.1.1.1/24|          |
*    +-------+                                                +----------+
*/
graph_t *FirstTopology() {
    graph_t *topology = NewGraph("Generic Graph");

    node_t *R0_re = InsertNode(topology, "R0_re");
    node_t *R1_re = InsertNode(topology, "R1_re");
    node_t *R2_re = InsertNode(topology, "R2_re");

    InsertLinkBetweenTwoNodes(R0_re, R1_re, "eth0/0", "eth0/1", 1);
    InsertLinkBetweenTwoNodes(R1_re, R2_re, "eth0/2", "eth0/3", 1);
    InsertLinkBetweenTwoNodes(R2_re, R0_re, "eth0/5", "eth0/4", 1);

    return topology;
}