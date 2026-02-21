#pragma once

#include <string>
#include <functional>

class Engine;

// a class to hold shared items among all the classes
class Ref {
public:
	// since there must be a reference to an engine for each Ref, the default constructor is deleted
	// Since we have deleted the default constructor
	// we do not need to delete the default constructor
	// of derived classes.
	Ref() = delete;
	// the constructor of the Ref class takes the reference of the engine and the id of the Ref
	Ref(Engine& engine_, std::string id_);
	/* Since there is just one engine,
	 * we should have special handling for the 
	 * copy constructor and copy assignment operator 
	 * to avoid using the delete copy constructor and copy assignment operator 
	 * of the engine;
	 * Since we are using the refence_wrapper to hold the reference of the engine
	 * both the new and old objects will refer to the same engine.
	 */
	Ref(const Ref& other);
	Ref& operator=(const Ref& other);
	Ref(Ref&& other) noexcept;
	Ref& operator=(Ref&& other) noexcept;

	bool operator==(const Ref& other) const;

	virtual ~Ref() = default;
	Engine& engine();
	const std::string& getId() const;

protected:
	// id used for testing
	std::string id;
	// the engineRef
	std::reference_wrapper<Engine> engineRef;
};