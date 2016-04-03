/*
 * UDPReciver.cpp
 *
 *  Created on: 29.12.2013
 *      Author: stepan
 */

#include "UDPReciver.h"

UDPReciver::UDPReciver() {
	            unsigned long nb = 1;
	            socklen_t addrlen = sizeof(remaddr);
	            if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		                perror("cannot create socket\n");
		               // exit();
		        }
	            ioctl(fd, FIONBIO, (unsigned long *) &nb);

		        /* bind the socket to any valid IP address and a specific port */

		        memset((char *)&myaddr, 0, sizeof(myaddr));
		        myaddr.sin_family = AF_INET;
		        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		        myaddr.sin_port = htons(PORT);

		        if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		                perror("bind failed");
		               // return 0;
		        }

}


signed int UDPReciver::GetMessage(char *message) {
	recvlen = read(fd, buf, BUFSIZE);//, 0, (struct sockaddr *)&remaddr, &addrlen);
	if(recvlen>0)memcpy(message,buf,recvlen);
	return recvlen;

}


UDPReciver::~UDPReciver() {
	// TODO Auto-generated destructor stub
}
