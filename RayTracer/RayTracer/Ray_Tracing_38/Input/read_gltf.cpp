#include "read_gltf.h"
#include <iostream>

read_gltf::read_gltf(const std::string& file_path_) : 
	file_path{ file_path_ },
	world{ std::make_unique<hittable_list>() },
	mtl_list{ std::make_unique<material_list>() }
{
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors);


	// as the file parsing might take a while
	// I would prefer lazy parsing
}

read_gltf::~read_gltf()
{
	tg3_model_free(&model);
	tg3_error_stack_free(&errors);
}

void read_gltf::parse()
{
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

	read_objects();
	read_materials();

	uint32_t material_count = model.materials_count;
	std::cout << "The number of materials is " << material_count << std::endl;
	uint32_t mesh_count = model.meshes_count;
	std::cout << "The number of meshes is " << mesh_count << std::endl; 

	std::cout << "the number of errors is " << errors.count << std::endl;
}

void read_gltf::read_objects()
{
	int mesh_count = model.meshes_count;

	// looping through meshes
	for (int i = 0; i < mesh_count; i++)
	{
		const tg3_mesh& mesh_i = model.meshes[i];
		uint32_t primitive_count = mesh_i.primitives_count;
		std::cout << "Reading mesh " << i;
		std::cout << " with name " << std::string(mesh_i.name.data, mesh_i.name.len) << std::endl;

		// looping through primitives
		for (int j = 0; j < primitive_count; j++)
		{
			// Boolean used to check if we have converted the vertex buffer format
			bool convertedToTriangleList = false;

			const tg3_primitive& primitive_j = mesh_i.primitives[j];
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
				std::cout << "\tindices: " << indices_array->size() << std::endl;;
				for (int k = 0; k < indices_array->size(); ++k) {
					index_vector.push_back(indices[k]);
				}
				std::cout << '\n';

			}

			switch (primitive_j.mode)
			{
			case TG3_MODE_TRIANGLE_FAN:
				if (!convertedToTriangleList) {
					std::cout << "TRIANGLE_FAN\n";
					// This only has to be done once per primitive
					convertedToTriangleList = true;

					// We steal the guts of the vector
					auto triangleFan = std::move(index_vector);
					faces.resize(triangleFan.size()-2);

					// Push back the indices that describe just one triangle one by one
					for (size_t i = 2 ; i < triangleFan.size(); ++i) {
						faces[i-2].indx.push_back(std::move(triangleFan[0]));
						faces[i-2].indx.push_back(std::move(triangleFan[i - 1]));
						faces[i-2].indx.push_back(std::move(triangleFan[i]));
					}
				}
			case TG3_MODE_TRIANGLE_STRIP:
				if (!convertedToTriangleList) {
					std::cout << "TRIANGLE_STRIP\n";
					// This only has to be done once per primitive
					convertedToTriangleList = true;

					auto triangleStrip = std::move(index_vector);
					faces.reserve(triangleStrip.size());

					for (size_t i = 2 ; i < triangleStrip.size(); ++i) {
						faces[i-2].indx.push_back(triangleStrip[i - 2]);
						faces[i-2].indx.push_back(triangleStrip[i - 1]);
						faces[i-2].indx.push_back(triangleStrip[i]);
					}
				}

			case TG3_MODE_TRIANGLES:
				if (!convertedToTriangleList) {
					std::cout << "TRIANGLES\n";
					faces.resize(index_vector.size() / 3);

					for (size_t i = 0; i < index_vector.size(); i += 3) {
						faces[i / 3].indx.push_back(index_vector[i]);
						faces[i / 3].indx.push_back(index_vector[i + 1]);
						faces[i / 3].indx.push_back(index_vector[i + 2]);
					}

				}
				uint32_t attribute_count = primitive_j.attributes_count;
				for (int k = 0; k < attribute_count; k++)
				{
					const tg3_str_int_pair& attribute_k = primitive_j.attributes[k];
					const tg3_str& attribute_key = attribute_k.key;
					const int32_t attribute_value = attribute_k.value;


					const tg3_accessor& accessor_k = model.accessors[attribute_value];
					const tg3_buffer_view& buffer_view_k = model.buffer_views[accessor_k.buffer_view];
					const tg3_buffer& buffer_k = model.buffers[buffer_view_k.buffer];
					const auto dataAddress = buffer_k.data.data + buffer_view_k.byte_offset +
						accessor_k.byte_offset;
					int32_t byteStride = tg3_accessor_byte_stride(&accessor_k, &buffer_view_k);
					const auto count = accessor_k.count;

					std::cout << "\tcurrent attribute has count " << count
						<< " and stride " << byteStride << " bytes\n";

					std::string attribute_name{ attribute_key.data, attribute_key.len };
					std::cout << "\tattribute string is : " <<
						attribute_name << '\n';

					if (!attribute_name.compare("POSITION")) {
						std::cout << "found position attribute\n";

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

						switch (accessor_k.type) {
						case TG3_TYPE_VEC3: {
							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT:
							{
								std::cout << "Type is FLOAT\n";
								v3fArray positions(
									arrayAdapter<v3f>(dataAddress, count, byteStride));

								std::cout << "positions's size : " << positions.size()
									<< '\n';

								for (size_t i = 0; i < positions.size(); ++i) {
									const auto& v = positions[i];

									vs.push_back(vec3{ v.x * scale,v.y * scale,v.z * scale });
								}
								break;
							}
							case TG3_COMPONENT_TYPE_DOUBLE: {
								std::cout << "Type is DOUBLE\n";
								// 3D vector of float
								v3dArray positions(
									arrayAdapter<v3d>(dataAddress, count, byteStride));

								std::cout << "positions's size : " << positions.size()
									<< '\n';

								for (size_t i = 0; i < positions.size(); ++i) {
									const auto& v = positions[i];

									vs.push_back(vec3{ v.x * scale,v.y * scale,v.z * scale });
								}
								break;
							}
							default:
							}
							break;
						default:
							break;
						}
						}
					}
					else if (!attribute_name.compare("NORMAL")) {
						std::cout << "found normal attribute\n";

						switch (accessor_k.type) {
						case TG3_TYPE_VEC3: {
							std::cout << "Normal is VEC3\n";
							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT: {
								std::cout << "Normal is FLOAT\n";
								v3fArray normals(
									arrayAdapter<v3f>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < normals.size(); ++i) {
									const auto& vn = normals[i];

									vns.push_back(vec3{ vn.x,vn.y,vn.z });
								}
							} break;
							case TG3_COMPONENT_TYPE_DOUBLE: {
								std::cout << "Normal is DOUBLE\n";
								v3dArray normals(
									arrayAdapter<v3d>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < normals.size(); ++i) {
									const auto& vn = normals[i];

									vns.push_back(vec3{ vn.x,vn.y,vn.z });
								}
							} break;
							default:
								std::cerr << "Unhandeled componant type for normal\n";
							}
						} break;
						default:
							std::cerr << "Unhandeled vector type for normal\n";
						}

					}
					else if (!attribute_name.compare("TEXCOORD_0"))
					{
						std::cout << "Found texture coordinates\n";
						switch (accessor_k.type) {
						case TG3_TYPE_VEC2: {
							std::cout << "TEXTCOORD is VEC2\n";

							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT: {
								std::cout << "Normal is FLOAT\n";
								v2fArray uvs(
									arrayAdapter<v2f>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < uvs.size(); ++i) {
									const auto& uv = uvs[i];
									vts.push_back(vec2{ uv.x,uv.y });
								}
							}
							case TG3_COMPONENT_TYPE_DOUBLE: {
								std::cout << "Normal is DOUBLE\n";
								v2dArray uvs(
									arrayAdapter<v2d>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < uvs.size(); ++i) {
									const auto& uv = uvs[i];
									vts.push_back(vec2{ uv.x,uv.y });
								}
							}
							default:
							}

						}
						default:

						}
					}
				}
				break;
			}


			int32_t material_indx = primitive_j.material;
			int32_t mode = primitive_j.mode;
			if (mode != -1 && mode != TG3_MODE_TRIANGLES)
			{
				std::cout << "Warning: Only triangle meshes are supported for now! Skipping primitive " << j << " in mesh " << i << std::endl;
				continue;
			}
			uint32_t arribute_count = primitive_j.attributes_count;

			// looping through attributes
			for (int k = 0; k < arribute_count; k++)
			{
				const tg3_str_int_pair& attribute_k = primitive_j.attributes[k];
				const tg3_str& attribute_key = attribute_k.key;
				const int32_t attribute_value = attribute_k.value;
			}
		}
	}
}

void read_gltf::read_materials()
{
	uint32_t material_count = model.materials_count;

	for (int i = 0; i < material_count; i++)
	{
		const tg3_material& material_i = model.materials[i];
		tg3_str material_name = material_i.name;
		std::cout << "Reading material " << i << " with name " << std::string(material_name.data, material_name.len) << std::endl;
		double emmissive_factor[3];
		emmissive_factor[0] = material_i.emissive_factor[0];
		emmissive_factor[1] = material_i.emissive_factor[1];
		emmissive_factor[2] = material_i.emissive_factor[2];
		std::string alpha_mode = std::string(material_i.alpha_mode.data, material_i.alpha_mode.len);
		int32_t double_sided = material_i.double_sided;
		tg3_pbr_metallic_roughness pbr_metallic_roughness_i = material_i.pbr_metallic_roughness;
		tg3_normal_texture_info normal_texture_i = material_i.normal_texture;
		tg3_occlusion_texture_info occlusion_texture_i = material_i.occlusion_texture;
		tg3_texture_info emissive_texture_i = material_i.emissive_texture;
	}
}
