#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

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
    int ok;

    graph_t *topology = graph_New("Generic Graph");

    node_t *R0_re = graph_InsertNode(topology, "R0_re");
    node_t *R1_re = graph_InsertNode(topology, "R1_re");
    node_t *R2_re = graph_InsertNode(topology, "R2_re");

    graph_InsertLinkBetweenTwoNodes(R0_re, R1_re, "eth0/0", "eth0/1", 1);
    graph_InsertLinkBetweenTwoNodes(R1_re, R2_re, "eth0/2", "eth0/3", 1);
    graph_InsertLinkBetweenTwoNodes(R2_re, R0_re, "eth0/5", "eth0/4", 1);

    // R0_re
    net_SetLoopbackAddrNode(&R0_re->node_net_props, "122.1.1.0");
    interface_t *eth00 = graph_GetNodeIFByName(R0_re, "eth0/0");
    if (eth00 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth00->intf_net_props, "20.1.1.1", 24);
    if (!ok) {
        return NULL;
    }
    interface_t *eth04 = graph_GetNodeIFByName(R0_re, "eth0/4");
    if (eth04 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth04->intf_net_props, "40.1.1.1", 24);
    if (!ok) {
        return NULL;
    }

    // R1_re
    net_SetLoopbackAddrNode(&R1_re->node_net_props, "122.1.1.1");
    interface_t *eth01 = graph_GetNodeIFByName(R1_re, "eth0/1");
    if (eth01 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth01->intf_net_props, "20.1.1.2", 24);
    if (!ok) {
        return NULL;
    }
    interface_t *eth02 = graph_GetNodeIFByName(R1_re, "eth0/2");
    if (eth02 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth02->intf_net_props, "30.1.1.1", 24);
    if (!ok) {
        return NULL;
    }

    // // R2_re
    net_SetLoopbackAddrNode(&R2_re->node_net_props, "122.1.1.2");
    interface_t *eth05 = graph_GetNodeIFByName(R2_re, "eth0/5");
    if (eth05 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth05->intf_net_props, "40.1.1.2", 24);
    if (!ok) {
        return NULL;
    }
    interface_t *eth03 = graph_GetNodeIFByName(R2_re, "eth0/3");
    if (eth03 == NULL) {
        return NULL;
    }
    ok = net_SetInterfaceIPAddr(&eth03->intf_net_props, "30.1.1.2", 24);
    if (!ok) {
        return NULL;
    }

    return topology;
}

int main() {
    graph_t *topology = FirstTopology();
    if (topology == NULL) {
        fprintf(stderr, "empty topology");
        return 1;
    }
    graph_GraphDump(topology);
    return 0;
}