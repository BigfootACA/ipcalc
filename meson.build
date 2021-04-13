project(
	'ipcalc',
	'c',
	version: '1.1.1.1',
	meson_version: '>= 0.50.0',
	default_options: [
		'warning_level=3',
		'strip=true',
		'werror=true',
		'default_library=both',
		'optimization=s'
	]
)
add_project_arguments(
	[
		'-DIPCALC_VERSION="'+meson.project_version()+'"',
		'-Wno-unused-function'
	],
	language:'c'
)
deps=[]
ipcalc_src=[
	'tools/ipcalc.c',
	'tools/ipcalc_result.c',
	'tools/ipcalc_usage.c'
]
libipcalculator_src=[
	'libipcalculator/ipcalc_internal.c',
	'libipcalculator/ipcalc_public.c',
	'libipcalculator/ipv4_print.c',
	'libipcalculator/ipv4_check.c',
	'libipcalculator/ipv4_convert.c',
	'libipcalculator/ipv4_get.c',
	'libipcalculator/ipv4_misc.c',
	'libipcalculator/ipv4_object.c',
	'libipcalculator/ipv4_parse.c',
	'libipcalculator/ipv4_type.c'
]
cc=meson.get_compiler('c')
if host_machine.system()=='windows'
	message('Build for Windows')
	message('Enable Resource file')
	message('Enable interface windows')
	win=import('windows')
	libipcalculator_src+=win.compile_resources('data/libipcalculator.rc')
	ipcalc_src+=win.compile_resources('data/ipcalc.rc')
	ipcalc_src+='tools/interface_windows.c'
	lib=dependency('iphlpapi',required:false)
	if not lib.found()
		lib=cc.find_library('iphlpapi',required:true)
	endif
	deps+=lib
	lib=dependency('ws2_32',required:false)
	if not lib.found()
		lib=cc.find_library('ws2_32',required:true)
	endif
	deps+=lib
endif
if host_machine.system()=='linux'
	message('Build for Linux')
	message('Enable interface netlink')
	message('Enable interface getifaddrs')
	ipcalc_src+='tools/interface_netlink.c'
	ipcalc_src+='tools/interface_getifaddrs.c'
endif
executable(
	'ipcalc',
	ipcalc_src,
	install:true,
	link_with:library(
		'libipcalculator',
		libipcalculator_src,
		soversion:1,
		version:'1.1.1'
	),
	dependencies:deps
)
install_headers('libipcalculator/ipv4.h')
