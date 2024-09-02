#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dhcp.h"

#define BITS_IN_BYTE 8
#define TOTAL_BITS 32
#define OCTET_MAX 255
#define PRE_ALLOCATED 2

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

static int CreateNode(node_t *parent, int side);
static void FreeNode(node_t *node);
static size_t CountFreeNodes(node_t *node);
static int IsReservedIp(const unsigned char *ip, const unsigned char *subnet, size_t mask);
static int CreateRequestedPath(node_t *node, int get_bit, int *ip_store, size_t total_bits);
static int CreateMinPath(node_t *node, int *ip_store, size_t total_bits);
static void CheckIsFull(node_t *node);
static int GetBits(const unsigned char *ip, size_t mask);

dhcp_t *DHCPCreate(const unsigned char subnet[SUBNET_BYTES], size_t mask)
{
    dhcp_t *dhcp;
    int res = 0, res2 = 0;

    dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
    if (NULL == dhcp)
        return NULL;

    dhcp->root = (node_t *)malloc(sizeof(node_t));

    if (NULL == dhcp->root)
    {
        free(dhcp);
        return NULL;
    }

    dhcp->mask = mask;
    dhcp->root->is_full = 0;
    dhcp->root->children[LEFT] = NULL;
    dhcp->root->children[RIGHT] = NULL;
    
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
    size_t total_ips;
    size_t used_ips;

    assert(NULL != dhcp);

    total_ips = (1 << (TOTAL_BITS - dhcp->mask));
    used_ips = CountFreeNodes(dhcp->root);

    return total_ips - used_ips;
}

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES])
{
    int ip_store = 0;
    int res = 0;
    size_t total_bits = TOTAL_BITS - dhcp->mask;
    if (!ip)
        res = CreateMinPath(dhcp->root, &ip_store, total_bits);
    else
    {
        res = CreateRequestedPath(dhcp->root, GetBits(ip, dhcp->mask), &ip_store, total_bits);
        if (!res)
            res = CreateMinPath(dhcp->root, &ip_store, total_bits);
    }
    return ip_store;
}

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES])
{
    node_t *current;
    size_t i, bit;
    int bit_val;

    current = dhcp->root;

    for (i = 0; i < SUBNET_BYTES; ++i)
    {
        for (bit = 0; bit < BITS_IN_BYTE; ++bit)
        {
            bit_val = (ip[i] >> (BITS_IN_BYTE - 1 - bit)) & 1;
            if (current->children[bit_val] == NULL)
                return 0;

            current = current->children[bit_val];
        }
    }

    if (0 == current->is_full)
        return 0;

    current->is_full = 0;
    return 1;
}

/* Helpers */
static int CreateNode(node_t *parent, int side)
{
    node_t *new_node = NULL;

    new_node = (node_t *)malloc(sizeof(node_t));
    if (NULL == new_node)
        return 0;


    new_node->is_full = 0;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    parent->children[side] = new_node;
    new_node->parent = parent;
    return 1;
}

static void FreeNode(node_t *node)
{
    if (node == NULL)
        return;

    FreeNode(node->children[LEFT]);
    FreeNode(node->children[RIGHT]);

    free(node);
}

static size_t CountFreeNodes(node_t *node)
{
    size_t count;

    if (NULL == node)
        return 0;

    count = (0 == node->is_full) ? 1 : 0;

    count += CountFreeNodes(node->children[LEFT]);
    count += CountFreeNodes(node->children[RIGHT]);

    return count;
}

static int GetBits(const unsigned char *ip, size_t mask)
{
    size_t i = 0;
    size_t total = TOTAL_BITS - mask;
    size_t bits = (total / BITS_IN_BYTE) + (total % BITS_IN_BYTE);
    int result = 0;
    int start = SUBNET_BYTES - 1 - (bits - 1);

    while (i < bits)
    {
        result <<= BITS_IN_BYTE;
        result |= ip[start];
        ++i;
        ++start;
    }
    return result;
}

