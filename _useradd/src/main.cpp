#include <cstdio>
#include <syscalls>
#include <tasks>
#include <users>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("useradd: missing operand\n");
		std::exit(1);
	}

	std::string& name = rtv.args[0];
	size_t uid = std::newUser(name);
	if(!uid) {
		std::printf("useradd: could not create user '%s'\n", name.c_str());
		std::exit(1);
	}

	std::exit();
}
