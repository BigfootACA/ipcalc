#include"../global.h"
#include<stdbool.h>
#ifndef _MSC_VER
#include<unistd.h>
#endif
#include<stdio.h>
#include"ipv4.h"
#include"ipcalc_internal.h"
#ifdef __cplusplus
extern "C"{
#endif
#ifndef ssize_t
#define ssize_t size_t
#endif
/*重复输出字符串到指定FD*/
size_t repet_chars(int fd,const char*chars,size_t size){
	if(!chars||size<=0)return 0;
	size_t repet=size,len=0,wr;
	while(repet-->0){
		if((wr=fd_print(fd,chars))<=0)break;
		len+=wr;
	}
	return len;
}
/*输出一个字符串到指定FD并返回是否完整输出*/
bool xprint(int fd,const char*chars,size_t*len){
	size_t wr;
	if((wr=fd_print(fd,chars))<=0)return false;
	if(len)(*len)+=wr;
	return true;
}
/*重复输出字符串到指定FD并返回是否完整输出*/
bool xrepet_chars(int fd,const char*chars,size_t size,size_t*len){
	size_t wr;
	if((wr=repet_chars(fd,chars,size))<=0)return false;
	if(len)(*len)+=wr;
	return true;
}
/*把长整型无符号数字转为字符串（注意释放）*/
char*ultostr(unsigned long ul){
	size_t s=sizeof(char)*(sizeof(unsigned long)*4);
	char*c=(char*)malloc(s);
	if(!c)return NULL;
	memset(c,0,s);
	return ultostr_xl(c,s,ul);
}
/*把长整型无符号数字转为字符串*/
char*ultostr_xl(char*buff,size_t len,unsigned long ul){
	if(!buff)return NULL;
	snprintf(buff,len,"%lu",ul);
	return buff;
}
/*打印表格行*/
size_t print_line(int fd,int*cols,const int*bcols,const char*repet,const char*left,const char*right,const char*center,const char*tcenter,const char*bcenter){
	if(!cols||!left||!right||!center||!tcenter||!bcenter)return 0;
	size_t idx=0,len=0;
	xprint(fd,left,&len);
	if(cols[idx]>0&&(!bcols||bcols[idx]>0))while(true){
		xrepet_chars(fd,repet,cols[idx],&len);
		idx++;
		if(cols[idx]<=0)break;
		const char*cc=center;
		if(bcols)switch(bcols[idx]){
			case 1:cc=center;break;
			case 2:cc=tcenter;break;
			case 3:cc=bcenter;break;
		}
		xprint(fd,cc,&len);
	}
	xprint(fd,right,&len);
	xprint(fd,"\n",&len);
	return len;
}
/*打印表格字符串行*/
size_t print_string_line(int fd,const int*cols,const char**strs,const char*split){
	if(!cols||!strs)return 0;
	size_t idx=-1,wr,len=0;
	ssize_t wrx;
	xprint(fd,split,&len);
	while(true){
		idx++;
		if(cols[idx]<=0||!strs[idx])break;
		if(split)xprint(fd," ",&len);
		len+=(wr=fd_print(fd,strs[idx]));
		wrx=cols[idx]-wr-(split?1:0);
		if(wrx>0)xrepet_chars(fd," ",(size_t)wrx,&len);
		xprint(fd,split,&len);
	}
	xprint(fd,"\n",&len);
	return len;
}
/*打印表格中间分隔*/
size_t print_center(int fd,int*cols,int*bcols,struct display_char*t){
	return
		t?print_line(
			fd,cols,bcols,
			t->horizon,t->left,t->right,
			t->center,t->top_center,t->bottom_center
		):0;
}
/*打印表格顶部*/
size_t print_top(int fd,int*cols,struct display_char*t){
	return
		t?print_line(
			fd,cols,NULL,
			t->horizon,t->top_left,t->top_right,
			t->top_center,t->top_center,t->top_center
		):0;
}
/*打印表格底部*/
size_t print_bottom(int fd,int*cols,struct display_char*t){
	return
		t?print_line(
			fd,cols,NULL,
			t->horizon,t->bottom_left,t->bottom_right,
			t->bottom_center,t->bottom_center,t->bottom_center
		):0;
}
#ifdef __cplusplus
}
#endif
