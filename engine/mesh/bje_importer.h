#pragma once

#include <cassert>
#include <iostream>
#include <map>

#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Export
#include "../common/common.h"

#include "picojson.h"
#include "../common/tiny_obj_loader.h"
#include "garbage/bje_garbage_collector.h"


namespace import
{
	class BJE_Importer
	{
	public:

		BJE_Importer() {}
		~BJE_Importer() {}

		void init(rpr_scene scene_, rpr_context rpr_context_);

		rpr_shape load_obj(const std::string& filename, rpr_scene scene_, rpr_context rpr_context_);



	private:

		BJE_Garbage garbage_collector_;

		

	};

}









