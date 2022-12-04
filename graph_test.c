#include "graph.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int TestNewGraph();
int TestGetNodeByName();
int TestGetNodeIFByName();

int main() {
    /* TestNewGraph tests the following methods:
    * graph_New
    */
    if (TestNewGraph() != 0) {
        fprintf(stderr, "TestNewGraph failed\n");
        exit(1);
    }

    /* TestGetNodeByName tests the following methods:
    * graph_New
    * graph_InsertNode
    * graph_GetNodeByName
    */
    if (TestGetNodeByName() != 0) {
        fprintf(stderr, "TestGetNodeByName failed\n");
        exit(1);
    }

    /* TestGetNodeIFByName tests the following methods:
    * graph_New
    * graph_InsertNode
    * graph_InsertLinkBetweenTwoNodes
    * graph_GetNodeIFByName
    */
    if (TestGetNodeIFByName() != 0) {
        fprintf(stderr, "TestGetNodeIFByName failed\n");
        exit(1);
    }

    fprintf(stdout, "Test Graph done!\n");
    exit(0);
}

int TestNewGraph() {
    graph_t *graph = graph_New("Generic Graph");

    if (strcmp(graph->topology_name, "Generic Graph") != 0) {
        fprintf(stderr, "unexpected topology name, receive '%s', wants 'Generic Graph'\n", graph->topology_name);
        return 1;
    }

    if (graph->node_list.right != NULL) {
        fprintf(stderr, "unexpected node list initialization (right node), receive not empty value, wants NULL\n");
        return 1;
    }

    if (graph->node_list.left != NULL) {
        fprintf(stderr, "unexpected node list initialization (left node), receive not empty value, wants NULL\n");
        return 1;
    }

    return 0;
}

int TestGetNodeByName() {
    graph_t *graph = graph_New("Generic Graph");
    if (strcmp(graph->topology_name, "Generic Graph") != 0) {
        fprintf(stderr, "unexpected topology name, receive '%s', wants 'Generic Graph'\n", graph->topology_name);
        return 1;
    }

    node_t *n1 = graph_InsertNode(graph, "Node 1");
    if (strcmp(n1->node_name, "Node 1") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 1'\n", n1->node_name);
        return 1;
    }

    node_t *n2 = graph_InsertNode(graph, "Node 2");
    if (strcmp(n2->node_name, "Node 2") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 2'\n", n2->node_name);
        return 1;
    }

    node_t *n3 = graph_InsertNode(graph, "Node 3");
    if (strcmp(n3->node_name, "Node 3") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 3'\n", n3->node_name);
        return 1;
    }

    node_t *nodeFound = graph_GetNodeByName(graph, "Node 2");
    if (nodeFound->node_name == NULL) {
        fprintf(stderr, "unexpected node empty (not found)\n");
        return 1;
    }
    if (strcmp(nodeFound->node_name, "Node 2") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 2'\n", nodeFound->node_name);
        return 1;
    }

    return 0;
}

int TestGetNodeIFByName() {
        graph_t *graph = graph_New("Generic Graph");
    if (strcmp(graph->topology_name, "Generic Graph") != 0) {
        fprintf(stderr, "unexpected topology name, receive '%s', wants 'Generic Graph'\n", graph->topology_name);
        return 1;
    }

    node_t *n1 = graph_InsertNode(graph, "Node 1");
    if (strcmp(n1->node_name, "Node 1") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 1'\n", n1->node_name);
        return 1;
    }

    node_t *n2 = graph_InsertNode(graph, "Node 2");
    if (strcmp(n2->node_name, "Node 2") != 0) {
        fprintf(stderr, "unexpected node name, receive '%s', wants 'Node 2'\n", n2->node_name);
        return 1;
    }

    graph_InsertLinkBetweenTwoNodes(n1, n2, "eth0/0", "eth0/1", 1);

    interface_t *if_n1 = graph_GetNodeIFByName(n1, "eth0/0");
    if (if_n1 == NULL) {
        fprintf(stderr, "unexpected interface[if_n1] empty (not found)\n");
        return 1;
    }
    if (strcmp(if_n1->if_name, "eth0/0") != 0) {
        fprintf(stderr, "unexpected interface name, receive '%s', wants 'eth0/0'\n", if_n1->if_name);
        return 1;
    }
    if (if_n1->att_node != n1) {
        fprintf(stderr, "unexpected interface node attached, wants n1, got another one\n");
        return 1;
    }

    interface_t *if_n2 = graph_GetNodeIFByName(n2, "eth0/1");
    if (if_n2 == NULL) {
        fprintf(stderr, "unexpected interface[if_n2] empty (not found)\n");
        return 1;
    }
    if (strcmp(if_n2->if_name, "eth0/1") != 0) {
        fprintf(stderr, "unexpected interface name, receive '%s', wants 'eth0/1'\n", if_n2->if_name);
        return 1;
    }
    if (if_n2->att_node != n2) {
        fprintf(stderr, "unexpected interface node attached, wants n2, got another one\n");
        return 1;
    }

    if (if_n2->link != if_n1->link) {
        fprintf(stderr, "unexpected link between interfaces [if_n2<->if_n1]\n");
        return 1;
    }

    return 0;
}