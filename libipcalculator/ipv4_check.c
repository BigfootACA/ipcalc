#include"../global.h"
#include"ipv4.h"
#ifdef __cplusplus
extern "C"{
#endif
/*判断地址是否在网段的地址池内*/
bool ipv4_is_addr_in_pool(struct ipv4_net*pool,struct ipv4_addr*ip){
	if(!pool||!ip)return false;
	unsigned long
		i=ipv4_addr_to_long(ip),
		s=ipv4_get_minn_addr_net(pool),
		e=ipv4_get_maxn_addr_net(pool);
	return i>=s&&i<=e;
}
/*判断地址是否在网段内（包括网络地址和广播地址）*/
bool ipv4_is_addr_in_net(struct ipv4_net*pool,struct ipv4_addr*ip){
	if(!pool||!ip)return false;
	unsigned long
		i=ipv4_addr_to_long(ip),
		s=ipv4_get_minn_addr_net(pool),
		e=ipv4_get_maxn_addr_net(pool);
	return i>=s-1&&i<=e+1;
}
/*判断地址是否在字符串网段的地址池内*/
bool ipv4_is_addr_in_pool_x(char*pool,struct ipv4_addr*ip){
	if(!pool||!ip)return false;
	struct ipv4_net*p=ipv4_string_to_net_with_prefix(pool);
	if(!p)return false;
	bool x=ipv4_is_addr_in_pool(p,ip);
	ipv4_free_net(p);
	return x;
}
/*判断地址是否在字符串网段内（包括网络地址和广播地址）*/
bool ipv4_is_addr_in_net_x(char*pool,struct ipv4_addr*ip){
	if(!pool||!ip)return false;
	struct ipv4_net*p=ipv4_string_to_net_with_prefix(pool);
	if(!p)return false;
	bool x=ipv4_is_addr_in_net(p,ip);
	ipv4_free_net(p);
	return x;
}
/*判断地址是否在指定网段的地址池内*/
bool ipv4_is_addr_in_pool_px(unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3,unsigned char pr,struct ipv4_addr*ip){
	if(!ip)return false;
	struct ipv4_net*p=ipv4_new_net(p0,p1,p2,p3,pr);
	if(!p)return false;
	bool x=ipv4_is_addr_in_pool(p,ip);
	ipv4_free_net(p);
	return x;
}
/*判断地址是否在指定网段内（包括网络地址和广播地址）*/
bool ipv4_is_addr_in_net_px(unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3,unsigned char pr,struct ipv4_addr*ip){
	if(!ip)return false;
	struct ipv4_net*p=ipv4_new_net(p0,p1,p2,p3,pr);
	if(!p)return false;
	bool x=ipv4_is_addr_in_net(p,ip);
	ipv4_free_net(p);
	return x;
}
/*判断地址是否为网络地址*/
bool ipv4_is_addr_network_address(struct ipv4_net*net,struct ipv4_addr*ip){
	struct ipv4_addr*x=ipv4_get_net_addr_net(net);
	bool b=
		ipv4_check_net(net)&&ip&&
		ipv4_addr_to_long(ip)>0&&x&&
		ipv4_compare_addr(x,ip);
	if(x)free(x);
	return b;
}
/*判断地址是否为广播地址*/
bool ipv4_is_addr_broadcast_address(struct ipv4_net*net,struct ipv4_addr*ip){
	struct ipv4_addr*x=ipv4_get_brd_addr_net(net);
	bool b=
		ipv4_check_net(net)&&ip&&
		ipv4_addr_to_long(ip)>0&&x&&
		ipv4_compare_addr(x,ip);
	if(x)free(x);
	return b;
}
/*判断地址是否为有效地址*/
bool ipv4_is_addr_usable(struct ipv4_addr*ip){
	return
		ipv4_addr_is_group(ip,IPV4_GRP_LOCAL)||
		ipv4_addr_is_group(ip,IPV4_GRP_PUBLIC);
}
/*判断IPv4地址字符串是否有效*/
bool ipv4_check_addr_length(const char*ip){
	return strlen(ip)>=7&&strlen(ip)<=15;
}
/*判断是否为IPv4地址的数字段有效的数字*/
bool ipv4_check_addr_int_part(long part){
	return part>=0&&part<=255;
}
/*判断地址是否为掩码地址*/
bool ipv4_addr_is_maskn(unsigned long i){
	return ipv4_prefix_to_maskn(ipv4_maskn_to_prefix(i))==i;
}
/*检查网络是否为空*/
bool ipv4_check_net(struct ipv4_net*n){
	return n&&n->net&&n->mask;
}
#ifdef __cplusplus
}
#endif
