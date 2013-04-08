#include"ping.h"

void readloop(void) {
	int size;
	char recv_buf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;

	sockfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	if (sockfd < 0) {
		printf("socket error %s\n", strerror(errno));
	}
	setuid(getuid());
	if (pr->finit) {
		(*pr->finit)();
	}
	size = 60 * 1024;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	sig_alrm(SIGALRM);
	iov.iov_base = recv_buf;
	iov.iov_len = sizeof(recv_buf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for (;;) {
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, 0);
		if (n < 0)
			if (errno == EINTR)
				continue;
			else {
				printf("recvmsg error:%s\n", strerror(errno));
				exit(0);
			}
		gettimeofday(&tval, NULL );
		(*pr->fproc)(recv_buf, n, &msg, &tval);
	}
}
