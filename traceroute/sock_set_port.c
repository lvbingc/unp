#include"trace.h"
void sock_set_port(struct sockaddr *sa, socklen_t salen, int port) {
	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in *sin = (struct sockaddr_in *) sa;
		sin->sin_port = port;
		return;
	}
		return;
	}
}
