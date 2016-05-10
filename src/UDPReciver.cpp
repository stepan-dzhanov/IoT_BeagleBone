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


int UDPReciver::GetMessage() {
	char message[32];

    for (int i=0;i<30;i++)in_message[i]=0;
	recvlen = read(fd, buf, BUFSIZE);//, 0, (struct sockaddr *)&remaddr, &addrlen);
	if(recvlen>0)	{
		memcpy(in_message,buf,recvlen);

      //  std::cout<<"UDP received "<<in_message<<'\n';
        sprintf(message,"on food");
        if( !memcmp(&in_message,&message,7) ) return UDP_MESSAGE_ON_FOOD;
        sprintf(message,"off food");
        if( !memcmp(&in_message,&message,8)) return UDP_MESSAGE_OFF_FOOD;
        sprintf(message,"cup");
        if( !memcmp(&in_message,&message,3) ) return UDP_MESSAGE_COOCK_SCH_UPDATE;
        return 0;

	}
	return 0;

}


void UDPReciver::GetData(char *data)	{
	memcpy(data,&in_message[3],16);

}


UDPReciver::~UDPReciver() {
	close(fd);
}
