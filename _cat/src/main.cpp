#include <cstdio>
#include <syscalls>
#include <tasks>
#include <fs>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("cat: missing operand\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("cat: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	std::Buffer buffer;
	auto ret = std::readWholeFile(path, buffer, true);
	if(ret != std::FS_OK) {
		std::printf("cat: cannot access '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(ret));
		std::exit(ret);
	}

	std::printf("%s", buffer.get());
	std::exit();
}
