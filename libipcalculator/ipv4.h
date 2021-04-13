#ifndef _IPV4_H
#define _IPV4_H
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define _STR_UNKNOWN "(Unknown)"
#ifdef __cplusplus
extern "C"{
#endif
#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _MSC_VER
#define EXTERN __declspec(dllexport)
#else
#define EXTERN extern
#endif

#ifndef _DEFINED_DEC2BIN
#define _DEFINED_DEC2BIN
/*十进制转二进制*/
EXTERN unsigned long dec2bin(unsigned long dec);
#endif

#ifndef _DEFINED_NEW_STRING
#define _DEFINED_NEW_STRING
/*新建字符串对象（注意释放）*/
EXTERN char*new_string(size_t size);
#endif

#ifndef _DEFINED_FREE_STRING
#define _DEFINED_FREE_STRING
/*释放字符串对象*/
EXTERN void free_string(char*ptr);
#endif

/*IPv4地址结构*/
struct ipv4_addr{unsigned char p0,p1,p2,p3;};

/*IPv4网络结构*/
struct ipv4_net{struct ipv4_addr*net,*mask;};

/*IPv4地址类*/
enum ipv4_class{
	IPV4_CLASS_UNKNOWN=0,
	IPV4_CLASS_A=0x111,
	IPV4_CLASS_B=0x112,
	IPV4_CLASS_C=0x113,
	IPV4_CLASS_D=0x114,
	IPV4_CLASS_E=0x115,
	IPV4_CLASS_M=0x116
};

/*IPv4保留地址块*/
enum ipv4_group{
	IPV4_GRP_UNKNOWN=0,
	IPV4_GRP_ANY=0x121,
	IPV4_GRP_LOOP=0x122,
	IPV4_GRP_CGN=0x123,
	IPV4_GRP_CHAIN=0x124,
	IPV4_GRP_BENCHMARK=0x125,
	IPV4_GRP_IP6TO4=0x126,
	IPV4_GRP_TEST_1=0x127,
	IPV4_GRP_TEST_2=0x128,
	IPV4_GRP_TEST_3=0x129,
	IPV4_GRP_IETF=0x12A,
	IPV4_GRP_LOCAL=0x12B,
	IPV4_GRP_EXPER=0x12C,
	IPV4_GRP_PUBLIC=0x12D,
	IPV4_GRP_BROADCAST=0x12E,
	IPV4_GRP_MULTICAST=0x12F
};

/*IPv4地址类型*/
enum ipv4_type{
	IPV4_TYPE_UNKNOWN=0,
	IPV4_TYPE_HOST=0x131,
	IPV4_TYPE_NET=0x132,
	IPV4_TYPE_BROADCAST=0x133
};

/*表格打印字符*/
struct display_char{
	const char
		*horizon,    *vertical,
		*top_left,   *top_center,   *top_right,
		*left,       *center,       *right,
		*bottom_left,*bottom_center,*bottom_right;
};

#ifdef __linux__
/*格式化输出IPv4网络为表格到FD*/
EXTERN void display_readable_table(int fd,struct ipv4_net*p,struct display_char*table);

/*格式化输出IPv4网络为变量到FD*/
EXTERN void display_var(int fd,struct ipv4_net*p);
#else
/*格式化输出IPv4网络为表格*/
EXTERN void display_readable_table(struct ipv4_net*p,struct display_char*table);

/*格式化输出IPv4网络为变量*/
EXTERN void display_var(struct ipv4_net*p);
#endif

/*新建空的地址结构（注意释放）*/
EXTERN struct ipv4_addr*ipv4_new_addr_obj(void);

/*新建空的网络结构（注意释放）*/
EXTERN struct ipv4_net*ipv4_new_net_obj(void);

/*设置地址结构的值*/
EXTERN struct ipv4_addr*ipv4_set_addr(
	struct ipv4_addr*ip,
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3
);

/*设置网络结构的值（使用掩码前缀）*/
EXTERN struct ipv4_net*ipv4_set_net(
	struct ipv4_net*net,
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3,
	unsigned char pr
);

/*设置网络结构的值（使用掩码地址）*/
EXTERN struct ipv4_net*ipv4_set_net_and_mask(
	struct ipv4_net*net,
	unsigned char i0,
	unsigned char i1,
	unsigned char i2,
	unsigned char i3,
	unsigned char n0,
	unsigned char n1,
	unsigned char n2,
	unsigned char n3
);

/*新建并设置地址结构（注意释放）*/
EXTERN struct ipv4_addr*ipv4_new_addr(
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3
);

/*新建并设置网络结构（使用掩码前缀）（注意释放）*/
EXTERN struct ipv4_net*ipv4_new_net(
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3,
	unsigned char pr
);

/*新建并设置网络结构（使用掩码地址）（注意释放）*/
EXTERN struct ipv4_net*ipv4_new_net_and_mask(
	unsigned char i0,
	unsigned char i1,
	unsigned char i2,
	unsigned char i3,
	unsigned char n0,
	unsigned char n1,
	unsigned char n2,
	unsigned char n3
);

/*判断地址是否在网段的地址池内*/
EXTERN bool ipv4_is_addr_in_pool(struct ipv4_net*pool,struct ipv4_addr*ip);

/*判断地址是否在网段内（包括网络地址和广播地址）*/
EXTERN bool ipv4_is_addr_in_net(struct ipv4_net*pool,struct ipv4_addr*ip);

/*判断地址是否在字符串网段的地址池内*/
EXTERN bool ipv4_is_addr_in_pool_x(char*pool,struct ipv4_addr*ip);

/*判断地址是否在字符串网段内（包括网络地址和广播地址）*/
EXTERN bool ipv4_is_addr_in_net_x(char*pool,struct ipv4_addr*ip);

/*判断地址是否在指定网段的地址池内*/
EXTERN bool ipv4_is_addr_in_pool_px(
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3,
	unsigned char pr,
	struct ipv4_addr*ip
);

/*判断地址是否在指定网段内（包括网络地址和广播地址）*/
EXTERN bool ipv4_is_addr_in_net_px(
	unsigned char p0,
	unsigned char p1,
	unsigned char p2,
	unsigned char p3,
	unsigned char pr,
	struct ipv4_addr*ip
);

/*将地址交换*/
EXTERN void ipv4_addr_swap(struct ipv4_addr*ip);

/*将地址反转*/
EXTERN void ipv4_addr_reverse(struct ipv4_addr*ip);

/*将地址结构转换为十进制字符串地址（传入缓冲和长度）*/
EXTERN char*ipv4_addr_to_string_xn(struct ipv4_addr*ip,char*buff,size_t s);

/*将地址结构转换为十六进制字符串地址（传入缓冲和长度）*/
EXTERN char*ipv4_addr_to_hex_string_xn(struct ipv4_addr*ip,char*buff,size_t s);

/*将地址结构转换为二进制字符串地址（传入缓冲和长度）*/
EXTERN char*ipv4_addr_to_bin_string_xn(struct ipv4_addr*ip,char*buff,size_t s);

/*将网络结构转换为十进制字符串地址（传入缓冲和长度）*/
EXTERN char*ipv4_net_to_string_xn(struct ipv4_net*net,char*buff,size_t s);

/*将地址结构转换为十进制字符串地址（注意释放）*/
EXTERN char*ipv4_addr_to_string(struct ipv4_addr*ip);

/*将地址结构转换为十六进制字符串地址（注意释放）*/
EXTERN char*ipv4_addr_to_hex_string(struct ipv4_addr*ip);

/*将地址结构转换为十进制字符串地址（注意释放）*/
EXTERN char*ipv4_addr_to_bin_string(struct ipv4_addr*ip);

/*将网络结构转换为十进制字符串地址（注意释放）*/
EXTERN char*ipv4_net_to_string(struct ipv4_net*net);

/*将十进制字符串地址转换为地址结构（传入缓冲）*/
EXTERN struct ipv4_addr*ipv4_string_to_addr_x(char*ip,struct ipv4_addr*buff);

/*将十进制字符串网络转换为网络结构（使用掩码前缀）（传入缓冲）*/
EXTERN struct ipv4_net*ipv4_string_to_net_with_prefix_x(char*ip,struct ipv4_net*buff);

/*将十进制字符串网络转换为网络结构（使用掩码地址）（传入缓冲）*/
EXTERN struct ipv4_net*ipv4_string_to_net_with_mask_x(char*ip,struct ipv4_net*buff);

/*将十进制字符串地址转换为地址结构（注意释放）*/
EXTERN struct ipv4_addr*ipv4_string_to_addr(char*ip);

/*将十进制字符串网络转换为网络结构（使用掩码前缀）（注意释放）*/
EXTERN struct ipv4_net*ipv4_string_to_net_with_prefix(char*ip);

/*将十进制字符串网络转换为网络结构（使用掩码地址）（注意释放）*/
EXTERN struct ipv4_net*ipv4_string_to_net_with_mask(char*ip);

/*将数字地址转换为地址结构（传入缓冲）*/
EXTERN struct ipv4_addr*ipv4_long_to_addr_x(unsigned long ip,struct ipv4_addr*buff);

/*将数字掩码地址转换为掩码前缀*/
EXTERN int ipv4_maskn_to_prefix(unsigned long mask);

/*将掩码前缀转换为掩码地址（注意释放）*/
EXTERN struct ipv4_addr*ipv4_prefix_to_mask(int prefix);

/*将掩码前缀转换为掩码地址（传入缓冲）*/
EXTERN struct ipv4_addr*ipv4_prefix_to_mask_x(struct ipv4_addr*buff,int prefix);

/*判断IPv4地址的所在地址类*/
EXTERN enum ipv4_class ipv4_get_addr_class(struct ipv4_addr*ip);

/*判断IPv4地址的地址类型*/
EXTERN enum ipv4_type ipv4_get_addr_type(struct ipv4_net*net,struct ipv4_addr*ip);

/*获取IPv4地址处于哪个保留地址块*/
EXTERN enum ipv4_group ipv4_get_addr_group(struct ipv4_addr*ip);

/*将地址类转换为短字符串*/
EXTERN const char*ipv4_class_to_short_string(enum ipv4_class type);

/*将地址类转换为可读字符串*/
EXTERN const char*ipv4_class_to_string(enum ipv4_class _class);

/*将地址类型转换为短字符串*/
EXTERN const char*ipv4_type_to_short_string(enum ipv4_type type);

/*将地址类型转换为可读字符串*/
EXTERN const char*ipv4_type_to_string(enum ipv4_type type);

/*将地址块转换为短字符串*/
EXTERN const char*ipv4_group_to_short_string(enum ipv4_group group);

/*将地址块转换为可读字符串*/
EXTERN const char*ipv4_group_to_string(enum ipv4_group group);

/*将地址结构转换为数字地址*/
EXTERN unsigned long ipv4_addr_to_long(struct ipv4_addr*ip);

/*将掩码前缀转换为数字掩码地址*/
EXTERN unsigned long ipv4_prefix_to_maskn(int prefix);

/*将掩码前缀转换为数字通配符地址*/
EXTERN unsigned long ipv4_prefix_to_wildcardn(int prefix);

/*使用数字掩码地址和数字地址获取数字广播地址*/
EXTERN unsigned long ipv4_get_brdn_addrn_with_maskn(unsigned long ip,unsigned long mask);

/*使用掩码前缀结构和数字地址获取数字广播地址*/
EXTERN unsigned long ipv4_get_brdn_addrn_with_prefix(unsigned long ip,int prefix);

/*使用网络结构获取数字网络地址*/
EXTERN unsigned long ipv4_get_netn_addr_net(struct ipv4_net*net);

/*使用网络结构获取数字广播地址*/
EXTERN unsigned long ipv4_get_brdn_addr_net(struct ipv4_net*net);

/*使用网络结构获取网络地址结构（注意释放）*/
EXTERN struct ipv4_addr*ipv4_get_net_addr_net(struct ipv4_net*net);

/*使用网络结构获取广播地址结构（注意释放）*/
EXTERN struct ipv4_addr*ipv4_get_brd_addr_net(struct ipv4_net*net);

/*使用数字掩码地址和数字地址获取数字最小（起始）地址*/
EXTERN unsigned long ipv4_get_minn_addrn_with_maskn(unsigned long ip,unsigned long mask);

/*使用数字掩码地址和数字地址获取数字最大（结束）地址*/
EXTERN unsigned long ipv4_get_maxn_addrn_with_maskn(unsigned long ip,unsigned long mask);

/*使用掩码前缀和数字地址获取数字最小（起始）地址*/
EXTERN unsigned long ipv4_get_minn_addrn_with_prefix(unsigned long ip,int prefix);

/*使用掩码前缀和数字地址获取数字最大（结束）地址*/
EXTERN unsigned long ipv4_get_maxn_addrn_with_prefix(unsigned long ip,int prefix);

/*使用网络结构获取数字最小（起始）地址*/
EXTERN unsigned long ipv4_get_minn_addr_net(struct ipv4_net*net);

/*使用网络结构获取数字最大（结束）地址*/
EXTERN unsigned long ipv4_get_maxn_addr_net(struct ipv4_net*net);

/*使用网络结构获取最小（起始）地址（注意释放）*/
EXTERN struct ipv4_addr*ipv4_get_min_addr_net(struct ipv4_net*net);

/*使用网络结构获取最大（结束）地址（注意释放）*/
EXTERN struct ipv4_addr*ipv4_get_max_addr_net(struct ipv4_net*net);

/*使用数字掩码地址和数字地址获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addrn_with_maskn(unsigned long ip,unsigned long mask);

/*使用掩码地址结构和数字地址获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addrn_with_mask(unsigned long ip,struct ipv4_addr*mask);

/*使用数字掩码地址和地址结构获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addr_with_maskn(struct ipv4_addr*ip,unsigned long mask);

/*使用掩码地址结构和地址结构获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addr_with_mask(struct ipv4_addr*ip,struct ipv4_addr*mask);

/*使用掩码前缀和数字地址获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addrn_with_prefix(unsigned long ip,int prefix);

/*使用掩码前缀和地址结构获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addr_with_prefix(struct ipv4_addr*ip,int prefix);

/*使用网络结构获取主机地址数量*/
EXTERN unsigned long ipv4_get_count_addr_net(struct ipv4_net*net);

/*判断地址是否为网络地址*/
EXTERN bool ipv4_is_addr_network_address(struct ipv4_net*net,struct ipv4_addr*ip);

/*判断地址是否为广播地址*/
EXTERN bool ipv4_is_addr_broadcast_address(struct ipv4_net*net,struct ipv4_addr*ip);

/*判断地址是否为有效地址*/
EXTERN bool ipv4_is_addr_usable(struct ipv4_addr*ip);

/*判断地址是否为掩码地址*/
EXTERN bool ipv4_addr_is_maskn(unsigned long i);

/*释放地址结构*/
EXTERN void ipv4_free_addr(struct ipv4_addr*ip);

/*释放网络结构*/
EXTERN void ipv4_free_net(struct ipv4_net*net);

/*复制地址结构*/
EXTERN struct ipv4_addr*ipv4_copy_addr(struct ipv4_addr*dst,struct ipv4_addr*src);

/*复制网络结构*/
EXTERN struct ipv4_net*ipv4_copy_net(struct ipv4_net*dst,struct ipv4_net*src);

/*克隆网络结构（注意释放）*/
EXTERN struct ipv4_net*ipv4_clone_net(struct ipv4_net*src);

/*比较地址结构*/
EXTERN bool ipv4_compare_addr(struct ipv4_addr*s1,struct ipv4_addr*s2);

/*比较网络结构*/
EXTERN bool ipv4_compare_net(struct ipv4_net*s1,struct ipv4_net*s2);

/*判断IPv4地址字符串是否有效*/
EXTERN bool ipv4_check_addr_length(const char*ip);

/*判断是否为IPv4地址的有效部分*/
EXTERN bool ipv4_check_addr_int_part(long part);

/*检查网络是否为空*/
EXTERN bool ipv4_check_net(struct ipv4_net*n);

/*克隆地址结构（注意释放）*/
#define ipv4_clone_addr(src)                            ipv4_copy_addr(new_ipv4_obj(),src)

/*将地址结构转换为十进制字符串地址（传入缓冲）*/
#define ipv4_to_string_x(ip,buff)                       ipv4_addr_to_string_xn(ip,buff,16)

/*将数字地址转换为地址结构（注意释放）*/
#define ipv4_long_to_addr(ip)                           ipv4_long_to_addr_x(ip,ipv4_new_addr_obj())

/*将数字掩码地址转换为数字通配符地址*/
#define ipv4_maskn_to_wildcardn(mask)                   ipv4_prefix_to_wildcardn(ipv4_maskn_to_prefix(mask))

/*将掩码地址转换为数字通配符地址*/
#define ipv4_mask_to_wildcardn(mask)                    ipv4_maskn_to_wildcardn(ipv4_addr_to_long(mask))

/*将掩码地址转换为掩码前缀*/
#define ipv4_mask_to_prefix(mask)                       ipv4_maskn_to_prefix(ipv4_addr_to_long(mask))

/*将掩码前缀转换为通配符地址（注意释放）*/
#define ipv4_prefix_to_wildcard(prefix)                 ipv4_long_to_addr(ipv4_prefix_to_wildcardn(prefix))

/*将数字掩码地址转换为通配符地址（注意释放）*/
#define ipv4_maskn_to_wildcard(mask)                    ipv4_long_to_addr(ipv4_maskn_to_wildcardn(mask))

/*将掩码地址转换为通配符地址（注意释放）*/
#define ipv4_mask_to_wildcard(mask)                     ipv4_long_to_addr(ipv4_mask_to_wildcardn(mask))

/*将掩码前缀转换为数字掩码地址（传入缓冲）*/
#define ipv4_prefix_to_maskx(buff,prefix)               ipv4_long_to_addr_x(ipv4_prefix_to_maskn(prefix),buff)

/*将掩码前缀转换为数字通配符地址（传入缓冲）*/
#define ipv4_prefix_to_wildcardx(buff,prefix)           ipv4_long_to_addr_x(ipv4_prefix_to_wildcardn(prefix),buff)

/*将数字掩码地址转换为通配符地址（传入缓冲）*/
#define ipv4_maskn_to_wildcardx(buff,mask)              ipv4_long_to_addr_x(ipv4_maskn_to_wildcardn(mask),buff)

/*将掩码地址转换为通配符地址（传入缓冲）*/
#define ipv4_mask_to_wildcardx(buff,mask)               ipv4_long_to_addr_x(ipv4_mask_to_wildcardn(mask),buff)

/*使用数字地址和数字掩码地址获取数字网络地址*/
#define ipv4_get_netn_addrn_with_maskn(ip,mask)        (ip&mask)

/*使用数字地址和掩码前缀获取数字网络地址*/
#define ipv4_get_netn_addrn_with_prefix(ip,prefix)     (ip&ipv4_prefix_to_maskn(prefix))

/*使用地址和数字掩码地址获取数字网络地址*/
#define ipv4_get_netn_addr_with_maskn(ip,mask)          ipv4_get_netn_addrn_with_maskn(ipv4_addr_to_long(ip),mask)

/*使用地址和数字掩码地址获取数字广播地址*/
#define ipv4_get_brdn_addr_with_maskn(ip,mask)          ipv4_get_brdn_addrn_with_maskn(ipv4_addr_to_long(ip),mask)

/*使用地址和掩码地址获取数字网络地址*/
#define ipv4_get_netn_addr_with_mask(ip,mask)           ipv4_get_netn_addr_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用地址和掩码地址获取数字广播地址*/
#define ipv4_get_brdn_addr_with_mask(ip,mask)           ipv4_get_brdn_addr_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用地址和掩码前缀获取数字网络地址*/
#define ipv4_get_netn_addr_with_prefix(ip,prefix)       ipv4_get_netn_addr_with_maskn(ip,ipv4_prefix_to_maskn(prefix))

/*使用地址和掩码前缀获取数字广播地址*/
#define ipv4_get_brdn_addr_with_prefix(ip,prefix)       ipv4_get_brdn_addr_with_maskn(ip,ipv4_prefix_to_maskn(prefix))

/*使用数字地址和数字掩码获取网络地址（注意释放）*/
#define ipv4_get_net_addrn_with_maskn(ip,mask)          ipv4_long_to_addr(ipv4_get_netn_addrn_with_maskn(ip,mask))

/*使用数字地址和掩码前缀获取网络地址（注意释放）*/
#define ipv4_get_net_addrn_with_prefix(ip,prefix)       ipv4_long_to_addr(ipv4_get_netn_addrn_with_prefix(ip,prefix))

/*使用数字掩码地址和数字地址获取广播地址（注意释放）*/
#define ipv4_get_brd_addrn_with_maskn(ip,mask)          ipv4_long_to_addr(ipv4_get_brdn_addrn_with_maskn(ip,mask))

/*使用掩码前缀和数字地址获取广播地址（注意释放）*/
#define ipv4_get_brd_addrn_with_prefix(ip,prefix)       ipv4_long_to_addr(ipv4_get_brdn_addrn_with_prefix(ip,prefix))

/*使用地址和数字掩码地址获取网络地址（注意释放）*/
#define ipv4_get_net_addr_with_maskn(ip,mask)           ipv4_long_to_addr(ipv4_get_netn_addr_with_maskn(ip,mask))

/*使用地址和数字掩码地址获取广播地址（注意释放）*/
#define ipv4_get_brd_addr_with_maskn(ip,mask)           ipv4_long_to_addr(ipv4_get_brdn_addr_with_maskn(ip,mask))

/*使用地址和掩码地址获取网络地址（注意释放）*/
#define ipv4_get_net_addr_with_mask(ip,mask)            ipv4_long_to_addr(ipv4_get_netn_addr_with_mask(ip,mask))

/*使用地址和掩码地址获取广播地址（注意释放）*/
#define ipv4_get_brd_addr_with_mask(ip,mask)            ipv4_long_to_addr(ipv4_get_brdn_addr_with_mask(ip,mask))

/*使用地址和掩码前缀获取网络地址（注意释放）*/
#define ipv4_get_net_addr_with_prefix(ip,prefix)        ipv4_long_to_addr(ipv4_get_netn_addr_with_prefix(ip,prefix))

/*使用地址和掩码前缀获取数字广播地址（注意释放）*/
#define ipv4_get_brd_addr_with_prefix(ip,prefix)        ipv4_long_to_addr(ipv4_get_brdn_addr_with_prefix(ip,prefix))

/*使用掩码地址和数字地址获取数字最小（起始）地址*/
#define ipv4_get_minn_addrn_with_mask(ip,mask)          ipv4_get_minn_addrn_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用掩码地址和数字地址获取数字最大（结束）地址*/
#define ipv4_get_maxn_addrn_with_mask(ip,mask)          ipv4_get_maxn_addrn_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用数字掩码地址和地址获取数字最小（起始）地址*/
#define ipv4_get_minn_addr_with_maskn(ip,mask)          ipv4_get_minn_addrn_with_maskn(ipv4_addr_to_long(ip),mask)

/*使用数字掩码地址和地址获取数字最大（结束）地址*/
#define ipv4_get_maxn_addr_with_maskn(ip,mask)          ipv4_get_maxn_addrn_with_maskn(ipv4_addr_to_long(ip),mask)

/*使用掩码前缀和地址获取数字最小（起始）地址*/
#define ipv4_get_minn_addr_with_prefix(ip,prefix)       ipv4_get_minn_addr_with_maskn(ip,ipv4_prefix_to_maskn(prefix))

/*使用掩码前缀和地址获取数字最大（结束）地址*/
#define ipv4_get_maxn_addr_with_prefix(ip,prefix)       ipv4_get_maxn_addrn_with_prefix(ipv4_addr_to_long(ip),prefix)

/*使用掩码地址和地址获取数字最小（起始）地址*/
#define ipv4_get_minn_addr_with_mask(ip,mask)           ipv4_get_minn_addr_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用掩码地址和地址获取数字最大（结束）地址*/
#define ipv4_get_maxn_addr_with_mask(ip,mask)           ipv4_get_maxn_addr_with_maskn(ip,ipv4_addr_to_long(mask))

/*使用数字掩码地址和数字地址获取最小（起始）地址（传入缓冲）*/
#define ipv4_get_minx_addrn_with_maskn(buff,ip,mask)    ipv4_long_to_addr_x(ipv4_get_minn_addrn_with_maskn(ip,mask),buff)

/*使用数字掩码地址和数字地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addrn_with_maskn(buff,ip,mask)    ipv4_long_to_addr_x(ipv4_get_maxn_addrn_with_maskn(ip,mask),buff)

/*使用掩码地址和数字地址获取最小（起始）地址（传入缓冲）*/
#define ipv4_get_minx_addrn_with_mask(buff,ip,mask)     ipv4_long_to_addr_x(ipv4_get_minn_addrn_with_mask(ip,mask),buff)

/*使用掩码地址和数字地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addrn_with_mask(buff,ip,mask)     ipv4_long_to_addr_x(ipv4_get_maxn_addrn_with_mask(ip,mask),buff)

/*使用掩码前缀和数字地址获取最小（起始）地址（传入缓冲）*/
#define ipv4_get_minx_addrn_with_prefix(buff,ip,prefix) ipv4_long_to_addr_x(ipv4_get_minn_addrn_with_prefix(ip,prefix),buff)

/*使用掩码前缀和数字地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addrn_with_prefix(buff,ip,prefix) ipv4_long_to_addr_x(ipv4_get_maxn_addrn_with_prefix(ip,prefix),buff)

/*使用数字掩码地址和地址获取最小（起始）地址（传入缓冲）*/
#define ipv4_get_minx_addr_with_maskn(buff,ip,mask)     ipv4_long_to_addr_x(ipv4_get_minn_addr_with_maskn(ip,mask),buff)

/*使用数字掩码地址和地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addr_with_maskn(buff,ip,mask)     ipv4_long_to_addr_x(ipv4_get_maxn_addr_with_maskn(ip,mask),buff)

/*使用掩码前缀和地址获取最小（起始）地（传入缓冲）*/
#define ipv4_get_minx_addr_with_prefix(buff,ip,prefix)  ipv4_long_to_addr_x(ipv4_get_minn_addr_with_prefix(ip,prefix),buff)

/*使用掩码前缀和地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addr_with_prefix(buff,ip,prefix)  ipv4_long_to_addr_x(ipv4_get_maxn_addr_with_prefix(ip,prefix),buff)

/*使用掩码地址和地址获取最小（起始）地址（传入缓冲）*/
#define ipv4_get_minx_addr_with_mask(buff,ip,mask)      ipv4_long_to_addr_x(ipv4_get_minn_addr_with_mask(ip,mask),buff)

/*使用掩码地址和地址获取最大（结束）地址（传入缓冲）*/
#define ipv4_get_maxx_addr_with_mask(buff,ip,mask)      ipv4_long_to_addr_x(ipv4_get_maxn_addr_with_mask(ip,mask),buff)

/*使用数字掩码地址和数字地址获取最小（起始）地址（注意释放）*/
#define ipv4_get_min_addrn_with_maskn(ip,mask)          ipv4_long_to_addr(ipv4_get_minn_addrn_with_maskn(ip,mask))

/*使用数字掩码地址和数字地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addrn_with_maskn(ip,mask)          ipv4_long_to_addr(ipv4_get_maxn_addrn_with_maskn(ip,mask))

/*使用掩码地址和数字地址获取最小（起始）地址（注意释放）*/
#define ipv4_get_min_addrn_with_mask(ip,mask)           ipv4_long_to_addr(ipv4_get_minn_addrn_with_mask(ip,mask))

/*使用掩码地址和数字地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addrn_with_mask(ip,mask)           ipv4_long_to_addr(ipv4_get_maxn_addrn_with_mask(ip,mask))

/*使用掩码前缀和数字地址获取最小（起始）地址（注意释放）*/
#define ipv4_get_min_addrn_with_prefix(ip,prefix)       ipv4_long_to_addr(ipv4_get_minn_addrn_with_prefix(ip,prefix))

/*使用掩码前缀和数字地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addrn_with_prefix(ip,prefix)       ipv4_long_to_addr(ipv4_get_maxn_addrn_with_prefix(ip,prefix))

/*使用数字掩码地址和地址获取最小（起始）地址（注意释放）*/
#define ipv4_get_min_addr_with_maskn(ip,mask)           ipv4_long_to_addr(ipv4_get_minn_addr_with_maskn(ip,mask))

/*使用数字掩码地址和地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addr_with_maskn(ip,mask)           ipv4_long_to_addr(ipv4_get_maxn_addr_with_maskn(ip,mask))

/*使用掩码前缀和地址获取最小（起始）地（注意释放）*/
#define ipv4_get_min_addr_with_prefix(ip,prefix)        ipv4_long_to_addr(ipv4_get_minn_addr_with_prefix(ip,prefix))

/*使用掩码前缀和地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addr_with_prefix(ip,prefix)        ipv4_long_to_addr(ipv4_get_maxn_addr_with_prefix(ip,prefix))

/*使用掩码地址和地址获取最小（起始）地址（注意释放）*/
#define ipv4_get_min_addr_with_mask(ip,mask)            ipv4_long_to_addr(ipv4_get_minn_addr_with_mask(ip,mask))

/*使用掩码地址和地址获取最大（结束）地址（注意释放）*/
#define ipv4_get_max_addr_with_mask(ip,mask)            ipv4_long_to_addr(ipv4_get_maxn_addr_with_mask(ip,mask))

/*将地址转换为地址类短字符串*/
#define ipv4_get_addr_class_short_string(ip)            ipv4_class_to_short_string(ipv4_get_addr_class(ip))

/*将地址转换为地址类字符串*/
#define ipv4_get_addr_class_string(ip)                  ipv4_class_to_string(ipv4_get_addr_class(ip))

/*将地址转换为保留地址块短字符串*/
#define ipv4_get_addr_group_short_string(ip)            ipv4_group_to_short_string(ipv4_get_addr_group(ip))

/*将地址转换为保留地址块字符串*/
#define ipv4_get_addr_group_string(ip)                  ipv4_group_to_string(ipv4_get_addr_group(ip))

/*将网络转换为地址类型短字符串*/
#define ipv4_get_addr_type_short_string(net,ip)         ipv4_type_to_short_string(ipv4_get_addr_type(net,ip))

/*将网络转换为地址类型字符串*/
#define ipv4_get_addr_type_string(net,ip)               ipv4_type_to_string(ipv4_get_addr_type(net,ip))

/*判断IPv4地址是否属于某个地址类*/
#define ipv4_addr_is_class(ip,_class)                   ipv4_get_addr_class(ip)==_class

/*判断IPv4地址是否属于某个保留地址块*/
#define ipv4_addr_is_group(ip, grp)                     ipv4_get_addr_group(ip)==grp

/*判断IPv4网络是否属于某个地址类型*/
#define ipv4_addr_is_type(net,ip,type)                  ipv4_get_addr_type(net,ip)==type

/*判断IPv4地址是不是一个掩码*/
#define ipv4_addr_is_mask(ip)                           ipv4_addr_is_maskn(ipv4_addr_to_long(ip))

/*将地址结构转换为十进制字符串地址（注意释放）*/
#define ipv4_dec_to_string                              ipv4_addr_to_string

/*将地址结构转换为十进制字符串地址（传入缓冲和长度）*/
#define ipv4_dec_to_string_xn                           ipv4_addr_to_string_xn

/*复制地址结构*/
#define ipv4_cpy_addr                                   ipv4_copy_addr

/*复制地址结构*/
#define ipv4_addrcpy                                    ipv4_copy_addr

/*复制网络结构*/
#define ipv4_cpy_net                                    ipv4_copy_net

/*复制网络结构*/
#define ipv4_netcpy                                     ipv4_copy_net

/*克隆地址结构（注意释放）*/
#define ipv4_dup_addr                                   ipv4_clone_addr

/*克隆地址结构（注意释放）*/
#define ipv4_addrdup                                    ipv4_clone_addr

/*克隆网络结构（注意释放）*/
#define ipv4_dup_net                                    ipv4_clone_net

/*克隆网络结构（注意释放）*/
#define ipv4_netdup                                     ipv4_clone_net

/*比较地址结构*/
#define ipv4_cmp_addr                                   ipv4_compare_addr

/*比较地址结构*/
#define ipv4_addrcmp                                    ipv4_compare_addr

/*比较网络结构*/
#define ipv4_cmp_net                                    ipv4_compare_net

/*比较网络结构*/
#define ipv4_netcmp                                     ipv4_compare_net

/*判断IPv4地址的数字是否有效*/
#define ipv4_check_addr_int(i0,i1,i2,i3)               (ipv4_check_addr_int_part(i0)&&\
                                                        ipv4_check_addr_int_part(i1)&&\
                                                        ipv4_check_addr_int_part(i2)&&\
                                                        ipv4_check_addr_int_part(i3))
#ifdef __cplusplus
}
#endif
#endif
