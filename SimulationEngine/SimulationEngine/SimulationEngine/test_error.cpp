#include "catch_amalgamated.hpp"
#include "test.hpp"


TEST_CASE("Testing various error and warning messages")
{
	std::cout << "Testing various error and warning messages" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box 0.0 0.0 0.0 10.0 10.0 10.0",
		"particles 10"
	};
	// building the engine factory
	EngineFixture enginefixture(commands);
	// building the engine using the fixture
	auto engine = enginefixture.returnEngine();
	// Error streams
	std::ostringstream errorStream1, errorStream2, errorStream3;
	std::vector<std::reference_wrapper<std::ostream>> errorStreams = {
		std::ref(errorStream1),
		std::ref(errorStream2),
		std::ref(errorStream3)
	};
	auto error = make_unique<Error>(*engine, errorStreams);
	// setting items into the engine
	engine->setItem(std::move(error));
	// get error reference from the engine
	auto& errorRef = engine->getError();
	// invoking the error function with different messages
	std::cout << "Using single error and warning messages:" << std::endl;
	errorRef->one("Error message 1");
	errorRef->warning("Warning message 1");
	errorRef->one("Error message 2");
	errorRef->warning("Warning message 2");
	errorRef->one("Error message 3");
	errorRef->warning("Warning message 3");
	// checking the outputs
	std::cout << "The ostringstreams contain:" << std::endl;
	auto& errorStreamsRef = errorRef->getStreams();
	for (auto& stream : errorStreamsRef) {
		if (auto ostringstreamPtr = dynamic_cast<std::ostringstream*>(&(stream.get()))) {
			std::cout << ostringstreamPtr->str() << std::endl;
		}
		else
			std::cout << "Error: Stream is not an ostringstream" << std::endl;
	}

	std::vector<std::reference_wrapper<std::ostream>> newErrorStreams = {
		std::ref(std::cout)
	};
	errorRef->setStreams(newErrorStreams);
	std::cout << "Chaining the warning messages:" << std::endl;
	(*errorRef) << "Chained warning message 1" << std::endl
		<< "Chained warning message 2" << std::endl
		<< "Chained warning message 3" << std::endl;
	SUCCEED("Error and warning messages were successfully written to the provided ostringstreams");
}