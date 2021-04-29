#include"../global.h"
#ifndef _MSC_VER
#include<unistd.h>
#endif
#include"ipv4.h"
#include"ipcalc_internal.h"
#ifdef __cplusplus
extern "C"{
#endif
/*输出地址行*/
static size_t display_ipv4_line(int fd,int*cols,const char*de,const char*tag,struct ipv4_addr*ip){
	if(!cols||!ip||!tag)return 0;
	char *ss[]={
		(char*)tag,
		ipv4_addr_to_string(ip),
		ipv4_addr_to_hex_string(ip),
		ipv4_addr_to_bin_string(ip),
		ultostr(ipv4_addr_to_long(ip)),
		NULL
	};
	size_t s=print_string_line(fd,cols,(const char**)ss,de);
	free(ss[1]);free(ss[2]);free(ss[3]);free(ss[4]);
	return s;
}
/*输出字符串行*/
static INLINE size_t display_string_line(int fd,int*cols,const char*de,const char*tag,const char*val){
	if(!cols||!val||!tag)return 0;
	const char*ss[]={tag,val,NULL};
	size_t s=print_string_line(fd,cols,ss,de);
	return s;
}
/*输出地址行并释放*/
static INLINE size_t display_ipv4_free_line(int fd,int*cols,const char*de,const char*tag,struct ipv4_addr*ip){
	size_t s=display_ipv4_line(fd,cols,de,tag,ip);
	if(ip)ipv4_free_addr(ip);
	return s;
}
/*输出字符串行并释放*/
static INLINE size_t display_string_free_line(int fd,int*cols,const char*de,const char*tag,char*val){
	size_t s=display_string_line(fd,cols,de,tag,val);
	if(val)free(val);
	return s;
}
/*输出数字变量（合并变量名）*/
static INLINE void display_var_ulong_line_pre(int fd,const char*pre,const char*tag,unsigned long val){
	if(val&&(tag||pre)){
		if(pre)fd_print(fd,pre);
		if(tag)fd_print(fd,tag);
		fd_printf(fd,"=%lu\n",val);
	}
}
/*输出字符串变量（合并变量名）*/
static INLINE void display_var_string_line_pre(int fd,const char*pre,const char*tag,const char*val){
	if(val&&(tag||pre)){
		if(pre)fd_print(fd,pre);
		if(tag)fd_print(fd,tag);
		fd_printf(fd,"=%s\n",val);
	}
}
/*输出数字变量*/
#define display_var_ulong_line(fd,tag,val)display_var_ulong_line_pre(fd, NULL,tag,val)
/*输出字符串变量*/
#define display_var_string_line(fd,tag,val)display_var_string_line_pre(fd,NULL,tag,val)
/*输出数字行*/
#define display_ulong_line(fd,cols,de,tag,val) display_string_free_line(fd,cols,de,tag,ultostr(val))
/*输出字符串变量并释放*/
static INLINE void display_var_string_free_line(int fd,const char*tag,char*val){
	display_var_string_line(fd,tag,val);
	if(val)free(val);
}
/*输出字符串变量并释放（合并变量名）*/
static INLINE void display_var_string_free_line_pre(int fd,const char*pre,const char*tag,char*val){
	display_var_string_line_pre(fd,pre,tag,val);
	if(val)free(val);
}
/*输出地址变量*/
static void display_var_ipv4_line(int fd,const char*tag,struct ipv4_addr*val){
	if(!val||!tag)return;
	display_var_string_free_line_pre(fd,tag,NULL,ipv4_addr_to_string(val));
	display_var_string_free_line_pre(fd,tag,"_HEX",ipv4_addr_to_hex_string(val));
	display_var_string_free_line_pre(fd,tag,"_BIN",ipv4_addr_to_bin_string(val));
	display_var_ulong_line_pre(fd,tag,"_NUM",ipv4_addr_to_long(val));
}
/*输出地址变量并释放*/
static INLINE void display_var_ipv4_free_line(int fd,const char*tag,struct ipv4_addr*val){display_var_ipv4_line(fd,tag,val);if(val)free(val);}
/*以变量格式输出*/
#ifdef __linux__
void display_var(int fd,struct ipv4_net*p){
#else
void display_var(struct ipv4_net*p){
	int fd;
#ifndef STDOUT_FILENO
	fd=1;
#else
	fd=STDOUT_FILENO;
#endif
#endif
	if(!p)return;
	display_var_ipv4_line(fd,"ADDRESS",p->net);
	display_var_string_free_line(fd,"CIDR",ipv4_net_to_string(p));
	display_var_ipv4_free_line(fd,"NETWORK",ipv4_get_net_addr_net(p));
	display_var_ipv4_line(fd,"NETMASK",p->mask);
	display_var_ulong_line(fd,"PREFIX",(int)ipv4_mask_to_prefix(p->mask));
	display_var_ipv4_free_line(fd,"WILDCARD",ipv4_mask_to_wildcard(p->mask));
	display_var_ipv4_free_line(fd,"MIN_ADDRESS",ipv4_get_min_addr_net(p));
	display_var_ipv4_free_line(fd,"MAX_ADDRESS",ipv4_get_max_addr_net(p));
	display_var_ipv4_free_line(fd,"BOARDCAST",ipv4_get_brd_addr_net(p));
	display_var_ulong_line(fd,"COUNT_ADDRESS",ipv4_get_count_addr_net(p));
	display_var_string_line(fd,"CLASS",ipv4_get_addr_class_short_string(p->net));
	display_var_string_line(fd,"TYPE",ipv4_get_addr_type_short_string(p,p->net));
	display_var_string_line(fd,"GROUP",ipv4_get_addr_group_short_string(p->net));
}
/*以可读表格式输出*/
#ifdef __linux__
void display_readable_table(int fd,struct ipv4_net*p,struct display_char*table){
#else
void display_readable_table(struct ipv4_net*p,struct display_char*table){
	int fd=1;
#endif
	if(!p||!table)return;
	int
		cs1[]={11,17,13,37,12,0},
		cs2[]={11,82,0},
		bt[]={1,1,3,3,3,0},
		prefix=ipv4_mask_to_prefix(p->mask);
	const char*ss[]={"Column","Address","Hexadecimal","Binary","Decimal",NULL};
	print_top(fd,cs1,table);
	print_string_line(fd,cs1,ss,table->vertical);
	print_center(fd,cs1,NULL,table);
	display_ipv4_line(fd,cs1,table->vertical,"Address",p->net);
	display_ipv4_line(fd,cs1,table->vertical,"Netmask",p->mask);
	display_ipv4_free_line(fd,cs1,table->vertical,"Wildcard",ipv4_mask_to_wildcard(p->mask));
	if(prefix<32){
		display_ipv4_free_line(fd,cs1,table->vertical,"Network",ipv4_get_net_addr_net(p));
		display_ipv4_free_line(fd,cs1,table->vertical,"Host Min",ipv4_get_min_addr_net(p));
		display_ipv4_free_line(fd,cs1,table->vertical,"Host Max",ipv4_get_max_addr_net(p));
		display_ipv4_free_line(fd,cs1,table->vertical,"Broadcast",ipv4_get_brd_addr_net(p));
	}
	print_center(fd,cs1,bt,table);
	display_string_free_line(fd,cs2,table->vertical,"CIDR",ipv4_net_to_string(p));
	display_ulong_line(fd,cs2,table->vertical,"Prefix",(unsigned long)ipv4_mask_to_prefix(p->mask));
	if(prefix<32)display_ulong_line(fd,cs2,table->vertical,"Count",ipv4_get_count_addr_net(p));
	display_string_line(fd,cs2,table->vertical,"IP Class",ipv4_get_addr_class_string(p->net));
	display_string_line(fd,cs2,table->vertical,"IP Type",ipv4_get_addr_type_string(p,p->net));
	display_string_line(fd,cs2,table->vertical,"IP Group",ipv4_get_addr_group_string(p->net));
	print_bottom(fd,cs2,table);
}
#ifdef __cplusplus
}
#endif
