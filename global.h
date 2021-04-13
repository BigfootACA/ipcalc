#ifndef _GLOBAL_H
#define _GLOBAL_H
#ifndef WIN32
#ifdef _MSC_VER
#define WIN32 1
#endif
#endif
#ifdef WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#endif
#if defined(linux) || defined(__linux)
#ifndef __linux__
#define __linux__ 1
#endif
#endif
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif
#ifndef IPCALC_VERSION
#define IPCALC_VERSION "1.1.1.1"
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifdef _MSC_VER
#define EXTERN __declspec(dllexport)
#else
#define EXTERN extern
#endif
#ifdef __STDC_VERSION__
#define INLINE inline
#else
#define INLINE
#endif
#endif
