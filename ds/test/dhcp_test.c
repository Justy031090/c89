#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dhcp.h"

#define TEST_SUBNET {192, 168, 1, 0}
#define TEST_MASK 24


enum children
{
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

struct node
{
    int is_full;
    node_t *parent;
    node_t *children[NUMBER_OF_CHILD];
};

struct dhcp
{
    node_t *root;
    size_t mask;
    unsigned char subnet[SUBNET_BYTES];
};


void TestDHCPCreateDestroy();
void TestCountFreeIps();
void TestAllocateIp();
void TestFreeIp();

int main()
{
    TestDHCPCreateDestroy();
    TestCountFreeIps();
    TestAllocateIp();
    TestFreeIp();
    return 0;
}

void TestDHCPCreateDestroy()
{
    dhcp_t *dhcp;
    unsigned char subnet[SUBNET_BYTES] = TEST_SUBNET;
    size_t mask = TEST_MASK;

    dhcp = DHCPCreate(subnet, mask);
    assert(dhcp != NULL);

    printf("DHCPCreate Success\n");
    printf("Subnet: %d.%d.%d.%d\n", dhcp->subnet[0], dhcp->subnet[1], dhcp->subnet[2], dhcp->subnet[3]);
    printf("Mask: %lu\n", dhcp->mask);

    DHCPDestroy(dhcp);
    printf("DHCPDestroy Success\n\n");
}

void TestCountFreeIps()
{
    dhcp_t *dhcp;
    unsigned char subnet[SUBNET_BYTES] = {192, 168, 1, 0};
    size_t mask = 24;
    unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
    unsigned char broadcast[SUBNET_BYTES] = {192, 168, 1, 255};
    unsigned char network[SUBNET_BYTES] = {192, 168, 1, 0};
    unsigned char server[SUBNET_BYTES] = {192, 168, 1, 254};
    unsigned char dest_ip[SUBNET_BYTES];
    unsigned char dest_broadcast[SUBNET_BYTES];
    unsigned char dest_network[SUBNET_BYTES];
    unsigned char dest_server[SUBNET_BYTES];
    size_t free_ips;

    dhcp = DHCPCreate(subnet, mask);
    assert(dhcp != NULL);

    free_ips = CountFreeIps(dhcp);
    printf("Initial free IPs count: %lu\n", free_ips);
    assert(free_ips == 256);

    if (AllocateIp(dhcp, ip, dest_ip))
    {
        printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
    }
    else
    {
        printf("FAIL !!! AllocateIp failed.\n");
    }

    free_ips = CountFreeIps(dhcp);
    printf("Free IPs count after one allocation: %lu\n", free_ips);
    assert(free_ips == 255);

    AllocateIp(dhcp, broadcast, dest_broadcast);
    if (memcmp(broadcast, dest_broadcast, SUBNET_BYTES) == 0)
    {
        printf("FAIL !!!- allocated Broadcast IP.\n");
        printf("Allocated IP: %d.%d.%d.%d\n", dest_broadcast[0], dest_broadcast[1], dest_broadcast[2], dest_broadcast[3]);
    }

    AllocateIp(dhcp, network, dest_network);
    if (memcmp(network, dest_network, SUBNET_BYTES) == 0)
    {
        printf("FAIL !!!- allocated Network IP.\n");
        printf("Allocated IP: %d.%d.%d.%d\n", dest_network[0], dest_network[1], dest_network[2], dest_network[3]);
    }

    AllocateIp(dhcp, server, dest_server);
    if (memcmp(server, dest_server, SUBNET_BYTES) == 0)
    {
        printf("FAIL !!!- allocated Server IP.\n");
        printf("Allocated IP: %d.%d.%d.%d\n", dest_server[0], dest_server[1], dest_server[2], dest_server[3]);
    }

    if (AllocateIp(dhcp, ip, dest_ip))
    {
        printf("2nd Allocation: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
    }
    FreeIp(dhcp, dest_ip);

    if (AllocateIp(dhcp, ip, dest_ip))
    {
        printf("After free 2nd Allocation: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
    }
    else
    {
        printf("Double allocation Success\n");
        printf("2nd Allocation AFTER: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        printf("2nd Allocation BEFORE: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
    }

    free_ips = CountFreeIps(dhcp);
    printf("Free IPs count after allocation and freeing: %lu\n", free_ips);
    assert(free_ips == 254);

    FreeIp(dhcp, dest_ip);
    FreeIp(dhcp, dest_ip);

    free_ips = CountFreeIps(dhcp);
    printf("Final free IPs count: %lu\n", free_ips);
    assert(free_ips == 256);

    DHCPDestroy(dhcp);
}

void TestAllocateIp()
{
    dhcp_t *dhcp;
    unsigned char subnet[SUBNET_BYTES] = TEST_SUBNET;
    size_t mask = TEST_MASK;
    unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
    unsigned char dest_ip[SUBNET_BYTES];
    unsigned char new_ip[SUBNET_BYTES] = {192, 168, 1, 2};
    unsigned char new_dest_ip[SUBNET_BYTES];
    unsigned char new_ip2[SUBNET_BYTES] = {192, 168, 1, 2};
    unsigned char new_dest_ip2[SUBNET_BYTES];
    int result;

    dhcp = DHCPCreate(subnet, mask);
    assert(dhcp != NULL);

    result = AllocateIp(dhcp, ip, dest_ip);
    assert(result == 1);
    printf("AllocateIp Success\n");
    printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);

    result = AllocateIp(dhcp, new_ip, new_dest_ip);
    assert(result == 1);
    printf("AllocateIp (new IP) Success\n");
    printf("Allocated IP: %d.%d.%d.%d\n", new_dest_ip[0], new_dest_ip[1], new_dest_ip[2], new_dest_ip[3]);

    result = AllocateIp(dhcp, new_ip2, new_dest_ip2);
    assert(result == 0);
    printf("AllocateIp DOUBLE ip check Failed - DID NOT ALLOCATE\n");

    DHCPDestroy(dhcp);
}

void TestFreeIp()
{
    dhcp_t *dhcp;
    unsigned char subnet[SUBNET_BYTES] = TEST_SUBNET;
    size_t mask = TEST_MASK;
    unsigned char ip[SUBNET_BYTES] = {192, 168, 1, 1};
    unsigned char dest_ip[SUBNET_BYTES];
    int result;

    dhcp = DHCPCreate(subnet, mask);
    assert(dhcp != NULL);

    result = AllocateIp(dhcp, ip, dest_ip);
    assert(result == 1);
    printf("Allocated IP: %d.%d.%d.%d\n", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);

    result = FreeIp(dhcp, ip);
    assert(result == 1);
    printf("FreeIp Success\n");

    result = FreeIp(dhcp, ip);
    assert(result == 0);
    printf("FreeIp (second attempt) Success\n");

    DHCPDestroy(dhcp);
}
