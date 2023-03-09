

#include <iostream>
#include "bje_radeon.h"
#include "GLFW/glfw3.h"

namespace bje_radeon
{
	void errorFunction(void* userPtr, enum RTCError error, const char* str)
	{
		printf("error %d: %s\n", error, str);
	}


	bool BJE_Radeon::init(config::BJE_window_config* window)
	{
		// Initialize Radeon
		rpr_int status = RPR_SUCCESS;

		rpr_int pluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME);
		CHECK_NE(pluginID, -1);

		rpr_int plugins[] = { pluginID };
		size_t numPlugins = sizeof(plugins) / sizeof(plugins[0]);

		// Create context
		status = rprCreateContext(RPR_API_VERSION, plugins, numPlugins, RPR_CREATION_FLAGS_ENABLE_GPU0, nullptr, nullptr, &rpr_context_);
		CHECK(status);

		// Set active plugin
		CHECK(rprContextSetActivePlugin(rpr_context_, plugins[0]));

		// Set context material
		//CHECK( rprContextSetSceneMaterialSystem(g_context, 0, mat) );

		std::cout << "Rpr context successfully created.." << std::endl;

		return true;
	}

	/*RTCDevice BJE_Radeon::get_device()
	{
		return embree_device_;
	}

	RTCScene BJE_Radeon::get_scene()
	{
		return embree_scene_;
	}*/

	void BJE_Radeon::init_render()
	{

		
	}

	void BJE_Radeon::post_render()
	{


	}

	void BJE_Radeon::quit_render()
	{
		std::cout << "Release memory.." << std::endl;

		CHECK(rprObjectDelete(camera_)); camera_ = nullptr;


		// last 
		CheckNoLeak(rpr_context_);
		CHECK(rprObjectDelete(rpr_context_));
		std::cout << "Embree is out.." << std::endl;
	}

}
