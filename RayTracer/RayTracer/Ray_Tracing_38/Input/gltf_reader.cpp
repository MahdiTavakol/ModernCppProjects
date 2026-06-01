#include "gltf_reader.h"
#include "../Geometry/triangle_mesh.h"
#include "../external/stb_image.h"
#include <iostream>
#include <sstream>
#include <fstream>

gltf_reader::gltf_reader(const std::string& file_path_, communicator* para_) :
	para{para_},
	file_path{ file_path_ },
	world{ std::make_unique<hittable_list>() },
	mtl_list{ std::make_unique<material_list>() }
{
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
	read_textures();
	read_samplers();
	load_images();
	int low = 0;
	int high = static_cast<int>(faces.size()-1);
	add_item(low, high);

}

void gltf_reader::read_objects()
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

					int offset = static_cast<int>(vs.size());
					for (size_t k = 0; k < index_vector.size(); k += 3) {
						std::vector<int> ind = { 
							index_vector[k] + offset,
							index_vector[k+1] + offset,
							index_vector[k+2] + offset };
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
							    message = "Warning the component type is unknown!";
								print_message(message,msg_level);
							    break;
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
					else if (!attribute_name.compare(0,9,"TEXCOORD_"))
					{
						msg_level=7;
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
									vts_array[coord_i].push_back(vec2{uv.x,uv.y});
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
									vts_array[coord_i].push_back(vec2{ uv.x,uv.y });
									vt_count_j++;
								}
								break;
							}
							default:
								message = "Warning the component type is unknown!";
								print_message(message,msg_level);
							}
							break;
						}
						default:
							message = "Warning unknown!";
							print_message(message,msg_level);
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
	int num_texture_coords = 0;
	for (auto& v : vts_array)
	{
		num_texture_coords += static_cast<int>(v.size());
	}
	message = "The total number of texture coordinates is " + std::to_string(num_texture_coords);
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

void gltf_reader::read_materials()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading materials from the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);



	message = "Adding materials to the material list";
	msg_level = 1;
	print_message(message, msg_level);

	uint32_t material_count = model.materials_count;
	for (int i = 0; i < material_count; i++)
	{
		const tg3_material& material_i = model.materials[i];
		std::string material_name = std::string(material_i.name.data,material_i.name.len);
		msg_level = 2;
		message = "Creating material " + std::to_string(i) +
			": " + material_name;
		print_message(message, msg_level);
		std::unique_ptr<material> mat_i = std::make_unique<PBR>(material_i);
		mtl_list->push_back(material_name,std::move(mat_i));
		std::unique_ptr<material> test_mat = std::make_unique<lambertian>(vec3(0.0, 1.0, 0.0));
		//mtl_list->push_back(material_name, std::move(test_mat));
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

void gltf_reader::read_textures()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading textures from the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);


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

		print_message(message, msg_level);
		read_textures[i] = tex_i;
	}

	msg_level = 1;
	message = "Finished reading all texture";
	print_message(message, msg_level);
	msg_level = 2;
	message = "Adding textures to the PBR class";
	print_message(message, msg_level);
	PBR::set_textures(read_textures, textures_count);

	msg_level = 1;
	message = "Cleaning up!";
	print_message(message, msg_level);
	delete[] read_textures;



	message = "The total number of texture is " + std::to_string(textures_count);
	print_message(message, msg_level + 1);
	message = std::string(52, '=');
	print_message(message, msg_level);
	msg_level = 0;
	message = "Finished reading textures from the glTF file.";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
}

void gltf_reader::read_samplers()
{
	std::string message;
	int msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Reading samplers from the glTF file...";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);


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

		print_message(message, msg_level);
		read_samplers[i] = smp_i;
	}

	msg_level = 1;
	message = "Finished reading all samplers";
	print_message(message, msg_level);
	msg_level = 2;
	message = "Adding samplers to the PBR class";
	print_message(message, msg_level);
	PBR::set_samplers(read_samplers,samplers_count);

	msg_level = 1;
	message = "Cleaning up!";
	print_message(message, msg_level);
	delete[] read_samplers;



	message = "The total number of samplers is " + std::to_string(samplers_count);
	print_message(message, msg_level + 1);
	message = std::string(52, '=');
	print_message(message, msg_level);
	msg_level = 0;
	message = "Finished reading samplers from the glTF file.";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
}

