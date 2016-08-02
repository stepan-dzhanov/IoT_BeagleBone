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
	 LoadCoockingSchedule();
     if ((msqid = msgget(QUEUES, IPC_CREAT | 0666)) < 0) {
		   perror("Scenario_msgget");
		   exit(1);
	 }
     fid = open("scenario_log.txt", O_RDWR | O_CREAT | O_APPEND );
          if (fid<0){
         	 perror("Scenario_fopen");
          }
          char str[32];
              sprintf(str,"----Start scenario----");

              Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", "Reboot system");
              Sendmail("natalidvoreckaya@mail.ru", "djanov@gmail.com", "От умного дома", "Я начал работать. Буду информировть тебя о событиях");
              AddEventToLog(str);
     step =0;

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

	static int last_day_water = 0;
    long int s_time;
    struct tm m_time;
    s_time = time (NULL);
    localtime_r (&s_time, &m_time);


    sprintf(rbuf.mtext, "%cwdl\n",0x01);
    char str[32], str1[32];
    sprintf(str,"Water delivery");


	if ((hum = GetHumidity())<0)	{
		std::cout<<"Scenario_ NO internet connection"<<"\n";
	}
	else {
		if ((hum<19)&&(w_flg ==0)){
			if (hum<15) return;
			if ((m_time.tm_mday - last_day_water)<2) return;
			last_day_water = m_time.tm_mday;
			w_flg=1;
			rbuf.mtype = MESSAGE_TYPE_COMMAND_1;
			sprintf((char *)rbuf.mtext, "1wdl20");
			buf_length = sizeof(message_buf_t) - sizeof(long);
			if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
					perror("Scenario_msgsnd");

			}
			std::cout<<"WDL MESSAGE is sent"<<"\n";

			Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", "Water delivery done");
			Sendmail("natalidvoreckaya@mail.ru", "djanov@gmail.com", "От умного дома", "Я полил цветок");
			AddEventToLog(str);
			memset(str,0,strlen(str));
			sprintf(str,"Humidity=");
			sprintf(str1,"%d",hum);
			memcpy(&str[strlen(str)],str1, strlen(str1));
			AddEventToLog(str);


		}
		if (hum>42) {
			if (hum>99) return;
			//memset(str,0,strlen(str));
		   // sprintf(str,"w_flag reset");
		   // AddEventToLog(str);
		   // memset(str,0,strlen(str));
		   // sprintf(str,"Hum=");
		    //sprintf(str1,"%d",hum);
		    //memcpy(&str[strlen(str)],str1, strlen(str1));
		   // AddEventToLog(str);




			w_flg =0;
		}
	}
}

void Scenario::UpdateCoockingSchedule(char *str){
	char  st[32];
	coock_sched[0].day =str[0]&0x0F;
	coock_sched[0].hour=10*(str[1]&0x0F)+1*(str[2]&0x0F);
	coock_sched[0].min=10*(str[3]&0x0F)+1*(str[4]&0x0F);
	coock_sched[0].timeout=10*(str[5]&0x0F)+1*(str[6]&0x0F);
    coock_sched[0].end=str[7]&0x0F;
    step = 0;

    sprintf(st,"Update coock schedule\n");
    AddEventToLog(st);
}

void Scenario::LoadCoockingSchedule()	{

	coock_sched[0].day =1;
	coock_sched[0].hour=15;
	coock_sched[0].min=30;
    coock_sched[0].timeout=10;
	coock_sched[0].end=1;

	coock_sched[1].day =2;
	coock_sched[1].hour=14;
	coock_sched[1].min=39;
	coock_sched[1].timeout=10;
	coock_sched[1].end=1;

	coock_sched[2].day =3;
	coock_sched[2].hour=14;
	coock_sched[2].min=38;
	coock_sched[2].timeout=10;
	coock_sched[2].end=1;

	coock_sched[3].day =9;
	coock_sched[3].hour=13;
	coock_sched[3].min=35;
	coock_sched[3].timeout=10;
	coock_sched[3].end=0;


}






void Scenario::Coocking(char mode)	{
#define DAY 8
#define HOUR 13
#define MIN  7

    message_buf_t  rbuf;
    static char on_mode=0;
    static char off_mode=0;
    static char coock_mode =0;

    long int s_time;
   	struct tm m_time;

    s_time = time (NULL);
    localtime_r (&s_time, &m_time);

    if ( (!mode)&&(!off_mode)){
    	rbuf.mtype = MESSAGE_TYPE_COMMAND_3;
    	sprintf(rbuf.mtext, "%crld\n",0x03);
        buf_length = sizeof(message_buf_t) - sizeof(long);
        if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
    		perror("Scenario_msgsnd");
        }
        off_mode = 1;
        on_mode = 0;
    }

    if ( (mode)&&(!on_mode)){
        rbuf.mtype = MESSAGE_TYPE_COMMAND_3;
        sprintf(rbuf.mtext, "%csld\n",0x03);
        buf_length = sizeof(message_buf_t) - sizeof(long);
        if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
        	perror("Scenario_msgsnd");
        }
        on_mode = 1;
        off_mode = 0;
    }
    if (!mode) return;



    if ( (m_time.tm_wday==coock_sched[step].day)&&(m_time.tm_hour==coock_sched[step].hour)&&( abs(m_time.tm_min-coock_sched[step].min)<10 )&&(m_time.tm_min>=coock_sched[step].min)   ){

    	rbuf.mtype = MESSAGE_TYPE_COMMAND_3;
        sprintf(rbuf.mtext, "%cwdl%d",0x03,coock_sched[step].timeout);//,coock_sched[step].timeout);
        buf_length = sizeof(message_buf_t) - sizeof(long);
    	if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
          	perror("Scenario_msgsnd");
    	}
    	step++;
    	if (!coock_sched[step].end) step =0;
    	char str[32];
    	sprintf(str,"Coocking start");
    	AddEventToLog(str);
    	std::cout<<"Coocking\n";

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
int Scenario::Sendmail(const char *to, const char *from, const char *subject,  char *message)
{
	 return 0;
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

void Scenario::EventProcessing (char *event)	{
	 char str_bat[] = "bat";
	 char str_dor[] = "dor";
	 char message [32];
	 sprintf (message,"Low battery in sensor ");
	 message[22] |=event[3];
	 message[23] = 0;


	if ( !memcmp(&event[5],&str_bat,3) )	{
		Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", message);
	}
	if ( !memcmp(&event[5],&str_dor,3) )	{
			Sendmail("unell@ukr.net", "djanov@gmail.com", "StepanSmartHouseNotification", "Door sensor");
			thingspeak_data = new ThingSpeakClient ;
		    thingspeak_data->PutDataToChannel(50,'1');
		    delete (thingspeak_data);
		    sprintf(message,"Door sensor");
		    AddEventToLog(message);
	}


}

void Scenario::Report()	{

	     static int last_day_report = 0;
	     long int s_time;
		 struct tm m_time;
	     s_time = time (NULL);
	     localtime_r (&s_time, &m_time);
	     if ((abs(last_day_report - m_time.tm_mday)>1)&&(m_time.tm_hour==10)&&(m_time.tm_min==0))	{
              ;
	     }



}

