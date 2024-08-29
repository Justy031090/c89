#include <stdlib.h> /*malloc*/
#include <stddef.h> /*size_t*/
#include <assert.h>

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
}

struct node {
    int is_full;
    node_t *parent;
    node_t *children[NUMBER_OF_CHILD];
}

/*Create-allocation to 3  IPs*/
dhcp_t *DHCPCreate(const unsigned char subnet[SUBNET_BYTES],  size_t mask)
{
    dhcp_t *dhcp = malloc(sizeof(dhcp_t));
    if(NULL == dhcp) return NULL;

    dhcp->root = CreateNode(NULL);
    if(NULL == dhcp->root)
    {
        free(dhcp);
        return NULL;
    }

    dhcp->mask = mask;
    dhcp->subnet = subnet;

    return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp)
    FreeNode(dhcp->root);
    free(dhcp);
}

size_t CountFreeIps(const dhcp_t *dhcp)
{   
    size_t total_ips = 0;
    size_t used_ips = 0;

    assert(NULL != dhcp)

    total_ips = (1 << (32 - dhcp->mask));
    used_ips = CountFreeNodes(dhcp->root);

    return total_ips - used_ips;
}






int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES]);

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES]);





static node_t *CreateNode(node_t *parent)
{
    node_t *new_node = malloc(sizeof(node_t));
    if(NULL == new_node) return NULL;

    new_node.is_full = 0;
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return new_node;
}

static void FreeNode(node_t *node)
{
    if (node == NULL) return;

    FreeNode(node->children[LEFT]);
    FreeNode(node->children[RIGHT]);

    free(node);
}

static size_t CountFreeNodes(node_t *node)
{
    size_t count = 0;
    if (NULL == node) return 0;

    count = 1 == node->is_full ? 0 : 1;

    count += CountFreeNodes(node->children[LEFT]);
    count += CountFreeNodes(node->children[RIGHT]);

    return count;


}