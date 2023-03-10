
#include <iostream>

#include "glad/glad.h"
#include "bje_window.h"

// imgui
#include "imgui.h"

// ui
#include "Math/mathutils.h"
#include "user_interface/bje_debug.h"

struct MOUSE_DRAG_INFO
{
	MOUSE_DRAG_INFO()
	{
		leftMouseButtonDown = false;
		mousePosAtMouseButtonDown_X = -1;
		mousePosAtMouseButtonDown_Y = -1;
	}

	RadeonProRender::float3 lookat;
	RadeonProRender::float3 up;
	RadeonProRender::float3 pos;
	RadeonProRender::matrix mat;

	int	mousePosAtMouseButtonDown_X;
	int	mousePosAtMouseButtonDown_Y;

	bool leftMouseButtonDown;
};
MOUSE_DRAG_INFO mouse_camera_;

bool BJE_window::init(int width, int height, const std::string& title)
{
	// data
	width_ = width;
	height_ = height;
	// print width and height
	/*std::cout << "Window width: " << width_ << std::endl;
	std::cout << "Window height: " << height_ << std::endl;*/

	title_ = title;

	// Context init
	opengl_context_->init(width_, height_, this);
	imgui_context_->init(width_, height_, this);
	radeon_context_->init(width_, height_, this);

	std::cout << "All data proccesing sucessfuly" << std::endl;

	// UI init
	debug_console_ = std::make_unique<ui_render::BJE_Debug>();






	std::cout << "Window initialized correctly" << std::endl;

	return is_running_;
}

BJE_window::~BJE_window()
{
	
}

// key callback
void BJE_window::key_callback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		key_states_[key] = 1;
		key_down_ = key;

		is_key_pressed_ = true;
	}
	else if (action == GLFW_RELEASE)
	{
		key_states_[key] = 0;
		key_up_ = key;
		is_key_pressed_ = false;
	}
}

// scroll callback
void BJE_window::scroll(double xoffset, double yoffset)
{
	mouse_wheel_ = yoffset;
}

// resize callback
void BJE_window::resize(int width, int height)
{
	width_ = width;
	height_ = height;

	render();
}

BJE_window* BJE_window::from_native_window(GLFWwindow* window)
{
	BJE_window* bje_window = new BJE_window();
	bje_window->set_native_window(window);
	return bje_window;
}



// update
void BJE_window::update(GLFWwindow* window)
{
	// input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		std::cout << "Escape key pressed" << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		is_key_pressed_ = true;
		key_down_ = GLFW_KEY_DOWN;
	}
	else
	{
		is_key_pressed_ = false;
		key_up_ = GLFW_KEY_DOWN;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		key_up_ = 'z';
		std::cout << "Key up pressed" << std::endl;
	}

	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		key_down_ = 's';
		std::cout << "Key down pressed" << std::endl;
	}

	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		key_up_ = 'q';
		std::cout << "Key left pressed" << std::endl;
	}

	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		key_down_ = 'd';
		std::cout << "Key right pressed" << std::endl;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		is_left_mouse_clicked_ = true;
	else
		is_left_mouse_clicked_ = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		is_right_mouse_clicked_ = true;
	else
		is_right_mouse_clicked_ = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		is_middle_mouse_clicked_ = true;
	else
		is_middle_mouse_clicked_ = false;

	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	mouse_motion_.x = xpos - mouse_position_.x;
	mouse_motion_.y = ypos - mouse_position_.y;

	mouse_position_.x = xpos;
	mouse_position_.y = ypos;

	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && ImGui::IsAnyMouseDown())
	{
		int delaX = (xpos - mouse_camera_.mousePosAtMouseButtonDown_X);
		int delaY = -(ypos - mouse_camera_.mousePosAtMouseButtonDown_Y);

		if (ImGui::IsMouseDown(0))
		{
			RadeonProRender::matrix rotZ = RadeonProRender::rotation(mouse_camera_.up, (float)delaX * 0.001);

			RadeonProRender::float3 lookAtVec = mouse_camera_.lookat - mouse_camera_.pos;
			lookAtVec.normalize();

			RadeonProRender::float3 left = RadeonProRender::cross(mouse_camera_.up, lookAtVec);
			left.normalize();

			RadeonProRender::matrix rotleft = RadeonProRender::rotation(left, (float)delaY * 0.001);
			RadeonProRender::matrix newMat = rotleft * rotZ * mouse_camera_.mat;

			rprCameraSetTransform(radeon_context_->get_camera(), false, &newMat.m00);


			ImVec2 mouse_delta = ImGui::GetMouseDragDelta(0, 0);
			//scene->
			ImGui::ResetMouseDragDelta(0);
		}
		else if (ImGui::IsMouseDown(1))
		{
			ImVec2 mouse_delta = ImGui::GetMouseDragDelta(1, 0);
			//scene->
			ImGui::ResetMouseDragDelta(1);
		}
		else if (ImGui::IsMouseDown(2))
		{
			ImVec2 mouse_delta = ImGui::GetMouseDragDelta(2, 0);
			//scene->
			ImGui::ResetMouseDragDelta(2);
		}
		//scene->dirty = true;
	}

	// quit window
	if (glfwWindowShouldClose(window))
	{
		is_running_ = false;

	}
}

// on close
void BJE_window::close()
{
	// move this here to avoid crash on close
	// the destructor is called after each
	// BJE_window object is destroyed

	imgui_context_->quit_render();
	opengl_context_->quit_render();
	radeon_context_->quit_render();

	// Shutdown the renderer
	is_running_ = false;
}



// render window
void BJE_window::render()
{
	// get fps
	int fps = ImGui::GetIO().Framerate;
	bool close_window = debug_console_->get_close_window();

	opengl_context_->init_render();
	imgui_context_->init_render();

	// Draw here




	
	radeon_context_->render_phase_01(); // render phase 01

	radeon_context_->render_phase_02(); // render phase 02








	const float clear_color[] = { 0.45f, 0.55f, 0.60f, 1.00f }; // clear color
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]); // set clear color
	
	// Debug console
	debug_console_->render();

	

	if (close_window == true)
	{
		is_running_ = false;
	}
	

	// End frame
	imgui_context_->post_render();
	opengl_context_->post_render();
	radeon_context_->post_render();

	// update window
	update(BJE_window_);
}



















































