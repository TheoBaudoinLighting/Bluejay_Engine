#pragma once
#include <string>

#include "RadeonProRender_v2.h"
#include "garbage/bje_garbage_collector.h"

namespace texture
{
	class BJE_Texture
	{
	public:

		BJE_Texture();
		~BJE_Texture();

		void load_texture(rpr_context& rpr_context_, rpr_material_node& material_node, std::string path);


	private:

		BJE_Garbage garbage_collector_;

	};




}

