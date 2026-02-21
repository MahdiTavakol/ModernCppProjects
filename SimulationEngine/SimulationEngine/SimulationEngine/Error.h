#pragma once
#include "Type.h"
#include <string>
#include <functional>
#include <iostream>
#include <vector>

class Error : public Type {
public:
	Error(Engine& engine_,
		const std::vector<std::reference_wrapper<std::ostream>>& errorStreams_,
		std::string id_ );
	Error(Engine& engine_,
		std::string id_ );

	void one(std::string message);
	void warning(std::string message);

	template<class T>
	Error& operator<<(const T& V) {
		for (auto& stream : errorStreams) {
			stream.get() << ;
		}
		return *this;
	}

private:
	std::vector<std::reference_wrapper<std::ofstream>> errorStreams;
};