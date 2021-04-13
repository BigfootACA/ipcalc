#include"../global.h"
#include<stdio.h>
#ifdef __linux__
#include<linux/in.h>
#elif WIN32
#include<ws2tcpip.h>
#else
#error "No in_addr header found."
#endif
#include"ipv4.h"
#ifdef _MSC_VER
#define sscanf sscanf_s
#endif
#ifdef __cplusplus
extern "C"{
#endif
/*将inaddr转换为地址结构（未测试）*/
struct ipv4_addr*ipv4_inaddr_to_addr_x(struct in_addr*ip,struct ipv4_addr*buff){
	return ip&&buff?ipv4_long_to_addr_x(ip->s_addr,buff):NULL;
}
/*将十进制字符串地址转换为地址结构（传入缓冲）*/
struct ipv4_addr*ipv4_string_to_addr_x(char*ip,struct ipv4_addr*buff){
	if(!ip||!buff||!ipv4_check_addr_length(ip))return NULL;
	int i0=-1,i1=-1,i2=-1,i3=-1;
	sscanf(ip,"%d.%d.%d.%d",&i0,&i1,&i2,&i3);
	if(!ipv4_check_addr_int(i0,i1,i2,i3))return NULL;
	return ipv4_set_addr(
		buff,
		(unsigned char)i0,
		(unsigned char)i1,
		(unsigned char)i2,
		(unsigned char)i3
	);
}
/*将十进制字符串网络转换为网络结构（使用掩码前缀）（传入缓冲）*/
struct ipv4_net*ipv4_string_to_net_with_prefix_x(char*ip,struct ipv4_net*buff){
	if(!ip||!buff)return NULL;
	int i0=-1,i1=-1,i2=-1,i3=-1,pr=-1;
	sscanf(ip,"%d.%d.%d.%d/%d",&i0,&i1,&i2,&i3,&pr);
	if(!ipv4_check_addr_int(i0,i1,i2,i3)||pr<1||pr>32)return NULL;
	return ipv4_set_net(
		buff,
		(unsigned char)i0,
		(unsigned char)i1,
		(unsigned char)i2,
		(unsigned char)i3,
		(unsigned char)pr
	);
}
/*将十进制字符串网络转换为网络结构（使用掩码地址）（传入缓冲）*/
struct ipv4_net*ipv4_string_to_net_with_mask_x(char*ip,struct ipv4_net*buff){
	if(!ip||!buff)return NULL;
	int
		i0=-1,i1=-1,i2=-1,i3=-1,
		n0=-1,n1=-1,n2=-1,n3=-1;
	sscanf(
		ip,
		"%d.%d.%d.%d/"
		"%d.%d.%d.%d",
		&i0,&i1,&i2,&i3,
		&n0,&n1,&n2,&n3
	);
	if(
		!ipv4_check_addr_int(i0,i1,i2,i3)||
		!ipv4_check_addr_int(n0,n1,n2,n3)
	)return NULL;
	return ipv4_set_net_and_mask(
		buff,
		(unsigned char)i0,(unsigned char)i1,(unsigned char)i2,(unsigned char)i3,
		(unsigned char)n0,(unsigned char)n1,(unsigned char)n2,(unsigned char)n3
	);
}
/*将十进制字符串地址转换为地址结构（注意释放）*/
struct ipv4_addr*ipv4_string_to_addr(char*ip){
	if(!ip)return NULL;
	struct ipv4_addr*buff,*r;
	buff=ipv4_new_addr_obj();
	if(!buff)return NULL;
	r=ipv4_string_to_addr_x(ip,buff);
	if(!r){
		ipv4_free_addr(buff);
		return NULL;
	}
	return r;
}
/*将十进制字符串网络转换为网络结构（使用掩码前缀）（注意释放）*/
struct ipv4_net*ipv4_string_to_net_with_prefix(char*ip){
	if(!ip)return NULL;
	struct ipv4_net*buff,*r;
	buff=ipv4_new_net_obj();
	if(!buff)return NULL;
	r=ipv4_string_to_net_with_prefix_x(ip,buff);
	if(!r){
		ipv4_free_net(buff);
		return NULL;
	}
	return r;
}
/*将十进制字符串网络转换为网络结构（使用掩码地址）（注意释放）*/
struct ipv4_net*ipv4_string_to_net_with_mask(char*ip){
	if(!ip)return NULL;
	struct ipv4_net*buff,*r;
	buff=ipv4_new_net_obj();
	if(!buff)return NULL;
	r=ipv4_string_to_net_with_mask_x(ip,buff);
	if(!r){
		ipv4_free_net(buff);
		return NULL;
	}
	return r;
}
#ifdef __cplusplus
}
#endif
