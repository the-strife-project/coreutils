#include <cstdio>
#include <syscalls>
#include <tasks>

static std::string bytesToPretty(size_t bytes) {
	// No floating point so this rounds down A LOT
	size_t aux = bytes;
	size_t order = 0;
	while(aux >= 1024) {
		aux /= 1024;
		++order;
	}

	std::string ret = std::uToStr(aux);
	switch(order) {
	case 0:
		ret += "B";
		break;
	case 1:
		ret += "KB";
		break;
	case 2:
		ret += "MB";
		break;
	}

	return ret;
}

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	size_t used = 0;
	if(!rtv.args.size()) {
		auto info = std::info(0);
		used = info.mem;
		std::printf("System-wide used memory: ");
	} else {
		size_t pid = std::strToU(rtv.args[0]);
		auto info = std::info(pid);
		if(!info.pid) {
			std::printf("mem: could not find PID: %d\n", pid);
			std::exit(1);
		}
		used = info.mem;
		std::printf("Memory usage of PID %d: ", pid);
	}

	std::printf("%s ", bytesToPretty(used).c_str());
	std::printf("(%d pages)\n", NPAGES(used));

	std::exit();
}
