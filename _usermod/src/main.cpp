#include <cstdio>
#include <syscalls>
#include <tasks>
#include <users>
#include <registry>

static size_t getuid(std::string& name) {
	size_t ret = std::nameToUID(name);
	if(!ret) {
		std::printf("usermod: user not found: %s\n", name.c_str());
		std::exit(99);
	}
	return ret;
}

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("Usage options:\n"
					"  usermod give <user> <perm>\n"
					"  usermod super <user> <user>\n");
		std::exit(1);
	}

	std::string& mode = rtv.args[0];
	if(mode == "give") {
		if(rtv.args.size() < 3) {
			std::printf("usermod give: missing arguments\n");
			std::exit(3);
		}

		size_t uid = getuid(rtv.args[1]);
		std::string path = "/u/";
		path += std::uToStr(uid);
		path += "/";
		path += rtv.args[2];

		auto ret = std::registry::create(path);
		if(ret != std::registry::OK) {
			std::printf("usermod give: failed: %d\n", ret);
			std::exit(4);
		}
	} else if(mode == "super") {
		if(rtv.args.size() < 3) {
			std::printf("usermod super: missing arguments\n");
			std::exit(4);
		}

		size_t uid = getuid(rtv.args[1]);
		IGNORE(uid);
		std::printf("TODO\n");
	} else {
		std::printf("usermod: invalid mode: %s\n", mode.c_str());
		std::exit(2);
	}

	std::exit();
}
