#pragma once
#include <string>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
using std::vector;


class Logger {
public:
    Logger() : strm_vec{ std::vector<std::reference_wrapper<std::ostream>>{std::ref(std::cout)} } {}

    explicit Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms)
        : strm_vec(strms) {
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = default;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = default;
    ~Logger() = default;

    void print(const std::string& input_) const {
        for (auto& strm : strm_vec) {
            strm.get() << input_ << std::endl;
        }
    }


    template<typename T>
    Logger& operator<<(const T val)
    {
        for (auto& strm : strm_vec)
        {
            strm.get() << val;
        }
        return *this;
    }

    Logger& operator<<(std::ostream& (*f)(std::ostream&))
    {
        for (auto& strm : strm_vec)
        {
            strm.get() << f;
        }
        return *this;
    }

private:
    std::vector<std::reference_wrapper<std::ostream>> strm_vec;
};