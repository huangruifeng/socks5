/*
 * transfer.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: huoyin
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pthread.h>

#include "common.h"

#include "transfer.h"

const int BUF_LEN = 1048576;

void *transfer(void *p) {
	TParam *param = (TParam *)p;
	int in = param->in;
	int out = param->out;
	int bro = param->bro;
	char buf[BUF_LEN];
	while (true) {
		int ret = recv(in, buf, BUF_LEN, 0);
		if (ret <= 0) {
			break;
		}
		//enc(buf, ret);
		ret = send_n(out, buf, ret);
		if (ret < 0) {
			break;
		}
	}
	close(in);
	close(out);
	pthread_cancel(bro);
	return NULL;
}


