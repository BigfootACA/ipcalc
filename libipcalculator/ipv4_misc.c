#include"../global.h"
#include"ipv4.h"
#ifdef __cplusplus
extern "C"{
#endif
/*释放地址结构*/
void ipv4_free_addr(struct ipv4_addr*ip){
	if(!ip)return;
	free(ip);
	ip=NULL;
}
/*释放网络结构*/
void ipv4_free_net(struct ipv4_net*net){
	if(!net)return;
	ipv4_free_addr(net->net);net->net=NULL;
	ipv4_free_addr(net->mask);net->mask=NULL;
	free(net);
	net=NULL;
}
/*复制地址结构*/
struct ipv4_addr*ipv4_copy_addr(struct ipv4_addr*dst,struct ipv4_addr*src){
	if(!dst)return NULL;
	dst->p0=src->p0;
	dst->p1=src->p1;
	dst->p2=src->p2;
	dst->p3=src->p3;
	return dst;
}
/*复制网络结构*/
struct ipv4_net*ipv4_copy_net(struct ipv4_net*dst,struct ipv4_net*src){
	if(!dst)return NULL;
	if(src->net)ipv4_copy_addr(dst->net,src->net);
	if(src->mask)ipv4_copy_addr(dst->mask,src->mask);
	return dst;
}
/*克隆网络结构（注意释放）*/
struct ipv4_net*ipv4_clone_net(struct ipv4_net*src){
	struct ipv4_net*dst=ipv4_new_net_obj(),*ptr=ipv4_copy_net(dst,src);
	if(!ptr){
		if(dst)ipv4_free_net(dst);
		return NULL;
	}
	if(src->net)ipv4_copy_addr(dst->net,src->net);
	if(src->mask)ipv4_copy_addr(dst->mask,src->mask);
	return dst;
}
/*比较地址结构*/
bool ipv4_compare_addr(struct ipv4_addr*s1,struct ipv4_addr*s2){
	return s1==s2||(
		s1&&s2&&
		s1->p0==s2->p0&&
		s1->p1==s2->p1&&
		s1->p2==s2->p2&&
		s1->p3==s2->p3
	);
}
/*比较网络结构*/
bool ipv4_compare_net(struct ipv4_net*s1,struct ipv4_net*s2){
	return s1==s2||(
		s1&&s2&&
		ipv4_compare_addr(s1->net,s2->net)&&
		ipv4_compare_addr(s1->mask,s2->mask)
	);
}
#ifdef __cplusplus
}
#endif
