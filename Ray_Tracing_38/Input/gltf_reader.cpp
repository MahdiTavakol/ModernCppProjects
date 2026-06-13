#include "gltf_reader.h"
#include "../Geometry/triangle_mesh.h"
#include "../Algorithms/bvh_triangles.h"
#include "../Materials/PBR.h"

#include <iostream>
#include <sstream>
#include <fstream>

gltf_reader::gltf_reader(const std::string& file_path_,Logger* error_, communicator* para_) :
	model_reader{file_path_,error_, para_}
{
	// creating the triangle_list 
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors);
	// as the file parsing might take a while
	// I would prefer lazy parsing

}

gltf_reader::~gltf_reader()
{
	tg3_model_free(&model);
	tg3_error_stack_free(&errors);
}

void gltf_reader::read()
{
	int msg_level = 0;
	std::string message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Reading the glTF file...";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);

	tg3_error_code err = tg3_parse_file(&model, &errors, file_path.c_str(), 10, &opts);
	if (err != TG3_OK)
	{
		for (int i = 0; i < errors.count; i++)
		{
			std::string error_text;
			error_text = std::to_string(errors.entries[i].severity);
			if (errors.entries[i].message)
				error_text += " " + std::string(errors.entries[i].message);
			std::cout << "Error " << i << ": " << error_text << std::endl;
		}
		throw std::runtime_error("Failed to parse the glTF file!");
	}

	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Finished reading the glTF file.";
	error->print_message(message, msg_level);
	std::stringstream iss;
	iss << "loaded glTF file has:\n"
		<< model.accessors_count << " accessors\n"
		<< model.animations_count << " animations\n"
		<< model.buffers_count << " buffers\n"
		<< model.buffer_views_count << " bufferViews\n"
		<< model.materials_count << " materials\n"
		<< model.meshes_count << " meshes\n"
		<< model.nodes_count << " nodes\n"
		<< model.textures_count << " textures\n"
		<< model.images_count << " images\n"
		<< model.skins_count << " skins\n"
		<< model.samplers_count << " samplers\n"
		<< model.cameras_count << " cameras\n"
		<< model.scenes_count << " scenes\n"
		<< model.lights_count << " lights\n";
	message = iss.str();
	error->print_message(message, msg_level);
	message = "The number of errors is " + std::to_string(errors.count);
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);

	// reading the scene 0
	read_scene();
	read_materials();
	read_textures();
	read_samplers();
	load_images();

	int low = 0;
	int high = static_cast<int>(primitives.size() - 1);
	add_item(low, high);

}

void gltf_reader::read_scene(size_t frame_)
{
	if (frame_ >= model.scenes_count)
		throw std::out_of_range("There is no such a scene in the model");
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	if (frame_ == 0)
	{
		message = "Reading the main scene from the glTF file...";
	}
	else
	{
		message = "Reading the scene " + std::to_string(frame_) + " from the glTF file...";
	}
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);

	const tg3_scene& scene_i = model.scenes[frame_];
	// looping through node of the scene
	uint32_t nodes_count = scene_i.nodes_count;
	for (int i = 0; i < nodes_count; i++)
	{
		msg_level = 0;
		int32_t node_id = scene_i.nodes[i];
		const tg3_node& node_i = model.nodes[node_id];
		std::string node_name = std::to_string(i);
		if (node_i.name.len > 0)
		{
			node_name = std::string(node_i.name.data, node_i.name.len);
		}
		message = "Reading the node " + node_name;
		error->print_message(message, msg_level);
		read_node(i);
	}



	size_t vs_size{ 0 }, vns_size{ 0 }, vts_size{ 0 }, faces_size{ 0 };
	for (auto& prim_info : primitives)
	{
		vs_size += prim_info.vs_vector.size();
		vns_size += prim_info.vns_vector.size();
		for (int k = 0; k < MAX_COORD; k++)
		{
			vts_size += prim_info.vts_array_vector[k].size();
		}
		faces_size += prim_info.faces.size();
	}

	message = "Finished reading all node";
	error->print_message(message, 0);
	message = "The total number of faces is " + std::to_string(faces_size);
	error->print_message(message, 1);
	message = "The total number of vertices is " + std::to_string(vs_size);
	error->print_message(message, 1);
	message = "The total number of texture coordinates is " + std::to_string(vts_size);
	error->print_message(message, 1);
	message = "The total number of normals is " + std::to_string(vns_size);
	error->print_message(message, 1);


	message = std::string(52, '=');
	error->print_message(message, 0);
	message = "Finished reading objects from the glTF file.";
	error->print_message(message, 0);
	message = std::string(52, '=');
	error->print_message(message, 0);
}

