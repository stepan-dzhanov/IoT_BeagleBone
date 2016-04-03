/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <ctime>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>






#include "STM32nFR24.h"
#include "ThingSpeakClient.h"
#include "UDPReciver.h"

#include "Scenario.h"
#include "protocol.h"

using namespace std;

//
// Hardware configuration
//

// CE and CSN pins 


uint8_t dataOut[32], dataIn[32];
char message[32];
char command[32];


void *RF_Service(void *threadid)
{
	int ch = 19;
	char buff[128];
	TM_NRF24L01_Transmit_Status_t transmissionStatus;
	STM32_nFR24 transmitter(115, 112);
	transmitter.TM_NRF24L01_Init(ch,32);
	while(1) {transmitter.GetEvent(NULL);}
}




int main(int argc, char** argv)
{
	pthread_t threads;
	int rc;
	int i,result;
	int msqid;
	key_t key;
    message_buf_t  rbuf;
    key = QUEUES;



    Scenario *scenario = new  Scenario();

    //udpReciver = new UDPReciver;
    //system("sudo rm /home/log.txt");
    //system("sudo /home/spi_enable.sh");


    rc = pthread_create(&threads, NULL,RF_Service, (void *)i);
    if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
    }
    sleep(1);


    if ((msqid = msgget(key, 0666)) < 0) {
    	        perror("msgget");
    	        exit(1);
    }


    while(1){
    	int i =msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_EVENT, IPC_NOWAIT);

		if ((i > 0)) {
				//perror("msgrcv");
			cout<<rbuf.mtext<<"\n";
				//exit(1);
		}
		scenario->WaterDelivery();
		sleep(1);



    }

    result = pthread_join(threads, NULL);
        if (result != 0) {
            perror("thread error");
            return EXIT_FAILURE;
        }
    cout << "Wait,"<< endl;









   // close(log_file);



}



// vim:ai:cin:sts=2 sw=2 ft=cpp
