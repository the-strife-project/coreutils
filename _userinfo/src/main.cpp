#include <cstdio>
#include <syscalls>
#include <tasks>
#include <users>
#include <registry>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		// List users
		size_t n = std::howManyUsers();
		for(size_t i=1; i<=n; ++i) {
			std::string name = std::uidToName(i);
			if(name.size())
				std::printf("[%d] %s\n", i, name.c_str());
		}

		std::exit(0);
	}

	std::string& name = rtv.args[0];
	size_t uid = std::nameToUID(name);
	if(!uid) {
		std::printf("userinfo: %s: user not found\n", name.c_str());
		std::exit(1);
	}

	std::printf("User %s has UID %d\n", name.c_str(), uid);

	std::string path = "/u/";
	path += std::uToStr(uid);
	std::printf("Permissions: ");
	for(auto& x : std::registry::list(path))
		std::printf("%s ", x.c_str());
	std::printf("\n");

	std::exit();
}