void gltf_reader::read_materials()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Reading materials from the glTF file...";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);



	message = "Adding materials to the material list";
	msg_level = 1;
	error->print_message(message, msg_level);

	uint32_t material_count = model.materials_count;
	for (int i = 0; i < material_count; i++)
	{
		const tg3_material& material_i = model.materials[i];
		std::string material_name = std::string(material_i.name.data,material_i.name.len);
		msg_level = 2;
		message = "Creating material " + std::to_string(i) +
			": " + material_name;
		error->print_message(message, msg_level);
		std::unique_ptr<material> mat_i = std::make_unique<PBR>(error,material_i);
		mtl_list->push_back(material_name,std::move(mat_i));
		std::unique_ptr<material> test_mat = std::make_unique<lambertian>(error,vec3(0.0, 1.0, 0.0));
	}

	msg_level = 0;
	message = "Finished reading all materials"; 
	error->print_message(message, msg_level);
	message = "The total number of materials is " + std::to_string(material_count);
	error->print_message(message, msg_level + 1);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Finished reading materials from the glTF file.";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
}

void gltf_reader::read_textures()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Reading textures from the glTF file...";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);


	uint32_t textures_count = model.textures_count;
	tg3_texture* read_textures = new tg3_texture[textures_count];

	for (int i = 0; i < textures_count; i++)
	{
		const tg3_texture& tex_i = model.textures[i];
		std::string tex_name = std::string(tex_i.name.data, tex_i.name.len);
		msg_level = 2;
		message = "Creating the texture " + std::to_string(i) +
			": " + tex_name;
		if (tex_i.name.len == 0)
			message = "Creating the texture " + std::to_string(i);

		error->print_message(message, msg_level);
		read_textures[i] = tex_i;
	}

	msg_level = 1;
	message = "Finished reading all texture";
	error->print_message(message, msg_level);
	msg_level = 2;
	message = "Adding textures to the PBR class";
	error->print_message(message, msg_level);
	PBR::set_textures(read_textures, textures_count);

	msg_level = 1;
	message = "Cleaning up!";
	error->print_message(message, msg_level);
	delete[] read_textures;



	message = "The total number of texture is " + std::to_string(textures_count);
	error->print_message(message, msg_level + 1);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	msg_level = 0;
	message = "Finished reading textures from the glTF file.";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
}

void gltf_reader::read_samplers()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Reading samplers from the glTF file...";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);


	uint32_t samplers_count = model.samplers_count;
	tg3_sampler* read_samplers = new tg3_sampler[samplers_count];

	for (int i = 0; i < samplers_count; i++)
	{
		const tg3_sampler& smp_i = model.samplers[i];
		std::string smp_name = std::string(smp_i.name.data, smp_i.name.len);
		msg_level = 2;
		message = "Creating the texture " + std::to_string(i) +
			": " + smp_name;
		if (smp_i.name.len == 0)
			message = "Creating the texture " + std::to_string(i);

		error->print_message(message, msg_level);
		read_samplers[i] = smp_i;
	}

	msg_level = 1;
	message = "Finished reading all samplers";
	error->print_message(message, msg_level);
	msg_level = 2;
	message = "Adding samplers to the PBR class";
	error->print_message(message, msg_level);
	PBR::set_samplers(read_samplers,samplers_count);

	msg_level = 1;
	message = "Cleaning up!";
	error->print_message(message, msg_level);
	delete[] read_samplers;



	message = "The total number of samplers is " + std::to_string(samplers_count);
	error->print_message(message, msg_level + 1);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	msg_level = 0;
	message = "Finished reading samplers from the glTF file.";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
}


