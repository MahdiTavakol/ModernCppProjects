#include "catch_amalgamated.hpp"
#include "test.hpp"


TEST_CASE("Testing various error and warning messages")
{
	std::cout << "Testing various error and warning messages" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// building the engin
	Engine engine;
	// Error streams
	std::ostringstream errorStream1, errorStream2, errorStream3;
	std::vector<std::reference_wrapper<std::ostream>> errorStreams = {
		std::ref(errorStream1),
		std::ref(errorStream2),
		std::ref(errorStream3)
	};
	auto error = make_unique<Error>(errorStreams);
	// setting items into the engine
	engine.setItem(std::move(error));
	// get error reference from the engine
	auto& errorRef = engine.getError();
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

TEST_CASE("Setting the error")
{
	std::cout << "Resetting the error object" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// building the engine
	Engine engine;
	// mocked box with an function to get the Error*
	std::unique_ptr<Box> box = std::make_unique<MockedBox>();
	// empty particles
	std::unique_ptr<Particles> particles = std::make_unique<MockedParticles>();
	// Error streams
	std::ostringstream stream1, stream2, stream3;
	std::vector<std::reference_wrapper<std::ostream>> errorStreams1 = {
		std::ref(stream1),
		std::ref(stream2)
	};
	std::vector<std::reference_wrapper<std::ostream>> errorStreams2 = {
		std::ref(stream3)
	};
	// Error objects
	std::unique_ptr<Error> error1 = std::make_unique<Error>(errorStreams1);
	std::unique_ptr<Error> error2 = std::make_unique<Error>(errorStreams2);
	// registering everything with the first Error object
	engine.setItem(std::move(box));
	engine.setItem(std::move(particles));
	engine.setItem(std::move(error1));
	// injecting the dependencies
	engine.injectDependencies();
	// returning the box and particles object
	auto& boxRef1 = engine.getBox();
	auto& particlesRef1 = engine.getParticles();
	// checking those references
	REQUIRE(boxRef1);
	REQUIRE(particlesRef1);
	// converting them
	auto mockedBoxPtr1 = dynamic_cast<MockedBox*>(boxRef1.get());
	auto mockedParticlesPtr1 = dynamic_cast<MockedParticles*>(particlesRef1.get());
	// checking the conversion
	REQUIRE(mockedBoxPtr1);
	REQUIRE(mockedParticlesPtr1);
	// getting the error refs
	Error* boxError1 = mockedBoxPtr1->getError();
	Error* particlesError1 = mockedParticlesPtr1->getError();
	// checking the streams
	auto& particlesStreams1 = particlesError1->getStreams();;
	auto& boxStreams1 = boxError1->getStreams();
	// testing the streams sizes
	REQUIRE(boxStreams1.size() == 2);
	REQUIRE(particlesStreams1.size() == 2);
	for (int i = 0; i < 2; i++) {
		std::cout << i << std::endl;
		REQUIRE(&boxStreams1[i].get() == &particlesStreams1[i].get());
	}
}

