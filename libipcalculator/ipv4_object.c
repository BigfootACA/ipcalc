#include"../global.h"
#include"ipv4.h"
#ifdef __cplusplus
extern "C"{
#endif
/*新建空的地址结构（注意释放）*/
struct ipv4_addr*ipv4_new_addr_obj(void){
	size_t s=sizeof(struct ipv4_addr);
	struct ipv4_addr*buff=(struct ipv4_addr*)malloc(s);
	if(!buff)return NULL;
	memset(buff,0,s);
	return buff;
}
/*新建空的网络结构（注意释放）*/
struct ipv4_net*ipv4_new_net_obj(void){
	size_t s=sizeof(struct ipv4_net);
	struct ipv4_net*buff=(struct ipv4_net*)malloc(s);
	if(!buff)return NULL;
	memset(buff,0,s);
	buff->net=ipv4_new_addr_obj();
	buff->mask=ipv4_new_addr_obj();
	if(!buff->net||!buff->mask){
		ipv4_free_net(buff);
		return NULL;
	}
	return buff;
}
/*设置地址结构的值*/
struct ipv4_addr*ipv4_set_addr(
	struct ipv4_addr*ip,
	unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3
){
	if(!ip)return NULL;
	ip->p0=p0;ip->p1=p1;ip->p2=p2;ip->p3=p3;
	return ip;
}
/*设置网络结构的值（使用掩码前缀）*/
struct ipv4_net*ipv4_set_net(
	struct ipv4_net*net,
	unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3,
	unsigned char pr
){
	return
		!ipv4_check_net(net)||
		!ipv4_set_addr(net->net,p0,p1,p2,p3)||
		!ipv4_prefix_to_mask_x(net->mask,pr)||
		!ipv4_addr_is_mask(net->mask)
		?NULL:net;
}
/*设置网络结构的值（使用掩码地址）*/
struct ipv4_net*ipv4_set_net_and_mask(
	struct ipv4_net*net,
	unsigned char i0,unsigned char i1,unsigned char i2,unsigned char i3,
	unsigned char n0,unsigned char n1,unsigned char n2,unsigned char n3
){
	return
		!ipv4_check_net(net)||
		!ipv4_set_addr(net->net,i0,i1,i2,i3)||
		!ipv4_set_addr(net->mask,n0,n1,n2,n3)||
		!ipv4_addr_is_mask(net->mask)
		?NULL:net;
}
/*新建并设置地址结构（注意释放）*/
struct ipv4_addr*ipv4_new_addr(unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3){
	struct ipv4_addr*buff=ipv4_new_addr_obj();
	return buff?ipv4_set_addr(buff,p0,p1,p2,p3):NULL;
}
/*新建并设置网络结构（使用掩码前缀）（注意释放）*/
struct ipv4_net*ipv4_new_net(unsigned char p0,unsigned char p1,unsigned char p2,unsigned char p3,unsigned char pr){
	struct ipv4_net*buff=ipv4_new_net_obj(),*ptr;
	if(!buff)return NULL;
	ptr=ipv4_set_net(buff,p0,p1,p2,p3,pr);
	if(!ptr){
		ipv4_free_net(buff);
		return NULL;
	}
	return ptr;
}
/*新建并设置网络结构（使用掩码地址）（注意释放）*/
struct ipv4_net*ipv4_new_net_and_mask(
	unsigned char i0,unsigned char i1,unsigned char i2,unsigned char i3,
	unsigned char n0,unsigned char n1,unsigned char n2,unsigned char n3
){
	struct ipv4_net*buff=ipv4_new_net_obj(),*ptr;
	if(!buff)return NULL;
	ptr=ipv4_set_net_and_mask(buff,i0,i1,i2,i3,n0,n1,n2,n3);
	if(!ptr){
		ipv4_free_net(buff);
		return NULL;
	}
	return ptr;
}
/*将地址交换*/
void ipv4_addr_swap(struct ipv4_addr*ip){
	if(!ip)return;
	unsigned char c;
	c=ip->p0;ip->p0=ip->p3;ip->p3=c;
	c=ip->p1;ip->p1=ip->p2;ip->p2=c;
}
/*将地址反转*/
void ipv4_addr_reverse(struct ipv4_addr*ip){
	if(!ip)return;
	ipv4_long_to_addr_x(0xFFFFFFFF-ipv4_addr_to_long(ip),ip);
}
#ifdef __cplusplus
}
#endif
