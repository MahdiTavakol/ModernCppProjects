#include "Ref.h"
#include "Engine.h"
#include "Error.h"
#include <stdexcept>

Ref::Ref(std::string id_):
	id{id_}
{}

Ref::Ref(const Ref& other):
	id{other.id + "_"}
{}

void Ref::injectDependencies(Engine& engine_) {
	error = engine_.getError().get();
	if (error == nullptr)
		throw std::invalid_argument("The error object is not present!");
	//checkPointer(error, "error");
}

void Ref::checkPointer(Ref* pointer_, std::string name_) const
{
	if (pointer_ == nullptr) {
		std::string errorMessage = "Error: The " + name_ + " pointer is null!";
		error->one(errorMessage);
	}	
}

const std::string& Ref::getId() const {
	return id;
}