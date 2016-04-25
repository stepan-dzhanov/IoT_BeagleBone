/*
 * STM32nFR24.cpp
 *
 *  Created on: 25.03.2016
 *      Author: stepan
 */

#include "STM32nFR24.h"



/* NRF24L01+ registers*/
#define NRF24L01_REG_CONFIG			0x00	//Configuration Register
#define NRF24L01_REG_EN_AA			0x01	//Enable ‘Auto Acknowledgment’ Function
#define NRF24L01_REG_EN_RXADDR		0x02	//Enabled RX Addresses
#define NRF24L01_REG_SETUP_AW		0x03	//Setup of Address Widths (common for all data pipes)
#define NRF24L01_REG_SETUP_RETR		0x04	//Setup of Automatic Retransmission
#define NRF24L01_REG_RF_CH			0x05	//RF Channel
#define NRF24L01_REG_RF_SETUP		0x06	//RF Setup Register
#define NRF24L01_REG_STATUS			0x07	//Status Register
#define NRF24L01_REG_OBSERVE_TX		0x08	//Transmit observe register
#define NRF24L01_REG_RPD			0x09
#define NRF24L01_REG_RX_ADDR_P0		0x0A	//Receive address data pipe 0. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P1		0x0B	//Receive address data pipe 1. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P2		0x0C	//Receive address data pipe 2. Only LSB
#define NRF24L01_REG_RX_ADDR_P3		0x0D	//Receive address data pipe 3. Only LSB
#define NRF24L01_REG_RX_ADDR_P4		0x0E	//Receive address data pipe 4. Only LSB
#define NRF24L01_REG_RX_ADDR_P5		0x0F	//Receive address data pipe 5. Only LSB
#define NRF24L01_REG_TX_ADDR		0x10	//Transmit address. Used for a PTX device only
#define NRF24L01_REG_RX_PW_P0		0x11
#define NRF24L01_REG_RX_PW_P1		0x12
#define NRF24L01_REG_RX_PW_P2		0x13
#define NRF24L01_REG_RX_PW_P3		0x14
#define NRF24L01_REG_RX_PW_P4		0x15
#define NRF24L01_REG_RX_PW_P5		0x16
#define NRF24L01_REG_FIFO_STATUS	0x17	//FIFO Status Register
#define NRF24L01_REG_DYNPD			0x1C	//Enable dynamic payload length
#define NRF24L01_REG_FEATURE		0x1D

/* Registers default values */
#define NRF24L01_REG_DEFAULT_VAL_CONFIG			0x08
#define NRF24L01_REG_DEFAULT_VAL_EN_AA			0x3F
#define NRF24L01_REG_DEFAULT_VAL_EN_RXADDR		0x03
#define NRF24L01_REG_DEFAULT_VAL_SETUP_AW		0x03
#define NRF24L01_REG_DEFAULT_VAL_SETUP_RETR		0x03
#define NRF24L01_REG_DEFAULT_VAL_RF_CH			0x02
#define NRF24L01_REG_DEFAULT_VAL_RF_SETUP		0x0E
#define NRF24L01_REG_DEFAULT_VAL_STATUS			0x0E
#define NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX		0x00
#define NRF24L01_REG_DEFAULT_VAL_RPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2		0xC3
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3		0xC4
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4		0xC5
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5		0xC6
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4		0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P0		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P1		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P2		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P3		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P4		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P5		0x00
#define NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS	0x11
#define NRF24L01_REG_DEFAULT_VAL_DYNPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_FEATURE		0x00

/* Configuration register*/
#define NRF24L01_MASK_RX_DR		6
#define NRF24L01_MASK_TX_DS		5
#define NRF24L01_MASK_MAX_RT	4
#define NRF24L01_EN_CRC			3
#define NRF24L01_CRCO			2
#define NRF24L01_PWR_UP			1
#define NRF24L01_PRIM_RX		0

/* Enable auto acknowledgment*/
#define NRF24L01_ENAA_P5		5
#define NRF24L01_ENAA_P4		4
#define NRF24L01_ENAA_P3		3
#define NRF24L01_ENAA_P2		2
#define NRF24L01_ENAA_P1		1
#define NRF24L01_ENAA_P0		0

/* Enable rx addresses */
#define NRF24L01_ERX_P5			5
#define NRF24L01_ERX_P4			4
#define NRF24L01_ERX_P3			3
#define NRF24L01_ERX_P2			2
#define NRF24L01_ERX_P1			1
#define NRF24L01_ERX_P0			0

