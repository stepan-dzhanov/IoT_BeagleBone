/*
 * protocol.h
 *
 *  Created on: Apr 2, 2016
 *      Author: stepan
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define QUEUES 10
#define QUEUES_MESSAGE_SIZE 32
typedef struct mymsgbuf {
         long    mtype;
       //  char    id;
       //  char    rf_addr;
         char    mtext[QUEUES_MESSAGE_SIZE];
} message_buf_t;

#define MESSAGE_TYPE_EVENT 1
#define MESSAGE_TYPE_COMMAND 2




#endif /* PROTOCOL_H_ */
