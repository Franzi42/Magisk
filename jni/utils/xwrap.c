/* xwrap.c - wrappers around existing library functions.
 *
 * Functions with the x prefix are wrappers that either succeed or kill the
 * program with an error message, but never return failure. They usually have
 * the same arguments and return value as the function they wrap.
 *
 */

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <selinux/selinux.h>

#include "magisk.h"
#include "utils.h"

FILE *xfopen(const char *pathname, const char *mode) {
	FILE *fp = fopen(pathname, mode);
	if (fp == NULL) {
		PLOGE("fopen: %s", pathname);
	}
	return fp;
}

int xopen(const char *pathname, int flags) {
	int fd = open(pathname, flags);
	if (fd < 0) {
		PLOGE("open: %s", pathname);
	}
	return fd;
}

ssize_t xwrite(int fd, const void *buf, size_t count) {
	if (count != write(fd, buf, count)) {
		PLOGE("write");
	}
	return count;
}

// Read error other than EOF
ssize_t xread(int fd, void *buf, size_t count) {
	int ret = read(fd, buf, count);
	if (ret < 0) {
		PLOGE("read");
	}
	return ret;
}

// Read exact same size as count
ssize_t xxread(int fd, void *buf, size_t count) {
	if (count != read(fd, buf, count)) {
		PLOGE("read");
	}
	return count;
}

int xpipe(int pipefd[2]) {
	if (pipe(pipefd) == -1) {
		PLOGE("pipe");
	}
	return 0;
}

int xsetns(int fd, int nstype) {
	if (setns(fd, nstype) == -1) {
		PLOGE("setns");
	}
	return 0;
}

DIR *xopendir(const char *name) {
	DIR *d = opendir(name);
	if (d == NULL) {
		PLOGE("opendir: %s", name);
	}
	return d;
}

struct dirent *xreaddir(DIR *dirp) {
	errno = 0;
	struct dirent *e = readdir(dirp);
	if (errno && e == NULL) {
		PLOGE("readdir");
	}
	return e;
}

pid_t xsetsid() {
	pid_t pid = setsid();
	if (pid == -1) {
		PLOGE("setsid");
	}
	return pid;
}

int xsetcon(char *context) {
	if (setcon(context) == -1) {
		PLOGE("setcon: %s", context);
	}
	return 0;
}

int xsocket(int domain, int type, int protocol) {
	int fd = socket(domain, type, protocol);
	if (fd == -1) {
		PLOGE("socket");
	}
	return fd;
}

int xbind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	if (bind(sockfd, addr, addrlen) == -1) {
		PLOGE("bind");
	}
	return 0;
}

int xconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	if (connect(sockfd, addr, addrlen) == -1) {
		PLOGE("bind");
	}
	return 0;
}

int xlisten(int sockfd, int backlog) {
	if (listen(sockfd, backlog) == -1) {
		PLOGE("listen");
	}
	return 0;
}

int xaccept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	int fd = accept(sockfd, addr, addrlen);
	if (fd == -1) {
		PLOGE("accept");
	}
	return fd;
}

void *xmalloc(size_t size) {
	void *p = malloc(size);
	if (p == NULL) {
		PLOGE("malloc");
	}
	return p;
}

void *xcalloc(size_t nmemb, size_t size) {
	void *p = calloc(nmemb, size);
	if (p == NULL) {
		PLOGE("calloc");
	}
	return p;
}

void *xrealloc(void *ptr, size_t size) {
	void *p = realloc(ptr, size);
	if (p == NULL) {
		PLOGE("realloc");
	}
	return p;
}

ssize_t xsendmsg(int sockfd, const struct msghdr *msg, int flags) {
	int sent = sendmsg(sockfd, msg, flags);
	if (sent == -1) {
		PLOGE("sendmsg");
	}
	return sent;
}

ssize_t xrecvmsg(int sockfd, struct msghdr *msg, int flags) {
	int rec = recvmsg(sockfd, msg, flags);
	if (rec == -1) {
		PLOGE("recvmsg");
	}
	return rec;
}

