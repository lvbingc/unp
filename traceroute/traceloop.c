#include "trace.h"
extern max_ttl;

void traceloop(void) {
	int seq, code, done;
	double rtt;
	struct rec *rec;
	struct timeval tvrecv;

	recvfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	setuid(getuid());
	sendfd = socket(pr->sasend->sa_family, SOCK_DGRAM, 0);
	pr->sabind->sa_family = pr->sasend->sa_family;
	sport = (getpid() & 0xffff) | 0x8000;
	sock_set_port(pr->sabind, pr->salen, htons(sport));
	bind(sendfd, pr->sabind, pr->salen);
	sig_alrm(SIGALRM);
	seq = 0;
	done = 0;
	for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
		setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int));
		bzero(pr->salast, pr->salen);
		printf("%2d", ttl);
		fflush(stdout);
		for (probe = 0; probe < nprobes; probe++) {
			rec = (struct rec *) sendbuf;
			rec->rec_seq = ++seq;
			rec->rec_ttl = ttl;
			gettimeofday(&rec->rec_tv, NULL );
			sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
			sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);
			if ((code = (*pr->recv)(seq, &tvrecv)) == -3) {
				printf("*");
			} else {
				char str[NI_MAXHOST];
				if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
					if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str),
							NULL, 0, 0) == 0) {
						printf(" %s (%s)", str,
								sock_ntop(pr->sarecv, pr->salen));
					} else {
						printf(" %s", sock_ntop(pr->sarecv, pr->salen));
					}
					memcpy(pr->salast, pr->sarecv, pr->salen);
				}
				tv_sub(&tvrecv, &rec->rec_tv);
				rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
				printf(" %.3f ms", rtt);
				if (code == -1) {
					done++;
				} else if (code >= 0) {
					printf("(ICMP %s)", (*pr->icmpcode)(code));
				}
			}
			fflush(stdout);
		}
		printf("\n");
	}
}
