#include "CreateLargeLogs.h"
#include <random>

CreateLargeLogs::CreateLargeLogs(const std::string& logFileName_, const int& num_lines_, const int& line_length_,
		const double& info_percent_, const double& warns_percent_, const double& errors_percent_,
		const bool& keep_temp_):
		 keep_temp{keep_temp_}
{
	if (info_percent_ + warns_percent_ + errors_percent_ > 100.0)
		throw std::invalid_argument("The total percent is larger than 100%!");
	std::unique_ptr<fileInfo> fileInfoTempPtr = std::make_unique<fileInfo>
		(logFileName_, num_lines_, info_percent_, warns_percent_, errors_percent_, line_length_);
	fileWriterPtr = std::make_unique<fileWriterSimple>(fileInfoTempPtr);
}

CreateLargeLogs::~CreateLargeLogs()
{
	if (!keep_temp)
	{
		/* 
		 * If we want to delete the file we have to close it,
		 * otherwise the C++ itself closes the file after an
		 * object of this class goes out of the scope.
		 */
		fileWriterPtr->close();
		fileWriterPtr->removeFile();
	}
 }

void CreateLargeLogs::generateLog()
{
	static std::random_device rd1, rd2;
	static std::mt19937_64 rng1{ rd1() }, rng2{ rd2() };
	static std::uniform_real_distribution<double> typedouble(0, 100);
	static std::uniform_int_distribution<int> letterint(32, 126);

	std::array<int,2> arg1;
	std::array<double,3> arg2;
	fileWriterPtr->inputArgs(arg1,arg2);

	int num_lines = arg1[0];
	int numChars = arg1[1];
	double info_percent = arg2[0];
	double warn_percent = arg2[1];
	double error_percent = arg2[2];
	
	std::array<int,3> nums = {0,0,0};

	for (int i = 0; i < num_lines; i++)
	{
		std::string line;
		line.reserve(numChars);
		double typeProbability = typedouble(rng1);


		double minProb = 0.0;


		if (typeProbability - minProb < info_percent) {
			line += "[INFO]";
			nums[0]++;
		} else {
			minProb += info_percent;
			if (typeProbability - minProb < warn_percent) {
				line += "[WARN]";
				nums[1]++;
			} else {
				minProb += warn_percent;
				if (typeProbability - minProb < error_percent) {
					line += "[ERROR]";
					nums[2]++;
				}
			}

		}

		int remainingChars = numChars - static_cast<int>(line.length());
		// one space is reserved for the std::endl;
		for (int j = 0; j < remainingChars - 1; j++)
		{
			char c = letterint(rng2);
			line += c;
		}


		fileWriterPtr->writeLine(line);
	}
	fileWriterPtr->setNums(nums);
}

int CreateLargeLogs::operator()(std::array<int,3>& nums_)
{
	int num_lines = (*fileWriterPtr)(nums_);
	return num_lines;
}