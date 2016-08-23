/*
 * client.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: huoyin
 */




#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include "protocol.h"
#include "common.h"
#include "transfer.h"

int proc_forward(int in, const char *ip, const char *port);

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cout << "Usage ./cli port peer_ip peer_port" << endl;
		return 1;
	}
	int lsnr = socket(AF_INET, SOCK_STREAM, 0);
	assert(lsnr != -1);

	sockaddr_in laddr;
	memset(&laddr, 0, sizeof(laddr));
	laddr.sin_family = AF_INET;
	laddr.sin_addr.s_addr = INADDR_ANY;
	laddr.sin_port = htons(atoi(argv[1]));
	assert(0 == bind(lsnr, (sockaddr *)&laddr, sizeof(laddr)));
	assert(0 == listen(lsnr, 5));

	while (true) {
		sockaddr caddr;
		socklen_t slen = sizeof(caddr);
		int in = accept(lsnr, &caddr, &slen);
		assert(in != -1);

		pid_t pid = fork();
		assert(pid >= 0);
		if (pid > 0) {
			waitpid(pid, NULL, 0);
			close(in);
		} else {
			pid_t pid2 = fork();
			assert(pid >= 0);
			if (pid2 > 0) {
				exit(0);
			} else {
				close(lsnr);
				return proc_forward(in, argv[2], argv[3]);
			}
		}
	}
}

int proc_forward(int in, const char *ip, const char *port) {
	int out = socket(AF_INET, SOCK_STREAM, 0);
	assert(out != -1);
	sockaddr_in raddr;
	memset(&raddr, 0, sizeof(raddr));
	raddr.sin_family = AF_INET;
	raddr.sin_addr.s_addr = inet_addr(ip);
	raddr.sin_port = htons(atoi(port));
	assert(0 == connect(out, (sockaddr *)&raddr, sizeof(raddr)));

	TParam up = {in, out, 0};
	TParam down = {out, in, 0};
	assert(0 == pthread_create(&down.bro, NULL, transfer, &up));
	assert(0 == pthread_create(&up.bro, NULL, transfer, &down));

	pthread_join(up.bro, NULL);
	pthread_join(down.bro, NULL);
	return 0;
}
