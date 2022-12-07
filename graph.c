#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private functions */
static inline node_t* getNeighborNode(interface_t *interface);
static inline int getNodeInterfaceAvailableSlot(node_t *node);
static inline node_t* glthreadToNode(glthread_t *glthread);
static void insertLinkIntoNode(link_t *link, interface_t *intf, node_t *node, char *IFName);

// InsertLinkBetweenTwoNodes insert and link two nodes by an interface
void graph_InsertLinkBetweenTwoNodes(node_t *n1, node_t *n2, char *fromIFName, char *toIFNamem, unsigned int cost) {
    link_t *link = malloc(sizeof(link_t));
    link->cost = cost;

    insertLinkIntoNode(link, &link->intf1, n1, fromIFName);
    insertLinkIntoNode(link, &link->intf2, n2, toIFNamem);
}

void graph_GraphDump(graph_t *graph) {
    glthread_t *source;
    node_t *node;
    int i;

    fprintf(stdout, "\n");

    source = &graph->node_list;
    while (source != NULL) {
        node = glthreadToNode(source);

        // verify if node exists
        if (node->node_name == NULL || strcmp(node->node_name, "") == 0) {
            source = gluethread_GetNext(source);
            continue;
        }

        fprintf(stdout, "Node name: %s\n", node->node_name);
        net_DumpNodeNetProps(&node->node_net_props, "   ");

        for (i=0 ; i < MAX_INTF_PER_NODE; i++) {
            if (node->intf[i] == NULL) {
                continue;
            }
            interface_t *intf = node->intf[i];
            node_t *nbrNode = getNeighborNode(node->intf[i]);

            fprintf(stdout, "Interface Name: %s\n", intf->if_name);
            fprintf(stdout, "   Local Node: %s | Neighbot Node: %s | Link Cost: %d\n", 
                intf->att_node->node_name,
                nbrNode->node_name,
                intf->link->cost
            );
            net_DumpInterfaceNetProps(&intf->intf_net_props, "   ");
        }

        fprintf(stdout, "\n\n");
        source = gluethread_GetNext(source);
    }
}

// New returns the a graph that represents a topology
graph_t* graph_New(char *topologyName) {
    graph_t *graph = malloc(sizeof(graph_t));

    strncpy(graph->topology_name, topologyName, TOPOLOGY_NAME_SIZE);
    graph->topology_name[TOPOLOGY_NAME_SIZE - 1] = '\0';

    gluethread_NewGLThread(&graph->node_list);
    return graph;
}

// GetNodeIFByName find and return the local interface of a node by the name of the interface. When not found, return NULL
interface_t* graph_GetNodeIFByName(node_t *node, char *IFName) {
    int i;
    for (i = 0 ; i < MAX_INTF_PER_NODE ; i++) {
        if (node->intf[i] == NULL) {
            continue;
        }
        if (strcmp(node->intf[i]->if_name, IFName) == 0) {
            return node->intf[i];
        }
    }
    return NULL;
}

// GetIFSubnettedWithIP returns the interface that shares the same subnet of IPAddr. In case of don't matches, it returns NULL
interface_t* graph_GetIFSubnettedWithIP(node_t *node, char *IPAddr) {
    int i;
    for (i = 0 ; i < MAX_INTF_PER_NODE ; i++) {
        interface_t *intf = node->intf[i];
        if (intf == NULL) {
            continue;
        }
        if(!intf->intf_net_props.is_ip_addr_available) {
            continue;
        }

        char mask = intf->intf_net_props.mask;
        char intfIPMask[IPV4_LENGTH];
        net_ApplyMask(intf->intf_net_props.ip.addr, intfIPMask, mask);

        char IPMask[IPV4_LENGTH];
        net_ApplyMask(IPAddr, IPMask, mask);

        if (strcmp(intfIPMask, IPMask) == 0) {
            return intf;
        }
    }
    return NULL;
}

// GetNodeByName find and return a node by its name, otherwise, return NULL
node_t* graph_GetNodeByName(graph_t *graph, char *nodeName) {
    glthread_t *source;
    node_t *node;

    source = &graph->node_list;
    while (source != NULL) {
        node = glthreadToNode(source);
        if (node->node_name == NULL || strcmp(node->node_name, "") == 0) {
            source = gluethread_GetNext(source);
            continue;
        }
        if (strcmp(node->node_name, nodeName) == 0) {
            return node;
        }
        source = gluethread_GetNext(source);
    }
    return NULL;
}

// InsertNode insert a new node into the graph (topology) and return this node
node_t* graph_InsertNode(graph_t *graph, char *nodeName) {
    node_t *node = malloc(sizeof(node_t));

    strncpy(node->node_name, nodeName, NODE_NAME_SIZE);
    node->node_name[NODE_NAME_SIZE - 1] = '\0';

    net_SetEmptyNodeNetworkProperties(&node->node_net_props);

    gluethread_NewGLThread(&node->graph_glue);
    gluethread_AddNext(&graph->node_list, &node->graph_glue);
    return node;
}

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

// glthreadToNode transform a glthread into a node_t. if it is not possible, any field in node_t will be empty (or NULL if pointer)
static inline node_t* glthreadToNode(glthread_t *glthread) {
    char *glthreadOffset;
    char *glthreadPointer;
    char *nodePointer;

    glthreadOffset = (char *)&((node_t *)NULL)->graph_glue;
    glthreadPointer = (char *)(glthread);
    nodePointer = (char *)(glthreadPointer - glthreadOffset);

    node_t *node = (node_t *)nodePointer;
    return node;
}

// insertLinkIntoNode it "plugs" an interface and a link into a node
static void insertLinkIntoNode(link_t *link, interface_t *intf, node_t *node, char *IFName) {
    int empty_slot = getNodeInterfaceAvailableSlot(node);
    if (empty_slot == -1) {
        fprintf(stdout, "Node: %s with no available slot.", node->node_name);
        return;
    }

    intf->att_node = node;
    strncpy(intf->if_name, IFName, IF_NAME_SIZE);
    intf->if_name[IF_NAME_SIZE - 1] = '\0';
    intf->link = link;
    net_SetEmptyInterfaceNetworkProperties(&intf->intf_net_props);
    net_AssignMACAddr(&intf->intf_net_props);

    node->intf[empty_slot] = intf;
}

// static void print

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