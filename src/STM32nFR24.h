/*
 * STM32nFR24.h
 *
 *  Created on: 25.03.2016
 *      Author: stepan
 */

#ifndef STM32NFR24_H_
#define STM32NFR24_H_



#include <stddef.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <ctime>




#include "spi.h"
#include "gpio.h"
#include "compatibility.h"
#include "protocol.h"
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









typedef enum {
	TM_NRF24L01_Transmit_Status_Lost = 0x00,   /*!< Message is lost, reached maximum number of retransmissions */
	TM_NRF24L01_Transmit_Status_Ok = 0x01,     /*!< Message sent successfully */
	TM_NRF24L01_Transmit_Status_Sending = 0xFF /*!< Message is still sending */
} TM_NRF24L01_Transmit_Status_t;

/**
 * @brief  Data rate enumeration
 */
typedef enum {
	TM_NRF24L01_DataRate_2M,  /*!< Data rate set to 2Mbps */
	TM_NRF24L01_DataRate_1M,  /*!< Data rate set to 1Mbps */
	TM_NRF24L01_DataRate_250k /*!< Data rate set to 250kbps */
} TM_NRF24L01_DataRate_t;

/**
 * @brief  Output power enumeration
 */
typedef enum {
	TM_NRF24L01_OutputPower_M18dBm,	/*!< Output power set to -18dBm */
	TM_NRF24L01_OutputPower_M12dBm, /*!< Output power set to -12dBm */
	TM_NRF24L01_OutputPower_M6dBm,  /*!< Output power set to -6dBm */
	TM_NRF24L01_OutputPower_0dBm    /*!< Output power set to 0dBm */
} TM_NRF24L01_OutputPower_t;

typedef struct {
	uint8_t PayloadSize;				//Payload size
	uint8_t Channel;					//Channel selected
	TM_NRF24L01_OutputPower_t OutPwr;	//Output power
	TM_NRF24L01_DataRate_t DataRate;	//Data rate
} TM_NRF24L01_t;

typedef struct {
	uint8_t id;
	uint8_t addr;
	uint8_t data[32];

}TM_NRF24L01_Event_t;


typedef struct {
	uint8_t min;
	uint8_t hour;
	uint8_t date;

}TM_NRF24L01_SensorTime_t;

struct TM_NRF24L01_Sensor_t {
	uint8_t id;
	uint8_t addr;
	uint8_t bat_status;
	TM_NRF24L01_SensorTime_t last_time;
	TM_NRF24L01_Sensor_t  *next;

};


/* Clear interrupt flags */
#define NRF24L01_CLEAR_INTERRUPTS   do { TM_NRF24L01_WriteRegister(0x07, 0x70); } while (0)

/* Gets interrupt status from device */
#define NRF24L01_GET_INTERRUPTS     TM_NRF24L01_GetStatus()

/* Interrupt masks */
#define NRF24L01_IRQ_DATA_READY     0x40 /*!< Data ready for receive */
#define NRF24L01_IRQ_TRAN_OK        0x20 /*!< Transmission went OK */
#define NRF24L01_IRQ_MAX_RT         0x10 /*!< Max retransmissions reached, last transmission failed */



#define LOW GPIO::OUTPUT_LOW
#define HIGH GPIO::OUTPUT_HIGH
#define INPUT GPIO::DIRECTION_IN
#define OUTPUT GPIO::DIRECTION_OUT
#define digitalWrite(pin, value) GPIO::write(pin, value)
#define pinMode(pin, direction) GPIO::open(pin, direction)
#define delay(milisec) __msleep(milisec)
#define delayMicroseconds(usec) __usleep(usec)















class STM32_nFR24 {
public:
	STM32_nFR24(uint8_t _cepin, uint8_t _cspin);
	virtual ~STM32_nFR24();


	uint8_t TM_NRF24L01_Init(uint8_t channel, uint8_t payload_size);
	bool GetEvent (TM_NRF24L01_Event_t* event);


private:

	TM_NRF24L01_t TM_NRF24L01_Struct;
	void TM_NRF24L01_SoftwareReset(void);
	uint8_t TM_NRF24L01_RxFifoEmpty(void);
	void TM_NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value);
	uint8_t TM_NRF24L01_ReadBit(uint8_t reg, uint8_t bit);
	uint8_t TM_NRF24L01_ReadRegister(uint8_t reg);
	void csn(int mode);
	void ce(int level);

	void TM_NRF24L01_SetMyAddress(uint8_t* adr);
		void TM_NRF24L01_SetTxAddress(uint8_t* adr);
		uint8_t TM_NRF24L01_GetRetransmissionsCount(void);
		void TM_NRF24L01_PowerUpTx(void);
		void TM_NRF24L01_PowerUpRx(void);
		void TM_NRF24L01_PowerDown(void);
		TM_NRF24L01_Transmit_Status_t TM_NRF24L01_GetTransmissionStatus(void);
		void TM_NRF24L01_Transmit(uint8_t *data);
		uint8_t TM_NRF24L01_DataReady(void);
		void TM_NRF24L01_GetData(uint8_t *data);
		void TM_NRF24L01_SetChannel(uint8_t channel);
		void TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_t DataRate, TM_NRF24L01_OutputPower_t OutPwr);
		uint8_t TM_NRF24L01_GetStatus(void);
		void TM_NRF24L01_WriteRegister(uint8_t reg, uint8_t value);
		void TM_NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count);
		void TM_NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count);

		void PrintSensorList();
		void AddSensorToList(uint8_t _id, uint8_t _addr, uint8_t _bat);
		bool UpdateSensorInfo(uint8_t _addr, uint8_t _bat);
		void ClearSensorList();
		void AddSensorToLog(char _id, char _bat);
		int str_counter;


















	message_buf_t message;
	key_t key;
	int buf_length;
	int msqid;

	uint8_t ce_pin; /**< "Chip Enable" pin, activates the RX or TX role */
	uint8_t csn_pin; /**< SPI Chip select */
	SPI* spi;
	char command[32];
	uint8_t dataIn[32];
	uint8_t dataOut[32];
	TM_NRF24L01_Transmit_Status_t transmissionStatus;
	TM_NRF24L01_Sensor_t *first;
	int fid;




};


#endif /* STM32NFR24_H_ */
