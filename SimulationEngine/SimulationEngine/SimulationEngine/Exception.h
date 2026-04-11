#pragma once

#include <exception>
#include <string>
#include "Comm_strategy.h"

class EngineException : public std::exception {
public:
	EngineException(const std::string& msg) :
		message{ msg }
	{}
	[[nodiscard]] const char* what() const noexcept override {
		return message.c_str();
	}

protected:
	std::string message;
};


class EngineAbortException : public EngineException
{
public:
	EngineAbortException(const std::string& msg_, std::unique_ptr<Comm_strategy>& universe_):
		EngineException{msg_}, universe{universe_.get()}
	{}
	[[nodiscard]] Comm_strategy* get_universe() const {
		return universe;
	}

protected:
	Comm_strategy* universe;
};


