#include "gluethread/gluethread.h"

#define NODE_NAME_SIZE 16
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

/* Forward Declaration */
typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct interface_ {
    char if_name[IF_NAME_SIZE];
    struct node_ *att_node;
    struct link_ *link;
} interface_t;

struct link_ {
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
};

struct node_ {
    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE];
    glthread_t graph_glue; // linked list
};

struct graph_ {
    char topology_name[32];
    glthread_t node_list; // linked list
};

static inline node_t* get_nbr_node(interface_t *interface);

// get_node_intf_available_slot return the interface slot available
static inline int get_node_intf_available_slot(node_t *node);
