#ifndef _GETOPT_H
#define _GETOPT_H
#ifdef __cplusplus
extern "C" {
#endif
#ifdef _MSC_VER
#define EXTERN __declspec(dllexport)
#else
#define EXTERN extern
#endif
struct option{
	const char*name;
	int has_arg;
	int*flag;
	int val;
};
EXTERN char *optarg;
EXTERN int optind,opterr,optopt,optreset;
EXTERN int getopt(int argc,char*const argv[],const char*optstring);
EXTERN int getopt_long(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx);
EXTERN int getopt_long_only(int argc,char*const*argv,const char*optstring,const struct option*longopts,int*idx);
#define no_argument 0
#define required_argument 1
#define optional_argument 2
#ifdef __cplusplus
}
#endif
#endif
