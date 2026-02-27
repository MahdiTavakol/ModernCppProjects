#pragma once
#include "Ref.h"
#include <string>
#include <functional>
#include <iostream>
#include <vector>

class Error : public Ref {
public:
	Error(std::vector<std::reference_wrapper<std::ostream>>& errorStreams_,
		  std::string id_ = "1");
	Error(std::string id_ = "1");
	Error(std::vector<std::string> args_);

	void one(std::string message);
	void warning(std::string message);
	void setStreams(const std::vector<std::reference_wrapper<std::ostream>>& newStreams);
	std::vector<std::reference_wrapper<std::ostream>>& getStreams();

	template<class T>
	Error& operator<<(const T& v) {
		for (auto& stream : errorStreams) {
			stream.get() << v;
		}
		return *this;
	}

	Error& operator<<(std::ostream& (*manip)(std::ostream&)) {
		for (auto& stream : errorStreams) {
			stream.get() << manip;
		}
		return *this;
	}


private:
	std::vector<std::reference_wrapper<std::ostream>> errorStreams;
};

