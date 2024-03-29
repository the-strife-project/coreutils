#include <cstdio>
#include <syscalls>
#include <tasks>
#include <fs>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("mkfile: missing operand\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("mkfile: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	auto err = std::mkfile(path);
	if(err != std::FS_OK) {
		std::printf("mkdir: error creating '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(err));
	}

	std::exit(err);
}
