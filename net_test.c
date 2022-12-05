#include "net.h"

#include<stdio.h>
#include<stdlib.h>

int TestSetEmptyNodeNetworkProperties();
int TestSetEmptyInterfaceNetworkProperties();

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