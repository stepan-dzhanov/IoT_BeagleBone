/*
 * Scenario.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: stepan
 */

#include "Scenario.h"
#include "protocol.h"

Scenario::Scenario() {

	 key = QUEUES;
     if ((msqid = msgget(QUEUES, IPC_CREAT | 0666)) < 0) {
		   perror("Scenario_msgget");
		   exit(1);
	 }
     fid = open("scenario_log.txt", O_RDWR | O_CREAT );
          if (fid<0){
         	 perror("Scenario_fopen");
          }
          char str[32];
              sprintf(str,"----Start scenario----");

              Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", "Reboot system");
              Sendmail("natalidvoreckaya@mail.ru", "djanov@gmail.com", "От умного дома", "Я начал работать. Буду информировть тебя о событиях");
              AddEventToLog(str);

}

Scenario::~Scenario() {
	 close(fid);
	// TODO Auto-generated destructor stub
}

int Scenario::GetHumidity()	{
	thingspeak_data = new ThingSpeakClient ;
	return thingspeak_data->GetField("<field5>","</field5>");
	delete (thingspeak_data);
}

void Scenario::WaterDelivery()	{
	static char w_flg=0;
	int hum;
	message_buf_t  rbuf;
	rbuf.mtype = 2;
    sprintf(rbuf.mtext, "1wdl");
    char str[32];
    sprintf(str,"Water delivery");


	if ((hum = GetHumidity())<0)	{
		std::cout<<"Scenario_ NO internet connection"<<"\n";
	}
	else {
		if ((hum<36)&&(w_flg ==0)){
			w_flg=1;
			rbuf.mtype = MESSAGE_TYPE_COMMAND;
			sprintf((char *)rbuf.mtext, "1wdl20");
			buf_length = sizeof(message_buf_t) - sizeof(long);
			if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
					perror("Scenario_msgsnd");
					exit(1);
			}
			std::cout<<"WDL MESSAGE is sent"<<"\n";

			Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", "Water delivery done");
			Sendmail("natalidvoreckaya@mail.ru", "djanov@gmail.com", "От умного дома", "Я полил цветок");
			AddEventToLog(str);

		}
		if (hum>42) w_flg =0;
	}
}


void Scenario::AddEventToLog(char *event )	{

     char buff[64];
	 long int s_time;
	 struct tm m_time;
     s_time = time (NULL);
     localtime_r (&s_time, &m_time);


     sprintf(buff,"EVENT-");
     write(fid, buff, strlen(buff));
     sprintf(buff,event);
     write(fid, buff, strlen(buff));
     sprintf(buff," Time-");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%i",(char*)m_time.tm_mday);
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
int Scenario::Sendmail(const char *to, const char *from, const char *subject, const char *message)
{
    int retval = -1;
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe != NULL) {
        fprintf(mailpipe, "To: %s\n", to);
        fprintf(mailpipe, "From: %s\n", from);
        fprintf(mailpipe, "Subject: %s\n\n", subject);
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
        retval = 0;
     }
     else {
         perror("Failed to invoke sendmail");
     }
     return retval;
}

