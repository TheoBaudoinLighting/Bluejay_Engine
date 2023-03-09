// Bluejay Engine Version: 1.0
// Author: Theo Baudoin
// Date: 2022-03-07
// Description: Main file of the Bluejay Engine

// Includes
#include <iostream>
#include <string>
#include <vector>

#include <BJE_OpenGL.h>

#include "RadeonProRender.h"
#include "RadeonProRender_GL.h"

#include "common.h"
#include <glm.hpp>

#include "bje_window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gtc/constants.hpp"


class core
{
public:

	core(int width, int height, const std::string& title)
	{
		bje_window_ = std::make_unique<BJE_window>();
		bje_window_->init(width, height, title);
	}

	static core& instance() { return *bje_instance_; } // if you want to use it as a singleton

	void main_loop()
	{
		while(bje_window_->is_running())
		{
			bje_window_->render();
		}
	}

private:
	static core* bje_instance_;

	std::unique_ptr<BJE_window> bje_window_;
};



// Main function
int main()
{
	auto core_BJE = std::make_unique<core>(1280, 800, "Bluejay Engine");

	core_BJE->main_loop();
}