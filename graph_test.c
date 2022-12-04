#include "graph.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int TestNewGraph();

int main() {
    if (TestNewGraph() != 0) {
        fprintf(stderr, "TestNewGraph failed\n");
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