#include"../global.h"
#include<stdbool.h>
#include"ipcalc.h"
int ipcalc_usage(int e,bool verbose){
	return_stderr_print(0,
		"Usage: ipcalc [-U|-A|-V|-N] [-q] "
		#ifndef WIN32
		"[-f|-o PATH] "
		#endif
		"[-l|-a|-i IFNAME] ADDRESS[/PREFIX|NETMASK] [NETMASK]\n"
	);/*two newline*/
	return return_stderr_print(e,verbose?
		"Arguments:\n"
		"\t ADDRESS:                IPv4 address (example: 192.168.0.1)\n"
		"\t PREFIX:                 IPv4 netmask prefix (example: 24)\n"
		"\t NETMASK:                IPv4 netmask (example: 255.255.255.0)\n"
		#ifdef WIN32
		"\t IFNAME:                 Interface name (example: eth0)\n"
		"\t FD:                     File descriptor (example: 1 (stdout))\n"
		"\t PATH:                   Output path (example: output.txt)\n"
		#else
		"\t IFNAME:                 Network Adapter name/desc/id (example: Ethernet0)\n"
		"\t FD:                     File descriptor (not support on Windows)\n"
		"\t PATH:                   Output path (not support on Windows)\n"
		#endif
		"\n"
		"Options:\n"
		"\t -n, --none:             Do not display result\n"
		"\t -U, --unicode:          Display result with Unicode table"
		#ifndef WIN32
		" (default)"
		#endif
		"\n"
		"\t -A, --ascii:            Display result with ASCII table"
		#ifdef WIN32
		" (default)"
		#endif
		"\n\t -N, --nosep:            Display result with no-separator table\n"
		"\t -V, --vabiable:         Display result with variable list\n"
		"\t -l, --list:             Display all interfaces\n"
		"\t -a, --all:              Display IPv4 address from all interfaces\n"
		"\t -i, --interface IFNAME: Display IPv4 address from interface IFNAME\n"
		#ifndef WIN32
		"\t -o, --output PATH:      Write result to PATH\n"
		"\t -f, --fd FD:            Write result to FD\n"
		#endif
		"\t -q, --quiet:            Do not show extra messages\n\n"
		#ifndef SIMPLEINIT
		"\t -v, --version:          Show version\n"
		#endif
		"\t -h,- -help:             Show this help\n\n"
		"Examples:\n"
		"\t $ ipcalc 192.168.0.1/24\n"
		"\t $ ipcalc 172.20.0.1 255.255.255.248\n"
		"\t $ ipcalc 10.10.0.0/255.255.0.0\n"
		"\t $ ipcalc -V -q 10.20.30.4/30 | awk -F= '$1==\"COUNT_ADDRESS\"{print $2}'\n"
		"\t $ ipcalc -a\n"
		#ifdef WIN32
		"\t $ ipcalc -i Ethernet0\n"
		#else
		"\t $ ipcalc -i eth0\n"
		#endif
		:"Use ipcalc --help to show verbose help"
	);
}
#ifndef SIMPLEINIT
int ipcalc_version(int e){
	return return_stderr_print(e,
		"IP Calculator v" IPCALC_VERSION "\n"
		"author by ClassFun BigfootACA\n\n"
		"Report bugs to https://classfun.cn/\n"
		"Or send mail to mailto:bigfoot@classfun.cn"
	);
}
#endif
