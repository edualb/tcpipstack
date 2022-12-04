#include<stdio.h>
#include "graph.h"

// getNodeInterfaceAvailableSlot return the interface slot available
static inline int getNodeInterfaceAvailableSlot(node_t *node) {
    int i;
    for (i=0 ; i < MAX_INTF_PER_NODE ; i++) {
        if (node->intf[i] == NULL) {
            return i;
        }
    }
    return -1;
}

// getNeighborNode returns the neighbor node or NULL if there is no neighbor node.
static inline node_t* getNeighborNode(interface_t *interface) {
    if (interface == NULL) {
        return NULL;
    }
    if (interface->att_node == NULL) {
        return NULL;
    }
    if (interface->link == NULL) {
        return NULL;
    }

    link_t *link = interface->link;
    if (&link->intf1 == interface) {
        return link->intf2.att_node;
    }
    return link->intf1.att_node;
}