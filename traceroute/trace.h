#ifndef __TRACE_HEADER__
#define __TRACE_HEADER__
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<signal.h>
#include<netinet/in_systm.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netdb.h>

#define BUFSIZE 1500
struct rec {
	u_short rec_seq;
	u_short rec_ttl;
	struct timeval rec_tv;
};

char recvbuf[BUFSIZE];
char sendbuf[BUFSIZE];

int datalen;
char *host;
u_short sport, dport;
int nsend;
pid_t pid;
int probe, nprobes;
int sendfd, recvfd;
int ttl, max_tt;
int verbose;

const char *icmpcode_v4(int);
int recv_v4(int, struct timeval *);
void sig_alrm(int);
void traceloop(void);
void tv_sub(struct timeval *, struct timeval *);

struct proto {
	const char *(*icmpcode)(int);
	int (*recv)(int, struct timeval *);
	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	struct sockaddr *salast;
	struct sockaddr *sabind;
	socklen_t salen;
	int icmpproto;
	int ttllevel;
	int ttloptname;
}*pr;
#endif

