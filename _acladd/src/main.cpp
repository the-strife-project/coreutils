#include <cstdio>
#include <tasks>
#include <fs>
#include <users>

extern "C" void _start(std::Runtime rt) {
	auto rtv = rt.parse();

	if(rtv.args.size() < 4) {
		std::printf("acladd: usage: acladd <path> <allow/deny> <user> <[rw]>\n");
		std::exit(1);
	}

	std::string& path = rtv.args[0];
	if(path[0] != '/') {
		if(!rtv.env.has("PWD")) {
			std::printf("acladd: no $PWD set\n");
			std::exit(2);
		}

		path = rtv.env["PWD"] + "/" + path;
	}
	path = std::simplifyPath(path);

	bool allow;
	if(rtv.args[1] == "allow") {
		allow = true;
	} else if(rtv.args[1] == "deny") {
		allow = false;
	} else {
		std::printf("acladd: unknown permission mode: %s\n", rtv.args[1].c_str());
		std::exit(3);
	}

	size_t uid = std::nameToUID(rtv.args[2]);
	if(!uid) {
		std::printf("acladd: could not find user: %s\n", rtv.args[2].c_str());
		std::exit(4);
	}

	std::ACLEntry entry;
	entry.raw = 0;
	entry.isUser = true; // Will support GIDs in the future
	entry.allow = allow;
	for(char c : rtv.args[3]) {
		switch(c) {
		case 'r':
			entry.read = true;
			break;
		case 'w':
			entry.write = true;
			break;
		default:
			std::printf("acladd: unknown permission key: %c\n", c);
			std::exit(5);
		}
	}

	size_t err = std::addACL(path, uid, entry);
	if(err != std::FS_OK) {
		std::printf("acladd: cannot access '%s': %s\n",
					path.c_str(),
					std::VFS::errStr(err));
		std::exit(err);
	}

	std::exit();
}
