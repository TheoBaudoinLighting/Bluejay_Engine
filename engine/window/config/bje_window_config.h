// Config file for the window module
#pragma once

#include <string>

namespace config
{
	// Window config class
	class BJE_window_config
	{
	public:
		// Constructor
		virtual void* get_window() = 0;
		virtual void set_native_window(void* window) = 0;

		// Callbacks
		virtual void scroll(double xoffset, double yoffset) = 0;
		virtual void key_callback(int key, int scancode, int action, int mods) = 0;
		virtual void resize(int width, int height) = 0;
		virtual void close() = 0;

		// Getters

		int width_;
		int height_;
		std::string title_;
	};
}
