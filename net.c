#include"net.h"
#include<memory.h>

void net_SetEmptyNodeNetworkProperties(node_net_prop_t *nodeNetProps) {
    nodeNetProps->is_lo_available = 0;
    memset(nodeNetProps->lo_ip.addr, 0, IPV4_LENGTH);
}

void net_SetEmptyInterfaceNetworkProperties(intf_net_prop_t *intfNetProps) {
    intfNetProps->is_ip_addr_available = 0;
    intfNetProps->mask = 0;
    memset(intfNetProps->mac.addr, 0, MAC_ADDR_LENGTH);
    memset(intfNetProps->ip.addr, 0, IPV4_LENGTH);
}