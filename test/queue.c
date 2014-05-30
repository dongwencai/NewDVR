#define __IN
#define __OUT
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>
#include "semaphore.h"
typedef unsigned int __u32;
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
}MSG;
/*
typedef struct{
	funcptr readmsg;
	funcptr insertmsg;
	funcptr delmsg;
	funcptr translatemsg;
}MSGOPT;
*/

typedef struct{	
	pthread_mutex_t msglock;
	sem_t  msgwait;
	int queueid;
	MSG	*msg;
	int size;
	int readmsg_ops;
	int writemsg_ops;
//	MSGOPT opt;
	__u32 reserve;
}MSGQUEUE;
list *MsgQueuehead=NULL;
static sem_t g_opslock;

QUEUE_STATUS InitQueue();
QUEUE_STATUS CreateMsgQueue(__OUT int *queueid,int msgsize);
QUEUE_STATUS ReleaseMsgQueue(int queueid);
QUEUE_STATUS SendMsg(int queueid,MSG msg);
QUEUE_STATUS RecvMsg(int queueid,__OUT MSG *msg);
QUEUE_STATUS ReleaseAllMsgQueue();

static list *LookUpMsgQueue(int queueid);
static QUEUE_STATUS	DelMsgQueueFromList(__IN list *link);
static int AllocQueueId();
static QUEUE_STATUS InsertMsgQueue2List(MSGQUEUE *node);
static void FreeMsgQueue(__IN MSGQUEUE *msgQueue);

QUEUE_STATUS CreateMsgQueue(__OUT int *queueid,int msgsize)
{
	MSGQUEUE *newQueue=NULL;
	pthread_mutex_lock(&g_opslock);
	if(msgsize<=0)		goto error;
	newQueue=(MSGQUEUE *)malloc(sizeof(MSGQUEUE));
	if(!newQueue)
	{
		goto error;
	}
	memset(newQueue,0,sizeof(MSGQUEUE));
	newQueue->size=msgsize;
	newQueue->msg=(MSG *)malloc(sizeof(MSG)*msgsize);
	if(!newQueue->msg)
	{
		goto error;
	}
	if(InsertMsgQueue2List(newQueue)==QUE_SUC)
	{

	    newQueue->queueid=AllocQueueId();
		pthread_mutex_init (&newQueue->msglock, NULL);
		sem_init (&newQueue->msgwait, 0, 0); 
		*queueid=newQueue->queueid;
		pthread_mutex_unlock(&g_opslock);
		return QUE_SUC;
	}
error:
	printf("Queue is created failt!\n");
	FreeMsgQueue(newQueue);
	pthread_mutex_unlock(&g_opslock);
	return QUE_FAILT;
}

QUEUE_STATUS ReleaseMsgQueue(int QueueId)
{
	list *link;
	MSGQUEUE *msgQueue=NULL;
	pthread_mutex_lock(&g_opslock);
	link=LookUpMsgQueue(QueueId);
	DelMsgQueueFromList(link);
	if(!link)
	{
		return QUE_NOTFOUND;
	}
	msgQueue=(MSGQUEUE *)link->data;
	sem_destroy(&msgQueue->msgwait);
	pthread_mutex_destroy(&msgQueue->msglock);
	FreeMsgQueue(msgQueue);
	link->data=NULL;
	pthread_mutex_unlock(&g_opslock);
	return QUE_SUC;
}

QUEUE_STATUS ReleaseAllMsgQueue()
{
	while(MsgQueuehead)
	{
		return QUE_SUC;
	}
}
static void FreeMsgQueue(__IN MSGQUEUE *msgQueue)
{
	if(msgQueue)
	{
		if(msgQueue->msg)
		{
			free(msgQueue->msg);
			msgQueue->msg=NULL;
		}
		free(msgQueue);
		msgQueue=NULL;
	}
}

static QUEUE_STATUS	DelMsgQueueFromList(__IN list *link)
{
	list *temp=MsgQueuehead,*prevlink=NULL;
	while(temp)
	{
		if(temp==link)
		{
			break;
		}
		prevlink=temp;
		temp=temp->next;
	}
	if(prevlink)
	{
		free(prevlink);
		prevlink->next=link->next;
		link->next=NULL;
	}
	else
	{
		/* Only one node or no node*/
		free(MsgQueuehead);
		MsgQueuehead=NULL;
	}
	return QUE_SUC;
}

static list *LookUpMsgQueue(int queueid)
{
	list *temp=MsgQueuehead;
	while(temp)
	{
		MSGQUEUE *msgQueue=(MSGQUEUE *)temp->data;
		if(msgQueue&&msgQueue->queueid==queueid)
		{
			return temp;
		}
	}
	return NULL;
}

