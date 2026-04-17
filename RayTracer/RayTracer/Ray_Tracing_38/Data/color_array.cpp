#include "color_array.h"
#include <set>

color_array::color_array(const int& _width, const int& _height, color_data** _color_data) :
    width(_width), height(_height)
{
    allocate();
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            this->array[i][j] = _color_data[i][j];

}

color_array::color_array(const int& _width, const int& _height, color_data* _color_data) :
    width(_width), height(_height)
{

    allocate();
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            this->array[i][j] = _color_data[i * height + j]; // 1D to 2D array conversion
        }

}

color_array::color_array(const color_array& c_a)
{
    this->width = c_a.width;
    this->height = c_a.height;
    this->allocate();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            this->array[i][j] = c_a.array[i][j];

}

color_array::color_array(const int& _width, const int& _height) :
    width(_width), height(_height)
{
    allocate();
}

color_array::color_array() : array(nullptr), width(0), height(0) {}

color_array::~color_array()
{
    deallocate();
}

void color_array::reset_size(const int& _width, const int& _height)
{
    if (this->width != _width || this->height != _height)
    {
        if (this->width && this->height) deallocate();
        this->width = _width;
        this->height = _height;
        allocate();
    }
}

void color_array::reset(const int& _width, const int& _height, color_data* _color_data)
{
    this->reset_size(_width, _height);
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            this->array[i][j] = _color_data[i * height + j]; // 1D to 2D array conversion
}

void color_array::set_range(const int& _width, const int& _height)
{
    this->width = _width;
    this->height = _height;
}

color_array& color_array::operator=(color_array second)
{
    if (this == &second)
    {
        return *this;
    }
    this->deallocate();
    this->width = second.width;
    this->height = second.height;
    this->allocate();
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            this->array[i][j].r = second.array[i][j].r;
            this->array[i][j].g = second.array[i][j].g;
            this->array[i][j].b = second.array[i][j].b;
        }
    return *this;
}

color_data**& color_array::return_array()
{
    return this->array;
}


bool color_array::equal(color_array* rhs_, const double& tol_,
                        const std::vector<std::array<int, 2>>& ignoredCoors_)
{
    if (width != rhs_->width || height != rhs_->height)
        return false;

    std::set<std::array<int, 2>> ignoredSet;

    for (auto& coor : ignoredCoors_)
    {
        ignoredSet.insert(coor);
    }

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            if (ignoredSet.find(std::array<int, 2>{i, j}) != ignoredSet.end())
                continue;
            if (compare_color_data(array[i][j], rhs_->array[i][j], tol_) == false)
                return false;
        }
    return true;
}

size_t color_array::return_color_data_size()
{
    return width * height * sizeof(color_data);
}

void color_array::return_size(int& width_, int& height_)
{
    width_ = width; height_ = height;
}

void color_array::allocate()
{
    color_data* temp = (color_data*)malloc(width * height * sizeof(color_data));
    this->array = (color_data**)malloc(width * sizeof(color_data*)); // Column major allocation since we use color_data[x_index][y_index]
    for (int i = 0; i < width; i++)
        this->array[i] = &temp[i * height];
}

void color_array::deallocate()
{
    if (this->array != nullptr)
    {
        if (this->array[0] != nullptr) free(this->array[0]);
        free(this->array);
    }
    this->array = nullptr;
}

void color_array::write(std::iostream& out_, const outputMode& mode_, const int& stride_) const
{
    if (mode_ == outputMode::P3) {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                out_ << array[i][j]; //This leads to strided access ==> might need to change the indexing to [height_index][width_index]
            }
        }
    }
    else if (mode_ == outputMode::P6)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                color_array::write_binary(out_, array[i][j]);
            }
            // move the put location stride_ bytes from the current location.
            out_.seekp(stride_, std::ios::cur);
        }
    }
}
