#define IPV4_LENGTH 16
#define MAC_ADDR_LENGTH 8

/* Forward declaration */
typedef struct ip_add_ ip_add_t;
typedef struct mac_add_ mac_add_t;
typedef struct node_net_prop_ node_net_prop_t;
typedef struct intf_net_prop_ intf_net_prop_t;

/* Struct implementation */
struct ip_add_ {
    char addr[IPV4_LENGTH];
};

struct mac_add_ {
    char addr[MAC_ADDR_LENGTH];
};

struct node_net_prop_ {
    int is_lo_available;
    ip_add_t lo_ip;
};

struct intf_net_prop_ {
    mac_add_t mac;

    int is_ip_addr_available;
    ip_add_t ip;
    char mask;
};

/* Exported functions */
void net_SetEmptyNodeNetworkProperties(node_net_prop_t *nodeNetProps);
void net_SetEmptyInterfaceNetworkProperties(intf_net_prop_t *intfNetProps);