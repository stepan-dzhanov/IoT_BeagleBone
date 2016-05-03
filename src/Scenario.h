/*
 * Scenario.h
 *
 *  Created on: Apr 2, 2016
 *      Author: stepan
 */

#ifndef SCENARIO_H_
#define SCENARIO_H_

#include "ThingSpeakClient.h"
#include "UDPReciver.h"
#include "protocol.h"

#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <ctime>
#include <math.h>

#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



class Scenario {
public:
	Scenario();
	virtual ~Scenario();

	void WaterDelivery();

	ThingSpeakClient *thingspeak_data;
	UDPReciver *udpReciver;
	int humidity;
	int GetHumidity();
	void AddEventToLog(char *event);
	int Sendmail(const char *to, const char *from, const char *subject,  char *message);
	void Report();
	void EventProcessing (char *event);
	int msqid;
	key_t key;
    int buf_length;
    int fid;





};

#endif /* SCENARIO_H_ */
