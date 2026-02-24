#include "Fix.h"
#include "Error.h"

Fix::Fix(Engine& engine_, std::vector<std::string>& args_):
	Ref{ engine_, args_[0] },
	nevery{ 1 }
{
	// fix type id nevery mask ......
	auto& error = engine().getError();
	int nargs = args_.size();
	if (nargs < 3) {
		engine().getError()->one("Not enough arguments for fix command! Expected at least 3, got " + std::to_string(nargs));
		return;
	}
	id = args_[2];
	nevery = std::stoi(args_[3]);
	parseMaskString(args_[4]);
}

Fix::Fix(Engine& engine_,
	std::string id_,
	int nevery_) :
	Ref{ engine_, id_ },
	nevery{ nevery_ }
{}

void Fix::parseMaskString(const std::string& maskString)
{
	mask = static_cast<FixMask>(0);
	if (maskString.find("init_integrate") != std::string::npos) {
		mask = static_cast<FixMask>(mask | INIT_INTEGRATE);
	}
	if (maskString.find("pre_force") != std::string::npos) {
		mask = static_cast<FixMask>(mask | PRE_FORCE);
	}
	if (maskString.find("post_force") != std::string::npos) {
		mask = static_cast<FixMask>(mask | POST_FORCE);
	}
	if (maskString.find("final_integrate") != std::string::npos) {
		mask = static_cast<FixMask>(mask | FINAL_INTEGRATE);
	}
}