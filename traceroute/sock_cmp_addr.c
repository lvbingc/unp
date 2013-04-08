#include"trace.h"

#ifdef	HAVE_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif

int sock_cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2,
		socklen_t salen) {
	if (sa1->sa_family != sa2->sa_family)
		return (-1);

	switch (sa1->sa_family) {
	case AF_INET: {
		return (memcmp(&((struct sockaddr_in *) sa1)->sin_addr,
				&((struct sockaddr_in *) sa2)->sin_addr, sizeof(struct in_addr)));
	}

	}
	return (-1);
}
