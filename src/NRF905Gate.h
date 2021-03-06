/*
 * NRF905Gate.h
 *
 *  Created on: Sep 23, 2016
 *      Author: stepan
 */

#ifndef NRF905GATE_H_
#define NRF905GATE_H_

#include <stddef.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <ctime>
#include <string>



#include "spi.h"
#include "gpio.h"
#include "compatibility.h"
#include "protocol.h"
#include "ComPort.hpp"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <iostream>







class NRF905Gate	{
public:
	NRF905Gate();
	virtual ~NRF905Gate();
	bool GetEvent();


private:
	ComPort *gate_port;
	message_buf_t  rbuf;
	key_t key ;
	int msqid;

	 char a;
	 char pointer_data_from_gate;
	 char data_from_gate [128];
	 char tmp_data [128];
	 char command[34];
	 int bytes_from_gate;
	 int i;
	 int fid;
	 int str_counter;
	 void AddSensorToLog(char _id, char _bat );
	 char GetMessageLenght();

	 char flg;



};


#endif /* NRF905GATE_H_ */