void gltf_reader::load_images()
{
	std::string message;
	int msg_level;

	msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Loading images";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);

	tg3_image_result* loaded_images;
	loaded_images = new tg3_image_result[model.images_count];

	for (int i = 0; i < model.images_count; i++)
	{
		msg_level = 2;
		message = "Loading image-" + std::to_string(i);
		print_message(message, msg_level);

		const tg3_image* image_i = &model.images[i];
		tg3_image_result& loaded_image_i = loaded_images[i];
		if (image_i->buffer_view && image_i->uri.data)
		{
			throw std::invalid_argument("Both the buffer_view and uri cannot be specified for an image at the same time!");
		}
		if (image_i->uri.data)
		{
			throw std::invalid_argument("The uri option for image is not supported yet!");
		}
		else if (image_i->buffer_view)
		{
			const tg3_buffer_view& buffer_view_i = model.buffer_views[image_i->buffer_view];
			const tg3_buffer& buffer_i = model.buffers[buffer_view_i.buffer];
			const auto dataAddress = buffer_i.data.data + buffer_view_i.byte_offset;
			int size = static_cast<int>(buffer_view_i.byte_length);
			
			int w = 0, h = 0, comp = 0, req_comp = 0;

			// decoding the image header
			if (!stbi_info_from_memory(dataAddress, size, &w, &h, &comp))
			{
				throw std::invalid_argument("Unknown format!");
			}

			int bits = 8;
			
			if (stbi_is_16_bit_from_memory(dataAddress, size))
			{
				bits = 16;
			}


			unsigned char* data = nullptr;
			if (bits == 16)
			{
				data = reinterpret_cast<unsigned char*>(
					stbi_load_16_from_memory(dataAddress, size, &w, &h, &comp, req_comp));
			}
			// load as 8 bit per channel
			if (!data)
			{
				data = stbi_load_from_memory(dataAddress, size, &w, &h, &comp, req_comp);
				if (!data)
				{
					throw std::invalid_argument("The stb cannot convert the image!");
				}
				bits = 8;
			}

			if ((w < 1) || (h < 1))
			{
				throw std::invalid_argument("Wrong image format");
			}

			loaded_image_i.width = w;
			loaded_image_i.height = h;
			loaded_image_i.component = comp;
			loaded_image_i.bits = bits;

			// allocating data
			int num_elements = w * h * comp * static_cast<int>(bits / 8);
			loaded_image_i.pixels = new uint8_t[num_elements];
			std::copy(data, data + num_elements, loaded_image_i.pixels);

			//cleaning up the data
			stbi_image_free(data);



			std::string debug_file = "test" + std::to_string(i) + ".ppm";

			if (i == 5)
				print_image(&loaded_image_i, debug_file);

		}


	}

	msg_level = 1;
	message = "Adding images to the PBR material";
	print_message(message, msg_level);
	PBR::set_images(loaded_images, model.images_count);

	msg_level = 1;
	message = "Cleaning up!";
	print_message(message, msg_level);
	delete[] loaded_images;


	msg_level = 0;
	message = std::string(52, '=');
	print_message(message, msg_level);
	message = "Finished adding images";
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
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
		//file << "655535" <<std::endl;
		//for (int i = 0; i < h * w; i++)
		//	file << static_cast<unsigned short>(image_->pixels[i]);
		file << "655535" << std::endl;
		for (int i = 0; i < bytes * h * w * comp; i++)
		{
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
		std::array<point2, 3> vts_i, vts_1_i;




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
				(j >= vns.size() && vns.size() > 0)) {
				throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
			}

			for (int k = 0; k < MAX_COORD; k++)
			{
				int vts_array_k_size = static_cast<int>(vts_array[k].size());
				if (vts_array_k_size > 0)
				{
					if (j >= vts_array_k_size)
						throw std::invalid_argument("Face index out of bounds for edge " + std::to_string(j));
				}
				else if (vts_array_k_size == 0)
				{
					break;
				}
			}


			// if we do not have the vs so there is nothing in this edge!
			vs_i[j] = vs[indx_j];

			if (vts_array[0].size() > 0)
			{
				vts_i[j] = vts_array[0][indx_j];
			}
			else
			{
				vts_i[j] = point2{};
			}
			if (vts_array[1].size() > 0)
			{
				vts_1_i[j] = vts_array[1][indx_j];
			}
			else
			{
				vts_1_i[j] = point2{};
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

		auto trngle = std::make_unique<triangle_mesh>(vs_i, vts_i,vts_1_i, vns_i, mat_indx);
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
	message = "The total number of items in the hittable_list is "
		+ std::to_string(world->size());
	print_message(message, msg_level);
	message = std::string(52, '=');
	print_message(message, msg_level);
}

void gltf_reader::estimate_vns(
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


std::unique_ptr<hittable_list> gltf_reader::return_world()
{
	if (world == nullptr)
		throw std::invalid_argument("The world is empty!");
	return std::move(world);
}

std::unique_ptr<material_list> gltf_reader::return_mtl_list()
{
	if (mtl_list == nullptr)
		throw std::invalid_argument("The material list is empty!");
	
	return std::move(mtl_list);
}