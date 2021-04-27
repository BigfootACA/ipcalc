// file from musl 1.2.0,for no getopt libc
#include<stddef.h>
#include<stdlib.h>
#include<limits.h>
#include<stdio.h>
#include"getopt.h"
#ifdef __cplusplus
extern "C" {
#endif
extern int __optpos,__optreset;
static void permute(char*const*argv,int dest,int src){
	char**av=(char**)argv,*tmp=av[src];
	int i;
	for(i=src;i>dest;i--)av[i]=av[i-1];
	av[dest]=tmp;
}
static int __getopt_long_core(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx,int longonly);
static int __getopt_long(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx,int longonly){
	int ret,skipped,resumed;
	if(!optind||__optreset){
		__optreset=0;
		__optpos=0;
		optind=1;
	}
	if(optind>=argc||!argv[optind])return -1;
	skipped=optind;
	if(optstring[0]!='+'&&optstring[0]!='-'){
		int i;
		for(i=optind;;i++){
			if(i>=argc||!argv[i])return -1;
			if(argv[i][0]=='-'&&argv[i][1])break;
		}
		optind=i;
	}
	resumed=optind;
	ret=__getopt_long_core(argc,argv,optstring,longopts,idx,longonly);
	if(resumed>skipped){
		int i,cnt=optind-resumed;
		for(i=0;i<cnt;i++)permute(argv,skipped,optind-1);
		optind=skipped + cnt;
	}
	return ret;
}
static int __getopt_long_core(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx,int longonly){
	optarg=0;
	if(longopts&&argv[optind][0]=='-'&&((longonly&&argv[optind][1]&&argv[optind][1]!='-')||(argv[optind][1]=='-'&&argv[optind][2]))){
		int colon=optstring[optstring[0]=='+'||optstring[0]=='-']==':';
		int i,cnt,match;
		char*arg=NULL,*opt,*start=argv[optind]+1;
		for(cnt=i=0;longopts[i].name;i++){
			const char*name=longopts[i].name;
			opt=start;
			if(*opt=='-')opt++;
			while(*opt&&*opt!='='&&*opt==*name)name++,opt++;
			if(*opt&&*opt!='=')continue;
			arg=opt;
			match=i;
			if(!*name){cnt=1;break;}
			cnt++;
		}
		if(cnt==1&&longonly&&arg-start==mblen(start,MB_LEN_MAX)){
			int l=(int)(arg-start);
			for(i=0;optstring[i];i++){
				int j;
				for(j=0;j<l&&start[j]==optstring[i+j];j++);
				if(j==l){cnt++;break;}
			}
		}
		if(cnt==1){
			i=match;
			opt=arg;
			optind++;
			if(*opt=='='){
				if(!longopts[i].has_arg){
					optopt=longopts[i].val;
					if(colon||!opterr)return '?';
					fprintf(stderr,"%s: option does not take an argument: %s\n",argv[0],longopts[i].name);
					return '?';
				}
				optarg=opt+1;
			}else if(longopts[i].has_arg==required_argument){
				if(!(optarg=argv[optind])){
					optopt=longopts[i].val;
					if(colon)return ':';
					if(!opterr)return '?';
					fprintf(stderr,"%s: option requires an argument: %s\n",argv[0],longopts[i].name);
					return '?';
				}
				optind++;
			}
			if(idx)*idx=i;
			if(longopts[i].flag){
				*longopts[i].flag=longopts[i].val;
				return 0;
			}
			return longopts[i].val;
		}
		if(argv[optind][1]=='-'){
			optopt=0;
			if(!colon&&opterr)fprintf(stderr,cnt?"%s: option is ambiguous: %s\n":"%s: unrecognized option: %s\n",argv[0],argv[optind]+2);
			optind++;
			return '?';
		}
	}
	return getopt(argc,argv,optstring);
}
int getopt_long(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx){return __getopt_long(argc,argv,optstring,longopts,idx,0);}
int getopt_long_only(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx){return __getopt_long(argc,argv,optstring,longopts,idx,1);}
#ifdef __cplusplus
}
#endif
