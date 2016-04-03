/*
 * UDPReciver.h
 *
 *  Created on: 29.12.2013
 *      Author: stepan
 */

#ifndef UDPRECIVER_H_
#define UDPRECIVER_H_

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>	/* for fprintf */
#include <string.h>	/* for memcpy */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <stdio.h>

#define PORT 1153
#define BUFSIZE 2048


class UDPReciver {
public:
	UDPReciver();
	signed int GetMessage(char *message);
	virtual ~UDPReciver();
private:
	 struct sockaddr_in myaddr;      /* our address */
	 struct sockaddr_in remaddr;     /* remote address */
	 socklen_t addrlen ;//= sizeof(remaddr);            /* length of addresses */
	 signed int recvlen;                    /* # bytes received */
	 int fd;                         /* our socket */
	 unsigned char buf[BUFSIZE];     /* receive buffer */
};

#endif /* UDPRECIVER_H_ */
