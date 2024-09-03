#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dhcp.h"

#define BITS_IN_BYTE 8
#define TOTAL_BITS 32
#define OCTET_MAX 255

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
static int CreateRequestedPath(node_t *node, int bit_rep, int *result, size_t num_of_bits);
static int CreateMinPath(node_t *node, int *result, size_t num_of_bits);
static void CheckIsFull(node_t *node);
static int GetBits(const unsigned char *ip, size_t mask);
static int FreeIpHandler(node_t *node, int bit_rep, size_t total_bits);

dhcp_t *DHCPCreate(const unsigned char subnet[SUBNET_BYTES], size_t mask)
{
    dhcp_t *dhcp = NULL;

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
    dhcp->root->parent = NULL;

    memcpy(dhcp->subnet, subnet, SUBNET_BYTES);
    AllocateIp(dhcp,network, dummy);
    AllocateIp(dhcp,broadcast, dummy);
    AllocateIp(dhcp, server, dummy);

    return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    FreeNode(dhcp->root);
    free(dhcp);
}

static size_t CountFreeNodes(node_t *node)
{
    size_t count = 0;

    if (node == NULL)
        return 0;

    count = (node->is_full == 1) ? 1 : 0;
    count += CountFreeNodes(node->children[LEFT]);
    count += CountFreeNodes(node->children[RIGHT]);

    return count;
}

size_t CountFreeIps(const dhcp_t *dhcp)
{
    size_t total_ips = 0;
    size_t used_ips = 0;

    assert(NULL != dhcp);

    total_ips = (1 << (TOTAL_BITS - dhcp->mask));
    used_ips = CountFreeNodes(dhcp->root);

    return total_ips - used_ips;
}

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES])
{
    int result = 0;
    int num_of_bits = TOTAL_BITS - dhcp->mask;
    int status = -1;

    if (ip == NULL)
    {
        status = CreateMinPath(dhcp->root, &result, num_of_bits);
    }
    else
    {
        status = CreateRequestedPath(dhcp->root, GetBits(ip, num_of_bits), &result, num_of_bits);
        if (!status)
        {
            status = CreateMinPath(dhcp->root, &result, num_of_bits);
        }
    }

    result = result << dhcp->mask | *(unsigned int*)dhcp->subnet;
    memcpy(dest_ip, &result, SUBNET_BYTES);

    return status;
}

static int CreateMinPath(node_t *node, int *result, size_t num_of_bits)
{
    if (num_of_bits == 0)
    {
        node->is_full = 1;
        return 1;
    }

    if (node->children[LEFT] == NULL)
    {
        if (!CreateNode(node, LEFT))
        {
            return -1;
        }
    }

    if (!node->children[LEFT]->is_full)
    {
        *result = (*result << 1);
        if (CreateMinPath(node->children[LEFT], result, num_of_bits - 1) == 1)
        {
            CheckIsFull(node);
            return 1;
        }
    }

    if (node->children[RIGHT] == NULL)
    {
        if (!CreateNode(node, RIGHT))
        {
            return -1;
        }
    }

    if (!node->children[RIGHT]->is_full)
    {
        *result = (*result << 1) | 1;
        if (CreateMinPath(node->children[RIGHT], result, num_of_bits - 1) == 1)
        {
            CheckIsFull(node);
            return 1;
        }
    }

    return -1;
}

static int CreateRequestedPath(node_t *node, int bit_representation, int *result, size_t num_of_bits)
{
    int bit = (1 << (num_of_bits - 1)) & bit_representation;

    if (num_of_bits == 0)
    {
        node->is_full = 1;
        return 1;
    }

    if (bit == LEFT)
    {
        if (node->children[LEFT] == NULL)
        {
            if (CreateNode(node, LEFT) == -1)
            {
                return -1;
            }
        }

        if (!node->children[LEFT]->is_full)
        {
            *result = (*result << 1);
            if (CreateRequestedPath(node->children[LEFT], bit_representation, result, num_of_bits - 1) == 1)
            {
                CheckIsFull(node);
                return 1;
            }
            else
            {
                *result = (*result >> 1);
                if (node->children[RIGHT] == NULL)
                {
                    if (CreateNode(node, RIGHT) == -1)
                    {
                        return -1;
                    }
                }
                *result = (*result << 1) | 1;
                if (CreateMinPath(node->children[RIGHT], result, num_of_bits - 1) == 1)
                {
                    CheckIsFull(node);
                    return 1;
                }
            }
        }
        else
        {
            if (CreateMinPath(node->children[RIGHT], result, num_of_bits - 1) == 1)
            {
                CheckIsFull(node);
                return 1;
            }
        }
    }
    else
    {
        if (node->children[RIGHT] == NULL)
        {
            if (CreateNode(node, RIGHT) == -1)
            {
                return -1;
            }
        }

        if (!node->children[RIGHT]->is_full)
        {
            *result = (*result << 1) | 1;
            if (CreateRequestedPath(node->children[RIGHT], bit_representation, result, num_of_bits - 1) == 1)
            {
                CheckIsFull(node);
                return 1;
            }
            else
            {
                *result = (*result >> 1);
            }
        }
    }

    return -1;
}

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES])
{
    return FreeIpHandler(dhcp->root, GetBits(ip, dhcp->mask), TOTAL_BITS - dhcp->mask);
}

static int FreeIpHandler(node_t *node, int bit_representation, size_t num_of_bits)
{
    int bit = 0;
    if (num_of_bits == 0)
    {
        node->is_full = 0;
        return 1;
    }

    bit = (1 << (num_of_bits - 1)) & bit_representation;
    bit = (bit == 0) ? LEFT : RIGHT;

    if (node->children[bit] == NULL)
    {
        return 2;
    }
    else
    {
        node->is_full = 0;
        return FreeIpHandler(node->children[bit], bit_representation, num_of_bits - 1);
    }
}

/* Helpers */
static int CreateNode(node_t *parent, int side)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (NULL == new_node)
        return 0;

    new_node->is_full = 0;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->parent = parent;
    parent->children[side] = new_node;

    return 1;
}

static void FreeNode(node_t *node)
{
    if (NULL == node)
        return;

    FreeNode(node->children[LEFT]);
    FreeNode(node->children[RIGHT]);

    free(node);
}

static int GetBits(const unsigned char *ip, size_t bit_num)
{
    int bytes = (bit_num + 7) / 8;
    int start = SUBNET_BYTES - bytes;
    int result = 0, i = 0;

    for (i = 0; i < bytes; i++)
    {
        result = (result << 8) | ip[start + i];
    }

    return result;
}

static void CheckIsFull(node_t *node)
{
    if (node == NULL)
        return;

    if (node->children[LEFT] == NULL && node->children[RIGHT] == NULL)
    {
        node->is_full = 0;
    }
    else
    {
        node->is_full = (node->children[LEFT] && node->children[LEFT]->is_full) &&
                        (node->children[RIGHT] && node->children[RIGHT]->is_full);
    }
}