#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dhcp.h"

enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

struct dhcp {
    node_t *root;
    size_t mask;
    unsigned char subnet[SUBNET_BYTES];
    
};

struct node {
    int is_full;
    node_t *parent;
    node_t *children[NUMBER_OF_CHILD];
};

void test_DHCPCreateDestroy() {
    unsigned char subnet[SUBNET_BYTES] = {192, 168, 1, 0};
    size_t mask = 24;

    dhcp_t *dhcp = DHCPCreate(subnet, mask);
    if (dhcp != NULL) {

        printf("DHCPCreate Success\n");
        printf("Subnet: %d.%d.%d.%d\n", dhcp->subnet[0], dhcp->subnet[1], dhcp->subnet[2], dhcp->subnet[3]);
        printf("Mask: %lu\n", dhcp->mask);

        DHCPDestroy(dhcp);

        printf("DHCPDestroy Success\n\n");
    } else {

        printf("DHCPCreate Failed\n");
    }
}

void test_CountFreeIps() {


    size_t mask = 24;
    unsigned char subnet[SUBNET_BYTES] = {192, 168, 1, 0};
    unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
    unsigned char ip2[SUBNET_BYTES] = {192, 168, 1, 2};
    unsigned char broadcast[SUBNET_BYTES] = {192, 168, 1, 255};
    unsigned char server[SUBNET_BYTES] = {192, 168, 1, 254};
    unsigned char network[SUBNET_BYTES] = {192, 168, 1, 0};
    unsigned char dest_ip[SUBNET_BYTES];
    unsigned char dest_ip2[SUBNET_BYTES];
    unsigned char dest_network[SUBNET_BYTES];
    unsigned char dest_broadcast[SUBNET_BYTES];
    unsigned char dest_server[SUBNET_BYTES];

    size_t free_ips = 0;
    dhcp_t *dhcp = DHCPCreate(subnet, mask);
    if (dhcp != NULL) {
        size_t free_ips = CountFreeIps(dhcp);

        if(free_ips != 253)
        {
             printf("FAIL !!! CountFreeIps Before Allocation: %lu\n", free_ips);
        }
        else
            printf("Free IP's before allocation Success.\n");
       

        if (AllocateIp(dhcp, ip, dest_ip)) {
            printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        } else {
            printf("FAIL !!! CountFreeIps AllocateIp failed.\n");
        }

        if (AllocateIp(dhcp, broadcast, dest_broadcast))
        {
            printf("FAIL !!!- allocated Broadcast IP.\n");
            printf("Allocated IP: %d.%d.%d.%d\n", dest_broadcast[0], dest_broadcast[1], dest_broadcast[2], dest_broadcast[3]);
        }
        
        if (AllocateIp(dhcp, network, dest_network))
        {
            printf("FAIL !!!- allocated Network IP.\n");
            printf("Allocated IP: %d.%d.%d.%d\n", dest_network[0], dest_network[1], dest_network[2], dest_network[3]);
        }
        if (AllocateIp(dhcp, server, dest_server))
        {
            printf("FAIL !!!- allocated Network IP.\n");
            printf("Allocated IP: %d.%d.%d.%d\n", dest_server[0], dest_server[1], dest_server[2], dest_server[3]);
        }

        /**/
        if(AllocateIp(dhcp, ip2, dest_ip2))
        {
            printf("2nd Allocation: %d.%d.%d.%d\n", dest_ip2[0], dest_ip2[1], dest_ip2[2], dest_ip2[3]);
        }
        if(AllocateIp(dhcp, ip2, dest_ip))
        {
            printf("FAIL !! allocated same address\n");
            
        }
        else
        {
            printf("Double allocation Success\n");
            printf("2nd Allocation: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        }
            



        free_ips = CountFreeIps(dhcp);
 
        if(free_ips != 251) 
        {
            printf("FAIL !!!   CountFreeIps after allocation: %lu\n", free_ips);
        }
        else
            printf("Free Ips Success for %lu mask\n.",mask);
        

        DHCPDestroy(dhcp);
    } else {
        printf("DHCPCreate Failed\n");
    }
}

void test_AllocateIp() {

    unsigned char subnet[SUBNET_BYTES] = {192, 168, 1, 0};
    size_t mask = 24;

    dhcp_t *dhcp = DHCPCreate(subnet, mask);
    if (dhcp != NULL) {
        unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
        unsigned char dest_ip[SUBNET_BYTES];
                unsigned char new_ip[SUBNET_BYTES] = {192, 168, 1, 2};
        unsigned char new_dest_ip[SUBNET_BYTES];

        if (AllocateIp(dhcp, ip, dest_ip)) {

            printf("AllocateIp Success\n");
            printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        } else {

            printf("AllocateIp Failed\n");
        }

        if (AllocateIp(dhcp, new_ip, new_dest_ip)) {

            printf("AllocateIp (new IP) Success\n");
            printf("Allocated IP: %d.%d.%d.%d\n", new_dest_ip[0], new_dest_ip[1], new_dest_ip[2], new_dest_ip[3]);
        } else {

            printf("AllocateIp (new IP) Failed\n");
        }

        DHCPDestroy(dhcp);
    } else {

        printf("DHCPCreate Failed\n");
    }
}

void test_FreeIp() {
    unsigned char subnet[SUBNET_BYTES] = {192, 168, 1, 0};
    size_t mask = 24;
            unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
        unsigned char dest_ip[SUBNET_BYTES];

    dhcp_t *dhcp = DHCPCreate(subnet, mask);
    if (dhcp != NULL) {

        if (AllocateIp(dhcp, ip, dest_ip)) {
            printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        } else {
            printf("AllocateIp failed\n");
        }


        if (FreeIp(dhcp, ip)) {
            printf("FreeIp Success\n");
        } else {

            printf("FreeIp Failed\n");
        }
        if (!FreeIp(dhcp, ip)) {

            printf("FreeIp (second attempt) Success\n");
        } else {

            printf("FreeIp (second attempt) Failed\n");
        }

        DHCPDestroy(dhcp);
    } else {

        printf("DHCPCreate Failed\n");
    }
}

int main() {

    test_CountFreeIps();
  /*
    test_AllocateIp();
    test_FreeIp();
    test_DHCPCreateDestroy();
    */
    return 0;
}