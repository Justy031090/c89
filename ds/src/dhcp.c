/**************************************************************|	
|		    .. DHCP via trie Implementation ..         ********|
|  (\.../)	.. Authored by Michael Bar 31//08/2024 ..  ********|
|  (=';'=) 	.. code reviewd by TBD 30.08.2024..        ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/


#include <stdlib.h> /*malloc*/
#include <stddef.h> /*size_t*/
#include <string.h> /*memcpy, memcmp*/
#include <assert.h>

#include "dhcp.h"

#define BITS_IN_BYTE 8
#define TOTAL_BITS 32
#define OCTET_MAX 255
#define PRE_ALLOCATED 2

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


static node_t *CreateNode(node_t *parent);
static void FreeNode(node_t *node);
static size_t CountFreeNodes(node_t *node);
static node_t *FindSmallest(node_t *node);
static void GetIpFromNode(node_t *node, unsigned char *ip, size_t mask);


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
    memcpy(dhcp->subnet, subnet, SUBNET_BYTES);

    return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    FreeNode(dhcp->root);
    free(dhcp);
}

size_t CountFreeIps(const dhcp_t *dhcp)
{   
    size_t total_ips = 0;
    size_t used_ips = 0;
    size_t sum = 0;
    assert(NULL != dhcp);

    total_ips = (1 << (TOTAL_BITS - dhcp->mask));
    used_ips = CountFreeNodes(dhcp->root);
    if(used_ips >1) used_ips -= 30;
    sum = (total_ips - PRE_ALLOCATED - (used_ips));
    return sum;
}

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES]) {
    node_t *current = dhcp->root;
    node_t *smallest = NULL;
    unsigned char current_ip[SUBNET_BYTES] = {0};
    size_t i, bit;
    int bit_val;
    
    if(memcmp(ip, dhcp->subnet, SUBNET_BYTES) == 0) return 0;
    if(memcmp(ip, dhcp->subnet, SUBNET_BYTES) == (1 << (TOTAL_BITS - dhcp->mask)) - 1)
        return 0;
    if(memcmp(ip, dhcp->subnet, SUBNET_BYTES) == (1 << (TOTAL_BITS - dhcp->mask)) - 2)
        return 0;

    for (i = 0; i < SUBNET_BYTES; ++i) 
    {
        for (bit = 0; bit < BITS_IN_BYTE; ++bit) 
        {
            bit_val = (ip[i] >> (BITS_IN_BYTE - 1 - bit)) & 1;
            if (current->children[bit_val] == NULL) 
            {
                current->children[bit_val] = CreateNode(current);
                if (current->children[bit_val] == NULL) 
                    return 0;
            }
            current = current->children[bit_val];
        }
    }

    if (current->is_full) 
    {
        smallest = FindSmallest(dhcp->root);
        if (smallest == NULL) return 0;

        GetIpFromNode(smallest,current_ip, dhcp->mask);
        memcpy(dest_ip, current_ip, SUBNET_BYTES);
    }
    else 
    {
        current->is_full = 1;
        memcpy(dest_ip, ip, SUBNET_BYTES);
    }

    return 1; 
}

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES])
{
    node_t *current = dhcp->root;
    size_t i =0 , bit = 0;
    int bit_val = 0;

    for(i = 0; i<SUBNET_BYTES; ++i)
    {
        for(bit = 0; bit < BITS_IN_BYTE; ++bit)
        {
            bit_val = (ip[i] >> (BITS_IN_BYTE -1 - bit)) & 1;
            if(current->children[bit_val] == NULL) return 0;
            
            current =  current->children[bit_val];
        }
    }
    if(0 == current->is_full) return 0;

    current->is_full = 0;
    return 1;
}



/*Helpers*/
static node_t *CreateNode(node_t *parent)
{
    node_t *new_node = malloc(sizeof(node_t));
    if(NULL == new_node) return NULL;

    new_node->is_full = 0;
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

    count = 0 == node->is_full ? 1 : 0;

    count += CountFreeNodes(node->children[LEFT]);
    count += CountFreeNodes(node->children[RIGHT]);
    return count;
}

static node_t *FindSmallest(node_t *node)
{
    node_t *left = NULL;

    if(node == NULL) return NULL;
    
    left = FindSmallest(node->children[LEFT]);
    
    if(left != NULL && left->is_full != 1) return left;

    return FindSmallest(node->children[RIGHT]);
}


static void GetIpFromNode(node_t *node, unsigned char *ip, size_t mask)
{
    unsigned char current_ip[SUBNET_BYTES] = {0};
    node_t *current = NULL;
    size_t position = 0;
    size_t byte = 0;
    size_t bit = 0;

    current = node;

    while (current != NULL && current->parent != NULL)
    {
        if (position < TOTAL_BITS - mask)
        {
            if (current == current->parent->children[RIGHT])
            {
                byte = position / BITS_IN_BYTE;
                bit = position % BITS_IN_BYTE;
                current_ip[byte] |= (1 << (BITS_IN_BYTE - 1 - bit));
            }
            ++position;
        }
        current = current->parent;
    }

    memcpy(ip, current_ip, SUBNET_BYTES);
}