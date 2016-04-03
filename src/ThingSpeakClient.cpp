/*
 * ThingSpeakClient.cpp
 *
 *  Created on: 22.03.2016
 *      Author: stepan
 */

#include "ThingSpeakClient.h"

ThingSpeakClient::ThingSpeakClient() {

	    sock = socket(AF_INET, SOCK_STREAM, 0);
	    if(sock < 0)
	    {
	        perror("socket");

	    }


	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(80); // или любой другой порт...
	    addr.sin_addr.s_addr = inet_addr("184.106.153.149");


}
int ThingSpeakClient::GetField(char *s_field, char *e_field) {
	char *s_fld = "<field5>";
	char *e_fld = "</field5>";

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	   {
		   perror("ThingSpeak_connect");
		   return -1;
	   }

	   char message[]= "GET /channels/46795/feeds.xml?results=2\r\n\r\n";
	   fcntl(sock, F_SETFL, O_NONBLOCK);
	   send(sock, message, sizeof(message), 0);
	   sleep(1);
	   if (recv(sock, buf, 16048, 0)<0) return -1;
	   else{
		   std::string sstr(buf);
		   pos = sstr.find("<feed>");
		   start_pos = sstr.find(s_field, pos)+8;
		   end_pos = sstr.find(e_field, start_pos)-1;
		   result[0] = sstr[start_pos];
		   result[1] = sstr[end_pos];
		   result[3]= 0;
		   humidity = (result[0]-48)*10+(result[1]-48);
		   close(sock);


		   return humidity;
	   }
}

ThingSpeakClient::~ThingSpeakClient() {
	// TODO Auto-generated destructor stub
}

