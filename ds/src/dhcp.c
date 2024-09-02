#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


struct node {
    int is_full;
    node_t *parent;
    node_t *children[NUMBER_OF_CHILD];
};

struct dhcp {
    node_t *root;
    size_t mask;
    unsigned char subnet[SUBNET_BYTES];
};

static node_t *CreateNode(node_t *parent);
static void FreeNode(node_t *node);
static size_t CountFreeNodes(node_t *node);
static void GetIpFromNode(node_t *node, unsigned char *ip, size_t mask);
static int IsReservedIp(const unsigned char *ip, const unsigned char *subnet, size_t mask);
static int FindSmallestAvailableIp(node_t *node, unsigned char *smallest_ip, size_t mask);
static int FindNextClosestAvailableIp(node_t *node, const unsigned char *ip, unsigned char *next_ip, size_t mask);
static node_t *FindSmallest2(node_t *parent);

dhcp_t *DHCPCreate(const unsigned char subnet[SUBNET_BYTES], size_t mask) {
    dhcp_t *dhcp;

    dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
    if (NULL == dhcp) return NULL;

    dhcp->root = CreateNode(NULL);
    if (NULL == dhcp->root) {
        free(dhcp);
        return NULL;
    }

    dhcp->mask = mask;
    memcpy(dhcp->subnet, subnet, SUBNET_BYTES);

    return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp) {
    assert(NULL != dhcp);
    FreeNode(dhcp->root);
    free(dhcp);
}

size_t CountFreeIps(const dhcp_t *dhcp) {
    size_t total_ips;
    size_t used_ips;

    assert(NULL != dhcp);

    total_ips = (1 << (TOTAL_BITS - dhcp->mask));
    used_ips = CountFreeNodes(dhcp->root);

    return total_ips - PRE_ALLOCATED - used_ips;
}

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES]) {
    node_t *current = dhcp->root;
    unsigned char cpy_ip[SUBNET_BYTES];
    size_t i, bit;
    int bit_val;
    unsigned char smallest_ip[SUBNET_BYTES];

    memcpy(cpy_ip, ip, SUBNET_BYTES);

    if (IsReservedIp(cpy_ip, dhcp->subnet, dhcp->mask)) {
        if (FindNextClosestAvailableIp(dhcp->root, ip, dest_ip, dhcp->mask)) {
            return 1;
        } else {
            return 0; 
        }
    }

    for (i = 0; i < SUBNET_BYTES; ++i) {
        for (bit = 0; bit < BITS_IN_BYTE; ++bit) {
            bit_val = (cpy_ip[i] >> (BITS_IN_BYTE - 1 - bit)) & 1;
            if (current->children[bit_val] == NULL) {
                current->children[bit_val] = CreateNode(current);
                if (current->children[bit_val] == NULL) return 0;
            current = current->children[bit_val];  
            }
            else if(current->is_full)
            {
                current = FindSmallest2(current->parent);
            }
        }
    }

        GetIpFromNode(current, dest_ip, dhcp->mask);

        current->is_full = 1;
        memcpy(dest_ip, cpy_ip, SUBNET_BYTES);

    return 1;
}




int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES]) {
    node_t *current;
    size_t i, bit;
    int bit_val;

    current = dhcp->root;

    for (i = 0; i < SUBNET_BYTES; ++i) {
        for (bit = 0; bit < BITS_IN_BYTE; ++bit) {
            bit_val = (ip[i] >> (BITS_IN_BYTE - 1 - bit)) & 1;
            if (current->children[bit_val] == NULL) return 0;

            current = current->children[bit_val];
        }
    }

    if (0 == current->is_full) return 0;

    current->is_full = 0;
    return 1;
}

/* Helpers */
static node_t *CreateNode(node_t *parent) {
    node_t *new_node;

    new_node = (node_t *)malloc(sizeof(node_t));
    if (NULL == new_node) return NULL;

    new_node->is_full = 0;
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return new_node;
}

static void FreeNode(node_t *node) {
    if (node == NULL) return;

    FreeNode(node->children[LEFT]);
    FreeNode(node->children[RIGHT]);

    free(node);
}

static size_t CountFreeNodes(node_t *node) {
    size_t count;

    if (NULL == node) return 0;

    count = (0 == node->is_full) ? 1 : 0;

    count += CountFreeNodes(node->children[LEFT]);
    count += CountFreeNodes(node->children[RIGHT]);
    
    return count;
}


static void GetIpFromNode(node_t *node, unsigned char *ip, size_t mask) {
    unsigned char current_ip[SUBNET_BYTES] = {0};
    node_t *current;
    size_t position;
    size_t byte;
    size_t bit;

    current = node;
    position = 0;

    while (current != NULL && current->parent != NULL) {
        if (position < TOTAL_BITS - mask) {
            if (current == current->parent->children[RIGHT]) {
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

static int IsReservedIp(const unsigned char *ip, const unsigned char *subnet, size_t mask) {
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
        memcmp(ip, server_ip, SUBNET_BYTES) == 0) {
        return 1;
    }

    return 0;
}


static int FindSmallestAvailableIp(node_t *node, unsigned char *smallest_ip, size_t mask) {
    if (node == NULL) return 0;

    if (0 == node->is_full) {
        GetIpFromNode(node, smallest_ip, mask);
        return 1;
    }

    if (FindSmallestAvailableIp(node->children[LEFT], smallest_ip, mask)) return 1;
    if (FindSmallestAvailableIp(node->children[RIGHT], smallest_ip, mask)) return 1;

}


static int FindNextClosestAvailableIp(node_t *node, const unsigned char *ip, unsigned char *next_ip, size_t mask) {
    unsigned char current_ip[SUBNET_BYTES];
    int found = 0;

    if (node == NULL) return 0;

    if (!node->is_full) {
        GetIpFromNode(node, current_ip, mask);
        if (memcmp(current_ip, ip, SUBNET_BYTES) > 0) {
            memcpy(next_ip, current_ip, SUBNET_BYTES);
            return 1;
        }
    }

    if (FindNextClosestAvailableIp(node->children[LEFT], ip, next_ip, mask)) return 1;
    if (FindNextClosestAvailableIp(node->children[RIGHT], ip, next_ip, mask)) return 1;

    return found;
}


static node_t *FindSmallest2(node_t *parent)
{
    while(parent != NULL)
    {
        if(parent->children[LEFT]->is_full && parent->children[RIGHT]->is_full)
        {
            parent->is_full = 1;
            parent = parent->parent;
        }
        if(parent->children[RIGHT] == NULL)
        {
            parent->children[RIGHT] = CreateNode(parent);
            return parent->children[RIGHT];
            
        }
        if(parent->children[RIGHT]->is_full = 0)
            return parent->children[RIGHT];
    }
}