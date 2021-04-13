#include"../global.h"
#include"ipv4.h"
#ifdef __cplusplus
extern "C"{
#endif
/*判断IPv4地址的所在地址类*/
enum ipv4_class ipv4_get_addr_class(struct ipv4_addr*ip){
	if(ip->p0<=127)return IPV4_CLASS_A;     /*0-127   A类*/
	else if(ip->p0<=191)return IPV4_CLASS_B;/*128-191 B类*/
	else if(ip->p0<=223)return IPV4_CLASS_C;/*192-223 C类*/
	else if(ip->p0<=239)return IPV4_CLASS_D;/*224-239 D类*/
	else if(ip->p0<=254)return IPV4_CLASS_E;/*240-254 E类*/
	else if(ip->p0==255)return IPV4_CLASS_M;/*255     广播*/
	else return IPV4_CLASS_UNKNOWN;         /*未知（不可能出现）*/
}
/*判断IPv4地址的地址类型*/
enum ipv4_type ipv4_get_addr_type(struct ipv4_net*net,struct ipv4_addr*ip){
	if(!ipv4_check_net(net)||!ip)return IPV4_TYPE_UNKNOWN;/*无效地址*/
	unsigned long
		b=ipv4_get_brdn_addr_net(net),/*获取网段的广播地址*/
		n=ipv4_get_netn_addr_net(net),/*获取网段的网络地址*/
		i=ipv4_addr_to_long(ip);
	if((i<b&&i>n)||ipv4_mask_to_prefix(net->mask)==32)return IPV4_TYPE_HOST;/*主机地址*/
	else if(i==b)return IPV4_TYPE_BROADCAST;/*广播地址*/
	else if(i==n)return IPV4_TYPE_NET;/*网络地址*/
	else return IPV4_TYPE_UNKNOWN;/*未知*/
}
/*获取IPv4地址处于哪个保留地址块*/
enum ipv4_group ipv4_get_addr_group(struct ipv4_addr*ip){
	if(!ip)return IPV4_GRP_UNKNOWN;/*无效地址*/
	if(     ipv4_is_addr_in_net_px(0,0,0,0,       8,ip))return IPV4_GRP_ANY;
	else if(ipv4_is_addr_in_net_px(10,0,0,0,      8,ip))return IPV4_GRP_LOCAL;
	else if(ipv4_is_addr_in_net_px(100,64,0,0,   10,ip))return IPV4_GRP_CGN;
	else if(ipv4_is_addr_in_net_px(127,0,0,0,     8,ip))return IPV4_GRP_LOOP;
	else if(ipv4_is_addr_in_net_px(169,254,0,0,  16,ip))return IPV4_GRP_CHAIN;
	else if(ipv4_is_addr_in_net_px(172,16,0,0,   12,ip))return IPV4_GRP_LOCAL;
	else if(ipv4_is_addr_in_net_px(192,0,0,0,    24,ip))return IPV4_GRP_IETF;
	else if(ipv4_is_addr_in_net_px(192,0,2,0,    24,ip))return IPV4_GRP_TEST_1;
	else if(ipv4_is_addr_in_net_px(192,88,99,0,  24,ip))return IPV4_GRP_IP6TO4;
	else if(ipv4_is_addr_in_net_px(192,168,0,0,  16,ip))return IPV4_GRP_LOCAL;
	else if(ipv4_is_addr_in_net_px(198,18,0,0,   15,ip))return IPV4_GRP_BENCHMARK;
	else if(ipv4_is_addr_in_net_px(198,51,100,0, 24,ip))return IPV4_GRP_TEST_2;
	else if(ipv4_is_addr_in_net_px(203,0,113,0,  24,ip))return IPV4_GRP_TEST_3;
	else if(ipv4_is_addr_in_net_px(224,0,0,0,     4,ip))return IPV4_GRP_MULTICAST;
	else if(ipv4_is_addr_in_net_px(240,0,0,0,     4,ip))return IPV4_GRP_EXPER;
	else if(ipv4_is_addr_in_net_px(254,0,0,0,     7,ip))return IPV4_GRP_BROADCAST;
	else return IPV4_GRP_PUBLIC;
}
/*将地址类转换为短字符串*/
const char*ipv4_class_to_short_string(enum ipv4_class type){
	switch(type){
		case IPV4_CLASS_A:return "A";
		case IPV4_CLASS_B:return "B";
		case IPV4_CLASS_C:return "C";
		case IPV4_CLASS_D:return "D";
		case IPV4_CLASS_E:return "E";
		case IPV4_CLASS_M:return "M";
		default:return "";
	}
}
/*将地址类转换为可读字符串*/
const char*ipv4_class_to_string(enum ipv4_class _class){
	switch(_class){
		case IPV4_CLASS_A:return "Class A";
		case IPV4_CLASS_B:return "Class B";
		case IPV4_CLASS_C:return "Class C";
		case IPV4_CLASS_D:return "Class D";
		case IPV4_CLASS_E:return "Class E";
		case IPV4_CLASS_M:return "Broadcast";
		default:return _STR_UNKNOWN;
	}
}
/*将地址类型转换为短字符串*/
const char*ipv4_type_to_short_string(enum ipv4_type type){
	switch(type){
		case IPV4_TYPE_NET:return "NET";
		case IPV4_TYPE_HOST:return "HOST";
		case IPV4_TYPE_BROADCAST:return "BROADCAST";
		default:return "";
	}
}
/*将地址类型转换为可读字符串*/
const char*ipv4_type_to_string(enum ipv4_type type){
	switch(type){
		case IPV4_TYPE_NET:return "Network address";
		case IPV4_TYPE_HOST:return "Host address";
		case IPV4_TYPE_BROADCAST:return "Broadcast address";
		default:return _STR_UNKNOWN;
	}
}
/*将地址块转换为短字符串*/
const char*ipv4_group_to_short_string(enum ipv4_group group){
	switch(group){
		case IPV4_GRP_ANY:return "ANY";
		case IPV4_GRP_LOOP:return "LOOP";
		case IPV4_GRP_CGN:return "CGN";
		case IPV4_GRP_CHAIN:return "CHAIN";
		case IPV4_GRP_BENCHMARK:return "BENCHMARK";
		case IPV4_GRP_IP6TO4:return "IP6TO4";
		case IPV4_GRP_TEST_1:return "TEST_1";
		case IPV4_GRP_TEST_2:return "TEST_2";
		case IPV4_GRP_TEST_3:return "TEST_3";
		case IPV4_GRP_IETF:return "IETF";
		case IPV4_GRP_LOCAL:return "LOCAL";
		case IPV4_GRP_EXPER:return "EXPER";
		case IPV4_GRP_PUBLIC:return "PUBLIC";
		case IPV4_GRP_BROADCAST:return "BROADCAST";
		case IPV4_GRP_MULTICAST:return "MULTICAST";
		default:return "";
	}
}
/*将地址块转换为可读字符串*/
const char*ipv4_group_to_string(enum ipv4_group group){
	switch(group){
		case IPV4_GRP_ANY:return "RFC1122 This host on this network";
		case IPV4_GRP_LOOP:return "RFC1122 Loopback";
		case IPV4_GRP_CGN:return "RFC6598 Shared Address Space";
		case IPV4_GRP_CHAIN:return "RFC3927 Link Local";
		case IPV4_GRP_BENCHMARK:return "RFC2544 Benchmarking";
		case IPV4_GRP_IP6TO4:return "RFC3068 6to4 Relay Anycast";
		case IPV4_GRP_TEST_1:return "RFC5737 Documentation (TEST-NET-1)";
		case IPV4_GRP_TEST_2:return "RFC5737 Documentation (TEST-NET-2)";
		case IPV4_GRP_TEST_3:return "RFC5737 Documentation (TEST-NET-3)";
		case IPV4_GRP_IETF:return "RFC6890 IETF Protocol Assignments";
		case IPV4_GRP_LOCAL:return "RFC1918 Private-Use";
		case IPV4_GRP_EXPER:return "RFC1112 Reserved";
		case IPV4_GRP_PUBLIC:return "Public network address";
		case IPV4_GRP_BROADCAST:return "BroadCast address";
		case IPV4_GRP_MULTICAST:return "MultiCast address";
		default:return _STR_UNKNOWN;
	}
}
#ifdef __cplusplus
}
#endif
