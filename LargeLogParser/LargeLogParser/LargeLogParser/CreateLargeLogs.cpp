#include "CreateLargeLogs.h"
#include <random>

void CreateLargeLogs::generateLog()
{
	static std::random_device rd1, rd2;
	static std::mt19937_64 rng1{ rd1() }, rng2{ rd2() };
	static std::uniform_real_distribution<double> typedouble(0, 100);
	static std::uniform_int_distribution<int> letterint(32, 126);

	for (int i = 0; i < num_lines; i++)
	{
		int numChars = 256;
		std::string line;
		line.reserve(numChars);
		double typeProbability = typedouble(rng1);


		double minProb = 0.0;


		if (typeProbability - minProb < info_percent) {
			line += "[INFO]";
			num_infos++;
		} else minProb += info_percent;

		if (typeProbability - minProb < warns_percent) {
			line += "[WARN]";
			num_warns++;
		} else minProb += warns_percent;

		if (typeProbability - minProb < errors_percent) {
			line += "[ERROR]";
			num_errors++;
		} else minProb += errors_percent;


		numChars -= line.length();
		// one space is reserved for the std::endl;
		for (int j = 0; j < numChars - 1; j++)
		{
			char c = letterint(rng2);
			line += c;
		}

		logFile << line << std::endl;
	}
}

int CreateLargeLogs::operator()(int& num_infos_, int& num_warns_, int& num_errors_)
{
	num_infos_ = num_infos;
	num_warns_ = num_warns;
	num_errors_ = num_errors;
	return num_lines;
}