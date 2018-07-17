#include<stdio.h>
#include"main.h"
//全局变量
int g_count = 0;
BookInfor* g_books[len];
int g_serial = 1;
//选择函数
int menu_choice(const int max){
	int c = 0;
	scanf("%d",&c);
	while(c<0&&c>max){
		printf("input an integer(0~%d):",max);
		scanf("%d",&c);
	}
	return c;
}
//set_time_infor函数
void set_time_infor(TimeInfor* time,
		const int year,
		const int month,
		const int day){
	time->year = year;
	time->month = month;
	time->day = day;
}
//compare_time 函数
int compare_time(TimeInfor* time_a,TimeInfor* time_b){
	if(time_a->year!=time_b->year){
		return time_a->year-time_b->year;
	}else if(time_a->month!=time_b->month){
		return time_a->month-time_b->month;
	}else{
		return time_a->day-time_b->day;
	}
}
//输出关于时间的信息
void print_time_infor(TimeInfor* time){
	printf("%04d:%02d:%02d",time->year,time->month,time->day);
}
//处理书籍函数
//set_book_infor函数
void set_book_infor(BookInfor* book,
		const char*name,
		const char* author,
		const char* publisher,
		const int serial,
		const int page,
		const TimeInfor* time_pub,
		const TimeInfor* time_in){
	strncpy(book->name,name,strlen(name));
	strncpy(book->author,author,strlen(author));
	strncpy(book->publisher,publisher, strlen(publisher));
	book->serial = serial;
	book->page = page;
	set_time_infor((&book->time_pub),time_pub->year,time_pub->month,time_pub->day);
	set_time_infor((&book->time_in),time_in->year,time_in->month,time_in->day);
}
//书籍数据的拷贝
void copy_book_infor(BookInfor* book_dst,BookInfor* book_src){
	set_book_infor(book_dst,
			book_src->name,
			book_src->author,
			book_src->publisher,
			book_src->serial,
			book_src->page,
			&book_src->time_pub,
			&book_src->time_in);
}
//输出书籍信息
void print_book_infor(BookInfor*book){
	printf("%4d:%s,%s,%s,%d pages, published on",book->serial,book->name,book->author,book->publisher,book->page);
	print_time_infor(&book->time_pub);
	printf("\n");
}
//增加书籍
extern int g_serial;
void book_add(const char* name,
		const char* author,
		const char* publisher,
		const int page,
		const TimeInfor * time_pub,
		const TimeInfor * time_in){
	BookInfor* unit = (BookInfor *)malloc(sizeof(BookInfor));
	memset(unit,0,sizeof(BookInfor));
	set_book_infor(unit,name,author,publisher,g_serial++,page,time_pub,time_in);
	g_books[g_count++]= unit;
}

//删除书籍
void book_delete_by_name(){
	;
}
void book_delete_by_pub_time(TimeInfor* time){
	int i=0;
	int j=0;
	while(i<len){
		if(compare_time(&(g_books[i]->time_pub),time)<=0){
			free(&g_books[i]);
			--g_count;
			++i;
			continue;
		}
		g_books[j++] = g_books[i];//复制指针
		g_books[i++] = NULL;       //将无用指针置为空
	}
    printf("删除书籍\n");
}
//查询书籍
void book_search_by_name(const char*name){
	int i =0;
	for(i=0;i<g_count;++i){
		if(NULL!=strstr(g_books[i]->name,name))
			print_book_infor(g_books[i]);
	}
}
void search_book_by_time(TimeInfor* time){
	int i = 0;
	for(i=0;i<g_count;i++){
		if(compare_time(&g_books[i]->time_pub,time)>=0)
			print_book_infor(g_books[i]);
	}
}
//导入书籍数据
void book_reload(char* filename){
	char name[LEN_BOOK_NAME];
	char author[LEN_AUTHOR_NAME];
	char publisher[LEN_PUBLISHER_NAME];
	int page;
	int year_p,month_p,day_p;
	int year_i,month_i,day_i;
	TimeInfor data_publishing;
	TimeInfor data_input;
	FILE* in = fopen(filename,"r");
	int i = 0;
	if(NULL == in){
		printf("can't open file %s when reload books.\n",filename);
		LOG_ACTIVITY("can't open file when reload.");
		return;
	}
	while(EOF!=fscanf(in,"%s%s%s%d%d-%d-%d%d-%d-%d",name,author,publisher,&page,&year_p,&month_p,&day_p,&year_i,&month_i,&day_i)){
		set_time_infor(&data_publishing,year_p,month_p,day_p);
		set_time_infor(&data_input,year_i,month_i,day_i);
		book_add(name,author,publisher,page,&data_publishing,&data_input);//增加书籍
	}
	fclose(in);
}
extern FILE* g_logfile;
//增加书籍菜单
void menu_add(void){
	//定义临时变量
	char name[LEN_BOOK_NAME] = "\0";
	char author[LEN_AUTHOR_NAME] = "\0";
	char publisher[LEN_PUBLISHER_NAME] = "\0";
	int serial;  //编号
	int page;
	int year_p,month_p,day_p;
	int year_i,month_i,day_i;
	TimeInfor time_pub;
	TimeInfor time_in;

	printf("please input book information:\n");
	printf("name:");
	getchar();   //消除一个换行
	gets(name);
	printf("author:");
	getchar();
    fflush(stdout);
	gets(author);
	printf("publisher:");
	getchar();
	gets(publisher);
	printf("page:");
	scanf("%d",&page);
	printf("publishing data:");
	scanf("%d-%d-%d",&year_p,&month_p,&day_p);
	set_time_infor(&time_pub,year_p,month_p,day_p);//设置出版日期
	printf("Input data:");
	scanf("%d-%d-%d",&year_i,&month_i,&day_i);
	set_time_infor(&time_in,year_i,month_i,day_i);//设置入库日期
	book_add(name,author,publisher,page,&time_pub,&time_in);//添加书籍
	LOG_ACTIVITY("success to add a book.");//记录日志
	menu_main();
}


