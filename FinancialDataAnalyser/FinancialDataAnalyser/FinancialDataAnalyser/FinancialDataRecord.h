#pragma once

#include "ChronoDate.h"
#include <string>

struct FinancialDataRecord {
	ChronoDate date;
	std::string ticker; // Added ticker field
	double open;
	double high;
	double low;
	double close;
	int volume;
};