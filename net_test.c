#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TestSetEmptyNodeNetworkProperties();
int TestSetEmptyInterfaceNetworkProperties();
int TestAssignMACAddr();
int TestSetLoopbackAddrNode();
int TestSetInterfaceIPAddr();

int main() {
    fprintf(stdout, "\n\nStarting Test net...\n");

    /* TestSetEmptyNodeNetworkProperties tests the following methods:
    * net_SetEmptyNodeNetworkProperties
    */
    if (TestSetEmptyNodeNetworkProperties() != 0) {
        fprintf(stderr, "TestSetEmptyNodeNetworkProperties: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "TestSetEmptyNodeNetworkProperties: OK\n");

    /* TestSetEmptyInterfaceNetworkProperties tests the following methods:
    * net_SetEmptyInterfaceNetworkProperties
    */
    if (TestSetEmptyInterfaceNetworkProperties() != 0) {
        fprintf(stderr, "TestSetEmptyInterfaceNetworkProperties: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "TestSetEmptyInterfaceNetworkProperties: OK\n");

    /* TestAssignMACAddr tests the following methods:
    * net_AssignMACAddr
    */
    if (TestAssignMACAddr() != 0) {
        fprintf(stderr, "TestAssignMACAddr: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "TestAssignMACAddr: OK\n");

    /* TestSetLoopbackAddrNode tests the following methods:
    * net_SetLoopbackAddrNode
    */
    if (TestSetLoopbackAddrNode() != 0) {
        fprintf(stderr, "TestSetLoopbackAddrNode: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "TestSetLoopbackAddrNode: OK\n");

    /* TestSetInterfaceIPAddr tests the following methods:
    * net_SetInterfaceIPAddr
    * net_UnsetInterfaceIPAddr
    */
    if (TestSetInterfaceIPAddr() != 0) {
        fprintf(stderr, "TestSetInterfaceIPAddr: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "TestSetInterfaceIPAddr: OK\n");

    fprintf(stdout, "Test net done!\n");
    exit(0);
}

int TestSetEmptyNodeNetworkProperties() {
    node_net_prop_t *netProps = malloc(sizeof(node_net_prop_t));
    net_SetEmptyNodeNetworkProperties(netProps);

    if (netProps->is_lo_available) {
        fprintf(stderr, "unexpected net properties loopback availability, receive '%d', wants '0'\n", netProps->is_lo_available);
        return 1;
    }

    int i;
    for (i = 0 ; i < IPV4_LENGTH ; i++) {
        if (netProps->lo_ip.addr[i] == 0) {
            continue;
        }
        fprintf(stderr, "unexpected net properties loopback ip address, receive '%c', wants '0'\n", netProps->lo_ip.addr[i]);
        return 1;
    }

    return 0;
}

int TestSetEmptyInterfaceNetworkProperties() {
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));
    net_SetEmptyInterfaceNetworkProperties(intfProps);

    if (intfProps->is_ip_addr_available) {
        fprintf(stderr, "unexpected interface properties ip availability, receive '%d', wants '0'\n", intfProps->is_ip_addr_available);
        return 1;
    }

    if (intfProps->mask != 0) {
        fprintf(stderr, "unexpected interface properties mask, receive '%c', wants '0'\n", intfProps->mask);
        return 1;
    }

    int i;
    for (i = 0 ; i < IPV4_LENGTH ; i++) {
        if (intfProps->ip.addr[i] == 0) {
            continue;
        }
        fprintf(stderr, "unexpected interface properties ip address, receive '%c', wants '0'\n", intfProps->ip.addr[i]);
        return 1;
    }
    for (i = 0 ; i < MAC_ADDR_LENGTH ; i++) {
        if (intfProps->mac.addr[i] == 0) {
            continue;
        }
        fprintf(stderr, "unexpected interface properties mac address, receive '%c', wants '0'\n", intfProps->mac.addr[i]);
        return 1;
    }

    return 0;
}

int TestAssignMACAddr() {
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));
    net_AssignMACAddr(intfProps);
    char *mac = intfProps->mac.addr;

    int i;
    for(i=0 ; i < (MAC_ADDR_LENGTH - 1) ; i++) {
        if ((i+1) % 3 != 0) {
            continue;
        }
        if (mac[i] != ':') {
            fprintf(stderr, "unexpected mac address (missing colon), receive '%c' (MAC ADDRESS: %s), wants ':'\n", mac[i], mac);
            return 1;
        }
    }
    return 0;
}

int TestSetLoopbackAddrNode() {
    int ok;
    node_net_prop_t *netProps = malloc(sizeof(node_net_prop_t));
    net_SetEmptyNodeNetworkProperties(netProps);

    ok = net_SetLoopbackAddrNode(netProps, "");
    if (ok) {
        fprintf(stderr, "unexpected net_SetLoopbackAddrNode response (with empty ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }

    ok = net_SetLoopbackAddrNode(netProps, NULL);
    if (ok) {
        fprintf(stderr, "unexpected net_SetLoopbackAddrNode response (with NULL ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }
    
    ok = net_SetLoopbackAddrNode(netProps, "255.255.255.255.255");
    if (ok) {
        fprintf(stderr, "unexpected net_SetLoopbackAddrNode response (with big ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }

    ok = net_SetLoopbackAddrNode(netProps, "255.255.255.255");
    if (!ok) {
        fprintf(stderr, "unexpected net_SetLoopbackAddrNode response, receive '%d' , wants '1'\n", ok);
        return 1;
    }
    if (!netProps->is_lo_available) {
        fprintf(stderr, "unexpected loopback availability, receive '%d' , wants '1'\n", netProps->is_lo_available);
        return 1;
    }
    if (strcmp(netProps->lo_ip.addr, "255.255.255.255") != 0) {
        fprintf(stderr, "unexpected loopback ip address, receive '%s', wants '255.255.255.255'\n", netProps->lo_ip.addr);
        return 1;
    }

    return 0;
}

int TestSetInterfaceIPAddr() {
    int ok, i;
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));
    net_SetEmptyInterfaceNetworkProperties(intfProps);

    ok = net_SetInterfaceIPAddr(intfProps, "", 24);
    if (ok) {
        fprintf(stderr, "unexpected net_SetInterfaceIPAddr response (with empty ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }

    ok = net_SetInterfaceIPAddr(intfProps, NULL, 24);
    if (ok) {
        fprintf(stderr, "unexpected net_SetInterfaceIPAddr response (with NULL ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }
    
    ok = net_SetInterfaceIPAddr(intfProps, "255.255.255.255.255", 24);
    if (ok) {
        fprintf(stderr, "unexpected net_SetInterfaceIPAddr response (with big ip), receive '%d' , wants '0'\n", ok);
        return 1;
    }

    ok = net_SetInterfaceIPAddr(intfProps, "255.255.255.255", 33);
    if (ok) {
        fprintf(stderr, "unexpected net_SetInterfaceIPAddr response (with wrong mask value), receive '%d' , wants '0'\n", ok);
        return 1;
    }

    ok = net_SetInterfaceIPAddr(intfProps, "255.255.255.255", 24);
    if (!ok) {
        fprintf(stderr, "unexpected net_SetInterfaceIPAddr response, receive '%d' , wants '1'\n", ok);
        return 1;
    }
    if (!intfProps->is_ip_addr_available) {
        fprintf(stderr, "unexpected ip address availability, receive '%d' , wants '1'\n", intfProps->is_ip_addr_available);
        return 1;
    }
    if (strcmp(intfProps->ip.addr, "255.255.255.255") != 0) {
        fprintf(stderr, "unexpected loopback ip address, receive '%s', wants '255.255.255.255'\n", intfProps->ip.addr);
        return 1;
    }

    ok = net_UnsetInterfaceIPAddr(intfProps);
    if (!ok) {
        fprintf(stderr, "unexpected net_UnsetInterfaceIPAddr response, receive '%d' , wants '1'\n", ok);
        return 1;
    }
    if (intfProps->is_ip_addr_available != 0) {
        fprintf(stderr, "unexpected ip address availability when unset, receive '%d' , wants '1'\n", intfProps->is_ip_addr_available);
        return 1;
    }
    if (intfProps->mask != 0) {
        fprintf(stderr, "unexpected mask when unset, receive '%d' , wants '1'\n", intfProps->mask);
        return 1;
    }
    for (i=0; i < IPV4_LENGTH ; i++) {
        if (intfProps->ip.addr[i] == 0) {
            continue;
        }
        fprintf(stderr, "unexpected ip address when unset, receive '%c' (IP address: %s) , wants '0'\n", intfProps->ip.addr[i], intfProps->ip.addr);
        return 1;
    }
    for (i=0; i < MAC_ADDR_LENGTH ; i++) {
        if (intfProps->mac.addr[i] == 0) {
            continue;
        }
        fprintf(stderr, "unexpected mac address when unset, receive '%c' (IP address: %s) , wants '0'\n", intfProps->mac.addr[i], intfProps->mac.addr);
        return 1;
    }

    return 0;
}