void menu_delete(void){
	int choice = 0;
	char tmp[LEN_BUFFER] = {0};
	int year,month,day;
	TimeInfor tmp_time;
	printf("\n************************************************\n");
	printf("\n***** 1 Delete book by name                   **\n");
	printf("\n***** 2 Delete book by author                 **\n");
	printf("\n***** 3 Delete book by publisher              **\n");
	printf("\n***** 4 Delete book by published before a data**\n");
	printf("\n***** 5 Delete book imported before a data    **\n");
	printf("\n***** 0 go back                               **\n");
	printf("\n***** please input your choice(0~5) ************\n");
	printf("\n************************************************\n");
	choice = menu_choice(5);
	switch(choice){
		case 1:
			printf("Input the name:");
			scanf("%s",tmp);
			book_delete_by_name(tmp);
			break;
	//	case 2:
	//		printf("Input the author:");
	//		scanf("%s",tmp);
	//		book_delete_by_author(tmp);
	//		break;
	//	case 3:
	//		printf("Input the publisher:");
	//		scanf("%s",tmp);
	//		book_delete_by_publisher(tmp);
	//		break;
		case 4:
			printf("Input the data(yyyy-mm-dd):");
			scanf("%d-%d-%d",&year,&month,&day);
			set_time_infor(&tmp_time,year,month,day);
			book_delete_by_pub_time(&tmp_time);
			break;
		case 5:
			printf("Input the data(yyyy-mm-dd):");
			scanf("%d-%d-%d",&year,&month,&day);
			set_time_infor(&tmp_time,year,month,day);
			book_delete_by_pub_time(&tmp_time);
		case 0:
			menu_main();
			break;
		default:
	//		menu_error();
			break;
	}
	LOG_ACTIVITY("Success to delete books.");
	menu_delete();
}

void menu_search(void){
	int choice = 0;
	char tmp[LEN_BUFFER] = {0};
	int year,month,day;
	TimeInfor tmp_time;
	printf("\n************************************************\n");
	printf("\n***** 1 search book by name                   **\n");
	printf("\n***** 2 search book by author                 **\n");
	printf("\n***** 3 search book by publisher              **\n");
	printf("\n***** 4 search book by published before a data**\n");
	printf("\n***** 5 search book imported before a data    **\n");
	printf("\n***** 0 go back                               **\n");
	printf("\n***** please input your choice(0~5) ************\n");
	printf("\n************************************************\n");
	choice = menu_choice(5);
	switch(choice){
		case 1:
			printf("Input the name:");
			scanf("%s",tmp);
			book_search_by_name(tmp);
			break;
	//	case 2:
	//		printf("Input the author:");
	//		scanf("%s",tmp);
	//		book_search_by_author(tmp);
	//		break;
	//	case 3:
	//		printf("Input the publisher:");
	//		scanf("%s",tmp);
	//		book_search_by_publisher(tmp);
	//		break;
	//	case 4:
	//		printf("Input the data(yyyy-mm-dd):");
	//		scanf("%d-%d-%d",&year,&month,&day);
	//		set_time_infor(&tmp_time,year,month,day);
	//		book_search_by_time(&tmp_time);
	//		break;
	//	case 5:
	//		printf("Input the data(yyyy-mm-dd):");
	//		scanf("%d-%d-%d",&year,&month,&day);
	//		set_time_infor(&tmp_time,year,month,day);
	//		book_search_by_time(&tmp_time);
		case 0:
			menu_main();
			break;
		default:
	//		menu_error();
			break;
	}
	LOG_ACTIVITY("Success to search books.");
	menu_search();

}

//设计导入图书菜单
void menu_reload(void){
	char file_name[LEN_FILE_NAME] = "\0";
	printf("reload which file:");
	scanf("%s",file_name);
	book_reload(file_name);
	LOG_ACTIVITY("End to reload books.");
	menu_main();
}
//记录历史错误
void log_error(const int line,char * file_name,char *mes){
	printf("<%s,%s:%d>|SYSTEM_ERROR|%s\n",\
			__TIME__,file_name,line,mes);
	fprintf(g_logfile,"<%s,%s:%d>|SYSTEM_ERROR|%s\n",\
			__TIME__,file_name,line,mes);
}
//记录历史行为
void log_activity(char* mes){
	printf("<%s>|ACTION|%s\n",__TIME__,mes);
	fprintf(g_logfile,"<%s>|ACTION|%s\n",__TIME__,mes);
}

//推出系统
void menu_exit(void){
	LOG_ACTIVITY("Quit the book management system.");
	exit(1);
}



//设计主界面的主函数
void menu_main(){
	int choice = 0;
	printf("\n***************************************\n");
	printf("\n****** my book Management system*******\n");
	printf("\n***************************************\n");
	printf("\n*******  1  Add a book      ***********\n");
	printf("\n*******  2  Delete book     ***********\n");
	printf("\n*******  3  Search book     ***********\n");
	printf("\n*******  4  reload database ***********\n");
	printf("\n*******  0  exit            ***********\n");
	printf("\n****Please select your choice(0~4)*****\n");
	printf("\n***************************************\n");
	//此选择功能将会被多次使用，所以将其打包成为函数
	choice = menu_choice(4);
	switch(choice){
		case 1:
			menu_add();
			break;
		case 2:
			menu_delete();
			break;
		case 3:
			menu_search();
			break;
		case 4:
			menu_reload();
			break;
		case 0:
			menu_exit();
			break;
		default:
	//		menu_error();
			break;
	}
}
