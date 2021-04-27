// file from musl 1.2.0,for no getopt libc
#ifndef _MSC_VER
#include<unistd.h>
#endif
#include<wchar.h>
#include<string.h>
#include<limits.h>
#include<stdlib.h>
#include<stdio.h>
#include"getopt.h"
#ifdef __cplusplus
extern "C"{
#endif
char *optarg;
int optind=1,opterr=1,optopt,__optpos,__optreset=0;
#define optpos __optpos
int getopt(int argc,char*const argv[],const char*optstring){
	int i,k,l;
	wchar_t c,d;
	char*optchar;
	if(!optstring)return -1;
	if(!optind||__optreset){
		__optreset=0;
		__optpos=0;
		optind=1;
	}
	if(optind >=argc||!argv[optind])return -1;
	if(argv[optind][0]!='-'){
		if(optstring[0]=='-'){
			optarg=argv[optind++];
			return 1;
		}
		return -1;
	}
	if(!argv[optind][1])return -1;
	if(argv[optind][1]=='-'&&!argv[optind][2])return optind++,-1;
	if(!optpos)optpos++;
	if((k=mbtowc(&c,argv[optind]+optpos,MB_LEN_MAX))<0){
		k=1;
		c=0xfffd;
	}
	optchar=argv[optind]+optpos;
	optpos+=k;
	if(!argv[optind][optpos]){
		optind++;
		optpos=0;
	}
	if(optstring[0]=='-'||optstring[0]=='+')optstring++;
	i=0;
	d=0;
	do{
		l=mbtowc(&d,optstring+i,MB_LEN_MAX);
		i+=l>0?l:1;
	}while(l&&d!=c);
	if(d!=c||c==':'){
		optopt=c;
		if(optstring[0]!=':'&&opterr)fprintf(stderr,"%s: unrecognized option: %s\n",argv[0],optchar);
		return '?';
	}
	if(optstring[i]==':'){
		optarg=0;
		if(optstring[i+1] !=':'||optpos){
			optarg=argv[optind++]+optpos;
			optpos=0;
		}
		if(optind>argc){
			optopt=c;
			if(optstring[0]==':')return ':';
			if(opterr)fprintf(stderr,"%s: option requires an argument: %s\n",argv[0],optchar);
			return '?';
		}
	}
	return c;
}
#ifdef __cplusplus
}
#endif