void gltf_reader::read_node(size_t node_id_, Matrix4d transform_)
{
	int msg_level = 1;
	std::string message;

	auto& node_i = model.nodes[node_id_];

	std::string name;
	name = std::string(node_i.name.data, node_i.name.len);
	message = "Reading the node " + name;
	error->print_message(message, msg_level);


	message = "Reading the transformations";
	error->print_message(message, msg_level + 1);
	const int32_t& has_matrix = node_i.has_matrix;
	Matrix4d matrix;

	if (has_matrix == 1)
	{
		const double* mat = node_i.matrix;
		size_t loc = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix(j, i) = mat[loc++];
			}
		}
	}
	else
	{
		const double* rotation = node_i.rotation;
		const double* scale = node_i.scale;
		const double* translation = node_i.translation;
		matrix = create_gltf_matrix(rotation, scale, translation);
	}

	transform_ = transform_ * matrix;

	int32_t mesh = node_i.mesh;
	if (mesh != -1)
		read_mesh(mesh, transform_);


	uint32_t children_count = node_i.children_count;
	for (int i = 0; i < children_count; i++)
	{
		int32_t child_i = node_i.children[i];
		read_node(child_i, transform_);
	}


}

void gltf_reader::read_mesh(size_t mesh_id_, Matrix4d transform_)
{
	// some debugging variables
	int msg_level = 1;
	int32_t face_count_i = 0;
	int32_t v_count_i = 0;
	int32_t vt_count_i = 0;
	int32_t vn_count_i = 0;
	int group_id = 0;

	// transformation for normals
	Eigen::Matrix3d upperleft = transform_.topLeftCorner<3, 3>();
	Eigen::Matrix3d normalsTransform = upperleft.inverse();


	const tg3_mesh& mesh_i = model.meshes[mesh_id_];
	uint32_t primitive_count = mesh_i.primitives_count;
	std::string object = std::string(mesh_i.name.data, mesh_i.name.len);
	std::string message = "Reading object " + std::to_string(mesh_id_) + ": " + object;
	error->print_message(message, msg_level);


	// looping through primitives
	for (int j = 0; j < primitive_count; j++)
	{
		// some debugging variables
		msg_level = 2;
		int32_t face_count_j = 0;
		int32_t v_count_j = 0;
		int32_t vt_count_j = 0;
		int32_t vn_count_j = 0;

		// adding new primitive
		primitive_struct prim_info_j;

		std::string group = std::to_string(group_id++);
		message = "Reading group " + group;
		error->print_message(message, msg_level);
		// Boolean used to check if we have converted the vertex buffer format
		bool convertedToTriangleList = false;
		const tg3_primitive& primitive_j = mesh_i.primitives[j];
		int32_t material_id = primitive_j.material;




		std::unique_ptr<intArrayBase> indices_array;
		{
			const tg3_accessor& accessor_j = model.accessors[primitive_j.indices];
			const tg3_buffer_view& buffer_view_j = model.buffer_views[accessor_j.buffer_view];
			const tg3_buffer& buffer_j = model.buffers[buffer_view_j.buffer];
			const auto dataAddress = buffer_j.data.data + buffer_view_j.byte_offset +
				accessor_j.byte_offset;
			int32_t byteStride = tg3_accessor_byte_stride(&accessor_j, &buffer_view_j);
			const auto count = accessor_j.count;


			switch (accessor_j.component_type) {
			case TG3_COMPONENT_TYPE_BYTE:
				indices_array =
					std::unique_ptr<intArray<char>>(new intArray<char>(
						arrayAdapter<char>(dataAddress, count, byteStride)));
				break;

			case TG3_COMPONENT_TYPE_UNSIGNED_BYTE:
				indices_array = std::unique_ptr<intArray<unsigned char> >(
					new intArray<unsigned char>(arrayAdapter<unsigned char>(
						dataAddress, count, byteStride)));
				break;

			case TG3_COMPONENT_TYPE_SHORT:
				indices_array =
					std::unique_ptr<intArray<short> >(new intArray<short>(
						arrayAdapter<short>(dataAddress, count, byteStride)));
				break;

			case TG3_COMPONENT_TYPE_UNSIGNED_SHORT:
				indices_array = std::unique_ptr<intArray<unsigned short> >(
					new intArray<unsigned short>(arrayAdapter<unsigned short>(
						dataAddress, count, byteStride)));
				break;

			case TG3_COMPONENT_TYPE_INT:
				indices_array = std::unique_ptr<intArray<int> >(new intArray<int>(
					arrayAdapter<int>(dataAddress, count, byteStride)));
				break;

			case TG3_COMPONENT_TYPE_UNSIGNED_INT:
				indices_array = std::unique_ptr<intArray<unsigned int> >(
					new intArray<unsigned int>(arrayAdapter<unsigned int>(
						dataAddress, count, byteStride)));
				break;
			default:
				break;
			}
		}

		const auto& indices = *indices_array;
		std::vector<int> index_vector;
		index_vector.reserve(indices_array->size());
		if (indices_array) {
			msg_level = 3;
			message = "Reading indices with count " + std::to_string(indices_array->size());
			error->print_message(message, msg_level);
			for (int k = 0; k < indices_array->size(); ++k) {
				index_vector.push_back(indices[k]);
			}

		}


		switch (primitive_j.mode)
		{
		case TG3_MODE_TRIANGLE_FAN:
			if (!convertedToTriangleList) {
				msg_level = 4;
				message = "Primitive mode is TRIANGLE_FAN, converting to triangle list";
				error->print_message(message, msg_level);
				// This only has to be done once per primitive
				convertedToTriangleList = true;

				// We steal the guts of the vector
				auto triangleFan = std::move(index_vector);

				// Push back the indices that describe just one triangle one by one
				for (size_t k = 2; k < triangleFan.size(); ++k) {
					std::vector<int> ind =
					{ triangleFan[0],
					  triangleFan[k - 1],
					  triangleFan[k] };
					int ind_size = static_cast<int>(ind.size());
					prim_info_j.faces.emplace_back(
						ind,
						std::vector<int>(),
						std::vector<int>(),
						object,
						group,
						ind_size,
						material_id);
					face_count_j++;
				}
			}
		case TG3_MODE_TRIANGLE_STRIP:
			if (!convertedToTriangleList) {
				message = "Primitive mode is TRIANGLE_STRIP, converting to triangle list";
				error->print_message(message, msg_level);
				// This only has to be done once per primitive
				convertedToTriangleList = true;

				auto triangleStrip = std::move(index_vector);

				for (size_t k = 2; k < triangleStrip.size(); ++k) {
					std::vector<int> ind =
					{ triangleStrip[k - 2],
					  triangleStrip[k - 1],
					  triangleStrip[k] };
					int ind_size = static_cast<int>(ind.size());
					prim_info_j.faces.emplace_back(
						ind,
						std::vector<int>(),
						std::vector<int>(),
						object,
						group,
						ind_size,
						material_id);
					face_count_j++;
				}
			}

		case TG3_MODE_TRIANGLES:
			if (!convertedToTriangleList) {
				message = "Primitive mode is TRIANGLES";
				error->print_message(message, msg_level);
				// This only has to be done once per primitive
				convertedToTriangleList = true;


				for (size_t k = 0; k < index_vector.size(); k += 3) {
					std::vector<int> ind = {
						index_vector[k],
						index_vector[k + 1],
						index_vector[k + 2] };
					int ind_size = static_cast<int>(ind.size());
					prim_info_j.faces.emplace_back(
						ind,
						std::vector<int>(),
						std::vector<int>(),
						object,
						group,
						ind_size,
						material_id);
					face_count_j++;
				}

			}
			uint32_t attribute_count = primitive_j.attributes_count;
			for (int k = 0; k < attribute_count; k++)
			{
				msg_level = 5;
				const tg3_str_int_pair& attribute_k = primitive_j.attributes[k];
				const tg3_str& attribute_key = attribute_k.key;
				const int32_t attribute_value = attribute_k.value;
				std::string attribute_name{ attribute_key.data, attribute_key.len };

				message = "Reading the attribute with name " + attribute_name;
				error->print_message(message, msg_level);


				const tg3_accessor& accessor_k = model.accessors[attribute_value];
				const tg3_buffer_view& buffer_view_k = model.buffer_views[accessor_k.buffer_view];
				const tg3_buffer& buffer_k = model.buffers[buffer_view_k.buffer];
				const auto dataAddress = buffer_k.data.data + buffer_view_k.byte_offset +
					accessor_k.byte_offset;
				int32_t byteStride = tg3_accessor_byte_stride(&accessor_k, &buffer_view_k);
				const auto count = accessor_k.count;

				message = "Current attribute has count " + std::to_string(count) +
					" and stride " + std::to_string(byteStride) + " bytes";
				msg_level = 6;
				error->print_message(message, msg_level);



				msg_level = 7;
				if (!attribute_name.compare("POSITION")) {
					message = "Parsing positions";
					error->print_message(message, msg_level);

					// get the position min/max for computing the boundingbox
					const double* minValues = accessor_k.min_values;
					const double* maxValues = accessor_k.max_values;
					uint32_t minValuesCount = accessor_k.min_values_count;
					uint32_t maxValuesCount = accessor_k.max_values_count;
					if (minValuesCount != 3 || maxValuesCount != 3) {
						std::cout << "Error: Position attribute should have exactly 3 min and max values\n";
						continue; // Skip this attribute if min/max counts are not correct
					}

					min = vec3{ minValues[0], minValues[1], minValues[2] };
					max = vec3{ maxValues[0], maxValues[1], maxValues[2] };

					msg_level = 8;
					switch (accessor_k.type) {
					case TG3_TYPE_VEC3: {
						message = "Type is vec3 ";
						switch (accessor_k.component_type) {
						case TG3_COMPONENT_TYPE_FLOAT:
						{
							message += "and component type is float";
							error->print_message(message, msg_level);
							v3fArray positions(
								arrayAdapter<v3f>(dataAddress, count, byteStride));



							for (size_t i = 0; i < positions.size(); ++i) {
								const auto& v = positions[i];

								Eigen::Vector4d vVec(v.x, v.y, v.z, 1.0);
								// 4X1 = 4X4 * 4X1
								vVec = transform_ * vVec;

								prim_info_j.vs_vector.push_back(vec3{ vVec[0] ,vVec[1] ,vVec[2] });
								v_count_j++;
							}
							break;
						}
						case TG3_COMPONENT_TYPE_DOUBLE: {
							message += "and component type is double";
							error->print_message(message, msg_level);
							// 3D vector of float
							v3dArray positions(
								arrayAdapter<v3d>(dataAddress, count, byteStride));


							for (size_t i = 0; i < positions.size(); ++i) {
								const auto& v = positions[i];

								Eigen::Vector4d vVec(v.x, v.y, v.z, 1.0);
								// 4X1 = 4X4 * 4X1
								vVec = transform_ * vVec;

								prim_info_j.vs_vector.push_back(vec3{ vVec[0] ,vVec[1] ,vVec[2] });
								v_count_j++;
							}
							break;
						}
						default:
							message = "Warning the component type is unknown!";
							error->print_message(message, msg_level);
							break;
						}
						break;
					default:
						break;
					}
					}
				}
				else if (!attribute_name.compare("NORMAL")) {

					msg_level = 7;
					message = "Parsing normals";
					error->print_message(message, msg_level);
					msg_level = 8;

					switch (accessor_k.type) {
					case TG3_TYPE_VEC3: {
						message = "Type is vec3 ";
						switch (accessor_k.component_type) {
						case TG3_COMPONENT_TYPE_FLOAT:
						{
							message += "and component type is float";
							error->print_message(message, msg_level);
							v3fArray normals(
								arrayAdapter<v3f>(dataAddress, count, byteStride));

							// For each triangle :
							for (size_t i = 0; i < normals.size(); ++i) {
								const auto& vn = normals[i];
								Eigen::Vector3d normalVec(vn.x, vn.y, vn.z);
								// 3X1 = 3X3 * 3X1
								normalVec = normalsTransform * normalVec;
								prim_info_j.vns_vector.push_back(vec3{ normalVec[0],normalVec[1],normalVec[2] });
								vn_count_j++;
							}
						}
						break;
						case TG3_COMPONENT_TYPE_DOUBLE:
						{
							message += "and component type is double";
							error->print_message(message, msg_level);
							v3dArray normals(
								arrayAdapter<v3d>(dataAddress, count, byteStride));

							// For each triangle :
							for (size_t i = 0; i < normals.size(); ++i) {
								const auto& vn = normals[i];
								Eigen::Vector3d normalVec(vn.x, vn.y, vn.z);
								// 3X1 = 3X3 * 3X1
								normalVec = normalsTransform * normalVec;
								prim_info_j.vns_vector.push_back(vec3{ normalVec[0],normalVec[1],normalVec[2] });
								vn_count_j++;
							}
						}
						break;
						default:
							std::cerr << "Unhandeled componant type for normal\n";
						}
					} break;
					default:
						std::cerr << "Unhandeled vector type for normal\n";
					}

				}
				else if (!attribute_name.compare(0, 9, "TEXCOORD_"))
				{
					msg_level = 7;
					int coord_i;
					{
						std::stringstream ss(attribute_name);
						std::string str;
						// TEXCOORD
						std::getline(ss, str, '_');
						// NUMBER
						std::getline(ss, str, '_');
						// clearing the stringstream
						ss.str("");
						ss.clear();
						// putting str into the ss
						ss << str;
						// putting it into the coord_i
						ss >> coord_i;
						// checking it
						if (coord_i < 0)
							throw std::invalid_argument("This should never have happened!");
						if (coord_i >= MAX_COORD)
							throw std::invalid_argument("There is a " + attribute_name);
					}


					message = "Parsing texture coordinates " + std::to_string(coord_i);
					error->print_message(message, msg_level);
					msg_level = 8;
					switch (accessor_k.type) {
					case TG3_TYPE_VEC2: {
						message = "Type is vec2 ";

						switch (accessor_k.component_type) {
						case TG3_COMPONENT_TYPE_FLOAT: {
							message += "and component type is float";
							error->print_message(message, msg_level);
							v2fArray uvs(
								arrayAdapter<v2f>(dataAddress, count, byteStride));

							// For each triangle :
							for (size_t i = 0; i < uvs.size(); ++i) {
								const auto& uv = uvs[i];
								prim_info_j.vts_array_vector[coord_i].push_back(vec2{ uv.x,uv.y });
								vt_count_j++;
							}
							break;
						}
						case TG3_COMPONENT_TYPE_DOUBLE: {
							message += "and component type is double";
							error->print_message(message, msg_level);
							v2dArray uvs(
								arrayAdapter<v2d>(dataAddress, count, byteStride));

							// For each triangle :
							for (size_t i = 0; i < uvs.size(); ++i) {
								const auto& uv = uvs[i];
								prim_info_j.vts_array_vector[coord_i].push_back(vec2{ uv.x,uv.y });
								vt_count_j++;
							}
							break;
						}
						default:
							message = "Warning the component type is unknown!";
							error->print_message(message, msg_level);
						}
						break;
					}
					default:
						message = "Warning unknown!";
						error->print_message(message, msg_level);
					}
				}
			}
			break;
		}

		msg_level = 2;
		message = "Finished reading the primitive " + std::to_string(j);
		error->print_message(message, msg_level);
		message = "The number of faces is " + std::to_string(face_count_j);
		error->print_message(message, msg_level + 1);
		message = "The number of vertices is " + std::to_string(v_count_j);
		error->print_message(message, msg_level + 1);
		message = "The number of texture coordinates is " + std::to_string(vt_count_j);
		error->print_message(message, msg_level + 1);
		message = "The number of normals is " + std::to_string(vn_count_j);
		error->print_message(message, msg_level + 1);

		message = "Adding the primitive " + std::to_string(j);
		error->print_message(message, msg_level);
		primitives.push_back(prim_info_j);



		face_count_i += face_count_j;
		v_count_i += v_count_j;
		vt_count_i += vt_count_j;
		vn_count_i += vn_count_j;
	}
	msg_level = 1;
	message = "Finished reading object " + std::to_string(mesh_id_) + ": " + object;
	error->print_message(message, msg_level);
	message = "The number of faces is " + std::to_string(face_count_i);
	error->print_message(message, msg_level + 1);
	message = "The number of vertices is " + std::to_string(v_count_i);
	error->print_message(message, msg_level + 1);
	message = "The number of texture coordinates is " + std::to_string(vt_count_i);
	error->print_message(message, msg_level + 1);
	message = "The number of normals is " + std::to_string(vn_count_i);
	error->print_message(message, msg_level + 1);
}

