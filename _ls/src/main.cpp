#include <cstdio>
#include <tasks>
#include <fs>
#include <set>

void list(std::string& path) {
	size_t err = std::_fs_select(path);
	if(err != std::FS_OK) {
		std::printf("ls: cannot access '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(err));
		std::exit(10 + err);
	}

	std::FileList fl;
	std::listFiles(path, fl);

	std::set<std::string> sorted;
	for(auto const& x : fl)
		sorted.insert(x);

	for(auto& x : sorted) {
		bool dir = std::isDir(path + "/" + x);
		if(dir)
			std::printf("\033\x0B%s\033\x0A\n", x.c_str());
		else
			std::printf("\033\x0A%s\n", x.c_str());
	}
}

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(rtv.args.size() > 1) {
		std::printf("ls: usage: ls [<path>]\n");
		std::exit(1);
	} else if(rtv.args.size() == 1) {
		list(rtv.args[0]);
	} else {
		if(!rtv.env.has("PWD")) {
			std::printf("ls: no $PWD set\n");
			std::exit(2);
		}

		list(rtv.env["PWD"]);
	}

	std::exit();
}
