#include"../global.h"
#include<stdbool.h>
#include"../libipcalculator/ipv4.h"
#include"ipcalc.h"
bool print_result(int fd,struct ipv4_net*ni,char s){
	#ifndef __linux__
	(void)fd;
	#endif
	struct display_char dis;
	switch(s){
		case 'n':return true;
		case 'A':dis=(struct display_char){
			"-","|",
			"+","+","+",
			"+","+","+",
			"+","+","+"
		};break;
		case 'U':dis=(struct display_char){
			"─","│",
			"┌","┬","┐",
			"├","┼","┤",
			"└","┴","┘"
		};break;
		case 'N':dis=(struct display_char){
			NULL,NULL,
			NULL,NULL,NULL,
			NULL,NULL,NULL,
			NULL,NULL,NULL
		};break;
		case 'V':display_var(
			#ifdef __linux__
			fd,
			#endif
			ni
		);return true;
		default:return return_stderr_print(false,"Unknown display mode");
	}
	display_readable_table(
		#ifdef __linux__
		fd,
		#endif
		ni,&dis
	);
	return true;
}