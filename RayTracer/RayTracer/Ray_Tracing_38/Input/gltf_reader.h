#pragma once
#include "../Algorithms/hittable_list.h"
#include "../Materials/material_list.h"
#include "tiny_gltf_v3.h"
#include "../Algorithms/communicator.h"
#include "model_reader.h"
#include <string>
#include <vector>

constexpr int MAX_COORD = 16;


class gltf_reader: public model_reader {
public:
	gltf_reader(const std::string& file_path, communicator* para_);
	~gltf_reader();
	void read() override;


protected:
	// temp variable for the number of materials for now!
	int material_num = 0;
	// the min and max of the simulation box
	vec3 min, max;
	// scaling the file
	double scale = 1.0;



	// data
	// TEXCOORDS
	std::array<std::vector<vec2>, MAX_COORD> vts_array;


	// helper functions
	// reading objects from the gltf
	void read_objects();
	// reading materials from the gltf
	void read_materials();
	// reading textures
	void read_textures();
	// reading the samplers
	void read_samplers();
	// loading images from the gltf
	void load_images();
	// printing the image to file just for debuggin purposes
	void print_image(tg3_image_result* image_, std::string file_name_);
	// just for debugging purposes
	static vec3 image_average_color(tg3_image_result* image_);
	// adding items to the world
	virtual void add_item(const int& _low, const int& _hi) override;




private:
	// tinygltf data structures
	tg3_parse_options opts;
	tg3_error_stack errors;
	tg3_model model;


	// helper functions taken from
	// https://github.com/syoyo/tinygltf/blob/release/examples/raytrace/gltf-loader.h

	template<typename T>
	struct v2 {
		T x, y;
	};

	template <typename T>
	struct v3 {
		T x, y, z;
	};


	using v2f = v2<float>;
	using v2d = v2<double>;
	using v3f = v3<float>;
	using v3d = v3<double>;

	/// Adapts an array of bytes to an array of T. Will advace of byte_stride each
	/// elements.
	template <typename T>
	struct arrayAdapter {
		/// Pointer to the bytes
		const unsigned char* dataPtr;
		/// Number of elements in the array
		const size_t elemCount;
		/// Stride in bytes between two elements
		const size_t stride;

		/// Construct an array adapter.
		/// \param ptr Pointer to the start of the data, with offset applied
		/// \param count Number of elements in the array
		/// \param byte_stride Stride betweens elements in the array
		arrayAdapter(const unsigned char* ptr, size_t count, size_t byte_stride)
			: dataPtr(ptr), elemCount(count), stride(byte_stride) {
		}

		/// Returns a *copy* of a single element. Can't be used to modify it.
		T operator[](size_t pos) const {
			if (pos >= elemCount)
				throw std::out_of_range(
					"Tried to access beyond the last element of an array adapter with "
					"count " +
					std::to_string(elemCount) + " while getting elemnet number " +
					std::to_string(pos));
			return *(reinterpret_cast<const T*>(dataPtr + pos * stride));
		}
	};

	/// Interface of any adapted array that returns ingeger data
	struct intArrayBase {
		virtual ~intArrayBase() = default;
		virtual unsigned int operator[](size_t) const = 0;
		virtual size_t size() const = 0;
	};

	/// Interface of any adapted array that returns float data
	struct floatArrayBase {
		virtual ~floatArrayBase() = default;
		virtual float operator[](size_t) const = 0;
		virtual size_t size() const = 0;
	};

	/// An array that loads interger types, returns them as int
	template <class T>
	struct intArray : public intArrayBase {
		arrayAdapter<T> adapter;

		intArray(const arrayAdapter<T>& a) : adapter(a) {}
		unsigned int operator[](size_t position) const override {
			return static_cast<unsigned int>(adapter[position]);
		}

		size_t size() const override { return adapter.elemCount; }
	};

	template <class T>
	struct floatArray : public floatArrayBase {
		arrayAdapter<T> adapter;

		floatArray(const arrayAdapter<T>& a) : adapter(a) {}
		float operator[](size_t position) const override {
			return static_cast<float>(adapter[position]);
		}

		size_t size() const override { return adapter.elemCount; }
	};

	struct v2fArray {
		arrayAdapter<v2f> adapter;
		v2fArray(const arrayAdapter<v2f>& a) : adapter(a) {}

		v2f operator[](size_t position) const { return adapter[position]; }
		size_t size() const { return adapter.elemCount; }
	};

	struct v2dArray {
		arrayAdapter<v2d> adapter;
		v2dArray(const arrayAdapter<v2d>& a) : adapter(a) {}
		v2d operator[](size_t position) const { return adapter[position]; }
		size_t size() const { return adapter.elemCount; }
	};

	struct v3fArray {
		arrayAdapter<v3f> adapter;
		v3fArray(const arrayAdapter<v3f>& a) : adapter(a) {}

		v3f operator[](size_t position) const { return adapter[position]; }
		size_t size() const { return adapter.elemCount; }
	};

	struct v3dArray {
		arrayAdapter<v3d> adapter;
		v3dArray(const arrayAdapter<v3d>& a) : adapter(a) {}
		v3d operator[](size_t position) const { return adapter[position]; }
		size_t size() const { return adapter.elemCount; }
	};


};
