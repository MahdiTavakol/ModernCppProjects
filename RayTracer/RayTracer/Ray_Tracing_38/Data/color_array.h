#ifndef COLOR_ARRAY_H
#define COLOR_ARRAY_H

#include "../Shared/rtweekend.h"



#include <stdlib.h>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "../Geometry/interval.h"

typedef struct { double r, g, b; } color_data;

class color_array
{
public:
    color_array(const int& _width, const int& _height, color_data** _color_data);
    color_array(const int& _width, const int& _height, color_data* _color_data);
    color_array(const int& _width, const int& _height);
    color_array(const color_array& );
    color_array();
    ~color_array();

    void reset_size(const int& _width, const int& _height);
    void reset(const int& _width, const int& _height, color_data* _color_data);
    void set_range(const int& _width, const int& _height);
    color_array& operator=(const color_array& second);
    color_data**& return_array();
    bool equal(color_array* rhs_, const double& tol_, 
               const std::vector<std::array<int,2>>& ignoredCoors_);
    size_t return_color_data_size();
    void return_size(int& width_, int& height_);
    inline static bool compare_color_data(const color_data& a_, const color_data& b_, double tol_)
    {
        if (std::abs(a_.r - b_.r) >= tol_)
            return false;
        if (std::abs(a_.g - b_.g) >= tol_)
            return false;
        if (std::abs(a_.b - b_.b) >= tol_)
            return false;
        return true;
    };
    inline color_data& operator()(const int& i_, const int& j_)
    {
        if (i_ < 0 ||
            j_ < 0 ||
            i_ >= width ||
            j_ >= height)
            throw std::invalid_argument("Out of bounds access");
        return array[i_][j_];
    }
    bool operator!=(color_array& _rhs)
    {
        if (this == &_rhs)
            return false;
        if (height != _rhs.height || width != _rhs.width)
            return true;
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                color_data& c_1 = (*this)(i, j);
                color_data& c_2 = _rhs(i, j);
                if (!compare_color_data(c_1, c_2, 1e-6))
                    return true;
            }
        return false;
    }

    void write(std::iostream& out_, const outputMode& mode_, const int& strid_ = 0) const;

    inline static void write_binary(std::ostream& out_, const color_data& c_data_)
    {
        std::uint8_t r =
            static_cast<std::uint8_t>(256 * std::clamp(c_data_.r, 0.0, 0.999));
        std::uint8_t g =
            static_cast<std::uint8_t>(256 * std::clamp(c_data_.g, 0.0, 0.999));
        std::uint8_t b =
            static_cast<std::uint8_t>(256 * std::clamp(c_data_.b, 0.0, 0.999));
        out_.write(reinterpret_cast<char*>(&r), 1);
        out_.write(reinterpret_cast<char*>(&g), 1);
        out_.write(reinterpret_cast<char*>(&b), 1);
    }

protected:
    int width, height;
    color_data** array;

    void allocate();
    void deallocate();
};

inline std::ostream& operator<<(std::ostream& out, const color_data& _c_data)
{
    static const interval intensity(0.000, 0.999);
    double r = _c_data.r;
    double g = _c_data.g;
    double b = _c_data.b;
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
    return out << rbyte << " " << gbyte << " " << bbyte << std::endl;
}

#endif
