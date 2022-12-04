#include "gluethread/gluethread.h"

#define NODE_NAME_SIZE 16
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

/* Forward declaration */
typedef struct node_ node_t;
typedef struct link_ link_t;
typedef struct interface_ interface_t;

/* Struct implementation */
struct interface_ {
    char if_name[IF_NAME_SIZE];
    struct node_ *att_node;
    struct link_ *link;
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
};

struct graph_ {
    char topology_name[32];
    glthread_t node_list; // linked list
};

/* Functions to be implemented */
static inline node_t* get_nbr_node(interface_t *interface);
static inline int get_node_intf_available_slot(node_t *node);
