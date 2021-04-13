#include"../global.h"
#include"ipv4.h"
#include<stdio.h>
#ifdef __cplusplus
extern "C"{
#endif
/*将长整型地址转换为地址结构（传入缓冲）*/
struct ipv4_addr*ipv4_long_to_addr_x(unsigned long ip,struct ipv4_addr*buff){
	if(!buff)return NULL;
	buff->p3=ip&255;ip=ip>>8;
	buff->p2=ip&255;ip=ip>>8;
	buff->p1=ip&255;ip=ip>>8;
	buff->p0=ip&255;
	return buff;
}
/*将长整型掩码地址转换为掩码前缀*/
int ipv4_maskn_to_prefix(unsigned long mask){
	unsigned long m=0x80000000;
	int p=0;
	while(m){
		if(mask&m)p++;
		m>>=1;
	}
	return p;
}
/*将掩码前缀转换为掩码地址（注意释放）*/
struct ipv4_addr*ipv4_prefix_to_mask(int prefix){
	unsigned long s=ipv4_prefix_to_maskn(prefix);
	if(s==0)return NULL;
	return ipv4_long_to_addr(s);
}
/*将掩码前缀转换为掩码地址（传入缓冲）*/
struct ipv4_addr*ipv4_prefix_to_mask_x(struct ipv4_addr*buff,int prefix){
	unsigned long s=ipv4_prefix_to_maskn(prefix);
	return (s==0)?NULL:ipv4_long_to_addr_x(s,buff);
}
/*将地址结构转换为长整型地址*/
unsigned long ipv4_addr_to_long(struct ipv4_addr*ip){
	return ((ip->p0<<24)|(ip->p1<<16)|(ip->p2<<8)|ip->p3)&0xFFFFFFFF;
}
/*将掩码前缀转换为长整型掩码地址*/
unsigned long ipv4_prefix_to_maskn(int prefix){
	return
		(prefix>32||prefix<1)?
		0:0xFFFFFFFF<<(32-prefix)&0xFFFFFFFF;
}
/*将掩码前缀转换为长整型通配符地址*/
unsigned long ipv4_prefix_to_wildcardn(int prefix){
	return
		(prefix>32||prefix<0)?
		0:0xFFFFFFFF-ipv4_prefix_to_maskn(prefix);
}
/*将地址结构转换为十进制字符串地址（传入缓冲和长度）*/
char*ipv4_addr_to_string_xn(struct ipv4_addr*ip,char*buff,size_t s){
	char*b=buff;
	if(!ip||!b)return NULL;
	size_t ss=snprintf(
		b,s,
		"%d.%d.%d.%d",
		ip->p0,
		ip->p1,
		ip->p2,
		ip->p3
	);
	if(ss<7||ss>15)b=NULL;
	return b;
}
/*将地址结构转换为十六进制字符串地址（传入缓冲和长度）*/
char*ipv4_addr_to_hex_string_xn(struct ipv4_addr*ip,char*buff,size_t s){
	char*b=buff;
	if(!ip||!b)return NULL;
	size_t ss=snprintf(
		b,s,
		"%02X.%02X.%02X.%02X",
		ip->p0,
		ip->p1,
		ip->p2,
		ip->p3
	);
	if(ss!=11)b=NULL;
	return b;
}
/*将地址结构转换为二进制字符串地址（传入缓冲和长度）*/
char*ipv4_addr_to_bin_string_xn(struct ipv4_addr*ip,char*buff,size_t s){
	char*b=buff;
	if(!ip||!b)return NULL;
	size_t ss=snprintf(
		b,s,
		"%08lu.%08lu.%08lu.%08lu",
		dec2bin(ip->p0),
		dec2bin(ip->p1),
		dec2bin(ip->p2),
		dec2bin(ip->p3)
	);
	if(ss!=35)b=NULL;
	return b;
}
/*将网络结构转换为十进制字符串地址（传入缓冲和长度）*/
char*ipv4_net_to_string_xn(struct ipv4_net*net,char*buff,size_t s){
	char*b=buff;
	if(!net||!b)return NULL;
	struct ipv4_addr*ip=net->net,*mask=net->mask;
	if(!ip||!mask)return NULL;
	size_t ss=snprintf(
		b,s,
		"%d.%d.%d.%d/%d",
		ip->p0,
		ip->p1,
		ip->p2,
		ip->p3,
		ipv4_mask_to_prefix(mask)
	);
	if(ss<9||ss>18)b=NULL;
	return b;
}
/*将地址结构转换为十进制字符串地址（注意释放）*/
char*ipv4_addr_to_string(struct ipv4_addr*ip){
	char*buff=new_string(16);
	char*r=ipv4_addr_to_string_xn(ip,buff,16);
	if(!r)free(buff);
	return r;
}
/*将地址结构转换为十六进制字符串地址（注意释放）*/
char*ipv4_addr_to_hex_string(struct ipv4_addr*ip){
	char*buff=new_string(12);
	char*r=ipv4_addr_to_hex_string_xn(ip,buff,12);
	if(!r)free(buff);
	return r;
}
/*将地址结构转换为十进制字符串地址（注意释放）*/
char*ipv4_addr_to_bin_string(struct ipv4_addr*ip){
	char*buff=new_string(36);
	char*r=ipv4_addr_to_bin_string_xn(ip,buff,68);
	if(!r)free(buff);
	return r;
}
/*将网络结构转换为十进制字符串地址（注意释放）*/
char*ipv4_net_to_string(struct ipv4_net*net){
	char*buff=new_string(20);
	char*r=ipv4_net_to_string_xn(net,buff,20);
	if(!r)free(buff);
	return r;
}
#ifdef __cplusplus
}
#endif