MatrixXd gltf_reader::create_gltf_matrix(const double* rotation_, const double* scale_, const double* translation_)
{
	Vector4d scaleVector = { scale_[0], scale_[1], scale_[2], 1.0 };
	Eigen::DiagonalMatrix<double, 4> dMat(scaleVector);
	Matrix4d scaleMat = dMat.toDenseMatrix();


	Matrix4d transMat = Matrix4d::Identity(4, 4);
	transMat(0, 3) = translation_[0];
	transMat(1, 3) = translation_[1];
	transMat(2, 3) = translation_[2];

	const double& qx = rotation_[0];
	const double& qy = rotation_[1];
	const double& qz = rotation_[2];
	const double& qw = rotation_[3];


	double xx = qx * qx;
	double yy = qy * qy;
	double zz = qz * qz;

	double xy = qx * qy;
	double xz = qx * qz;
	double yz = qy * qz;

	double wx = qw * qx;
	double wy = qw * qy;
	double wz = qw * qz;

	Matrix4d rotMat = Matrix4d::Identity();

	rotMat(0, 0) = 1.0 - 2.0 * (yy + zz);
	rotMat(0, 1) = 2.0 * (xy - wz);
	rotMat(0, 2) = 2.0 * (xz + wy);

	rotMat(1, 0) = 2.0 * (xy + wz);
	rotMat(1, 1) = 1.0 - 2.0 * (xx + zz);
	rotMat(1, 2) = 2.0 * (yz - wx);

	rotMat(2, 0) = 2.0 * (xz - wy);
	rotMat(2, 1) = 2.0 * (yz + wx);
	rotMat(2, 2) = 1.0 - 2.0 * (xx + yy);

	rotMat(3, 3) = 1.0;


	return transMat * rotMat * scaleMat;
}

