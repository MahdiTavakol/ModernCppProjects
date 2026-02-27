#include "FixList.h"
#include <stdexcept>
#include <iostream>

FixList::FixList():
	Fix{}
{}

FixList::~FixList() = default;

void FixList::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	for (auto& fix : fixVec)
		fix->injectDependencies(engine_);
}

void FixList::addFix(std::unique_ptr<Fix>&& fix_) {
	fixVec.push_back(std::move(fix_));
}

std::unique_ptr<Fix>& FixList::returnFixById(std::string id_) {
	for (auto& fix : fixVec) {
		if (fix->getId() == id_) {
			return fix;
		}
	}
	std::string errorMessage = "The fix with id " + id_ + " was not found!";
	throw std::invalid_argument(errorMessage.c_str());

}

void FixList::init() {
	for (auto& fix : fixVec)
		fix->init();
}

void FixList::setup()
{
	for (auto& fix : fixVec)
		fix->setup();
}

void FixList::initial_integrate()
{
	for (auto& fix : fixVec) {
		if (fix->mask & INIT_INTEGRATE)
		{
			fix->initial_integrate();
		}
	}
}

void FixList::pre_force()
{
	for (auto& fix : fixVec) {
		if (fix->mask & PRE_FORCE)
			fix->pre_force();
	}
}

void FixList::post_force() {
	for (auto& fix : fixVec) {
		if (fix->mask & POST_FORCE)
			fix->post_force();
	}
}
void FixList::final_integrate() {
	for (auto& fix : fixVec) {
		if (fix->mask & FINAL_INTEGRATE)
			fix->final_integrate();
	}
}