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

Error::Error(Engine& engine_, std::vector<std::string> args_) :
	Ref{ engine_,std::string("Error-1") }
{
	int nargs = args_.size();
	if (nargs < 2) {
		one("Not enough arguments for error command! Expected at least 2, got " + std::to_string(nargs));
		return;
	}
	if (args_[1] == "screen") {
		errorStreams = { std::ref(std::cout) };
	}
	else {
		one("Invalid argument for error command! Expected 'screen' or 'file', got '" + args_[1] + "'");
	}
}

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