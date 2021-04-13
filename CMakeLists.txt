cmake_minimum_required(VERSION 3.0)
project(ipcalc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DIPCALC_VERSION='\"1.1.1.1\"'")
if(TARGET_OS STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
	enable_language("RC")
	if(NOT CMAKE_RC_COMPILER_WORKS)
		message(SEND_ERROR "CMAKE_RC_COMPILER_WORKS = ${CMAKE_RC_COMPILER_WORKS}")
	endif()
	set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()
macro(resource_compiler _NAME)
	set(_IN "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.rc")
	set(_OUT "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_NAME}.dir/${_NAME}_rc.obj")
	set(_WINDRES_INCLUDE_DIRS -I${CMAKE_CURRENT_SOURCE_DIR})
	foreach(wx_include_dir ${wxWidgets_INCLUDE_DIRS})
		set(_WINDRES_INCLUDE_DIRS ${_WINDRES_INCLUDE_DIRS} -I${wx_include_dir})
	endforeach(wx_include_dir ${wxWidgets_INCLUDE_DIRS})
	set(_ARGS ${_WINDRES_INCLUDE_DIRS} -i${_IN} -o${_OUT})
	add_custom_command(OUTPUT ${_OUT}
			COMMAND ${CMAKE_RC_COMPILER}
			ARGS ${_ARGS}
			COMMENT "Compiling ${_NAME}'s resource file"
			VERBATIM)
	string(TOUPPER ${_NAME} _NAME_UPPER)
	set(${_NAME_UPPER}_RESOURCES ${_OUT})
endmacro(resource_compiler)
if(TARGET_OS STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_library(
		ipcalculator
		SHARED
		libipcalculator/ipv4_check.c
		libipcalculator/ipv4_convert.c
		libipcalculator/ipv4_get.c
		libipcalculator/ipv4_misc.c
		libipcalculator/ipv4_type.c
		libipcalculator/ipv4_object.c
		libipcalculator/ipv4_parse.c
		libipcalculator/ipv4_print.c
		libipcalculator/ipcalc_internal.c
		libipcalculator/ipcalc_public.c
		data/libipcalculator.rc
	)
	add_executable(
		ipcalc
		tools/ipcalc_usage.c
		tools/ipcalc_result.c
		tools/ipcalc.c
		tools/interface_windows.c
		data/ipcalc.rc
	)
	target_link_libraries(ipcalc iphlpapi ws2_32 ipcalculator)
else()
	add_library(
		ipcalculator
		SHARED
		libipcalculator/ipv4_check.c
		libipcalculator/ipv4_convert.c
		libipcalculator/ipv4_get.c
		libipcalculator/ipv4_misc.c
		libipcalculator/ipv4_type.c
		libipcalculator/ipv4_object.c
		libipcalculator/ipv4_parse.c
		libipcalculator/ipv4_print.c
		libipcalculator/ipcalc_internal.c
		libipcalculator/ipcalc_public.c
	)
	add_executable(
		ipcalc
		tools/ipcalc.c
		tools/ipcalc_usage.c
		tools/ipcalc_result.c
		tools/interface_netlink.c
		tools/interface_getifaddrs.c
	)
	target_link_libraries(ipcalc ipcalculator)
endif()
