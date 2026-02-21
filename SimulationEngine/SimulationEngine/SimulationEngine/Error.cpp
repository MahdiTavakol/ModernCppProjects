#include "Error.h"

Error::Error(Engine& engine_, std::string id_) :
	Type{ engine_, id_ },
{
	errorStreams.push_back(std::ref(std::cout));
}

Error::Error(Engine& engine_,
	const std::vector<std::reference_wrapper<std::ostream>>& errorStreams_,
	std::string id_):
	Type{ engine_, id_ },
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