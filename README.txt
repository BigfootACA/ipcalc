IP Calculator library for C/C++ and IP Calculator 
Version v1.1.1.1
Author by ClassFun BigfootACA

If you need a Windows GUI version, please see ipcalc-gui.

IP Calculator Usage:
	ipcalc [-U|-A|-V|-N] [-q] [-f|-o PATH] [-l|-a|-i IFNAME] ADDRESS[/PREFIX|NETMASK] [NETMASK]\n"

Example: 
	$ ipcalc 192.168.0.1/24
	$ ipcalc 172.20.0.1 255.255.255.248
	$ ipcalc 10.10.0.0/255.255.0.0
	$ ipcalc -V -q 10.20.30.4/30 | awk -F= '$1=="COUNT_ADDRESS"{print $2}'
	$ ipcalc -a
	$ ipcalc -i eth0


Report bugs to https://classfun.cn/
Or send mail to mailto:bigfoot@classfun.cn

See license in License.txt
See change log in Changelog.txt

Compile:
	Makefile compile:
	$ make

	CMake compile:
	$ cmake -S . -B build
	$ make -C build

	Meson compile:
	$ meson build .
	$ ninja -C build

	Static binary:
	$ make ipcalc-static

	Use clang Compiler:
	$ make CC=clang

Cross-Compile:
	Windows MinGW32 GCC with Meson:
	$ meson --cross-file data/meson-mingw32.ini build .
	$ ninja -C build

	Windows MinGW64 GCC with Meson:
	$ meson --cross-file data/meson-mingw64.ini build .
	$ ninja -C build

	Windows MinGW32 GCC with CMake:
	$ cmake -DCMAKE_TOOLCHAIN_FILE=data/cmake-mingw32.cmake -B build -S .
	$ make -C build

	Windows MinGW64 GCC with CMake:
	$ cmake -DCMAKE_TOOLCHAIN_FILE=data/cmake-mingw64.cmake -B build -S .
	$ make -C build

	Windows MinGW32 GCC with Makefile:
	$ make WIN32=1 CROSS_COMPILE=i686-w64-mingw32-

	Windows MinGW64 GCC with Makefile:
	$ make WIN32=1 CROSS_COMPILE=x86_64-w64-mingw32-

	Windows MinGW GCC static binary with Makfile:
	$ make WIN32=1 CROSS_COMPILE=x86_64-w64-mingw32- ipcalc-static.exe

	aarch64 GCC:
	$ make CROSS_COMPILE=aarch64-linux-gnu-