void gltf_reader::load_images()
{
	std::string message;
	int msg_level;

	msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Loading images";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);


	msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Finished reading images";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
}

void gltf_reader::print_image(tg3_image_result* image_, std::string file_name_)
{
	std::ofstream file(file_name_);
	int32_t w = image_->width;
	int32_t h = image_->height;
	int32_t comp = image_->component;
	int32_t bits = image_->bits;
	int32_t bytes = bits / 8;


	file << "P6" << std::endl;
	file << w << " " << h << std::endl;

	switch (bytes)
	{
	case 1:
		file << "255" <<std::endl;
		for (int i = 0; i < bytes * h * w *comp; i++)
		{
			int pixelNumber = i / bytes;
			// skipping the component 
			if (pixelNumber% comp >= 3)
				continue;
			file.write(reinterpret_cast<char*>(&image_->pixels[i]), 1);
		}
		break;
	case 2:
		file << "655535" << std::endl;
		for (int i = 0; i < bytes * h * w * comp; i++)
		{
			int pixelNumber = i / bytes;
			// skipping the component
			if (pixelNumber % comp >= 3)
				continue;
			file.write(reinterpret_cast<char*>(image_->pixels[i]), 1);
		}
		break;
	default:
		throw std::invalid_argument("Unsupported!");
	}

}

