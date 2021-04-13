#include"../global.h"
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../libipcalculator/ipv4.h"
#include"ipcalc.h"
#ifdef _MSC_VER
#include"../getopt/getopt.h"
#else
#include<unistd.h>
#include<getopt.h>
#endif
bool quiet=false;
static struct ipv4_net*ni=NULL;
static int fd=STDOUT_FILENO;
static char*file=NULL;
static void ipcalc_cleanup(void){
	#ifdef __linux__
	if(file)close(fd);
	#endif
	ipv4_free_net(ni);
}
#ifdef SIMPLEINIT
/*simple-init兼容applet*/
int ipcalc_main(int argc,char**argv){
#else
int main(int argc,char**argv){
#endif
	const struct option lopts[]={
		{"unicode",   no_argument,      NULL,'U'},
		{"ascii",     no_argument,      NULL,'A'},
		{"nosep",     no_argument,      NULL,'N'},
		{"none",      no_argument,      NULL,'n'},
		{"variable",  no_argument,      NULL,'V'},
		{"quiet",     no_argument,      NULL,'q'},
		{"help",      no_argument,      NULL,'h'},
		#ifndef SIMPLEINIT
		/*simple-init不允许显示版本*/
		{"version",   no_argument,      NULL,'v'},
		#endif
		{"all",       no_argument,      NULL,'a'},
		{"list",      no_argument,      NULL,'l'},
		{"fd",        required_argument,NULL,'f'},
		{"output",    required_argument,NULL,'o'},
		{"interface", required_argument,NULL,'i'},
		{NULL,0,NULL,0}
	};
	char s;
	#ifdef WIN32
	s='A';/*在Windows下使用ASCII显示避免乱码*/
	#else
	s='U';/*UTF-8字符*/
	#endif
	;
	char*interface=NULL;
	bool all=false,list=false;
	int c,off=-1;
	atexit(ipcalc_cleanup);
	#ifndef _MSC_VER
	char*term=getenv("TERM");
	if(
		!term||(
		strncmp(term,"xterm",5)!=0&&
		strncmp(term,"screen",6)!=0&&
		strncmp(term,"putty",5)!=0&&
		strncmp(term,"cygwin",6)!=0
	))s='A';/*在未知类型的终端使用ASCII*/
	#endif
	#ifdef __linux__
	if(!isatty(STDOUT_FILENO))s='V';/*当标准输出不是tty时使用变量形式输出*/
	#endif
	while((c=getopt_long(argc,argv,"UANVnhq"
	#ifndef SIMPLEINIT
	"v"
	#endif
	"lai:o:f:",lopts,NULL))!=-1)switch(c){
		case 'U':case 'A':case 'V':case 'N':case 'n':s=(char)c;break;
		case 'a':all=true;break;
		case 'l':list=true;break;
		case 'i':interface=optarg;break;
		case 'f':fd=(int)strtol(optarg,NULL,9);break;
		case 'o':file=optarg;break;
		#ifndef SIMPLEINIT
		case 'v':return ipcalc_version(0);
		#endif
		case 'q':quiet=true;break;
		case 'h':return ipcalc_usage(0,true);
		default:return ipcalc_usage(1,false);
	}
	argc-=optind-1;
	argv+=optind-1;
	#ifdef __linux__
	if(file&&(fd=open(file,O_WRONLY|O_CREAT|O_NOCTTY|O_TRUNC,0644))<0)return return_perror(2,"open");
	#else
	if(file)return return_stderr_print(1,"Print result to file only support in Linux.");
	if(fd!=STDOUT_FILENO)return return_stderr_print(1,"Print result to FD only support in Linux.");
	#endif
	if(all||interface||list){/*当使用全部选项或列表选项或指定接口时调用print_interface*/
		if(
			(all&&interface)||
			(all&&list)||
			(interface&&list)
		)return return_stderr_print(1,"Do not use -a, -l and -i IFNAME at the same time");/*一次只能使用一个选项*/
		else if(argc>1)return return_stderr_print(1,"Too many arguments.");
		else if(all)return print_interface(fd,s,NULL,false);
		else if(list)return print_interface(fd,s,NULL,true);
		else return print_interface(fd,s,interface,false);
	}
	if(argc<2||argc>3)return ipcalc_usage(1,false);
	char*addr=argv[1],*mask=argc==3?argv[2]:NULL,*x;
	if(!addr)return ipcalc_usage(1,false);
	ni=ipv4_new_net(0,0,0,0,32);
	if(!ipv4_check_net(ni))return return_stderr_print(2,"Failed to allocate object");
	if(
		mask&&(
			!ni||
			!ipv4_string_to_addr_x(mask,ni->mask)||
			!ipv4_addr_is_mask(ni->mask)
		)
	)return return_stderr_print(1,"Failed to parse netmask");
	while(addr[++off]!=0&&addr[off]!='/');
	if(addr[off]=='/'){
		if(mask)return return_stderr_print(1,"More than one netmask has been entered");
		struct ipv4_net*net=NULL;
		int xx=(int)strlen(addr)-off-1;
		if(xx>=1&&xx<=2)net=ipv4_string_to_net_with_prefix(addr);
		else if(xx>=7&&xx<=15)net=ipv4_string_to_net_with_mask(addr);
		if(
			!ni||
			!net||
			!ipv4_check_net(net)||
			!ipv4_check_net(ni)
		)return return_stderr_print(1,"Failed to parse address/prefix");
		ipv4_copy_addr(ni->net,net->net);
		ipv4_copy_addr(ni->mask,net->mask);
		ipv4_free_net(net);
	}else if(!ni||!ipv4_string_to_addr_x(addr,ni->net))return return_stderr_print(1,"Failed to parse address");
	if(!quiet&&(x=ipv4_net_to_string(ni))){
		fprintf(stderr,"Input: %s\n",x);
		fflush(stderr);
		free(x);
	}
	return print_result(fd,ni,s)?0:1;
}
