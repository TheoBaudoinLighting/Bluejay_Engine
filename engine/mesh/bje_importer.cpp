

#include "bje_importer.h"

#include <filesystem>

void import::BJE_Importer::init(rpr_scene scene_, rpr_context rpr_context_)
{
	

}

rpr_shape import::BJE_Importer::load_obj(const std::string& filename, rpr_scene scene_, rpr_context rpr_context_)
{
	std::filesystem::path path = filename;

	std::cout << "Loading mesh -> " << path.filename() << std::endl;

	rpr_shape mesh = nullptr;

	{
		mesh = ImportOBJ(filename.c_str(), scene_, rpr_context_);
		garbage_collector_.add(mesh);

		RadeonProRender::matrix m = RadeonProRender::rotation_x(MY_PI); // rotate 180 degrees around X axis
		CHECK(rprShapeSetTransform(mesh, RPR_TRUE, &m.m00)); // set transform
	}

	std::cout << "Mesh " << path.filename() << " sucessfully load." << std::endl;
	
	return mesh;
}





