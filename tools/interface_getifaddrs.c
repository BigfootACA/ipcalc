#include"../global.h"
#ifdef __linux__
#include<stdio.h>
#include<unistd.h>
#include<ifaddrs.h>
#include<arpa/inet.h>
#include"../libipcalculator/ipv4.h"
#include"ipcalc.h"
/*适用于Linux平台的getifaddrs网卡地址获取接口*/
int print_interface_getifaddrs(int fd,char display,const char*ifname,bool list){
	struct ifaddrs*ia=NULL,*i;
	if(getifaddrs(&ia)!=0)return return_perror(1,"getifaddrs");
	i=ia;
	do{
		if(!i->ifa_addr||i->ifa_addr->sa_family!=AF_INET)continue;
		if(ifname&&(!i->ifa_name||strcmp(ifname,i->ifa_name)!=0))continue;
		void*p1=&((struct sockaddr_in*)i->ifa_addr)->sin_addr;
		void*p2=&((struct sockaddr_in*)i->ifa_netmask)->sin_addr;
		char ip[INET_ADDRSTRLEN],mask[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,p1,ip,INET_ADDRSTRLEN);
		inet_ntop(AF_INET,p2,mask,INET_ADDRSTRLEN);
		struct ipv4_net*net=ipv4_new_net_obj();
		if(!ipv4_check_net(net))continue;
		if(
			ipv4_string_to_addr_x(ip,net->net)&&
			ipv4_string_to_addr_x(mask,net->mask)
		){
			char*c;
			if((c=ipv4_net_to_string(net))){
				if(!quiet)dprintf(list?fd:STDERR_FILENO,"Interface %s IPv4 address: %s\n",i->ifa_name,c);
				else if(list)dprintf(fd,"%s=%s\n",i->ifa_name,c);
				fflush(stderr);
				free(c);
			}
			if(!list){
				print_result(fd,net,display);
				if(display=='N')dprintf(fd,"\n");
			}
		}
		ipv4_free_net(net);
	}while((i=i->ifa_next));
	freeifaddrs(ia);
	return 0;
}
#else
extern int __AVOID_MSVC_C4206;
#endif
