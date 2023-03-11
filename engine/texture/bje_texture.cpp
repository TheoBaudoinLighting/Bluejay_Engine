#include "bje_texture.h"

#include <iostream>

#include "common.h"


void texture::BJE_Texture::load_texture(rpr_context& rpr_context_, rpr_material_node& material_node, std::string path)
{
	std::cout << "Loading texture -> " << path << std::endl;

	rpr_image image;
	rpr_int status = rprContextCreateImageFromFile(rpr_context_, path.c_str(), &image);

	if (status != RPR_SUCCESS)
	{
		std::cout << "Failed to load texture -> " << path << std::endl;
		return;
	}

	CHECK(status);

	CHECK(rprMaterialNodeSetInputImageDataByKey(material_node, RPR_MATERIAL_INPUT_DATA, image));

	garbage_collector_.add(image);

}