/* Setup of address width */
#define NRF24L01_AW				0 //2 bits

/* Setup of auto re-transmission*/
#define NRF24L01_ARD			4 //4 bits
#define NRF24L01_ARC			0 //4 bits

/* RF setup register*/
#define NRF24L01_PLL_LOCK		4
#define NRF24L01_RF_DR_LOW		5
#define NRF24L01_RF_DR_HIGH		3
#define NRF24L01_RF_DR			3
#define NRF24L01_RF_PWR			1 //2 bits

/* General status register */
#define NRF24L01_RX_DR			6
#define NRF24L01_TX_DS			5
#define NRF24L01_MAX_RT			4
#define NRF24L01_RX_P_NO		1 //3 bits
#define NRF24L01_TX_FULL		0

/* Transmit observe register */
#define NRF24L01_PLOS_CNT		4 //4 bits
#define NRF24L01_ARC_CNT		0 //4 bits

/* FIFO status*/
#define NRF24L01_TX_REUSE		6
#define NRF24L01_FIFO_FULL		5
#define NRF24L01_TX_EMPTY		4
#define NRF24L01_RX_FULL		1
#define NRF24L01_RX_EMPTY		0

//Dynamic length
#define NRF24L01_DPL_P0			0
#define NRF24L01_DPL_P1			1
#define NRF24L01_DPL_P2			2
#define NRF24L01_DPL_P3			3
#define NRF24L01_DPL_P4			4
#define NRF24L01_DPL_P5			5

/* Transmitter power*/
#define NRF24L01_M18DBM			0 //-18 dBm
#define NRF24L01_M12DBM			1 //-12 dBm
#define NRF24L01_M6DBM			2 //-6 dBm
#define NRF24L01_0DBM			3 //0 dBm

/* Data rates */
#define NRF24L01_2MBPS			0
#define NRF24L01_1MBPS			1
#define NRF24L01_250KBPS		2

/* Configuration */
#define NRF24L01_CONFIG			((1 << NRF24L01_EN_CRC) | (0 << NRF24L01_CRCO))

/* Instruction Mnemonics */
#define NRF24L01_REGISTER_MASK				0x1F

#define NRF24L01_READ_REGISTER_MASK(reg)	(0x00 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_WRITE_REGISTER_MASK(reg)	(0x20 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_R_RX_PAYLOAD_MASK			0x61
#define NRF24L01_W_TX_PAYLOAD_MASK			0xA0
#define NRF24L01_FLUSH_TX_MASK				0xE1
#define NRF24L01_FLUSH_RX_MASK				0xE2
#define NRF24L01_REUSE_TX_PL_MASK			0xE3
#define NRF24L01_ACTIVATE_MASK				0x50
#define NRF24L01_R_RX_PL_WID_MASK			0x60
#define NRF24L01_NOP_MASK					0xFF

/* Flush FIFOs */
#define NRF24L01_FLUSH_TX					do { csn(LOW); spi->transfer(NRF24L01_FLUSH_TX_MASK); csn(HIGH); } while (0)
#define NRF24L01_FLUSH_RX					do { csn(LOW); spi->transfer(NRF24L01_FLUSH_RX_MASK); csn(HIGH); } while (0)

#define NRF24L01_TRANSMISSON_OK 			0
#define NRF24L01_MESSAGE_LOST   			1

#define NRF24L01_CHECK_BIT(reg, bit)       (reg & (1 << bit))



