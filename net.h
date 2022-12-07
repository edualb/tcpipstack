#define IPV4_LENGTH 16
#define MAC_ADDR_LENGTH 6

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
    unsigned char addr[MAC_ADDR_LENGTH];
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
void net_AssignMACAddr(intf_net_prop_t *intfProps);
void net_SetBroadcastMACAddr(intf_net_prop_t *intfProps);
void net_DumpNodeNetProps(node_net_prop_t *nodeProps, char *prefix);
void net_DumpInterfaceNetProps(intf_net_prop_t *intfProps, char *prefix);
void net_ApplyMask(char *fromIP, char *toIP, char mask);

int net_SetLoopbackAddrNode(node_net_prop_t *nodeProps, char *IPAddr);
int net_SetInterfaceIPAddr(intf_net_prop_t *intfProps, char *IPAddr, char mask);
int net_UnsetInterfaceIPAddr(intf_net_prop_t *intfProps);
int net_IsMACBroadcast(intf_net_prop_t *intfProps);