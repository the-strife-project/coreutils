#include <cstdio>
#include <tasks>
#include <fs>
#include <users>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	bool effective = false;
	if(rtv.args.size() && rtv.args[0] == "-e") {
		effective = true;
		rtv.args.pop_front();
	}

	std::string path;
	if(!rtv.args.size()) {
		path = ".";
	} else {
		if(!rtv.args.size()) {
			std::printf("aclget: missing operand\n");
			std::exit(1);
		}
		path = rtv.args[0];
	}

	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("aclget: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	std::ACL acl;
	size_t err;
	if(effective)
		err = std::getEACL(path, acl);
	else
		err = std::getACL(path, acl);
	if(err != std::FS_OK) {
		std::printf("aclget: cannot access '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(err));
		std::exit(err);
	}

	if(effective)
		std::printf("Effective ");
	else
		std::printf("Literal ");
	std::printf("ACL for %s\n", path.c_str());
	for(auto const& x : acl) {
		std::string name = std::uidToName(x.f);
		if(name.size())
			std::printf("  %s: ", name.c_str());
		else
			std::printf("  Unknown %d: ", x.f);

		auto entry = x.s;
		if(!effective) {
			if(entry.allow)
				std::printf("ALLOW ");
			else
				std::printf("DENY ");
		}

		if(entry.read)
			std::printf("READ ");
		if(entry.write)
			std::printf("WRITE ");
		std::printf("\n");
	}

	std::exit();
}