uint8_t STM32_nFR24::TM_NRF24L01_Init(uint8_t channel, uint8_t payload_size) {

	uint8_t MyAddress[] = {

	    0x7E,
	    0x7E,
	    0x7E,
	    0x7E,
	    0x7E
	};
	/* Receiver address */
	uint8_t TxAddress[] = {
			 0xE7,
			 0xE7,
			 0xE7,
			 0xE7,
			 0xE7

	};


	/* Initialize SPI */


	/* Max payload is 32bytes */
	if (payload_size > 32) {
		payload_size = 32;
	}

	/* Fill structure */
	STM32_nFR24::TM_NRF24L01_Struct.Channel = !channel; /* Set channel to some different value for TM_NRF24L01_SetChannel() function */
	TM_NRF24L01_Struct.PayloadSize = payload_size;
	TM_NRF24L01_Struct.OutPwr = TM_NRF24L01_OutputPower_0dBm;
	TM_NRF24L01_Struct.DataRate = TM_NRF24L01_DataRate_2M;

	/* Reset nRF24L01+ to power on registers values */
	TM_NRF24L01_SoftwareReset();

	/* Channel select */
	TM_NRF24L01_SetChannel(channel);

	/* Set pipeline to max possible 32 bytes */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, TM_NRF24L01_Struct.PayloadSize); // Auto-ACK pipe
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, TM_NRF24L01_Struct.PayloadSize); // Data payload pipe
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, TM_NRF24L01_Struct.PayloadSize);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, TM_NRF24L01_Struct.PayloadSize);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, TM_NRF24L01_Struct.PayloadSize);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, TM_NRF24L01_Struct.PayloadSize);

	/* Set RF settings (2mbps, output power) */
	TM_NRF24L01_SetRF(TM_NRF24L01_Struct.DataRate, TM_NRF24L01_Struct.OutPwr);

	/* Config register */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG);

	/* Enable auto-acknowledgment for all pipes */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_EN_AA, 0x3F);

	/* Enable RX addresses */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_EN_RXADDR, 0x3F);

	/* Auto retransmit delay: 1000 (4x250) us and Up to 15 retransmit trials */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_SETUP_RETR, 0x4F);

	/* Dynamic length configurations: No dynamic length */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, (0 << NRF24L01_DPL_P0) | (0 << NRF24L01_DPL_P1) | (0 << NRF24L01_DPL_P2) | (0 << NRF24L01_DPL_P3) | (0 << NRF24L01_DPL_P4) | (0 << NRF24L01_DPL_P5));

	/* Clear FIFOs */
	NRF24L01_FLUSH_TX;
	NRF24L01_FLUSH_RX;

	/* Clear interrupts */
	NRF24L01_CLEAR_INTERRUPTS;

	/* Go to RX mode */
	TM_NRF24L01_PowerUpRx();

	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k, TM_NRF24L01_OutputPower_0dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);

	/* Return OK */
	return 1;
}
void STM32_nFR24::TM_NRF24L01_SetMyAddress(uint8_t *adr) {
	ce(LOW);
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, adr, 5);
	ce(HIGH);
}

void STM32_nFR24::TM_NRF24L01_SetTxAddress(uint8_t *adr) {
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, adr, 5);
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, adr, 5);
}

void STM32_nFR24::TM_NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value) {
	uint8_t tmp;
	/* Read register */
	tmp = TM_NRF24L01_ReadRegister(reg);
	/* Make operation */
	if (value) {
		tmp |= 1 << bit;
	} else {
		tmp &= ~(1 << bit);
	}
	/* Write back */
	TM_NRF24L01_WriteRegister(reg, tmp);
}

uint8_t STM32_nFR24::TM_NRF24L01_ReadBit(uint8_t reg, uint8_t bit) {
	uint8_t tmp;
	tmp = TM_NRF24L01_ReadRegister(reg);
	if (!NRF24L01_CHECK_BIT(tmp, bit)) {
		return 0;
	}
	return 1;
}

uint8_t STM32_nFR24::TM_NRF24L01_ReadRegister(uint8_t reg) {
	uint8_t value;
	csn(LOW);
	spi->transfer(NRF24L01_READ_REGISTER_MASK(reg));
	value = spi->transfer(NRF24L01_NOP_MASK);
	csn(HIGH);

	return value;
}

void STM32_nFR24::TM_NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count) {
	csn(LOW);
	spi->transfer(NRF24L01_READ_REGISTER_MASK(reg));
	for (int i = 0; i < count; i++) data[i]= spi->transfer(0xff);
	csn(HIGH);
}

void STM32_nFR24::TM_NRF24L01_WriteRegister(uint8_t reg, uint8_t value) {
	csn(LOW);
	spi->transfer(NRF24L01_WRITE_REGISTER_MASK(reg));
	spi->transfer(value);
	csn(HIGH);
}

void STM32_nFR24::TM_NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count) {
	csn(LOW);
	spi->transfer(NRF24L01_WRITE_REGISTER_MASK(reg));
	for (int i = 0; i < count; i++) spi->transfer(data[i]);
	csn(HIGH);
}

void STM32_nFR24::TM_NRF24L01_PowerUpTx(void) {
	NRF24L01_CLEAR_INTERRUPTS;
	TM_NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | (0 << NRF24L01_PRIM_RX) | (1 << NRF24L01_PWR_UP));
}

