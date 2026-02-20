#pragma once

#include <string>
#include <functional>

class Engine;

// a class to hold shared items among all the classes
class Type {
public:
	Type(Engine& engine_, std::string id_) :
		id{ id_ },
		engineRef{ engine_ }
	{}
	virtual ~Type() = default;
	Engine& engine() {
		return engineRef.get();
	}
	const std::string& getId() const {
		return id;
	}
protected:
	// id used for testing
	std::string id;
	// the engineRef
	std::reference_wrapper<Engine> engineRef;
};