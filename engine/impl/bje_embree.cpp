

#include "bje_embree.h"

#include <iostream>

#include "GLFW/glfw3.h"

namespace bje_embree
{
	void errorFunction(void* userPtr, enum RTCError error, const char* str)
	{
		printf("error %d: %s\n", error, str);
	}


	bool BJE_Embree::init(config::BJE_window_config* window)
	{
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
		_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

#if defined(EMBREE_SYCL_SUPPORT)
		try {
			sycl::device sycl_device(rtcSYCLDeviceSelector);
			sycl::context sycl_context(sycl_device);
			RTCDevice device = rtcNewSYCLDevice(sycl_context, "verbose=1");
			embree::printAllSYCLDevices();
			rtcReleaseDevice(device);
		}
		catch (std::exception& e) {
			std::cerr << "Failed to SYCL Embree GPU device. Reason: " << e.what() << std::endl;
			embree::printAllSYCLDevices();
			return 1;
		}
#else


		// Initialize Embree
		embree_device_ = rtcNewDevice("verbose=1");
		embree_scene_  = rtcNewScene(embree_device_);

		if (!embree_device_)
			printf("Error %d: cannot create device\n", rtcGetDeviceError(nullptr));

		// Show Embree version
		rtcReleaseDevice(embree_device_);

		// Set error function
		rtcSetDeviceErrorFunction(embree_device_, errorFunction, NULL);
		
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

#endif

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
		std::cout << "Quit Embree" << std::endl;
		// Terminate Embree
		rtcReleaseScene(embree_scene_);
		rtcReleaseDevice(embree_device_);
		std::cout << "Embree is out.." << std::endl;
	}

}
