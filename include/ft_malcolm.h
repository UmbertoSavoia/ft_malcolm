#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <signal.h>

#define EXIT_MSG "Exiting program..."
#define print_mac(m) printf("%02x:%02x:%02x:%02x:%02x:%02x\n", \
    m[0], m[1], m[2], m[3], m[4], m[5])

int     check(int ac, char **av);

#endif

/*
 // https://elixir.bootlin.com/linux/v4.1/source/include/uapi/linux/if_ether.h#L137
 struct ethhdr {
    unsigned char   h_dest[ETH_ALEN];   // destination eth addr
    unsigned char   h_source[ETH_ALEN]; // source ether addr
    __be16          h_proto;            // packet type ID field
    } __attribute__((packed));

 // https://sites.uclouvain.be/SystInfo/usr/include/net/if_arp.h.html
 struct arphdr {
    unsigned short int ar_hrd;   // Format of hardware address.
    unsigned short int ar_pro;   // Format of protocol address.
    unsigned char ar_hln;        // Length of hardware address.
    unsigned char ar_pln;        // Length of protocol address.
    unsigned short int ar_op;    // ARP opcode (command).
};
// https://sites.uclouvain.be/SystInfo/usr/include/netinet/if_ether.h.html
 struct	ether_arp {
    struct	arphdr ea_hdr;  // fixed-size header
    u_char	arp_sha[6];     // sender hardware address
    u_char	arp_spa[4];     // sender protocol address
    u_char	arp_tha[6];     // target hardware address
    u_char	arp_tpa[4];     // target protocol address
};
 */