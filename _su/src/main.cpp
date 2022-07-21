#include <switcher>
#include <tasks>
#include <users>
#include <cstdio>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("su: missing operand\n");
		std::exit(99);
	}

	size_t uid = std::nameToUID(rtv.args[0]);
	if(!uid) {
		std::printf("su: could not find user: %s\n", rtv.args[0].c_str());
		std::exit(98);
	}

	auto ret = std::switchUser(uid);
	if(ret != std::switcher::OK) {
		std::printf("su: error: %d\n", ret);
		std::exit(ret);
	}

	auto pid = std::run("/cd/bin/shell", {}, rtv.env);
	if(std::getLastLoaderError()) {
		std::printf("su: could not launch shell\n");
		std::exit(97);
	}
	std::wait(pid);
	std::exit();
}
