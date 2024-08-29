#ifndef __DHCP_H__
#define __DHCP_H__

#define SUBNET_BYTES (4)

typedef struct dhcp dhcp_t;

typedef struct node node_t;


dhcp_t* DHCPCreate(const unsigned char subnet[SUBNET_BYTES],  size_t mask);

void DHCPDestroy(dhcp_t *dhcp);

int AllocateIp(dhcp_t *dhcp, const unsigned char ip[SUBNET_BYTES], unsigned char dest_ip[SUBNET_BYTES]);

int FreeIp(dhcp_t *dhcp, unsigned char ip[SUBNET_BYTES]);

size_t CountFreeIps(const dhcp_t *dhcp);

#endif /*__DHCP_H__*/