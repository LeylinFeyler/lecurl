#ifndef NET_H
#define NET_H

int dns_resolve(const char *host, char *ip_str);
int tcp_connect(const char *host, int port);

#endif