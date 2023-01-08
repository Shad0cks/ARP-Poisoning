#ifndef INCLUDE_H
#define INCLUDE_H

#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>

#define ETH2_HEADER_LEN 14
#define HW_TYPE 1
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ARP_REQUEST 0x01
#define BUF_SIZE 60

# define print_ipv4(x) printf("%d.%d.%d.%d\n", x[0], x[1], x[2], x[3]);
# define print_mac(x) printf("%02x:%02x:%02x:%02x:%02x:%02x\n", x[0], x[1], x[2], x[3], x[4], x[5], x[6]);

struct arp_header
{
  unsigned short hardware_type;
  unsigned short protocol_type;
  unsigned char hardware_len;
  unsigned char  protocol_len;
  unsigned short opcode;
  unsigned char sender_mac[MAC_LENGTH];
  unsigned char sender_ip[IPV4_LENGTH];
  unsigned char target_mac[MAC_LENGTH];
  unsigned char target_ip[IPV4_LENGTH];
};

struct ft_malcolm
{
  unsigned char sender_mac[MAC_LENGTH];
  unsigned char sender_ip[IPV4_LENGTH];
  unsigned char target_mac[MAC_LENGTH];
  unsigned char target_ip[IPV4_LENGTH];
  struct sockaddr_ll * socket_address;
};

void            convert_mac(const char * mac_addr,  unsigned char * dest);
void*           ft_memset(void *pointer, int value, size_t count);
int	            ft_memcmp(const void *str1, const void *str2, size_t n);
void*           ft_memcpy( void *destination, const void *source, size_t size );
int             compare_mac(unsigned char * mac1,  unsigned char * mac2);
int             dns_lookup(char * addr);
size_t	        ft_strlen(const char *str);
int             check_mac_format(char *mac);
int             atoi_i (char *str, int *i);
#endif