vec3 gltf_reader::image_average_color(tg3_image_result* image_)
{
	// Implementation for calculating average color
	vec3 avg = vec3{ 0, 0, 0 };
	int w = image_->width;
	int h = image_->height;
	int cpp = image_->component;

	for (int i = 0; i < w * h * cpp; i += cpp) {
		avg[0] += image_->pixels[i];     // Red
		avg[1] += image_->pixels[i + 1]; // Green
		avg[2] += image_->pixels[i + 2]; // Blue
	}
	avg /= static_cast<double>(w * h);
	return avg;
}

void gltf_reader::add_item(const int& _low, const int& _hi)
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
 	error->print_message(message, msg_level);
	message = "Adding objects to the hittable_list...";
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);

	
	int low = _low;
	if (low < 0)
		low = 0;
	int hi = _hi;
	if (hi >= primitives.size())
		hi = static_cast<int>(primitives.size());




	int num_triangles = 0;
	std::string current_group;
	std::string current_obj;

	// adding primitives
	for (int i = _low; i < hi; i++)
	{
		std::vector<vec3>& vs_ref = primitives[i].vs_vector;
		std::vector<vec3>& vns_ref = primitives[i].vns_vector;
		std::array<std::vector<vec2>, MAX_COORD>& vts_array_ref = primitives[i].vts_array_vector;

		// seeing what is there and what is not there
		size_t v_size = vs_ref.size();
		size_t vn_size = vns_ref.size();
		std::array<size_t, MAX_COORD> vts_array_size;
		for (auto& size : vts_array_size)
			size = vts_array_ref[&size - vts_array_size.data()].size();

		// adding primitive i
		for (auto& face : primitives[i].faces)
		{
			// adding face from the primitive i
			std::string group = face.group;
			std::string object = face.object;
			std::array<point3, 3> vs_i, vns_i;
			std::array<point2, 3> vts_i, vts_1_i;



			if (!silent && !object.empty() && object.compare(current_obj))
			{
				current_obj = object;
				msg_level = 1;
				error->print_message("Adding object " + object, msg_level);
			}

			int num_edges = static_cast<int>(face.v_indx.size());

			for (int j = 0; j < num_edges; j++)
			{
				// adding edge j 
				int indx_j = face.v_indx[j];

				if (object == "Cube.005" && j == 2)
				{
					int stop_here = 0;
				}

				// bound checking for the indices
				// as vts and vns are optional we check them only if they are mentioned in the face
				if (indx_j < 0 ||
					indx_j >= v_size) {
					throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
				}

				vs_i[j] = vs_ref[indx_j];

				if (vn_size > 0) {
					if (indx_j >= vn_size)
					{
						throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
					}
					vns_i[j] = vns_ref[indx_j];
				}


				for (int k = 0; k < MAX_COORD; k++)
				{
					if (vts_array_size[k] > 0)
					{
						if (indx_j >= vts_array_size[k])
							throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
						if (k == 0)
							vts_i[j] = vts_array_ref[k][indx_j];
						else if (k == 1)
							vts_1_i[j] = vts_array_ref[k][indx_j];
						else
							break;
					}
					else {
						if (k == 0)
							vts_i[j] = point2{};
						else if (k == 1)
							vts_1_i[j] = point2{};
						else
							break;
					}
				}
				vs_i[j] = vs_ref[indx_j];
			}

			// estimating the vns if necessary
			if (vn_size == 0)
			{
				estimate_vns(vs_i, vns_i);
			}

			int mat_indx = face.mat_indx;
			auto trngle = std::make_unique<triangle_mesh>(vs_i, vts_i, vts_1_i, vns_i, mat_indx);
			double trngle_area = trngle->get_area();
			if (trngle_area <= 0)
			{
				trngle->flip();
				trngle_area = trngle->get_area();
				if (trngle_area <= 0) {
					int prim_level = i;
					int face_level = &face - primitives[i].faces.data();
					message = "triangle area from the primitive " +
						std::to_string(prim_level) +
						" and face " +
						std::to_string(face_level) +
						" is negative,\ndespite flipping it!" +
						std::to_string(trngle_area) +
						"\n\t" + 
						object + " " + group + " " +
						std::to_string(face.v_indx[0]) + " " 
						+ std::to_string(vs_i[0][0]) + " " + std::to_string(vs_i[0][1]) + " " + std::to_string(vs_i[0][2]) +
						"\n\t" +
						object + " " + group + " " + std::to_string(face.v_indx[1]) + " "
						+ std::to_string(vs_i[1][0]) + " " + std::to_string(vs_i[1][1]) + " " + std::to_string(vs_i[1][2]) +
						"\n\t" +
						object + " " + group + " " + std::to_string(face.v_indx[2]) + " "
						+ std::to_string(vs_i[2][0]) + " " + std::to_string(vs_i[2][1]) + " " + std::to_string(vs_i[2][2]);
					error->print_message(message, -1);
				}
				continue;
			}
			trngle->add_label(object);
			trngle->add_label(group);
			world->add(std::move(trngle));

		}
	}

	// setting all the read objects as main label
	world->add_label("main");


	msg_level = 0;
	message = std::string(52, '=');
	error->print_message(message, msg_level);
	message = "Finished adding objects to the hittable_list.";
	error->print_message(message, msg_level);
	message = "The total number of items in the hittable_list is "
		+ std::to_string(world->size());
	error->print_message(message, msg_level);
	message = std::string(52, '=');
	error->print_message(message, msg_level);
}
