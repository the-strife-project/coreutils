#include <cstdio>
#include <syscalls>
#include <tasks>
#include <fs>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("mkdir: missing operand\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("mkdir: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	std::exit(std::mkdir(path));
}
