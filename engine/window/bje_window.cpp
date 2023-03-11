
#include <iostream>

#include "bje_window.h"

// imgui
#include "imgui.h"

// ui
#include "Math/mathutils.h"
#include "user_interface/bje_debug.h"

bje_radeon::BJE_Radeon* radeon_context_;


struct mouse_drag_info
{
	mouse_drag_info()
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
mouse_drag_info mouse_camera_;
//auto camera_ = radeon_context_->get_camera();

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

void BJE_window::update(GLFWwindow* window)
{
	key_pressed = false;

	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && ImGui::IsAnyMouseDown())
	{
		if (ImGui::IsMouseDown(0))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(0, 0);
			//camera_->offset_orientation(mouseDelta.x, mouseDelta.y);
			
			RadeonProRender::matrix rotZ = RadeonProRender::rotation(mouse_camera_.up, (float)mouseDelta.x * 0.001);

			RadeonProRender::float3 look_at_vec = mouse_camera_.lookat - mouse_camera_.pos;
			look_at_vec.normalize();

			RadeonProRender::float3 left = RadeonProRender::cross(mouse_camera_.up, look_at_vec);
			left.normalize();

			RadeonProRender::matrix rot_left = RadeonProRender::rotation(left, (float)mouseDelta.y * 0.001);
			RadeonProRender::matrix new_mat = rot_left * rotZ * mouse_camera_.mat;

			//rprCameraSetTransform(mouse_camera_, false, &new_mat.m00);

			//CHECK(rprFrameBufferClear(radeon_context_->get_frame_buffer()));

			ImGui::ResetMouseDragDelta(0);
		}
		else if (ImGui::IsMouseDown(1))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(1, 0);
			






			ImGui::ResetMouseDragDelta(1);
		}
		else if (ImGui::IsMouseDown(2))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2, 0);







			ImGui::ResetMouseDragDelta(2);
		}

	}

	//CHECK(rprFrameBufferClear(radeon_context_->get_frame_buffer()));

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

	//update(BJE_window_);
	//
}




















































