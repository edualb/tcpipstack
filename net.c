#include "graph.h"

#include <stdlib.h>
#include <memory.h>

/* Private functions */
static unsigned int hashCode(void *ptr, unsigned int size);
void assignMACAddr(interface_t *intf);

// SetEmptyNodeNetworkProperties set empty values into interface network properties
void net_SetEmptyNodeNetworkProperties(node_net_prop_t *nodeProps) {
    nodeProps->is_lo_available = 0;
    memset(nodeProps->lo_ip.addr, 0, IPV4_LENGTH);
}

// SetEmptyInterfaceNetworkProperties set empty values into interface network properties
void net_SetEmptyInterfaceNetworkProperties(intf_net_prop_t *intfProps) {
    intfProps->is_ip_addr_available = 0;
    intfProps->mask = 0;
    memset(intfProps->mac.addr, 0, MAC_ADDR_LENGTH);
    memset(intfProps->ip.addr, 0, IPV4_LENGTH);
}

void net_DumpNetGraph(graph_t *graph) {
    return;
}

// SetLoopbackAddrNode sets a loopback ip address into a node properties. in case of success, return 1, otherwise, 0
int net_SetLoopbackAddrNode(node_net_prop_t *nodeProps, char *IPAddr) {
    if (IPAddr == NULL) {
        return 0;
    }
    if (strcmp(IPAddr, "") == 0) {
        return 0;
    }

    nodeProps->is_lo_available = 1;

    strcpy(nodeProps->lo_ip.addr, IPAddr);
    nodeProps->lo_ip.addr[IPV4_LENGTH - 1] = '\0';

    return 1;
}

int net_SetInterfaceIPAddrNode(node_t *node, char *localIF, char *IPAddr, char mask) {
    return 0;
}

int net_UnsetInterfaceIPAddrNode(node_t *node, char *localIF) {
    return 0;
}

// AssignMACAddr assign a random value to MAC Address
void net_AssignMACAddr(intf_net_prop_t *intfProps) {
    char *mac = intfProps->mac.addr;

    memset(mac, 0, sizeof(mac));
    int i;
    for (i=0 ; i < (MAC_ADDR_LENGTH - 1) ; i++) {
        if ((i+1) % 3 == 0) {
            mac[i]=':';
            continue;
        }
        char randomletter = "12ABC34DEF56GHI78JKL90MNOPQRSTUVWXYZ"[random () % 36];
        mac[i]=randomletter;
    }
    mac[MAC_ADDR_LENGTH - 1] = '\0';
}