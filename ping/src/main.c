#include"ping.h"

struct proto proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };

int datalen = 56;

int main(int argc, char **argv) {
	int c;
	struct addrinfo *ai = (struct addrinfo *) malloc(sizeof(struct addrinfo));
	char h[128];
	opterr = 0;
	while ((c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbose++;
			break;
		case '?':
			printf("unrecognized option:%c", c);
		}
	}
	if (optind != argc - 1) {
		printf("usage:ping [-v] <hostname>");
		exit(0);

	}
	host = argv[optind];
	pid = getpid() & 0xffff;
	signal(SIGALRM, sig_alrm);
	ai = host_serv(host, NULL, 0, 0);
	//h = sock_ntop(ai->ai_addr, ai->ai_addrlen);
	inet_ntop(AF_INET, &((struct sockaddr_in *) ai->ai_addr)->sin_addr, h,
			sizeof(h));
	printf("PING %s(%s):%d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h, h, datalen);
	if (ai->ai_family == AF_INET) {
		pr = &proto_v4;
	} else {
		printf("unkown address family %d", ai->ai_family);
		exit(0);
	}
	pr->sasend = ai->ai_addr;
	pr->sarecv = calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;
	readloop();
	exit(0);
}

