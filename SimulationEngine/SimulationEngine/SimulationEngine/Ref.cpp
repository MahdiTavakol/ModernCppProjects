#include "Ref.h"

Ref::Ref(Engine& engine_, std::string id_) :
	id{ id_ },
	engineRef{ engine_ }
{}

Ref::Ref(const Ref& other):
	id{other.id + "_"},
	engineRef{ other.engineRef }
{}

Ref& Ref::operator=(const Ref& other) {
	if (*this != other) {
		id = other.id + "_";
		engineRef = other.engineRef;
	}
	return *this;
}

Ref::Ref(Ref&& other) noexcept :
	id{ std::move(other.id) },
	engineRef{ std::move(other.engineRef) }
{}

Ref& Ref::operator=(Ref&& other) noexcept {
	if (*this != other) {
		id = std::move(other.id);
		engineRef = std::move(other.engineRef);
	}
	return *this;
}

bool Ref::operator==(const Ref& other) const
{
	return id == other.id && (&(engineRef.get())) == (&(engineRef.get()));
}

void Engine::injectDependencies(Engine& engine_) {
	error 
}

Engine& Ref::engine() {
	return engineRef.get();
}
const std::string& Ref::getId() const {
	return id;
}