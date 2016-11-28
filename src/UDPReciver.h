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

#define UDP_MESSAGE_ON_FOOD           0x31
#define UDP_MESSAGE_OFF_FOOD          0x32
#define UDP_MESSAGE_COOCK_SCH_UPDATE  0x33
#define UDP_MESSAGE_GET_TEMP_VALUE    0x34
#define UDP_MESSAGE_START_COOCKING    0x35













class UDPReciver {
public:
	UDPReciver();
    int GetMessage();
    void GetData(char *data);
    int SentMessage(char *message);
	virtual ~UDPReciver();
private:
	 struct sockaddr_in myaddr;      /* our address */
	 struct sockaddr_in remaddr;     /* remote address */
	 socklen_t addrlen ;//= sizeof(remaddr);            /* length of addresses */
	 signed int recvlen;                    /* # bytes received */
	 int fd;                         /* our socket */
	 unsigned char buf[BUFSIZE];     /* receive buffer */
	 char in_message[1024];
};

#endif /* UDPRECIVER_H_ */
