#ifndef LIBJVSERVER_H
#define LIBJVSERVER_H

//云视通头文件中间层。
//目的是：直接把云视通库里的头文件拷贝过来就能用


//V2.0版本的移植过程
//1.把云视通库里的JVNSDKDef.h和JvServer.h拷贝过来，改名为JVNSDKDef-V2.h和JvServer-V2.h
//2.编译，把报错的“引用”改为“指针”
#if YST_2_0

/*在头文件里已经定义了
#include <sys/socket.h>
typedef struct _GUID 
{ 
unsigned long   Data1; 
unsigned short  Data2; 
unsigned short  Data3; 
unsigned char   Data4[8]; 
}GUID;
*/

#define BYTE unsigned char
typedef struct sockaddr SOCKADDR;
#define JVSERVER_API

//#define MAX_PATH 255	//这个值是跟王敏问的

#include "JvServer-V2.h"

//V1.0版本的云视通不再更新了，头文件也就不改了
#else
#include "JVNSDKDef.h"
#endif

#endif
