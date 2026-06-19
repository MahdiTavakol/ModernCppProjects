#pragma once
#include "../Algorithms/hittable_list.h"
#include "../Materials/material_list.h"
#include "../Materials/PBR.h"
#include "tiny_gltf_v3.h"
#include "../Algorithms/communicator.h"
#include "../Output/profiler.h"
#include "model_reader.h"
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <memory>


using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::Vector4d;
using Eigen::Matrix4d;

constexpr int MAX_COORD = 16;

struct primitive_struct
{
	std::vector<face_indx> faces;
	std::vector<vec3> vs_vector, vns_vector;
	std::array<std::vector<vec2>, MAX_COORD> vts_array_vector;
};

// async running parameters
// 8 for now
struct async_parameters {
	const int async_threads;
	std::atomic<int> next_item;
	const int items_per_thread;
	const int item_low;
	const int item_high;

	async_parameters(
		int async_threads_,
		int item_low_,
		int item_high_,
		int items_per_thread_ = 1) :
		async_threads{ async_threads },
		next_item{item_low_},
		item_low{ item_low_ },
		item_high{ item_high_ },
		items_per_thread{items_per_thread_}
	{}
};


enum GLTF_Reader_Mode
{
	ASYNC,
	SERIAL
};

class gltf_reader: public model_reader {
public:
	gltf_reader(
		const std::string& file_path,
		Logger* error_,
		communicator* para_,
		profiler* timer_);
	~gltf_reader();
	void read() override;


protected:
	GLTF_Reader_Mode reader_mode = GLTF_Reader_Mode::ASYNC;
	// temp variable for the number of materials for now!
	int material_num = 0;
	// the min and max of the simulation box
	vec3 min, max;


	/// async running
	int async_threads = 8;


	// data
	std::vector<primitive_struct> primitives;

	// the resources for the PBR material
	std::shared_ptr<PBR_Resources> pbr_resources;


	// helper functions
	// reading nodes (which own meshes) from the gltf
	void read_scene(size_t frame_ = 0);
	// reading materials from the gltf
	void read_materials();
	// reading textures
	void read_textures();
	// reading the samplers
	void read_samplers();
	// loading images from the gltf
	void load_images();
	// loading image in a async way
	void load_image_async(async_parameters& params_);
	// printing the image to file just for debuggin purposes
	void print_image(tg3_image_result* image_, std::string file_name_);
	// just for debugging purposes
	static vec3 image_average_color(tg3_image_result* image_);
	// adding items to the world
	virtual void add_item(const int& _low, const int& _hi) override;
	// the async version of the add_item function
	virtual void add_item_async(async_parameters& params_, std::vector<std::unique_ptr<hittable>>& triangles_);
	// serial version of the add_items to be used either by one thread or in the serial version
	void add_items_range(std::vector<std::unique_ptr<hittable>>& triangles_, const int& first_item_, const int& last_item_);




	// reading nodes form gltf
	void read_node(size_t node_id_, Matrix4d transform_ = Matrix4d::Identity());
	// reading meshes of a node in the gltf
	void read_mesh(size_t mesh_id_, Matrix4d transform_ = Matrix4d::Identity());


	// the transformation helpers
	MatrixXd create_gltf_matrix(const double* rotation_, const double* scale_, const double* translation_);

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
