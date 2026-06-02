#include "catch_amalgamated.hpp"
#include "test_shared.h"
#include "../Output/Logger.h"
#include <sstream>

TEST_CASE("Testing the logger object")
{
	std::stringstream oss1, oss2, oss3;
	std::vector<std::reference_wrapper<std::ostream>> strms = {
		std::ref(oss1), std::ref(oss2), std::ref(oss3)
	};

	std::unique_ptr<Logger> logger_ptr =
		std::make_unique<Logger>(strms);

	std::vector<std::string> texts;
	std::vector<std::string> logVec;
	std::vector<std::string> expectedLog;
	

	SECTION("log-1")
	{
		texts.emplace_back("info: running the information");
		texts.emplace_back("warning: running the information");
		texts.emplace_back("error: running the information");
	}

	SECTION("log-2")
	{
		texts.emplace_back("info: initialization complete");
		texts.emplace_back("debug: entering main loop");
		texts.emplace_back("warning: low memory detected");
		texts.emplace_back("error: failed to open file");
		texts.emplace_back("trace: function call xyz()");
	}

	SECTION("log-3")
	{
		texts.emplace_back("ok");
		texts.emplace_back("");
		texts.emplace_back("error");
	}

	SECTION("log-4")
	{
		texts.emplace_back("INFO [2026-04-03 10:15:32]: System started successfully.");
		texts.emplace_back("WARNING [module=IO]: Disk usage at 92%!");
		texts.emplace_back("ERROR [code=500]: Unexpected failure occurred @ process#42");
		texts.emplace_back("DEBUG: value(x)=3.14159, value(y)=2.71828");
	}


	for (const auto& text : texts)
	{
		expectedLog.push_back(text);
		logger_ptr->print(text);
	}
	

	REQUIRE_THAT(&oss1, StringStreamMatcher(&expectedLog));
	REQUIRE_THAT(&oss2, StringStreamMatcher(&expectedLog));
	REQUIRE_THAT(&oss3, StringStreamMatcher(&expectedLog));

}