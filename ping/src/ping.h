#ifndef __PING_HEADER__
#define __PING_HEADER__
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in_systm.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<netdb.h>

#define BUFSIZE 1500

char sendbuf[BUFSIZE];

int datalen;
char *host;
int nsent;
pid_t pid;
int sockfd;
int verbose;

void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);

struct proto {
	void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
	void (*fsend)(void);
	void (*finit)(void);
	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	socklen_t salen;
	int icmpproto;
}*pr;
#endif
