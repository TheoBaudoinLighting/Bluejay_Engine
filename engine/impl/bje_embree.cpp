

#include "embree4/rtcore.h"
#include "bje_embree.h"

#include <iostream>

#include "GLFW/glfw3.h"

namespace bje_embree
{
	bool BJE_Embree::init(config::BJE_window_config* window)
	{
		// Initialize Embree
		embree_device_ = rtcNewDevice(NULL);
		embree_scene_  = rtcNewScene(embree_device_);
		
		if (!embree_device_)
		{
			std::cout << "Error: cannot create Embree device" << std::endl;
			return false;
		}

		if (!embree_scene_)
		{
			std::cout << "Error: cannot create Embree scene" << std::endl;
			return false;
		}


		return true;
	}

	void BJE_Embree::init_render()
	{

		
	}

	void BJE_Embree::post_render()
	{


	}

	void BJE_Embree::quit_render()
	{
		// Terminate Embree
		rtcReleaseScene(embree_scene_);
		rtcReleaseDevice(embree_device_);
	}

}
