#include <cstdio>
#include <syscalls>

extern "C" void _start() {
	std::printf("Hello world\n");
	std::exit();
}
