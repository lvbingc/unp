#include"ping.h"

void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv) {
	int hlen1, icmplen;
	double rtt;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;

	ip = (struct ip*) ptr;
	hlen1 = ip->ip_hl << 2;
	if (ip->ip_p != IPPROTO_ICMP) {
		return;
	}
	icmp = (struct icmp*) (ptr + hlen1);
	if ((icmplen = len - hlen1) < 8)
		return;
	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		if (icmp->icmp_id != pid) {
			return;
		}
		if (icmplen < 16) {
			return;
		}
		tvsend = (struct timeval *) icmp->icmp_data;
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
		printf("%d bytes from %s :seq=%u,ttl=%d,rtt=%.3fms\n", icmplen,
				sock_ntop(pr->sarecv, pr->salen), icmp->icmp_seq, ip->ip_ttl,
				rtt);
	} else if (verbose) {
		printf("%d bytes from %s :type=%d,code=%d\n", icmplen,
				sock_ntop(pr->sarecv, pr->salen), icmp->icmp_type,
				icmp->icmp_code);
	}
}
