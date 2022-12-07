#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>

// IsBigEndian return 1 in case of Big-endian CPU
int cpu_IsBigEndian() {
    int flag = 1;
    // In Little endian the memory of flag 1 (0x00000001) is equal to
    // 0x01000000
    if ( ((unsigned char *)&flag)[0] == 0x01 ) {
        return 0;
    }
    return 1;
}