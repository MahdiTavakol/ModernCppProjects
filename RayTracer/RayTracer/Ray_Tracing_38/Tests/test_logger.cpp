#include "catch_amalgamated.hpp"
#include "test_shared.h"
#include "../Output/Logger.h"
#include <sstream>

TEST_CASE("Testing the logger object")
{
	std::ostringstream oss1, oss2, oss3;
	std::vector<std::reference_wrapper<std::ostream>> strms = {
		std::ref(oss1), std::ref(oss2), std::ref(oss3)
	};

	std::unique_ptr<Logger> logger_ptr =
		std::make_unique<Logger>(strms);


	std::string text1 = "info: running the information";
	std::string text2 = "warning: running the information";
	std::string text3 = "error: running the information";
	std::vector<std::string> expectedLog;
	expectedLog.push_back(text1);
	expectedLog.push_back(text2);
	expectedLog.push_back(text3);

	logger_ptr->print(text1);
	logger_ptr->print(text2);
	logger_ptr->print(text3);

	REQUIRE_THAT(&oss1, OStringStreamMatcher(&expectedLog));

}