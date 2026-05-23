#include "read_gltf.h"
#include "../Geometry/triangle_mesh.h"
#include <iostream>
#include <sstream>

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
	int msg_level = 0;
	std::string message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);

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
	print_message(message, msg_level);
	message = "Finished reading the glTF file.";
	print_message(message, msg_level);
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
	print_message(message, msg_level);
	message = "The number of errors is " + std::to_string(errors.count);
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);

	read_objects();
	read_materials();
	int low = 0;
	int high = static_cast<int>(faces.size()-1);
	add_item(low, high);

}

void read_gltf::read_objects()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading objects from the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);

	int mesh_count = model.meshes_count;

	std::string object;
	std::string group;
	int32_t material_id;
	int group_id = 0;

	// looping through meshes
	for (int i = 0; i < mesh_count; i++)
	{
		// some debugging variables
		msg_level = 1;
		int32_t face_count_i = 0;
		int32_t v_count_i = 0;
		int32_t vt_count_i = 0;
		int32_t vn_count_i = 0;


		const tg3_mesh& mesh_i = model.meshes[i];
		uint32_t primitive_count = mesh_i.primitives_count;
		object = std::string(mesh_i.name.data, mesh_i.name.len);
		message = "Reading object " + std::to_string(i) + ": " + object;
		print_message(message, msg_level);


		// looping through primitives
		for (int j = 0; j < primitive_count; j++)
		{
			// some debugging variables
			msg_level = 2;
			int32_t face_count_j = 0;
			int32_t v_count_j = 0;
			int32_t vt_count_j = 0;
			int32_t vn_count_j = 0;

			group = std::to_string(group_id++);
			message = "Reading group " + group;
			print_message(message, msg_level);
			// Boolean used to check if we have converted the vertex buffer format
			bool convertedToTriangleList = false;
			const tg3_primitive& primitive_j = mesh_i.primitives[j];
			material_id = primitive_j.material;




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
				print_message(message, msg_level);
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
					print_message(message, msg_level);
					// This only has to be done once per primitive
					convertedToTriangleList = true;

					// We steal the guts of the vector
					auto triangleFan = std::move(index_vector);


					// Push back the indices that describe just one triangle one by one
					for (size_t k = 2 ; k < triangleFan.size(); ++k) {
						std::vector<int> ind = { triangleFan[0],triangleFan[k - 1],triangleFan[k] };
						int ind_size = static_cast<int>(ind.size());
						faces.emplace_back(ind, object, group, ind_size, material_id);
						face_count_j++;
					}
				}
			case TG3_MODE_TRIANGLE_STRIP:
				if (!convertedToTriangleList) {
					message = "Primitive mode is TRIANGLE_STRIP, converting to triangle list";
					print_message(message, msg_level);
					// This only has to be done once per primitive
					convertedToTriangleList = true;

					auto triangleStrip = std::move(index_vector);


					for (size_t k = 2 ; k < triangleStrip.size(); ++k) {
						std::vector<int> ind = { triangleStrip[k - 2],triangleStrip[k - 1],triangleStrip[k] };
						int ind_size = static_cast<int>(ind.size());
						faces.emplace_back(ind, object, group, ind_size, material_id);
						face_count_j++;
					}
				}

			case TG3_MODE_TRIANGLES:
				if (!convertedToTriangleList) {
					message = "Primitive mode is TRIANGLES";
					print_message(message, msg_level);
					// This only has to be done once per primitive
					convertedToTriangleList = true;


					for (size_t k = 0; k < index_vector.size(); k += 3) {
						std::vector<int> ind = { index_vector[k],index_vector[k+1],index_vector[k+2] };
						int ind_size = static_cast<int>(ind.size());
						face_indx2 face_i = { ind, object, group, ind_size, material_id };
						faces.push_back(face_i);
						face_count_j++;
					}

				}
				uint32_t attribute_count = primitive_j.attributes_count;
				for (int k = 0; k < attribute_count; k++)
				{
					msg_level=5;
					const tg3_str_int_pair& attribute_k = primitive_j.attributes[k];
					const tg3_str& attribute_key = attribute_k.key;
					const int32_t attribute_value = attribute_k.value;
					std::string attribute_name{ attribute_key.data, attribute_key.len };

					message = "Reading the attribute with name " + attribute_name;
					print_message(message, msg_level);


					const tg3_accessor& accessor_k = model.accessors[attribute_value];
					const tg3_buffer_view& buffer_view_k = model.buffer_views[accessor_k.buffer_view];
					const tg3_buffer& buffer_k = model.buffers[buffer_view_k.buffer];
					const auto dataAddress = buffer_k.data.data + buffer_view_k.byte_offset +
						accessor_k.byte_offset;
					int32_t byteStride = tg3_accessor_byte_stride(&accessor_k, &buffer_view_k);
					const auto count = accessor_k.count;

					message = "Current attribute has count " + std::to_string(count) +
						" and stride " + std::to_string(byteStride) + " bytes";
					msg_level=6;
					print_message(message, msg_level);

					

					msg_level=7;
					if (!attribute_name.compare("POSITION")) {
						message = "Parsing positions";
						print_message(message, msg_level);

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

						msg_level=8;
						switch (accessor_k.type) {
						case TG3_TYPE_VEC3: {
							message = "Type is vec3 ";
							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT:
							{
								message += "and component type is float";
								print_message(message, msg_level);
								v3fArray positions(
									arrayAdapter<v3f>(dataAddress, count, byteStride));



								for (size_t i = 0; i < positions.size(); ++i) {
									const auto& v = positions[i];

									vs.push_back(vec3{ v.x * scale,v.y * scale,v.z * scale });
									v_count_j++;
								}
								break;
							}
							case TG3_COMPONENT_TYPE_DOUBLE: {
								message += "and component type is double";
								print_message(message, msg_level);
								// 3D vector of float
								v3dArray positions(
									arrayAdapter<v3d>(dataAddress, count, byteStride));


								for (size_t i = 0; i < positions.size(); ++i) {
									const auto& v = positions[i];
									vs.push_back(vec3{ v.x * scale,v.y * scale,v.z * scale });
									v_count_j++;
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
						msg_level=7;
						message = "Parsing normals";
						print_message(message, msg_level);
						msg_level=8;

						switch (accessor_k.type) {
						case TG3_TYPE_VEC3: {
							message = "Type is vec3 ";
							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT:
							{
								message += "and component type is float";
								print_message(message, msg_level);
								v3fArray normals(
									arrayAdapter<v3f>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < normals.size(); ++i) {
									const auto& vn = normals[i];
									vns.push_back(vec3{ vn.x,vn.y,vn.z });
									vn_count_j++;
								}
							}
							break;
							case TG3_COMPONENT_TYPE_DOUBLE:
							{
								message += "and component type is double";
								print_message(message, msg_level);
								v3dArray normals(
									arrayAdapter<v3d>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < normals.size(); ++i) {
									const auto& vn = normals[i];
									vns.push_back(vec3{ vn.x,vn.y,vn.z });
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
					else if (!attribute_name.compare("TEXCOORD_0"))
					{
						msg_level=7;
						message = "Parsing texture coordinates";
						print_message(message, msg_level);
						msg_level=8;
						switch (accessor_k.type) {
						case TG3_TYPE_VEC2: {
							message = "Type is vec2 ";

							switch (accessor_k.component_type) {
							case TG3_COMPONENT_TYPE_FLOAT: {
								message += "and component type is float";
								print_message(message, msg_level);
								v2fArray uvs(
									arrayAdapter<v2f>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < uvs.size(); ++i) {
									const auto& uv = uvs[i];
									vts.push_back(vec2{ uv.x,uv.y });
									vt_count_j++;
								}
								break;
							}
							case TG3_COMPONENT_TYPE_DOUBLE: {
								message += "and component type is double";
								print_message(message, msg_level);
								v2dArray uvs(
									arrayAdapter<v2d>(dataAddress, count, byteStride));

								// For each triangle :
								for (size_t i = 0; i < uvs.size(); ++i) {
									const auto& uv = uvs[i];
									vts.push_back(vec2{ uv.x,uv.y });
									vt_count_j++;
								}
								break;
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

			msg_level = 2;
			message = "Finished reading primitive " + std::to_string(j);
			print_message(message, msg_level);
			message = "The number of faces is " + std::to_string(face_count_j);
			print_message(message, msg_level + 1);
			message = "The number of vertices is " + std::to_string(v_count_j);
			print_message(message, msg_level + 1);
			message = "The number of texture coordinates is " + std::to_string(vt_count_j);
			print_message(message, msg_level + 1);
			message = "The number of normals is " + std::to_string(vn_count_j);
			print_message(message, msg_level + 1);

			face_count_i += face_count_j;
			v_count_i += v_count_j;
			vt_count_i += vt_count_j;
			vn_count_i += vn_count_j;
		}
		msg_level = 1;
		message = "Finished reading object " + std::to_string(i) + ": " + object;
		print_message(message, msg_level);
		message = "The number of faces is " + std::to_string(face_count_i);
		print_message(message, msg_level+1);
		message = "The number of vertices is " + std::to_string(v_count_i);
		print_message(message, msg_level + 1);
		message = "The number of texture coordinates is " + std::to_string(vt_count_i);
		print_message(message, msg_level + 1);
		message = "The number of normals is " + std::to_string(vn_count_i);
		print_message(message, msg_level + 1);
	}

	message = "Finished reading all meshes";
	print_message(message, 0);
	message = "The total number of faces is " + std::to_string(faces.size());
	print_message(message, 1);
	message = "The total number of vertices is " + std::to_string(vs.size());
	print_message(message, 1);
	message = "The total number of texture coordinates is " + std::to_string(vts.size());
	print_message(message, 1);
	message = "The total number of normals is " + std::to_string(vns.size());
	print_message(message, 1);


	message = std::string(52, '=');
	print_message(message, 0);
	message = "Finished reading objects from the glTF file.";
	print_message(message, 0);
	message = std::string(52, '=');
	print_message(message, 0);
}

void read_gltf::read_materials()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading materials from the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);

	uint32_t material_count = model.materials_count;

	for (int i = 0; i < material_count; i++)
	{
		const tg3_material& material_i = model.materials[i];
		tg3_str material_name = material_i.name;
		msg_level = 1;
		message = "Reading material " + std::to_string(i) +
			": " + std::string(material_name.data, material_name.len);
		print_message(message, msg_level);
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

	msg_level = 0;
	message = "Finished reading all materials"; 
	print_message(message, msg_level);
	message = "The total number of materials is " + std::to_string(material_count);
	print_message(message, msg_level + 1);
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Finished reading materials from the glTF file.";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
}


void read_gltf::add_item(const int& _low, const int& _hi)
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Adding objects to the hittable_list...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);


	int low = _low;
	if (low < 0)
		low = 0;
	int hi = _hi;
	if (hi >= faces.size())
		hi = static_cast<int>(faces.size());


	int num_triangles = 0;
	std::string current_group;
	std::string current_obj;

	for (int i = _low; i < hi; i++)
	{
		std::string group = faces[i].group;
		std::string object = faces[i].object;
		std::array<point3, 3> vs_i, vns_i;
		std::array<point2, 3> vts_i;



		if (!silent && !object.empty() && object.compare(current_obj))
		{
			current_obj = object;
			msg_level = 1;
			print_message("Adding object " + object, msg_level);
		}
		if (!silent && !group.empty() && group.compare(current_group))
		{
			current_group = group;
			msg_level = 2;
			print_message("Adding group " + group, msg_level);
		}
		else if (!silent && group.empty() && (i-low) % 100 == 0)
		{
			msg_level = 1;
			print_message("Adding item " + 
				std::to_string(i) + 
				" out of " + 
				std::to_string(faces.size()), msg_level);
		}

		auto& face = faces[i];
		object = face.object;
		group = face.group;

		int num_edges = static_cast<int>(face.indx.size());

		for (int j = 0; j < num_edges; j++)
		{
			int indx_j = face.indx[j];

			// bound checking for the indices
			// as vts and vns are optional we check them only if they are mentioned in the face
			if (j < 0 ||
				j >= vs.size() ||
				(j >= vts.size() && vts.size() > 0) ||
				(j >= vns.size() && vns.size() > 0)) {
				throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
			}

			// if we do not have the vs so there is nothing in this edge!
			vs_i[j] = vs[indx_j];
			// if the vt has not been mentioned so we do not care about possibly!
			if (vts.size() > 0) {
				vts_i[j] = vts[indx_j];
			}
			else {
				vts_i[j] = point2{ 0, 0 };
			}
			if (vns.size() > 0) {
				vns_i[j] = vns[indx_j];
			}
			else {
				vns_i[j] = point3();
			}
		}
		// mapping from obj material into the mtl file material
		// we ourselves set the mat_indx so it is zero based!
		int mat_indx = face.mat_indx;

		// estimating missing vt vectors
		if (!vns.size())
		{
			estimate_vns(vs_i,vns_i);
		}

		auto trngle = std::make_unique<triangle_mesh>(vs_i, vts_i, vns_i, mat_indx);
		trngle->add_label(object);
		trngle->add_label(group);
		world->add(std::move(trngle));
	}


	// setting all the read objects as main label
	world->add_label("main");


	msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Finished adding objects to the hittable_list.";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
}

void read_gltf::estimate_vns(
	const std::array<point3, 3>& vs_,
	std::array<point3, 3>& vns_)
{
	constexpr int num_edges = 3;
	for (int i = 0; i < num_edges; i++)
	{
		int prev = i ? i - 1 : num_edges - 1;
		int next = (i != num_edges - 1) ? i + 1 : 0;
		vec3 a = vs_[next] - vs_[i];
		vec3 b = vs_[prev] - vs_[i];
		vec3 normal = cross(a, b);
		vec3 unit_normal = unit_vector(normal);
		vns_[i] = unit_normal;
	}
}
