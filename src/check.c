#include "../include/ft_malcolm.h"

int     ft_isdigit(char c, int base)
{
    if (base == 10)
        return (c >= '0' && c <= '9');
    else if (base == 16)
        return (c >= '0' && c <= '9' ||
            (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int     check_addr(char *addr, int base, int len, char tok)
{
    int n = 0;

    while (*addr) {
        if (*addr && ft_isdigit(*addr, base) && strtol(addr, 0, base) <= 255)
            ++n;
        else
            return -1;
        while (ft_isdigit(*addr, base) && *addr)
            ++addr;
        if (*addr == tok)
            ++addr;
    }

    return (n == len) ? 0 : -1;
}

int     check_ip(char *ip)
{
    return check_addr(ip, 10, 4, '.');
}

int     check_mac(char *mac)
{
    return check_addr(mac, 16, 6, ':');
}

int     check(int ac, char **av, char *verbose)
{
    int max = MAX_ARG;

    if ((ac == 6 && av[5][0] == '-' && av[5][1] == 'v')) {
        ++max;
        *verbose = 1;
    }
    if (getuid() != 0) {
        return -1;
    } else if (ac != max) {
        printf("%s: Wrong arguments -v\n", av[0]);
        return -1;
    } else if (check_ip(av[1]) < 0) {
        printf("%s: unknown host or invalid IP address: (%s)\n", av[0], av[1]);
        return -1;
    } else if (check_ip(av[3]) < 0) {
        printf("%s: unknown host or invalid IP address: (%s)\n", av[0], av[3]);
        return -1;
    } else if (check_mac(av[2]) < 0) {
        printf("%s: unknown host or invalid MAC address: (%s)\n", av[0], av[2]);
        return -1;
    } else if (check_mac(av[4]) < 0) {
        printf("%s: unknown host or invalid MAC address: (%s)\n", av[0], av[4]);
        return -1;
    }
    return 0;
}