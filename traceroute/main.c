#include"trace.h"

struct proto proto_v4 = { icmpcode_v4, recv_v4, NULL, NULL, NULL, NULL, 0,
		IPPROTO_ICMP, IPPROTO_IP, IP_TTL };

int datalen = sizeof(struct rec);
int max_ttl = 30;
int nprobes = 3;

u_short dport = 32768 + 666;

int main(int argc, char **argv) {
	int c;
	struct addrinfo *ai;
	char *h;
	opterr = 0;
	while ((c = getopt(argc, argv, "m:v")) != -1) {
		switch (c) {
		case 'm':
			if ((max_ttl = atoi(optarg)) <= 1) {
				printf("invalid -m value");
				exit(0);
			}
			break;
		case 'v':
			verbose++;
			break;
		case '?':
			printf("unrecognized option:%c", c);
		}
	}
	if (optind != argc - 1) {
		printf("usage:traceroute [ -m <maxttl> -v] <hostname>");
		exit(0);
	}
	host = argv[optind];
	pid = getpid();
	signal(SIGALRM, sig_alrm);
	ai = host_serv(host, NULL, 0, 0);
	h = sock_ntop(ai->ai_addr, ai->ai_addrlen);
	printf("traceroute to %s(%s):%d hops max,%d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h, h, max_ttl, datalen);
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
	} else {
		printf("unknown addr family %d", ai->ai_family);
		exit(0);
	}
	pr->sasend = ai->ai_addr;
	pr->sarecv = calloc(1, ai->ai_addrlen);
	pr->salast = calloc(1, ai->ai_addrlen);
	pr->sabind = calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;
	traceloop();
	exit(0);
}