void STM32_nFR24::TM_NRF24L01_PowerUpRx(void) {
	/* Disable RX/TX mode */
	ce(LOW);
	/* Clear RX buffer */
	NRF24L01_FLUSH_RX;
	/* Clear interrupts */
	NRF24L01_CLEAR_INTERRUPTS;
	/* Setup RX mode */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | 1 << NRF24L01_PWR_UP | 1 << NRF24L01_PRIM_RX);
	/* Start listening */
	ce(HIGH);
}

void STM32_nFR24::TM_NRF24L01_PowerDown(void) {
	ce(LOW);
	TM_NRF24L01_WriteBit(NRF24L01_REG_CONFIG, NRF24L01_PWR_UP, 0);
}

void STM32_nFR24::TM_NRF24L01_Transmit(uint8_t *data) {
	uint8_t count = TM_NRF24L01_Struct.PayloadSize;

	/* Chip enable put to low, disable it */
	ce(LOW);

	/* Go to power up tx mode */
	TM_NRF24L01_PowerUpTx();

	/* Clear TX FIFO from NRF24L01+ */
	NRF24L01_FLUSH_TX;

	/* Send payload to nRF24L01+ */
	csn(LOW);
	/* Send write payload command */
	spi->transfer(NRF24L01_W_TX_PAYLOAD_MASK);
	/* Fill payload with data*/
	for (int i = 0; i < count; i++) spi->transfer(data[i]);
	/* Disable SPI */
	csn(HIGH);

	/* Send data! */
	ce(HIGH);
}

void STM32_nFR24::TM_NRF24L01_GetData(uint8_t* data) {
	/* Pull down chip select */
	uint64_t i;
	csn(LOW);
	/* Send read payload command*/
	spi->transfer(NRF24L01_R_RX_PAYLOAD_MASK);
	/* Read payload */
	for (i = 0; i<(TM_NRF24L01_Struct.PayloadSize); i++) data[i]= spi->transfer(0xff);
	/* Pull up chip select */
	csn(HIGH);

	/* Reset status register, clear RX_DR interrupt flag */
	TM_NRF24L01_WriteRegister(NRF24L01_REG_STATUS, (1 << NRF24L01_RX_DR));
}

uint8_t STM32_nFR24::TM_NRF24L01_DataReady(void) {
	uint8_t status = TM_NRF24L01_GetStatus();

	if (NRF24L01_CHECK_BIT(status, NRF24L01_RX_DR)) {
		return 1;
	}
	return !TM_NRF24L01_RxFifoEmpty();
}

uint8_t STM32_nFR24::TM_NRF24L01_RxFifoEmpty(void) {
	uint8_t reg = TM_NRF24L01_ReadRegister(NRF24L01_REG_FIFO_STATUS);
	return NRF24L01_CHECK_BIT(reg, NRF24L01_RX_EMPTY);
}

uint8_t STM32_nFR24::TM_NRF24L01_GetStatus(void) {
	uint8_t status;

	csn(LOW);
	/* First received byte is always status register */
	status = spi->transfer(NRF24L01_NOP_MASK);
	/* Pull up chip select */
	csn(HIGH);

	return status;
}

TM_NRF24L01_Transmit_Status_t STM32_nFR24::TM_NRF24L01_GetTransmissionStatus(void) {
	uint8_t status = TM_NRF24L01_GetStatus();
	if (NRF24L01_CHECK_BIT(status, NRF24L01_TX_DS)) {
		/* Successfully sent */
		return TM_NRF24L01_Transmit_Status_Ok;
	} else if (NRF24L01_CHECK_BIT(status, NRF24L01_MAX_RT)) {
		/* Message lost */
		return TM_NRF24L01_Transmit_Status_Lost;
	}

	/* Still sending */
	return TM_NRF24L01_Transmit_Status_Sending;
}

