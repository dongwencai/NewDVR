#ifndef __SW_QUEUE_H__
#define __SW_QUEUE_H__

#include "semaphore.h"
#ifndef __IN
#define __IN
#define __OUT
#endif

typedef enum{
	QUE_SUC,
	QUE_FAILT,
	QUE_NOTFOUND,
	QUE_MSGFULL,
	QUE_MSGEMPTY,
}QUEUE_STATUS;

typedef struct {
	void *data;
	void *next;
}list;


		
typedef struct{
	int message;
	void *param;
}MSG,*pMSG;

typedef struct{	
	pthread_mutex_t msglock;
	sem_t  msgwait;
	int queueid;
	MSG	*msg;
	int size;
	int readmsg_ops;
	int writemsg_ops;
	unsigned long reserve;
}MSGQUEUE;

QUEUE_STATUS InitQueue();
QUEUE_STATUS CreateMsgQueue(__OUT int *queueid,int msgsize);
QUEUE_STATUS ReleaseMsgQueue(int queueid);
QUEUE_STATUS SendMsg(int queueid,MSG msg);
QUEUE_STATUS RecvMsg(int queueid,__OUT MSG *msg);
QUEUE_STATUS ReleaseAllMsgQueue();

#endif