
#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"
#include "bje_opengl.h"

#include <iostream>
#include <RadeonProRender_v2.h>

#include "GLFW/glfw3.h"


namespace bje_opengl
{
	bool BJE_OpenGL::init(int width, int height, config::BJE_window_config* window)
	{
		std::cout << "Initialization OpenGL.." << std::endl;

		// Set the window
		__super::init(width_, height_, window);

		auto glsl_version = "#version 460";

		// Initialize GLFW
		if (!glfwInit())
		{
			std::cout << "Error: Failed to initialize GLFW" << std::endl;
			return false;
		}

		// Create a window
		GLFWwindow* window_BJE = glfwCreateWindow(window->width_, window->height_, window->title_.c_str(), nullptr,
		                                          nullptr);
		window->set_native_window(window_BJE);


		if (!window_BJE)
		{
			std::cout << "Error: Failed to create a window" << std::endl;
			glfwTerminate(); // Destroy the window
			return false;
		}

		// Make the window's context current
		glfwSetWindowUserPointer(window_BJE, window);
		glfwMakeContextCurrent(window_BJE);

		// Initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Error: Failed to initialize GLAD" << std::endl;
			return false;
		}

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_NONE);

		glfwSwapInterval(1); // Enable vsync

		std::cout << "OpenGL initialized correctly" << std::endl;

		return window;
	}

	void BJE_OpenGL::init_render()
	{
		// Set states
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		glCullFace(GL_NONE);
		glDisable(GL_DEPTH_TEST);

		// Set the clear color
		//const float clear_color[] = {0.45f, 0.55f, 0.60f, 1.00f};

		// Set the viewport
		//glViewport(0, 0, window_->width_, window_->height_);

		//glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
		const int clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		glClear(clear_mask);

		// Enable texturing
		glEnable(GL_TEXTURE_2D);
	}

	void BJE_OpenGL::post_render()
	{

		//glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Poll the events
		glfwPollEvents();

		// Swap the buffers
		glfwSwapBuffers(static_cast<GLFWwindow*>(window_->get_window()));
	}

	void BJE_OpenGL::quit_render()
	{
		// Destroy the window
		glfwDestroyWindow(static_cast<GLFWwindow*>(window_->get_window()));

		// Terminate GLFW
		glfwTerminate();
	}
}
