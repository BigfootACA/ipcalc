#ifndef _IPVALC_H
#define _IPVALC_H
#ifndef IPCALC_VERSION
#error "IPCALC_VERSION not define"
#endif
#include<errno.h>
#include<stdio.h>
#include<stdbool.h>
#include"../libipcalculator/ipv4.h"
#include"../global.h"
extern bool quiet;
static INLINE int return_perror(int e,const char*err){if(!quiet)perror(err);return e;}
static INLINE int return_stderr_print(int e,const char*cont){if(!quiet)fprintf(stderr,"%s\n",cont);return e;}
extern bool print_result(int fd,struct ipv4_net*ni,char s);
extern int ipcalc_usage(int e,bool verbose);
#ifndef SIMPLEINIT
extern int ipcalc_version(int e);
#endif
#ifdef __linux__
extern int print_interface_netlink(int fd,char display,char*ifname,bool list);
extern int print_interface_getifaddrs(int fd,char display,const char*ifname,bool list);
#define print_interface print_interface_netlink
#endif
#ifdef WIN32
extern int print_interface_windows(int fd,char display,char*ifname,bool list);
#define print_interface print_interface_windows
#endif
#endif