static int IsReservedIp(const unsigned char *ip, const unsigned char *subnet, size_t mask)
{
    unsigned char reserved_start[SUBNET_BYTES];
    unsigned char reserved_end[SUBNET_BYTES];
    unsigned char server_ip[SUBNET_BYTES];

    memcpy(reserved_start, subnet, SUBNET_BYTES);
    memcpy(reserved_end, subnet, SUBNET_BYTES);
    memcpy(server_ip, subnet, SUBNET_BYTES);

    reserved_start[SUBNET_BYTES - 1] = 0;
    reserved_end[SUBNET_BYTES - 1] = OCTET_MAX;
    server_ip[SUBNET_BYTES - 1] = OCTET_MAX - 1;

    if (memcmp(ip, reserved_start, SUBNET_BYTES) == 0 ||
        memcmp(ip, reserved_end, SUBNET_BYTES) == 0 ||
        memcmp(ip, server_ip, SUBNET_BYTES) == 0)
    {
        return 1;
    }

    return 0;
}

static int CreateMinPath(node_t *node, int *ip_store, size_t total_bits)
{
    int result = 0;
    if( 0 == total_bits)
    {
        node->is_full = 1;
        return 1;
    }
    if (NULL == node->children[LEFT])
    {
        result = CreateNode(node, LEFT);
        if (!result)
            return 0;
    }

    if (0 == node->children[LEFT]->is_full)
    {
        *ip_store <<= 1;
        result = CreateMinPath(node->children[LEFT], ip_store, total_bits-1);
        CheckIsFull(node);
        return result;
    }

    if(NULL == node->children[RIGHT])
    {
        result = CreateNode(node, RIGHT);
        if(!result) return 0;
    }

    if (0 == node->children[RIGHT]->is_full)
    {
        *ip_store <<= 1;
        *ip_store |= 1;
        result = CreateMinPath(node->children[RIGHT], ip_store, total_bits -1);
        CheckIsFull(node);
        return result;
    }

    return 0;
}

static int CreateRequestedPath(node_t *node, int bit_rep, int *ip_store, size_t total_bits)
{
    int result;
    int bit = 1 << (total_bits-1);
    bit &= bit_rep;

    if(0 == total_bits)
    {
        node->is_full = 1;
        return 1;
    }
    if(LEFT == bit)
    {
        if (NULL == node->children[LEFT])
        {
            result = CreateNode(node, LEFT);
            if (!result) return 0;
        }

        if (0 == node->children[LEFT]->is_full)
        {
            *ip_store <<= 1;
            result = CreateRequestedPath(node->children[LEFT], bit_rep, ip_store, total_bits - 1);
            if(!result)
            {
                *ip_store >>= 1;
                if(NULL == node->children[RIGHT] || 0 == node->children[RIGHT]->is_full)
                {
                    *ip_store <<= 1;
                    *ip_store |= 1;
                    if(NULL == node->children[RIGHT])
                    {
                        result = CreateNode(node, RIGHT);
                        if(!result) return 0;
                    }
                    result = CreateMinPath(node->children[RIGHT], ip_store, total_bits - 1);
                    CheckIsFull(node);
                    return result;
                }
                return 0;
            }
        CheckIsFull(node);
        return result;
        }
        else return CreateMinPath(node->children[RIGHT], ip_store, total_bits -1);
    }
    else
    {
        if (NULL == node->children[RIGHT])
        {
            result = CreateNode(node, RIGHT);
            if(!result) return 0;
        }

        if(0 == node->children[RIGHT]->is_full)
        {
            *ip_store <<= 1;
            *ip_store |= 1;

            result = CreateRequestedPath(node->children[RIGHT], bit_rep, ip_store, total_bits - 1);
            if(!result)
            {
                *ip_store >>=1;
                return 0;
            }
            CheckIsFull(node);
            return result;
        }
        else if (node->children[RIGHT]->is_full) return 0;
    }

    return 0;
}
static void CheckIsFull(node_t *node)
{
    if (node->children[RIGHT] && node->children[LEFT] && node->children[RIGHT]->is_full && node->children[LEFT]->is_full)
        node->is_full = 1;
}