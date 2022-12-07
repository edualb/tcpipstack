#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TestSetEmptyNodeNetworkProperties();
int TestSetEmptyInterfaceNetworkProperties();
int TestSetBroadcastMACAddr();
int TestAssignMACAddr();
int TestSetLoopbackAddrNode();
int TestSetInterfaceIPAddr();
int TestApplyMask();
int TestIsMACBroadcast();

int main() {
    fprintf(stdout, "\n\nStarting Test net...\n");

    /* TestSetEmptyNodeNetworkProperties tests the following methods:
    * net_SetEmptyNodeNetworkProperties
    */
    if (TestSetEmptyNodeNetworkProperties() != 0) {
        fprintf(stderr, "TestSetEmptyNodeNetworkProperties: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestSetEmptyNodeNetworkProperties: OK\n");

    /* TestSetEmptyInterfaceNetworkProperties tests the following methods:
    * net_SetEmptyInterfaceNetworkProperties
    */
    if (TestSetEmptyInterfaceNetworkProperties() != 0) {
        fprintf(stderr, "TestSetEmptyInterfaceNetworkProperties: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestSetEmptyInterfaceNetworkProperties: OK\n");

    /* TestSetBroadcastMACAddr tests the following methods:
    * net_SetBroadcastMACAddr
    */
    if (TestSetBroadcastMACAddr() != 0) {
        fprintf(stderr, "TestSetBroadcastMACAddr: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestSetBroadcastMACAddr: OK\n");

    /* TestAssignMACAddr tests the following methods:
    * net_AssignMACAddr
    */
    if (TestAssignMACAddr() != 0) {
        fprintf(stderr, "TestAssignMACAddr: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestAssignMACAddr: OK\n");

    /* TestSetLoopbackAddrNode tests the following methods:
    * net_SetLoopbackAddrNode
    */
    if (TestSetLoopbackAddrNode() != 0) {
        fprintf(stderr, "TestSetLoopbackAddrNode: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestSetLoopbackAddrNode: OK\n");

    /* TestSetInterfaceIPAddr tests the following methods:
    * net_SetInterfaceIPAddr
    * net_UnsetInterfaceIPAddr
    */
    if (TestSetInterfaceIPAddr() != 0) {
        fprintf(stderr, "TestSetInterfaceIPAddr: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestSetInterfaceIPAddr: OK\n");

    /* TestSetInterfaceIPAddr tests the following methods:
    * net_ApplyMask
    * cpu_IsBigEndian
    */
    if (TestApplyMask() != 0) {
        fprintf(stderr, "TestApplyMask: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestApplyMask: OK\n");

    /* TestIsMACBroadcast tests the following methods:
    * net_ApplyMask
    * cpu_IsBigEndian
    */
    if (TestIsMACBroadcast() != 0) {
        fprintf(stderr, "TestIsMACBroadcast: FAIL\n");
        exit(1);
    }
    fprintf(stdout, "\tTestIsMACBroadcast: OK\n");

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

int TestSetBroadcastMACAddr() {
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));
    net_SetBroadcastMACAddr(intfProps);
    unsigned char *mac = intfProps->mac.addr;
    int i;
    for(i=0 ; i < MAC_ADDR_LENGTH ; i++) {
        if (mac[i] == 0xFF) {
            continue;
        }
        fprintf(stderr, "unexpected mac address in byte %d, receive '%02hhX', wants 'FF'\n", i, mac[i]);
        return 1;
    }
    return 0;
}

int TestAssignMACAddr() {
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));
    net_AssignMACAddr(intfProps);
    char *mac = intfProps->mac.addr;

    int i;
    for(i=0 ; i < MAC_ADDR_LENGTH ; i++) {
        if (i == 4 || i == 5) {
            if (mac[i] == 0) {
                continue;
            };
            fprintf(stderr, "unexpected mac address in byte %d, receive '%d', wants '0'\n", i, mac[i]);
            return 1;
        }
        if (mac[i] != 0) {
            continue;
        }
        fprintf(stderr, "unexpected mac address in byte %d, receive '0', wants non zero value '0'\n", i);
        return 1;
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

int TestApplyMask() {
    char *IP = "122.1.1.1";
    char mask = 24;

    // We can't use 'char *IPWithMask; or char *IPWithMask = "";' because it is a string literal, and string literal can't be modified.
    // more information here: https://www.tutorialspoint.com/what-are-string-literals-in-c-language
    // If we want to send a modified char* we can use one of the follwing methods:
    //
    // - char *IPWithMask = malloc(IPV4_LENGTH * sizeof(char));
    // - char IPWithMask[IPV4_LENGTH];
    char *IPWithMask = malloc(IPV4_LENGTH * sizeof(char));

    net_ApplyMask(IP, IPWithMask, mask);
    if (strcmp(IPWithMask, "122.1.1.0") != 0) {
        fprintf(stderr, "unexpected ip with mask, receive '%s' , wants '122.1.1.0'\n", IPWithMask);
        return 1;
    }
    return 0;
}

int TestIsMACBroadcast() {
    intf_net_prop_t *intfProps = malloc(sizeof(intf_net_prop_t));

    net_AssignMACAddr(intfProps);
    int isBroadcast = net_IsMACBroadcast(intfProps);
    if (isBroadcast) {
        unsigned char *mac = intfProps->mac.addr;
        fprintf(stderr, "unexpected mac address, receive '%02hhX %02hhX %02hhX %02hhX %02hhX %02hhX' , wants not a broadcast\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return 1;
    }

    net_SetBroadcastMACAddr(intfProps);
    isBroadcast = net_IsMACBroadcast(intfProps);
    if (!isBroadcast) {
        unsigned char *mac = intfProps->mac.addr;
        fprintf(stderr, "unexpected mac address, receive '%02hhX %02hhX %02hhX %02hhX %02hhX %02hhX' , wants 'FF FF FF FF FF FF'\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return 1;
    }
    return 0;
}