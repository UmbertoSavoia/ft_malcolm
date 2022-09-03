#include "../include/ft_malcolm.h"

void    *ft_memcpy(void *dest, const void *src, size_t n)
{
    char        *temp;
    const char  *s;

    if (!dest && !src)
        return (0);
    s = src;
    temp = dest;
    while (n--)
        *temp++ = *s++;
    return (dest);
}

int     usage(char *program)
{
    printf("\n %s\n Usage:\n  sudo %s <source ip> <source mac address> <target ip> <target mac address>\n",
           "This tool must be run as root", program);
    return 1;
}

void    print_info(struct ethhdr *eth_frame, struct ether_arp *arp, char *header)
{
    printf("\n- %s -\n", header);
    printf("Ethernet frame header:\n");
    printf(" Destination MAC : ");
    print_mac(eth_frame->h_dest);
    printf(" Source MAC : ");
    print_mac(eth_frame->h_source);
    printf(" Type: %s (0x%04x)\n", (ntohs(eth_frame->h_proto) == ETH_P_ARP) ? "ARP" : "???", ntohs(eth_frame->h_proto));
    printf("\nARP header request:\n");
    printf(" Hardware type : %s (%d)\n", (ntohs(arp->ea_hdr.ar_hrd) == ARPHRD_ETHER) ? "Ethernet" : "???", ntohs(arp->ea_hdr.ar_hrd));
    printf(" Protocol type : %s (0x%04x)\n", (ntohs(arp->ea_hdr.ar_pro) == ETH_P_IP) ? "IPv4" : "???", ntohs(arp->ea_hdr.ar_pro));
    printf(" Hardware size : %d\n", arp->ea_hdr.ar_hln);
    printf(" Protocol size : %d\n", arp->ea_hdr.ar_pln);
    printf(" Opcode : %s (%d)\n", (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) ? "request" :
        (ntohs(arp->ea_hdr.ar_op) == ARPOP_REPLY) ? "reply" : "???",
        ntohs(arp->ea_hdr.ar_op));
    printf(" Sender MAC address : ");
    print_mac(arp->arp_sha);
    printf(" Sender IP address : ");
    print_ip(arp->arp_spa);
    printf(" Target MAC address : ");
    print_mac(arp->arp_tha);
    printf(" Target IP address : ");
    print_ip(arp->arp_tpa);
}

int     interface_from_index(int ifindex, struct ifaddrs *iface)
{
    struct ifaddrs *tmp = 0;

    if (getifaddrs(&tmp) < 0)
        return -1;

    for (struct ifaddrs *t = tmp; t; t = t->ifa_next)
        if (((struct sockaddr_ll *)t->ifa_addr)->sll_ifindex == ifindex)
            ft_memcpy(iface, t, sizeof(struct ifaddrs));
    freeifaddrs(tmp);
    return 0;
}

void    string_to_addr(char *s, unsigned char *arr, int base, int len, char tok)
{
    for (int i = 0; i < len; ++i) {
        arr[i] = strtol(s, &s, base);
        if (*s == tok) ++s;
    }
}