/*
 * common.h
 *
 *  Created on: Aug 21, 2016
 *      Author: huoyin
 */

#ifndef COMMON_H_
#define COMMON_H_


int recv_n(int sock, char *buf, int len);

int send_n(int sock, char *buf, int len);

void enx(char *data, int len);
#endif /* COMMON_H_ */
