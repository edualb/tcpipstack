#include "gluethread/gluethread.h"
#include "net.h"

#define NODE_NAME_SIZE 16
#define TOPOLOGY_NAME_SIZE 32
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

/* Forward declaration */
typedef struct node_ node_t;
typedef struct link_ link_t;
typedef struct interface_ interface_t;
typedef struct graph_ graph_t;

/* Struct implementation */
struct interface_ {
    char if_name[IF_NAME_SIZE];
    struct node_ *att_node;
    struct link_ *link;
    intf_net_prop_t intf_net_props;
};

struct link_ {
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
};

struct node_ {
    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE];
    glthread_t graph_glue; // linked list
    node_net_prop_t node_net_props;
};

struct graph_ {
    char topology_name[TOPOLOGY_NAME_SIZE];
    glthread_t node_list; // linked list
};

/* Exported functions */
void graph_InsertLinkBetweenTwoNodes(node_t *n1, node_t *n2, char *fromIFName, char *toIFNamem, unsigned int cost);
void graph_GraphDump(graph_t *graph);

graph_t* graph_New(char *topologyName);

node_t* graph_InsertNode(graph_t *graph, char *nodeName);
node_t* graph_GetNodeByName(graph_t *graph, char *nodeName);

interface_t* graph_GetNodeIFByName(node_t *node, char *IFName);