void STM32_nFR24::TM_NRF24L01_SoftwareReset(void) {
	uint8_t data[5];

	TM_NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, 		NRF24L01_REG_DEFAULT_VAL_CONFIG);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_EN_AA,		NRF24L01_REG_DEFAULT_VAL_EN_AA);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_EN_RXADDR, 	NRF24L01_REG_DEFAULT_VAL_EN_RXADDR);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_SETUP_AW, 	NRF24L01_REG_DEFAULT_VAL_SETUP_AW);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_SETUP_RETR, 	NRF24L01_REG_DEFAULT_VAL_SETUP_RETR);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RF_CH, 		NRF24L01_REG_DEFAULT_VAL_RF_CH);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RF_SETUP, 	NRF24L01_REG_DEFAULT_VAL_RF_SETUP);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_STATUS, 		NRF24L01_REG_DEFAULT_VAL_STATUS);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_OBSERVE_TX, 	NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RPD, 		NRF24L01_REG_DEFAULT_VAL_RPD);

	//P0
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4;
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, data, 5);

	//P1
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4;
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, data, 5);

	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5);

	//TX
	data[0] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4;
	TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, data, 5);

	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P0);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P1);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P2);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P3);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P4);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P5);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_FIFO_STATUS, NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, 		NRF24L01_REG_DEFAULT_VAL_DYNPD);
	TM_NRF24L01_WriteRegister(NRF24L01_REG_FEATURE, 	NRF24L01_REG_DEFAULT_VAL_FEATURE);
}

uint8_t STM32_nFR24::TM_NRF24L01_GetRetransmissionsCount(void) {
	/* Low 4 bits */
	return TM_NRF24L01_ReadRegister(NRF24L01_REG_OBSERVE_TX) & 0x0F;
}

void STM32_nFR24::TM_NRF24L01_SetChannel(uint8_t channel) {
	if (channel <= 125 && channel != TM_NRF24L01_Struct.Channel) {
		/* Store new channel setting */
		TM_NRF24L01_Struct.Channel = channel;
		/* Write channel */
		TM_NRF24L01_WriteRegister(NRF24L01_REG_RF_CH, channel);
	}
}

void STM32_nFR24::TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_t DataRate, TM_NRF24L01_OutputPower_t OutPwr) {
	uint8_t tmp = 0;
	TM_NRF24L01_Struct.DataRate = DataRate;
	TM_NRF24L01_Struct.OutPwr = OutPwr;

	if (DataRate == TM_NRF24L01_DataRate_2M) {
		tmp |= 1 << NRF24L01_RF_DR_HIGH;
	} else if (DataRate == TM_NRF24L01_DataRate_250k) {
		tmp |= 1 << NRF24L01_RF_DR_LOW;
	}
	/* If 1Mbps, all bits set to 0 */

	if (OutPwr == TM_NRF24L01_OutputPower_0dBm) {
		tmp |= 3 << NRF24L01_RF_PWR;
	} else if (OutPwr == TM_NRF24L01_OutputPower_M6dBm) {
		tmp |= 2 << NRF24L01_RF_PWR;
	} else if (OutPwr == TM_NRF24L01_OutputPower_M12dBm) {
		tmp |= 1 << NRF24L01_RF_PWR;
	}

	TM_NRF24L01_WriteRegister(NRF24L01_REG_RF_SETUP, tmp);
}


void STM32_nFR24::csn(int mode)
{

  digitalWrite(csn_pin,mode);

}

/****************************************************************************/

void STM32_nFR24::ce(int level)
{
  digitalWrite(ce_pin,level);

}



STM32_nFR24::STM32_nFR24(uint8_t _cepin, uint8_t _cspin)	{
	  ce_pin=_cepin;
	  csn_pin=_cspin;

	  spi = new SPI();


	  __start_timer();
	  // Initialize pins
	  pinMode(ce_pin,OUTPUT);
	  pinMode(csn_pin,OUTPUT);

	  ce(LOW);
	  csn(HIGH);
	  first = NULL;
	  key = QUEUES;
	  if ((msqid = msgget(QUEUES, IPC_CREAT | 0666)) < 0) {
	        perror("ms1gget error");
	         exit(1);
	  }
	  fid = open("sensor_log.txt", O_RDWR | O_CREAT );
	           if (fid<0){
	          	 perror("nRF24_fopen");
	           }

	           AddSensorToLog('X','Y');






}



