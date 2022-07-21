#include <cstdio>
#include <syscalls>
#include <tasks>
#include <fs>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(!rtv.args.size()) {
		std::printf("append: missing operand\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("append: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	// How much is there to write?
	std::string contents;
	for(size_t i=1; i<rtv.args.size(); ++i) {
		contents += rtv.args[i];
		contents += (i == rtv.args.size()-1) ? "\n" : " ";
	}

	// Where do we start?
	auto info = std::getFileInfo(path);
	if(info.error != std::FS_OK) {
		std::printf("append: cannot access '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(info.error));
		std::exit(info.error);
	}

	size_t start = info.size;
	auto ret = std::writeFile(path, contents.c_str(), start, contents.size());
	if(ret != std::FS_OK) {
		std::printf("append: cannot write to '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(info.error));
		std::exit(ret);
	}

	std::exit();
}
