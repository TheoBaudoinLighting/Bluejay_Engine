
#include <iostream>

#include "bje_window.h"


#include "imgui.h"

bool BJE_window::init(int width, int height, const std::string& title)
{
	width_ = width;
	height_ = height;
	title_ = title;

	opengl_context_->init(this);
	imgui_context_->init(this);
	embree_context_->init(this);

	key_up_ = 0;
	key_down_ = 0;
	mouse_wheel_ = 0;

	is_clicked_X_ = false;
	is_key_pressed_ = false;

	is_left_mouse_clicked_ = false;
	is_right_mouse_clicked_ = false;
	is_middle_mouse_clicked_ = false;

	mouse_motion_ = glm::vec2(0.0f, 0.0f);
	mouse_position_ = glm::vec2(0.0f, 0.0f);

	std::cout << "Window initialized" << std::endl;

	return is_running_;
}

BJE_window::~BJE_window()
{
	embree_context_->quit_render();
	imgui_context_->quit_render();
	opengl_context_->quit_render();
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

// update
void BJE_window::update(GLFWwindow* window)
{
	// input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(BJE_window_, true);
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
		if (ImGui::IsMouseDown(0))
		{
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
	is_running_ = false;
}

// render window
void BJE_window::render()
{
	opengl_context_->init_render();
	imgui_context_->init_render();
	embree_context_->init_render();

	// Draw here

	// Little window
	ImGui::Begin("Bluejay window");

	ImGui::Text("Hello world!");

	// Button to quit
	ImGui::Button("Quit");
	if (ImGui::IsItemClicked())
	{
		is_running_ = false;
	}

	ImGui::End();
	



	// End frame
	imgui_context_->post_render();
	opengl_context_->post_render();
	update(BJE_window_);
}



















