bool STM32_nFR24::GetEvent (TM_NRF24L01_Event_t* event) {
	 message_buf_t  rbuf;
	if (TM_NRF24L01_DataReady()) {
		sprintf((char *)command, "1tst");

		TM_NRF24L01_GetData(dataIn);


	    sprintf((char *)dataOut, "iam");
	    if(!memcmp(&dataIn,&dataOut,3)){
	    	int i =msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_COMMAND, IPC_NOWAIT);
	    			if ((i > 0)) {
	    				memcpy(command, rbuf.mtext,5);
	    				cout<<"wdl command is sent"<<"\n";

	    		    }
	    	AddSensorToList(dataIn[4], dataIn[3], dataIn[5]);
	    	AddSensorToLog(dataIn[3],dataIn[5]);
	    	//PrintSensorList() ;


	    	rbuf.mtype = MESSAGE_TYPE_EVENT;
	    	memcpy(rbuf.mtext, &dataIn, 5); // ToDo correct lenght

	    	buf_length = sizeof(message_buf_t) - sizeof(long);
	    	if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
	    	        perror("RF_module_thead");
	    	       // exit(1);
	    	}
		//	cout<<"sent message"<<"\n";
			TM_NRF24L01_Transmit((uint8_t*)command);
			do {	transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
			} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
			TM_NRF24L01_PowerUpRx();

        }
	    sprintf((char *)dataOut, "OK");
	    if(!memcmp(&dataIn,&dataOut,2)){
	    	//cout<<"ACK"<<"\n";
	    }
	    return true;

	}
	return false;

}

void STM32_nFR24::AddSensorToList(uint8_t _id, uint8_t _addr, uint8_t _bat)	{

	     TM_NRF24L01_Sensor_t* current = first;
		 while (current !=NULL){
			 TM_NRF24L01_Sensor_t* temp = current;
			 if (temp->addr==_addr)	{
				 UpdateSensorInfo(_addr,_bat);
				return;
			 }
			 current = current->next;

		 }


	    TM_NRF24L01_Sensor_t* new_sensor = new TM_NRF24L01_Sensor_t;
		new_sensor->id = _id;
		new_sensor->addr = _addr;
		new_sensor->next = first;
		first = new_sensor;
		UpdateSensorInfo(_addr,_bat);
}

bool STM32_nFR24::UpdateSensorInfo(uint8_t _addr, uint8_t _bat)	{

	    long int s_time;
	    struct tm m_time;
	    s_time = time (NULL);
	    localtime_r (&s_time, &m_time);

	    TM_NRF24L01_Sensor_t* current = first;
	    TM_NRF24L01_Sensor_t* next;
		if(current==NULL){
			return false;
		}
		while (current){
			if(current->addr == _addr){
				current->last_time.date = (m_time.tm_mday);
				current->last_time.hour = (m_time.tm_hour);
				current->last_time.min  = (m_time.tm_min);
				current->bat_status = _bat;
			}
			current = current->next;



		}
		return true;
}
void STM32_nFR24::ClearSensorList()	{
	 TM_NRF24L01_Sensor_t* current = first;
	 while (current !=NULL){
		 TM_NRF24L01_Sensor_t* temp = current;
		 current = current->next;
		 delete temp;

	 }

}
void STM32_nFR24::PrintSensorList()	{
	 char buf[16];
	 TM_NRF24L01_Sensor_t* current = first;
	 while (current !=NULL){
		 TM_NRF24L01_Sensor_t* temp = current;
		 current = current->next;
		 cout<<"Sensor_ID="<<(temp->id)<<" ADDR="<<(temp->addr)<<" BAT="<<(temp->bat_status);
		 cout<<" Time ";
		 char x = temp->last_time.hour;
		 sprintf(buf,"%d",x);
		 cout<<buf<<":";
		 x = temp->last_time.min;
		 sprintf(buf,"%d",x);
 		 cout<<buf<<"\n";

	 }

}


STM32_nFR24::~STM32_nFR24() {
	close (fid);
	// TODO Auto-generated destructor stub
}

void STM32_nFR24::AddSensorToLog(char _id, char _bat )	{

     char buff[64];
	 long int s_time;
	 struct tm m_time;
     s_time = time (NULL);
     localtime_r (&s_time, &m_time);


     sprintf(buff,"Sensor-");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",_id);
     write(fid, buff, strlen(buff));
     sprintf(buff,"Battery-");
     write(fid, buff, strlen(buff));

     write(fid, &_bat, 1);
     sprintf(buff," Time-");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char*)m_time.tm_mday);
     write(fid, buff, strlen(buff));
     sprintf(buff,".");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char*)m_time.tm_hour);
     write(fid, buff, strlen(buff));
     sprintf(buff,":");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char*)m_time.tm_min);
     write(fid, buff, strlen(buff));
     sprintf(buff,"\n");
     write(fid, buff, strlen(buff));



}


