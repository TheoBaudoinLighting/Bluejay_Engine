// Window class for the engine
#pragma once

#include <memory>


#include "BJE_Radeon.h"
#include "bje_imgui.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "config/bje_window_config.h"
#include "bje_opengl.h"
#include "user_interface/bje_debug.h"


class BJE_window : public config::BJE_window_config
{
public:
	// Constructor
	BJE_window() : BJE_window_(nullptr), is_running_(true), is_fullscreen_(nullptr), key_states_{ 0 }, key_up_(0),
		key_down_(0), mouse_wheel_(0), is_clicked_X_(false), is_key_pressed_(false),
		is_left_mouse_clicked_(false), is_right_mouse_clicked_(false), is_middle_mouse_clicked_(false),
		mouse_position_(0.0f), mouse_motion_(0.0f)
	{
		opengl_context_ = std::make_unique<bje_opengl::BJE_OpenGL>();
		imgui_context_ = std::make_unique<bje_imgui::BJE_Imgui>();
		radeon_context_ = std::make_unique<bje_radeon::BJE_Radeon>();


	}

	~BJE_window();

	// Callbacks
	bool init(int width, int height, const std::string& title);
	void render();
	void* get_window() override { return BJE_window_; }

	void set_native_window(void* window) override
	{
		BJE_window_ = static_cast<GLFWwindow*>(window);
	}

	static BJE_window* from_native_window(GLFWwindow* window);

	void scroll(double xoffset, double yoffset) override;
	void key_callback(int key, int scancode, int action, int mods) override;
	void resize(int width, int height) override;
	void close() override;
	bool is_running()
	{
		return is_running_;
	}

	int get_height() 
	{
		return height_;
	}

	int get_width() 
	{
		return width_;
	}

	void update(GLFWwindow* window);

private:
	GLFWwindow* BJE_window_;

	std::unique_ptr<bje_opengl::BJE_OpenGL> opengl_context_;
	std::unique_ptr<bje_imgui::BJE_Imgui> imgui_context_;
	std::unique_ptr<bje_radeon::BJE_Radeon> radeon_context_;
	

	// UI components
	std::unique_ptr<ui_render::BJE_Debug> debug_console_;

	/*GLuint vertex_buffer_id_ = radeon_context_->get_vertex_buffer();
	GLuint index_buffer_id_ = radeon_context_->get_index_buffer();

	GLuint texture_ = radeon_context_->get_texture();*/



	unsigned int batch_size_ = 15;

	bool is_running_;
	bool is_fullscreen_;

	// Inputs

	char key_states_[256];

	char key_up_;
	char key_down_;

	int mouse_wheel_;
	bool is_clicked_X_;
	bool is_key_pressed_;

	bool is_left_mouse_clicked_;
	bool is_right_mouse_clicked_;
	bool is_middle_mouse_clicked_;

	glm::vec2 mouse_position_;
	glm::vec2 mouse_motion_;
};
