#pragma once

#include <string>
#include <functional>

class Engine;
class Error;

// a class to hold shared items among all the classes
class Ref {
public:
	Ref() = default;
	// the constructor of the Ref class takes and id_
	Ref(std::string id_);
	/* 
	 * default constructors and assignment operators 
	 */
	Ref(const Ref& other);
	Ref& operator=(const Ref& other) = default;
	Ref(Ref&& other) noexcept = default;
	Ref& operator=(Ref&& other) noexcept = default;
	bool operator==(const Ref& other) const = default;
	virtual ~Ref() = default;
	virtual void injectDependencies(Engine& engine_);
	// the initialization processes which are dependent on other classes
	virtual void init() {}
	void checkPointer(Ref* pointer_, std::string name_) const;
	const std::string& getId() const;

protected:
	// Error
	Error* error;
	// id used for testing
	std::string id;
};