static QUEUE_STATUS InsertMsgQueue2List(MSGQUEUE *node)
{
	list *newnode=(list *)malloc(sizeof(list));
	if(!newnode)
	{
		return QUE_FAILT;
	}
	newnode->data=(void *)node;
	if(MsgQueuehead)
	{
		newnode->next=MsgQueuehead;
		MsgQueuehead=newnode;
	}
	else
	{
		MsgQueuehead=newnode;
		newnode->next=NULL;
	}
	return QUE_SUC;
}

static int AllocQueueId()
{
	list *temp=MsgQueuehead;
	int ret=rand();
	while(temp)
	{
		MSGQUEUE *msgQueue=(MSGQUEUE *)temp->data;
		if(msgQueue)
		{
			if(msgQueue->queueid==ret)
			{
				temp=MsgQueuehead;
				ret=rand();
				continue;
			}
		}
        temp=temp->next;
	}
	return ret;
}

QUEUE_STATUS SendMsg(int queueid,MSG msg)
{
	list *link=NULL;
	MSGQUEUE *msgQueue=NULL;
	link=LookUpMsgQueue(queueid);
	if(!link)
	{
		return QUE_NOTFOUND;
	}
	msgQueue=(MSGQUEUE *)link->data;
	pthread_mutex_lock(&msgQueue->msglock);
	if((msgQueue->writemsg_ops+1)%msgQueue->size==msgQueue->readmsg_ops)
	{
		pthread_mutex_unlock(&msgQueue->msglock);
		return QUE_MSGFULL;
	}
	msgQueue->msg[msgQueue->writemsg_ops]=msg;
	msgQueue->writemsg_ops++;
	if(msgQueue->writemsg_ops>=msgQueue->size)
	{
		msgQueue->writemsg_ops=0;
	}
	pthread_mutex_unlock(&msgQueue->msglock);
	sem_post (&msgQueue->msgwait); 
	return QUE_SUC;
}

QUEUE_STATUS RecvMsg(int queueid, __OUT MSG * msg)
{
	list *link=NULL;
	link=LookUpMsgQueue(queueid);
    if(!link)   return QUE_NOTFOUND;
	MSGQUEUE *pstmsgQueue=(MSGQUEUE *)link->data;
	if(sem_trywait(&pstmsgQueue->msgwait))
        return QUE_MSGEMPTY;
	pthread_mutex_lock (&pstmsgQueue->msglock); 
	if (pstmsgQueue->readmsg_ops != pstmsgQueue->writemsg_ops) 
	{ 
	    memcpy(msg,pstmsgQueue->msg + pstmsgQueue->readmsg_ops,sizeof(MSG)); 
	    pstmsgQueue->readmsg_ops ++; 
	    if (pstmsgQueue->readmsg_ops >= pstmsgQueue->size) 
	        pstmsgQueue->readmsg_ops = 0; 
	}
 	else
	{
		pthread_mutex_unlock (&pstmsgQueue->msglock);
        msg=NULL;
		return QUE_MSGEMPTY;
	}
	pthread_mutex_unlock (&pstmsgQueue->msglock);
    return QUE_SUC; 
} 
QUEUE_STATUS InitQueue()
{
	pthread_mutex_init(&g_opslock,NULL);
	return QUE_SUC;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void *product(void *param)
{
    int queueid=*(int *)param,i=1;
    MSG msg;
    while(1)
    {
        int ret=0;
        msg.message=rand();
        msg.param=(void *)malloc(sizeof(pthread_t));
        *(pthread_t *)msg.param=pthread_self();
        if((ret=SendMsg(queueid,msg))!=QUE_SUC)
        {

            if(ret=QUE_MSGFULL)
            {
                usleep(100);
            }
            free(msg.param);
        }
        msg.message=0;
        msg.param=NULL;
        i--;
    }
}
void *customer(void *param)
{
    int queueid=*(int *)param,ret=0;
    MSG pmsg;
    memset(&pmsg,0,sizeof(MSG));
    while(1)
    {
        if((ret=RecvMsg(queueid,&pmsg))==QUE_SUC)
        {
            printf("customer:%u product id:%u SendMsg :%d\n",pthread_self(),*(pthread_t *)pmsg.param,pmsg.message);
            pmsg.message=0;
            free(pmsg.param);
            pmsg.param=NULL;
        }
        else if(ret==QUE_MSGEMPTY)
        {
            usleep(100);
        }
    }
}
int main()
{
    pthread_t thread1,thread2,thread3,thread4,thread5;
    int queueid;
    InitQueue();
    if(CreateMsgQueue(&queueid,10)==QUE_SUC)
    {
        printf("Queue created success!\n");
    }
    else
    {
        printf("Queue created failt!\n");
        exit(1);

    }
    pthread_create(&thread1,NULL,product,&queueid);
    pthread_create(&thread2,NULL,product,&queueid);
    pthread_create(&thread3,NULL,product,&queueid);
    pthread_create(&thread4,NULL,customer,&queueid);
    pthread_create(&thread5,NULL,customer,&queueid);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    pthread_join(thread5,NULL);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
