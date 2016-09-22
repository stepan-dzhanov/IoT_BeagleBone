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
#include <pwd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/mman.h>






#include "STM32nFR24.h"
#include "ThingSpeakClient.h"
#include "UDPReciver.h"
#include "ComPort.hpp"

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
	while(1) {
		transmitter.GetEvent(NULL);
		usleep(300000);
	}
}


void *RF_Gate(void *threadid)
{
    char a =0;
    char pointer_data_from_gate=0;
	char data_from_gate [128];
	char tmp_data [128];
	char command[34];
	int bytes_from_gate;
	int i;
	ComPort *gate_port;
	message_buf_t  rbuf;
	key_t key ;
	int msqid;


	gate_port = new ComPort("/dev/ttyO4", 9600);
    key = QUEUES;
    if ((msqid = msgget(key, 0666)) < 0) {
        	        perror("msgget");
        	        exit(1);
    }

	//if ((msqid = msgget(QUEUES, IPC_CREAT | 0666)) < 0) {
		//perror("ms1gget error");
		//exit(-1);
	//}





	if (gate_port<=0)	{
		perror ("Error COM port");
		exit (-1);
	}




	while(1) {
		bytes_from_gate = gate_port->ReadDataPort( tmp_data , 64);


	    if ((bytes_from_gate>0)){
					memcpy(&data_from_gate[pointer_data_from_gate],tmp_data,bytes_from_gate);
					pointer_data_from_gate += bytes_from_gate;
		    }
		if (pointer_data_from_gate>=34){
			for (i=0; i<128; i++) { if ( (data_from_gate[i]==0x0D)&&(data_from_gate[i-1]==0x0A) ) break;}

			if (i<=33) {
				if (i==33){


					rbuf.mtype = MESSAGE_TYPE_EVENT;
					memcpy(rbuf.mtext,data_from_gate, 32);
					//cout<<rbuf.mtext<<'\n';
					int buf_length = sizeof(message_buf_t) - sizeof(long);
					if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
						perror("RF_gate_thead");
					}
					usleep(300000);
					if(msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_COMMAND_4, IPC_NOWAIT)>0)	{
						//sprintf(command,"%ctst\n\r",0x05);
						memcpy(command,rbuf.mtext,32);
						command[32] = 0x0A;
						command[33] = 0x0D;
						gate_port->WriteDataPort(command,7);
						//cout<<"COMMAND"<<command;
						usleep(500000);
						bytes_from_gate = gate_port->ReadDataPort( tmp_data , 64);
						//if (bytes_from_gate){
						//cout<<"data"<<tmp_data;
						//}
					}
				}
				memcpy(tmp_data,&data_from_gate[i+1],pointer_data_from_gate-(i+1));
				memcpy(data_from_gate,tmp_data,pointer_data_from_gate-i);
				pointer_data_from_gate = pointer_data_from_gate-i-1;


			}



		}




		//usleep(300);

	}
}



int main(int argc, char** argv)
{
	pthread_t threads;
	char command[64];
	char rx[64];

	int rc;
	int i,result;
	int msqid;
	key_t key;
    message_buf_t  rbuf;
    key = QUEUES;
    char mode=0;
    sleep(10);
    signal(SIGPIPE, SIG_IGN);


    Scenario *scenario = new  Scenario();

    UDPReciver *udpReciver = new UDPReciver();

    /*ComPort *gate_port;
    gate_port = new ComPort("/dev/ttyO4", 9600);
    sprintf(command,"hello world");
    sprintf(rx,"ffffffffffffff");
    gate_port->WriteDataPort(command,7);
    sleep(1);
    gate_port->ReadDataPort(rx,32);
    cout<<rx;
    exit(0);*/



    //system("sudo rm /home/log.txt");
    //system("sudo /home/spi_enable.sh");


    rc = pthread_create(&threads, NULL,RF_Service, (void *)i);
    if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
    }
    sleep(1);

    rc = pthread_create(&threads, NULL,RF_Gate, (void *)i);
       if (rc){
               cout << "Error:unable to create thread," << rc << endl;
               exit(-1);
    }


    if ((msqid = msgget(key, 0666)) < 0) {
    	        perror("msgget");
    	        exit(1);
    }


    while(1){
    	int i =msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_EVENT, IPC_NOWAIT);

		if ((i > 0)) {
				//perror("msgrcv");
			char pbuf[32];
			memcpy(&pbuf,rbuf.mtext,8);
			if(pbuf[3]==0x05) scenario->TestOK(0x05);
			pbuf[8]=0;
			pbuf[3] |= 0x30;
			pbuf[4] |= 0x30;
			cout<<pbuf<<"\n";


			scenario->EventProcessing(pbuf);
				//exit(1);
		}

		scenario->WaterDelivery();
		char udp_message;
		udp_message = udpReciver->GetMessage();

		if (udp_message==UDP_MESSAGE_ON_FOOD) {
			mode =1;
			cout<<"ON_COOCKING\n";
		}
		if (udp_message==UDP_MESSAGE_OFF_FOOD)	{
			mode =0;
			cout<<"OFF_COOCKING\n";
		}
		if (udp_message==UDP_MESSAGE_COOCK_SCH_UPDATE)	{
			char data[32];
			udpReciver->GetData(data);

			scenario->UpdateCoockingSchedule(data);
			cout<<"UPDATE_SCHEDULE\n";
	    }

		scenario->Coocking(mode);
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
