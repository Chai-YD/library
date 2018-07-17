//图书馆系统开发
#include<stdio.h>
#include"main.h"
FILE* g_logfile;
//在程序开始时初始化或配置各种变量和环境
void configure(void){
	g_logfile = fopen("log","w");
	if(NULL == g_logfile){
		printf("error:can't open log file log");
		exit(-1);
	}
}

//负责程序结束后的处理工作
void clear(void){
	//此处还可以添加其他清除操作
	fclose(g_logfile);
}

//主函数
int main(void){
	configure();
	menu_main();
	//在此处可以增加其他操作
	clear();
}


