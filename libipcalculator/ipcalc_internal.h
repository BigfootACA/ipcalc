#ifndef _IPCALC_INTERNAL_H
#define _IPCALC_INTERNAL_H
#ifndef _MSC_VER
#include<unistd.h>
#endif
#include<stdio.h>
#include<stdarg.h>
#include"ipv4.h"
#include"../global.h"
#ifdef __cplusplus
extern "C"{
#endif
#ifdef __linux__
/**
 * 输出字符串到指定FD
 * @param fd 输出目标FD
 * @param chars 要输出的字符串
 * @return 输出的长度
 */
static INLINE size_t fd_print(int fd,const char*chars){return dprintf(fd,"%s",chars);}
/**
 * 输出格式化的字符串到指定FD
 * @param fd 输出目标FD
 * @param chars 要输出的格式化的字符串
 * @return 输出的长度
 */
static INLINE size_t fd_printf(int fd,const char*chars,...){
        va_list ap;
        va_start(ap,chars);
        size_t r=vdprintf(fd,chars,ap);
        va_end(ap);
        return r;
}
#else
//非Linux平台不使用FD
/**
 * 输出字符串
 * @param fd 未使用
 * @param chars 要输出的字符串
 * @return 输出的长度
 */
static INLINE size_t fd_print(int fd,const char*chars){(void)fd;return printf("%s",chars);}
/**
 * 输出格式化的字符串
 * @param fd 未使用
 * @param chars 要输出的格式化的字符串
 * @return 输出的长度
 */
static INLINE size_t fd_printf(int fd,const char*chars,...){
	(void)fd;
        va_list ap;
        va_start(ap,chars);
        size_t r=vprintf(chars,ap);
        va_end(ap);
        return r;
}
#endif
/**
 * 重复输出字符串到指定FD
 * @example:
 *   repet_chars((int)STDOUT_FILENO, (const char*)"abc", (size_t)3) = (size_t)9
 *   输出结果："abcabcabc"
 * @param fd 输出目标FD(在linux外的平台无效)
 * @param chars 重复输出的字符串
 * @param size 重复的次数
 * @return 输出的长度
 */
extern size_t repet_chars(int fd,const char*chars,size_t size);
/**
 * 输出一个字符串到指定FD并返回是否完整输出。
 * 如果因为某些原因导致字符串不能完整的输出，将会返回false
 * @param fd 输出目标FD(在linux外的平台无效)
 * @param chars 输出的字符串
 * @param len 传入一个size_t的指针，会自动将输出的长度追加到该指针指向的数字(可用NULL忽略)
 * @return 是否完整输出
 */
extern bool xprint(int fd,const char*chars,size_t*len);
/**
 * 重复输出字符串到指定FD并返回是否完整输出。
 * 如果因为某些原因导致字符串不能完整的输出，将会返回false
 * @param fd 输出目标FD(在linux外的平台无效)
 * @param chars 重复输出的字符串
 * @param size 重复的次数
 * @param len 传入一个size_t的指针，会自动将输出的长度追加到该指针指向的数字(可用NULL忽略)
 * @return 是否完整输出
 */
extern bool xrepet_chars(int fd,const char*chars,size_t size,size_t*len);
/**
 * 把长整型无符号数字转为字符串。
 * 使用后需要使用free释放
 * @example:
 *   char*x=ultostr((unsigned long)1024);
 *   if(x){
 *       printf("%s\n",x);
 *       free(x);
 *   }
 *   输出结果："1024"
 *
 * @param ul 长整型无符号数字
 * @return 字符串
 */
extern char*ultostr(unsigned long ul);
/**
 * 把长整型无符号数字转为字符串。
 * @example:
 *   size_t len=sizeof(char)*64;
 *   char*x=malloc(len);
 *   if(x){
 *      memset(x,0,len);
 *      if(ultostr_xl(x,len,(unsigned long)1024))printf("%s\n",x);
 *      free(x);
 *   }
 *   输出结果："1024"
 * @param ul 长整型无符号数字
 * @return 字符串
 */
extern char*ultostr_xl(char*buff,size_t len,unsigned long ul);
/**
 * 打印表格行
 * @param fd 输出目标FD(在linux外的平台无效)
 * @param cols 列宽度
 * @param bcols
 *
 */
extern size_t print_line(int fd,int*cols,const int*bcols,const char*repet,const char*left,const char*right,const char*center,const char*tcenter,const char*bcenter);
extern size_t print_string_line(int fd,const int*cols,const char**strs,const char*split);
extern size_t print_center(int fd,int*cols,int*bcols,struct display_char*t);
extern size_t print_top(int fd,int*cols,struct display_char*t);
extern size_t print_bottom(int fd,int*cols,struct display_char*t);
#ifdef __cplusplus
}
#endif
#endif
