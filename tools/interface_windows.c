#include"../global.h"
#ifdef WIN32
#include<winsock2.h>
#include<windows.h>
#include<ws2tcpip.h>
#include<iphlpapi.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"../libipcalculator/ipv4.h"
#include"ipcalc.h"
#ifdef _MSC_VER
#define strdup _strdup
#define wcsdup _wcsdup
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif
/*字符数组转宽字符数组*/
static wchar_t*char2wchar(char*c){
	if(!c)return NULL;
	char*cc=strdup(c);
	if(!cc)return NULL;
	size_t l=sizeof(wchar_t)*MultiByteToWideChar(CP_ACP,0,cc,(int)strlen(cc),NULL,0)+1;
	wchar_t*w=(wchar_t*)malloc(l);
	if(w){
		memset(w,0,l);
		MultiByteToWideChar(CP_ACP,0,cc,(int)strlen(cc),w,(int)(l-1));
	}
	free(cc);
	return w;
}
/*宽字符数组转字符数组*/
static char*wchar2char(wchar_t*w){
	if(!w)return NULL;
	wchar_t*ww=wcsdup(w);
	if(!ww)return NULL;
	size_t l=sizeof(char)*WideCharToMultiByte(CP_ACP,0,ww,(int)wcslen(ww),NULL,0,NULL,NULL)+1;
	char*c=(char*)malloc(l);
	if(c){
		memset(c,0,l);
		WideCharToMultiByte(CP_ACP,0,ww,(int)wcslen(ww),c,(int)(l-1),NULL,NULL);
	}
	free(ww);
	return c;
}
/*适用于Windows平台的网卡地址获取接口*/
int print_interface_windows(int fd,char display,char*ifname,bool list){
	(void)fd;
	int ret=0;
	char*msg=NULL;
	ULONG len=sizeof(IP_ADAPTER_ADDRESSES),iters=0;
	IP_ADAPTER_ADDRESSES*curs=NULL,*addrs=NULL;
	do{
		addrs=(IP_ADAPTER_ADDRESSES*)malloc(len);
		if(!addrs){
			printf("malloc failed for IP_ADAPTER_ADDRESSES.\n");
			exit(1);
		}
		ret=GetAdaptersAddresses(AF_UNSPEC,GAA_FLAG_INCLUDE_PREFIX,NULL,addrs,&len);
		if(ret==ERROR_BUFFER_OVERFLOW){
			free(addrs);
			addrs=NULL;
		}else break;
		iters++;
	}while((ret==ERROR_BUFFER_OVERFLOW)&&(iters<3));
	if(ret==NO_ERROR){
		ret=0;
		curs=addrs;
		char*desc=NULL,*name=NULL;
		bool i=false;
		if(curs)do{
			desc=wchar2char(curs->Description);
			name=wchar2char(curs->FriendlyName);
			if(
				!ifname||
				(curs->AdapterName&&strcmp(ifname,curs->AdapterName)==0)||
				(desc&&strcmp(ifname,desc)==0)||
				(name&&strcmp(ifname,name)==0)
			){
				IP_ADAPTER_UNICAST_ADDRESS*ip=curs->FirstUnicastAddress;
				IP_ADAPTER_PREFIX*pre=curs->FirstPrefix;
				i=true;
				if(ip&&pre)do{
					if(ip->Address.lpSockaddr->sa_family==AF_INET){
						struct sockaddr_in*s=(struct sockaddr_in*)(ip->Address.lpSockaddr);
						struct ipv4_net*net=ipv4_new_net(
							(unsigned char)(s->sin_addr.S_un.S_un_b.s_b1),
							(unsigned char)(s->sin_addr.S_un.S_un_b.s_b2),
							(unsigned char)(s->sin_addr.S_un.S_un_b.s_b3),
							(unsigned char)(s->sin_addr.S_un.S_un_b.s_b4),
							(unsigned char)(pre->PrefixLength)
						);
						printf(
							"%d.%d.%d.%d/%d\n",
							(int)(s->sin_addr.S_un.S_un_b.s_b1),
							(int)(s->sin_addr.S_un.S_un_b.s_b2),
							(int)(s->sin_addr.S_un.S_un_b.s_b3),
							(int)(s->sin_addr.S_un.S_un_b.s_b4),
							(int)(pre->PrefixLength)
						);
						if(!net){
							if(!quiet)fprintf(
								stderr,
								"Failed to parse IPv4 address for interface '%s'(%s).\n",
								name,desc
							);
							continue;
						}
						char*c=NULL;
						if((c=ipv4_net_to_string(net))){
							if(!quiet)fprintf(
								list?stdout:stderr,
								"Interface '%s'(%s): IPv4 %s\n",
								name,desc,c
							);
							else if(list)fprintf(stdout,"%s=%s\n",curs->AdapterName,c);
							fflush(stderr);
							free(c);
						}
						if(!list){
							print_result(STDOUT_FILENO,net,display);
							if(display=='N')printf("\n");
						}
						ipv4_free_net(net);
					}
				}while((ip=ip->Next)&&(pre=pre->Next));
			}
			if(name)free(name);
			if(desc)free(desc);
		}while((curs=curs->Next));
		if(!i){
			fprintf(stderr,"Failed to found interface %s\n",ifname);
			ret=1;
		}
	}else{
		fprintf(stderr,"GetAdaptersAddresses failed: %d\n",ret);
		if(ret!=ERROR_NO_DATA){
			if(FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				ret,
				MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(LPTSTR)&msg,0,NULL)
			){
				fprintf(stderr,"Error: %s\n",msg);
				LocalFree(msg);
				if(addrs)free(addrs);
				exit(1);
			}
		}else if(!quiet)fprintf(stderr,"No any addresses found.\n");
	}
	if(addrs)free(addrs);
	return ret;
}
#endif
