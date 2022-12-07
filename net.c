#include "net.h"
#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include <arpa/inet.h> 

// Global variables
int is_seeded = 0;

// Private functions
unsigned int randomValue();

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

// DumpNodeNetProps prints all information from an interface and node network properties
void net_DumpNodeNetProps(node_net_prop_t *nodeProps, char *prefix) {
    if (nodeProps->is_lo_available) {
        fprintf(stdout, "%sLoopback ip address: %s\n", prefix, nodeProps->lo_ip.addr);
    } else {
        fprintf(stdout, "%sLoopback ip address not available\n", prefix);
    }
}

// DumpInterfaceNetProps prints all information from an interface and node network properties
void net_DumpInterfaceNetProps(intf_net_prop_t *intfProps, char *prefix) {
    char *mac = intfProps->mac.addr;
    char macAddress[19];
    sprintf(macAddress, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX%c", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], '\0');
    if (intfProps->is_ip_addr_available) {
        fprintf(stdout, "%sMAC address: %s | Interface ip address: %s/%d\n", prefix, macAddress, intfProps->ip.addr, intfProps->mask);
    } else {
        fprintf(stdout, "%sMAC address: %s | Interface ip address not available\n", prefix, macAddress);
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
    unsigned char *mac = intfProps->mac.addr;
    memset(mac, 0, sizeof(mac));

    // get random value from 0 to 4294967295
    unsigned int value = randomValue();

    // cast the random value into char* and set this information into the mac address. More information:
    // https://stackoverflow.com/questions/2733960/pointer-address-type-casting
    unsigned char *valueIntoChar = (unsigned char *)&value;
    memcpy(mac, valueIntoChar, sizeof(unsigned int));
}

// SetBroadcastMACAddr assign the Broadcast value to MAC Address (FF:FF:FF:FF:FF:FF)
void net_SetBroadcastMACAddr(intf_net_prop_t *intfProps) {
    unsigned char *mac = intfProps->mac.addr;
    int i;
    for (i = 0 ; i < MAC_ADDR_LENGTH ; i++) {
        mac[i] = 0xFF;
    }
}

int net_IsMACBroadcast(intf_net_prop_t *intfProps) {
    unsigned char *mac = intfProps->mac.addr;
    for (int i = 0 ; i < MAC_ADDR_LENGTH ; i++) {
        if (mac[i] == 0xFF) {
            continue;
        }
        return 0;
    }
    return 1;
}

// ApplyMask
void net_ApplyMask(char *fromIP, char *toIP, char mask) {
    int i;
    uint32_t binaryIP = 0;
    uint32_t binaryMask;

    if (mask == 32) {
        strncpy(fromIP, toIP, IPV4_LENGTH);
        toIP[IPV4_LENGTH - 1] = '\0';
        return;
    }

    if (cpu_IsBigEndian()) {
        // The representation when Big Endian and mask = 24 will be: 0xFFFFFF00 but in the memory is 00 FF FF FF
        binaryMask = 0xFFFFFFFF << (32 - mask);
    } else {
        // The representation when Little Endian and mask = 24 will be: 0x00FFFFFF but in the memory is FF FF FF 00
        binaryMask = 0xFFFFFFFF >> (32 - mask);
    }

    inet_pton(AF_INET, fromIP, &binaryIP);
    for (i=0 ; i < sizeof(uint32_t) ; i++) {
        /* When IP: 122.1.1.1, the bytes is 0101017A but the representation will be different depending on the order:
        Big Endian: 01 01 01 7A
        Little Endian: 7A 01 01 01

        If we get the example of the Little Endian, we will get these values in bytes:
        Mask: FF FF FF 00
        IP: 7A 01 01 01

        If we apply the AND operator in each byte, we will get the following mask:
        Mask applied: 7A 01 01 00 (122.1.1.0)
        */
        unsigned char *byteIP = ((unsigned char *)&binaryIP);
        unsigned char *byteMask = ((unsigned char *)&binaryMask);
        byteIP[i] = byteIP[i] & byteMask[i];
    }
    inet_ntop(AF_INET, &binaryIP, toIP, IPV4_LENGTH);
}

// randomValue returns a random value from 0 to 4294967295;
unsigned int randomValue() {
    if (!is_seeded) {
        srand(time(NULL));
        is_seeded = 1;
    }
    return (unsigned int)(rand() % (UINT_MAX)) + 1;
}