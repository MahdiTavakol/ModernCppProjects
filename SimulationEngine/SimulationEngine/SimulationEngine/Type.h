#pragma once

#include <string>
#include <functional>

#include "Engine.h"

using std::string, std::reference_wrapper;

// a class to hold shared items among all the classes
class Type {
public:
	Type(Engine& engine_, string id_) :
		id{ id_ },
		engineRef{ engine_ }
	{}
	Engine& engine() {
		return engineRef.get();
	}
	string getId() {
		return id;
	}
protected:
	// id used for testing
	string id;
	// the engineRef
	reference_wrapper<Engine> engineRef;
};