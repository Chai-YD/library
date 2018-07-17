#ifndef __MAIN_H__
#define __MAIN_H__
#include "menu.h"
#include<stdlib.h>
#include<string.h>
#define LEN_BUFFER 50
#define LEN_FILE_NAME 20
#define LEN_BOOK_NAME 20
#define LEN_AUTHOR_NAME 10
#define LEN_PUBLISHER_NAME 20
#define len 100


#ifdef LOG_ENABLE
#define LOG_ERROR(msg)\
	log_error(__LINE__,__FILE__,msg)
#define LOG_ACTIVITY(msg)\
	log_activity(__TIME__,__FILE__,msg)
#else
#define LOG_ERROR(msg)\
{}
#define LOG_ACTIVITY(msg)\
{}
#endif
//时间信息
typedef struct{
	unsigned year:16;
	unsigned month:8;
	unsigned day:8;
}TimeInfor;

//图书信息
typedef struct{
	char name[LEN_BOOK_NAME];
	char author[LEN_AUTHOR_NAME];
	char publisher[LEN_PUBLISHER_NAME];
	int serial;  //编号
	int page;
	TimeInfor time_pub;
	TimeInfor time_in;
}BookInfor;
#endif
