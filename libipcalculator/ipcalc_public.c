#include"../global.h"
#include"ipv4.h"
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#ifdef __cplusplus
extern "C"{
#endif
/*十进制转二进制*/
unsigned long dec2bin(unsigned long dec){
	long p=1,y;
	unsigned long r=0;
	while(true){
		y=dec%2;
		dec/=2;
		r+=y*p;
		p*=10;
		if(dec<2){
			r+=dec*p;
			break;
		}
	}
	return r;
}
/*新建字符串对象（注意释放）*/
char*new_string(size_t size){
	size_t s=sizeof(char)*size;
	char*buff=(char*)malloc(s);
	if(!buff)return NULL;
	memset(buff,0,s);
	return buff;
}
/*释放字符串对象*/
void free_string(char*ptr){
	if(!ptr)return;
	free(ptr);
	ptr=0;
}
#ifdef __cplusplus
}
#endif
