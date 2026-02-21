#include "Error.h"

Error::Error(Engine& engine_, std::string id_) :
	Ref{ engine_, id_ },
	errorStreams{ std::ref(std::cout) }
{}

Error::Error(Engine& engine_,
	std::vector<std::reference_wrapper<std::ostream>>& errorStreams_,
	std::string id_):
	Ref{ engine_, id_ },
	errorStreams{ errorStreams_ }
{ }

void Error::one(std::string message) {
	for (auto& stream : errorStreams) {
		stream.get() << "Error: " << message << std::endl;
	}
}

void Error::warning(std::string message) {
	for (auto& stream : errorStreams) {
		stream.get() << "Warning: " << message << std::endl;
	}
}

void Error::setStreams(const std::vector<std::reference_wrapper<std::ostream>>& newStreams) {
	errorStreams = newStreams;
}

std::vector<std::reference_wrapper<std::ostream>>& Error::getStreams() {
	return errorStreams;
}