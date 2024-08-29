#include <stdlib.h> /*malloc*/
#include <stddef.h> /*size_t*/

#include "dhcp.h"


enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

struct dhcp {
    node_t root;
    size_t *mask;
    unsigned char subnet[SUBNET_BYTES];
}

struct node {
    int is_full;
    node_t *parent;
    node_t children[NUMBER_OF_CHILD];
}

/*Create-allocation to 3  IPs*/
dhcp_t *DHCPCreate(const unsigned char subnet[SUBNET_BYTES],  size_t mask)
{
    dhcp_t *dhcp = malloc(sizeof(dhcp_t));
    if(NULL == dhcp) return NULL;

}

void DHCPDestroy(dhcp_t *dhcp)
{
    /*free children*/;
    free(dhcp);
}

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES]);

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES]);

size_t CountFreeIps(const dhcp_t *dhcp);



