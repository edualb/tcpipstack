# tcpipstack

This project is based on the following courses:

- [Part A - Networking Projects - Implement TCP/IP Stack in C](https://www.udemy.com/course/tcpipstack/)
- Course Github: [tcpip_stack](https://github.com/sachinites/tcpip_stack)

Despite of is based on the course above, you will be able different implementation because I decided to make some different logic to achieve the same goal. I put the course github if anyone wants to compare the implementation.

## Test

In order to run all tests, you can run the following command:
```shell
$ make test
```

## Print topology

In order to print topologies, you need to do the following steps:
1. Edit the `main` functon from the `topologies.c` file with the topology that you want:
```c
// ...
int main() {
    // FirstTopology function is just an example. You can create new ones using the FirstTopology function as an example.
    graph_t *topology = FirstTopology();
    if (topology == NULL) {
        fprintf(stderr, "empty topology");
        return 1;
    }
    graph_GraphDump(topology);
    return 0;
}
/*
Output:

Node name: R2_re
   Loopback ip address: 122.1.1.2
Interface Name: eth0/3
   Local Node: R2_re | Neighbot Node: R1_re | Link Cost: 1
   MAC address: 6U:T5:LE:EF:II:CH:XF:W7 | Interface ip address: 30.1.1.2
Interface Name: eth0/5
   Local Node: R2_re | Neighbot Node: R0_re | Link Cost: 1
   MAC address: 9J:Y0:HT:1R:KF:A7:AW:7I | Interface ip address: 40.1.1.2


Node name: R1_re
   Loopback ip address: 122.1.1.1
Interface Name: eth0/1
   Local Node: R1_re | Neighbot Node: R0_re | Link Cost: 1
   MAC address: OK:8S:ZE:7X:QQ:QV:LL:3K | Interface ip address: 20.1.1.2
Interface Name: eth0/2
   Local Node: R1_re | Neighbot Node: R2_re | Link Cost: 1
   MAC address: KU:3B:28:VY:XF:F6:WH:I9 | Interface ip address: 30.1.1.1


Node name: R0_re
   Loopback ip address: 122.1.1.0
Interface Name: eth0/0
   Local Node: R0_re | Neighbot Node: R1_re | Link Cost: 1
   MAC address: LY:FD:3D:5G:X2:IV:ID:NM | Interface ip address: 20.1.1.1
Interface Name: eth0/4
   Local Node: R0_re | Neighbot Node: R2_re | Link Cost: 1
   MAC address: DO:OQ:WG:76:QL:OO:E1:LS | Interface ip address: 40.1.1.1
*/
```

2. Execute the following command:
```shell
$ make print-topology
```

## Build Objects
If you want to build `.o` objects of the project, you can run the following command:
```shell
$ make build-objects
```

If you want to delete all `.o` objects execute the following command:
```shell
$ make clean
```