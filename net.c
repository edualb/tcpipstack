#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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

// DumpNetProps prints all information from an interface and node network properties
void net_DumpNetProps(node_net_prop_t *nodeProps, intf_net_prop_t *intfProps) {
    if (nodeProps->is_lo_available) {
        fprintf(stdout, "Loopback ip address: %s\n", nodeProps->lo_ip.addr);
    } else {
        fprintf(stdout, "Loopback ip address not available\n");
    }
    
    fprintf(stdout, "MAC address: %s\n", intfProps->mac.addr);
    
    if (intfProps->is_ip_addr_available) {
        fprintf(stdout, "Interface ip address: %s\n", intfProps->ip.addr);
    } else {
        fprintf(stdout, "Interface ip address not available\n");
    }
}

// SetLoopbackAddrNode sets a loopback ip address into a node properties. in case of success, return 1, otherwise, 0
int net_SetLoopbackAddrNode(node_net_prop_t *nodeProps, char *IPAddr) {
    if (IPAddr == NULL) {
        return 0;
    }
    if (strcmp(IPAddr, "") == 0) {
        return 0;
    }
    if (strlen(IPAddr) > IPV4_LENGTH) {
        return 0;
    }

    nodeProps->is_lo_available = 1;

    strcpy(nodeProps->lo_ip.addr, IPAddr);
    nodeProps->lo_ip.addr[IPV4_LENGTH - 1] = '\0';

    return 1;
}

// SetInterfaceIPAddr sets all information to an interface network properties
int net_SetInterfaceIPAddr(intf_net_prop_t *intfProps, char *IPAddr, char mask) {
    if (IPAddr == NULL) {
        return 0;
    }
    if (strcmp(IPAddr, "") == 0) {
        return 0;
    }
    if (strlen(IPAddr) > IPV4_LENGTH) {
        return 0;
    }
    if (mask > 32) {
        return 0;
    }

    intfProps->mask = mask;
    intfProps->is_ip_addr_available = 1;
    strcpy(intfProps->ip.addr, IPAddr);
    intfProps->ip.addr[IPV4_LENGTH - 1] = '\0';

    return 1;
}

// UnsetInterfaceIPAddr unsets all information for an interface network properties
int net_UnsetInterfaceIPAddr(intf_net_prop_t *intfProps) {
    char *macAddr = intfProps->mac.addr;
    char *ipAddr = intfProps->ip.addr;

    intfProps->mask = 0;
    intfProps->is_ip_addr_available = 0;
    memset(macAddr, 0, strlen(macAddr));
    memset(ipAddr, 0, strlen(ipAddr));

    return 1;
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