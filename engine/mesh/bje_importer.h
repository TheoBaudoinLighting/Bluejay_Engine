#pragma once

#include <cassert>
#include <iostream>
#include <map>

#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Export
#include "../common/common.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "picojson.h"
#include "../common/tiny_obj_loader.h"
// Importer

class BJE_Importer
{
public:

	/*struct BJE_Shape_Setting
	{
		std::string name = "teapot";
		std::string path = "Meshes/teapot.obj";

		RadeonProRender::float3 position = RadeonProRender::float3(0.0f, 0.0f, 0.0f);
		RadeonProRender::float3 rotation = RadeonProRender::float3(0.0f, 0.0f, 0.0f);
		RadeonProRender::float3 scale = RadeonProRender::float3(1.0f, 1.0f, 1.0f);
	};

	const BJE_Shape_Setting parse_shape_settings(const picojson::value& config);

	void import_obj(rpr_context& context, rpr_scene& scene, rpr_material_system matsys, const BJE_Shape_Setting);*/









};











