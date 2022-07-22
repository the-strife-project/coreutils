#include <cstdio>
#include <tasks>
#include <fs>
#include <users>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("acltree: missing operand\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("acltree: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	std::string stack = "/";
	auto parts = path.split('/');
	if(!parts[parts.size()-1].size())
		parts.pop_back();

	for(size_t i=0; i<parts.size(); ++i) {
		stack += parts[i];
		if(i != parts.size()-1)
			stack += "/";

		std::Args args;
		args.push_back(stack);
		std::PID pid = std::run("/cd/bin/aclget", args, rtv.env);
		if(std::getLastLoaderError()) {
			std::printf("acltree: bad ELF\n");
			std::exit(3);
		}
		std::wait(pid);
	}

	std::exit();
}
