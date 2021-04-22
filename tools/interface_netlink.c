#include"../global.h"
#ifdef __linux__
#include<fcntl.h>
#include<errno.h>
#include<stdbool.h>
#include<stdio.h>
#include<ifaddrs.h>
#include<unistd.h>
#include<net/if.h>
#include<arpa/inet.h>
#include<linux/netlink.h>
#include<linux/rtnetlink.h>
#include"../libipcalculator/ipv4.h"
#include"ipcalc.h"
static int sock=-1;
static int _print_interface_netlink(int fd,char display,char*ifname,bool list){
	bool success=false;
	struct sockaddr_nl snl;
	socklen_t namelen;
	if((sock=socket(AF_NETLINK,SOCK_RAW,NETLINK_ROUTE))<0){
		return_perror(0,"netlink socket");
		sock=-1;
		return print_interface_getifaddrs(fd,display,ifname,list);
	}
	if(fcntl(sock,F_SETFL,O_NONBLOCK)<0)return return_perror(1,"netlink fcntl");
	memset(&snl,0,sizeof(snl));
	snl.nl_family=AF_NETLINK;
	snl.nl_groups=0;
	if(bind(sock,(struct sockaddr*)&snl,sizeof(snl))<0)return return_perror(1,"netlink bind");
	namelen=sizeof(snl);
	if(getsockname(sock,(struct sockaddr*)&snl,&namelen)<0||namelen!=sizeof(snl))return return_perror(1,"netlink getsocketname");
	struct{
		struct nlmsghdr nlh;
		struct rtgenmsg g;
	}req;
	memset(&snl,0,sizeof(snl));
	memset(&req,0,sizeof(req));
	snl.nl_family=AF_NETLINK;
	req.nlh.nlmsg_len=sizeof(req);
	req.nlh.nlmsg_type=RTM_GETADDR;
	req.nlh.nlmsg_flags=NLM_F_ROOT|NLM_F_MATCH|NLM_F_REQUEST;
	req.nlh.nlmsg_pid=0;
	req.nlh.nlmsg_seq=1;
	req.g.rtgen_family=AF_INET;
	if(sendto(
		sock,
		(void*)&req,
		sizeof(req),
		0,
		(struct sockaddr*)&snl,
		sizeof(snl)
	)<0)return return_perror(1,"netlink sendto");
	int status;
	while(1){
		char buf[4096];
		struct iovec iov={buf,sizeof buf};
		struct sockaddr_nl snls;
		struct msghdr msg={
			.msg_name=(void*)&snls,
			.msg_namelen=sizeof(snls),
			.msg_iov=&iov,
			.msg_iovlen=1,
			.msg_control=NULL,
			.msg_controllen=0,
			.msg_flags=0
		};
		struct nlmsghdr*h;
		if((status=recvmsg(sock,&msg,0))<0){
			if(errno==EINTR)continue;
			if(errno==EWOULDBLOCK||errno==EAGAIN)break;
			return_perror(0,"netlink recvmsg");
			if(errno==EPIPE)return 1;
			continue;
		}
		if(snls.nl_pid!=0)continue;
		if(status==0)return return_stderr_print(1,"netlink EOF");
		if(msg.msg_namelen!=sizeof(snls))return return_stderr_print(1,"netlink sender address length error");
		for(h=(struct nlmsghdr*)buf;NLMSG_OK(h,(unsigned int)status);h=NLMSG_NEXT(h,status)){
			if(h->nlmsg_type==NLMSG_DONE)goto ex;
			if(h->nlmsg_type==NLMSG_ERROR){
				struct nlmsgerr *err=(struct nlmsgerr*)NLMSG_DATA(h);
				if(err->error==0){
					if(!(h->nlmsg_flags&NLM_F_MULTI))goto ex;
					continue;
				}
				if(h->nlmsg_len<NLMSG_LENGTH(sizeof(struct nlmsgerr)))return return_stderr_print(1,"netlink error: message truncated");
				errno=-err->error;
				return return_perror(1,"netlink error");
			}
			if(h->nlmsg_pid==snls.nl_pid)continue;
			int len;
			struct ifaddrmsg*ifa;
			struct rtattr*tb[IFA_MAX+1];
			unsigned long ifa_addr=0;
			char ifa_label[IFNAMSIZ+1];
			if(
			#ifdef __cplusplus
				(ifa=(ifaddrmsg*)NLMSG_DATA(h))->ifa_family!=AF_INET||/*C++编译时的奇怪报错...*/
			#else
				(ifa=NLMSG_DATA(h))->ifa_family!=AF_INET||
			#endif
				(
					h->nlmsg_type!=RTM_NEWADDR&&
					h->nlmsg_type!=RTM_DELADDR
				)
			)continue;
			if((len=h->nlmsg_len-NLMSG_LENGTH(sizeof(struct ifaddrmsg)))>=0){
				memset(tb,0,sizeof tb);
				int max=IFA_MAX;
				struct rtattr*rta=IFA_RTA(ifa);
				while(RTA_OK(rta,len)){
					if(rta->rta_type<=max)tb[rta->rta_type]=rta;
					rta=RTA_NEXT(rta,len);
				}
				if(tb[IFA_ADDRESS]==NULL)tb[IFA_ADDRESS]=tb[IFA_LOCAL];
				if(tb[IFA_ADDRESS])ifa_addr=*(uint32_t*)RTA_DATA(tb[IFA_ADDRESS]);
				if(tb[IFA_LABEL])strncpy(ifa_label,(const char*)RTA_DATA(tb[IFA_LABEL]),IFNAMSIZ);
				if(ifname&&strcmp(ifname,ifa_label)!=0)continue;
				success=true;
				struct ipv4_net*net=ipv4_new_net_obj();
				if(!ipv4_check_net(net))return return_stderr_print(1,"Failed to allocate object");
				if(!ipv4_long_to_addr_x(ifa_addr,net->net)){
					if(!quiet)fprintf(stderr,"Failed to parse address in interface %s\n",ifa_label);
					ipv4_free_net(net);
					continue;
				}
				ipv4_addr_swap(net->net);
				ipv4_prefix_to_maskx(net->mask,ifa->ifa_prefixlen);
				char*c=NULL;
				if((c=ipv4_net_to_string(net))){
					if(!quiet)dprintf(list?fd:STDERR_FILENO,"Interface %s: IPv4 %s\n",ifa_label,c);
					else if(list)dprintf(fd,"%s=%s\n",ifa_label,c);
					fflush(stderr);
					free(c);
				}
				if(!list){
					print_result(fd,net,display);
					if(display=='N')dprintf(fd,"\n");
				}
				ipv4_free_net(net);
			}else return return_stderr_print(1,"netlink function error");
		}
		if(msg.msg_flags&MSG_TRUNC)continue;
		if(status)return return_stderr_print(1,"netlink error: data remnant size non-zero");
	}
	ex:
	if(ifname&&!success){
		if(!quiet)fprintf(stderr,"%s: interface not found\n",ifname);
		return 1;
	}
	return 0;
}
/*适用于Linux平台的Netlink网卡地址获取接口*/
int print_interface_netlink(int fd,char display,char*ifname,bool list){
	int i=_print_interface_netlink(fd,display,ifname,list);
	if(sock>=0)close(sock);
	sock=-1;
	return i;
}
#else
extern int __AVOID_MSVC_C4206;
#endif
