#include"../global.h"
#include"ipv4.h"
#ifdef __cplusplus
extern "C"{
#endif
/*使用数字掩码地址和数字地址获取数字广播地址*/
unsigned long ipv4_get_brdn_addrn_with_maskn(unsigned long ip,unsigned long mask){
	return
		ipv4_get_netn_addrn_with_maskn(ip,mask)+
		ipv4_maskn_to_wildcardn(mask);
}
/*使用掩码前缀结构和数字地址获取数字广播地址*/
unsigned long ipv4_get_brdn_addrn_with_prefix(unsigned long ip,int prefix){
	return
		ipv4_get_netn_addrn_with_prefix(ip,prefix)+
		ipv4_prefix_to_wildcardn(prefix);
}
/*使用网络结构获取数字网络地址*/
unsigned long ipv4_get_netn_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_netn_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取数字广播地址*/
unsigned long ipv4_get_brdn_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_brdn_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取网络地址结构（注意释放）*/
struct ipv4_addr*ipv4_get_net_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_net_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取广播地址结构（注意释放）*/
struct ipv4_addr*ipv4_get_brd_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_brd_addr_with_mask(net->net,net->mask):0;
}
/*使用数字掩码地址和数字地址获取数字最小（起始）地址*/
unsigned long ipv4_get_minn_addrn_with_maskn(unsigned long ip,unsigned long mask){
	return
		mask==0xFFFFFFFF?
		ip:(ipv4_get_netn_addrn_with_maskn(ip,mask)+1);
}
/*使用数字掩码地址和数字地址获取数字最大（结束）地址*/
unsigned long ipv4_get_maxn_addrn_with_maskn(unsigned long ip,unsigned long mask){
	return
		mask==0xffffffff?
		ip:(ipv4_get_brdn_addrn_with_maskn(ip,mask)-1);
}
/*使用掩码前缀和数字地址获取数字最小（起始）地址*/
unsigned long ipv4_get_minn_addrn_with_prefix(unsigned long ip,int prefix){
	return
		prefix==32?
		ip:ipv4_get_minn_addrn_with_maskn(ip,ipv4_prefix_to_maskn(prefix));
}
/*使用掩码前缀和数字地址获取数字最大（结束）地址*/
unsigned long ipv4_get_maxn_addrn_with_prefix(unsigned long ip,int prefix){
	return
		prefix==32?
		ip:(ipv4_get_brdn_addrn_with_prefix(ip,prefix)-1);
}
/*使用网络结构获取数字最小（起始）地址*/
unsigned long ipv4_get_minn_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_minn_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取数字最大（结束）地址*/
unsigned long ipv4_get_maxn_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_maxn_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取最小（起始）地址（注意释放）*/
struct ipv4_addr*ipv4_get_min_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_min_addr_with_mask(net->net,net->mask):0;
}
/*使用网络结构获取最大（结束）地址（注意释放）*/
struct ipv4_addr*ipv4_get_max_addr_net(struct ipv4_net*net){
	return
		ipv4_check_net(net)?
		ipv4_get_max_addr_with_mask(net->net,net->mask):0;
}
/*使用数字掩码地址和数字地址获取主机地址数量*/
unsigned long ipv4_get_count_addrn_with_maskn(unsigned long ip,unsigned long mask){
	return
		ipv4_get_maxn_addrn_with_maskn(ip,mask)-
		ipv4_get_minn_addrn_with_maskn(ip,mask)+1;
}
/*使用掩码地址结构和数字地址获取主机地址数量*/
unsigned long ipv4_get_count_addrn_with_mask(unsigned long ip,struct ipv4_addr*mask){
	return
		ipv4_get_maxn_addrn_with_mask(ip,mask)-
		ipv4_get_minn_addrn_with_mask(ip,mask)+1;
}
/*使用数字掩码地址和地址结构获取主机地址数量*/
unsigned long ipv4_get_count_addr_with_maskn(struct ipv4_addr*ip,unsigned long mask){
	return
		ipv4_get_maxn_addr_with_maskn(ip,mask)-
		ipv4_get_minn_addr_with_maskn(ip,mask)+1;
}
/*使用掩码地址结构和地址结构获取主机地址数量*/
unsigned long ipv4_get_count_addr_with_mask(struct ipv4_addr*ip,struct ipv4_addr*mask){
	return
		ipv4_get_maxn_addr_with_mask(ip,mask)-
		ipv4_get_minn_addr_with_mask(ip,mask)+1;
}
/*使用掩码前缀和数字地址获取主机地址数量*/
unsigned long ipv4_get_count_addrn_with_prefix(unsigned long ip,int prefix){
	return
		ipv4_get_maxn_addrn_with_prefix(ip,prefix)-
		ipv4_get_minn_addrn_with_prefix(ip,prefix)+1;
}
/*使用掩码前缀和地址结构获取主机地址数量*/
unsigned long ipv4_get_count_addr_with_prefix(struct ipv4_addr*ip,int prefix){
	return
		ipv4_get_maxn_addr_with_prefix(ip,prefix)-
		ipv4_get_minn_addr_with_prefix(ip,prefix)+1;
}
/*使用网络结构获取主机地址数量*/
unsigned long ipv4_get_count_addr_net(struct ipv4_net*net){
	return
		ipv4_get_maxn_addr_net(net)-
		ipv4_get_minn_addr_net(net)+1;
}
#ifdef __cplusplus
}
#endif
