#include "../include/ft_malcolm.h"

int fdsock = 0;

void    print_info(struct ethhdr *eth_frame, struct ether_arp *arp)
{
    printf("\nEthernet frame header:\n");
    printf("Destination MAC : ");
    print_mac(eth_frame->h_dest);
    printf("Source MAC : ");
    print_mac(eth_frame->h_source);
    printf("Type: %s (0x%04x)\n", (ntohs(eth_frame->h_proto) == ETH_P_ARP) ? "ARP" : "???", ntohs(eth_frame->h_proto));
    printf("\nARP header request:\n");
    printf("Hardware type : %s (%d)\n", (ntohs(arp->ea_hdr.ar_hrd) == ARPHRD_ETHER) ? "Ethernet" : "???", ntohs(arp->ea_hdr.ar_hrd));
    printf("Protocol type : %s (0x%04x)\n", (ntohs(arp->ea_hdr.ar_pro) == ETH_P_IP) ? "IPv4" : "???", ntohs(arp->ea_hdr.ar_pro));
    printf("Hardware size : %d\n", arp->ea_hdr.ar_hln);
    printf("Protocol size : %d\n", arp->ea_hdr.ar_pln);
    printf("Opcode : %s (%d)\n", (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) ? "request" : "???", ntohs(arp->ea_hdr.ar_op));
}

void    handle_sig_int(int signum)
{
    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
    exit(signum);
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
        return 1;

    if ((fdsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
        return printf("%s: error socket\n", av[0]);
    while ((ntohs(eth_frame->h_proto) != ETH_P_ARP) || (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST)) {
        recvfrom(fdsock, packet, sizeof(packet), 0, (struct sockaddr*)&src_addr, &src_addr_len);
    }

    print_info(eth_frame, arp);
    printf("\n%s\n", EXIT_MSG);
    close(fdsock);
}