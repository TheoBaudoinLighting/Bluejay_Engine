
#include <iostream>

#include "glad/glad.h"
#include "bje_window.h"

// imgui
#include "imgui.h"

// ui
#include "user_interface/bje_debug.h"



bool BJE_window::init(int width, int height, const std::string& title)
{
	// data
	width_ = width;
	height_ = height;
	title_ = title;

	// Context init
	opengl_context_->init(this);
	imgui_context_->init(this);
	radeon_context_->init(this);

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
	// Créer une instance de la classe "BJE_window"
	BJE_window* bje_window = new BJE_window();

	// Stocker le pointeur de la fenêtre native GLFW dans l'instance de la classe "BJE_window"
	bje_window->set_native_window(window);

	// Retourner l'instance de la classe "BJE_window"
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
	// move this here to avoid crash on close
	// the destructor is called after each
	// BJE_window object is destroyed

	radeon_context_->quit_render();
	imgui_context_->quit_render();
	opengl_context_->quit_render();

	// Shutdown the renderer
	is_running_ = false;
}


// render all data here 

// render window
void BJE_window::render()
{
	// get fps
	int fps = ImGui::GetIO().Framerate;
	bool close_window = debug_console_->get_close_window();

	opengl_context_->init_render();
	imgui_context_->init_render();
	radeon_context_->init_render();

	// Draw here

	
	radeon_context_->render();



	// Créer une texture OpenGL pour stocker le rendu de Radeon ProRender
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Créer un framebuffer Radeon ProRender lié à la texture OpenGL
	rpr_framebuffer_desc desc = { width, height };
	desc.format = RPR_FRAMEBUFFER_FORMAT_FLOAT4;
	desc.depth = 1;
	rpr_framebuffer frame_buffer;
	rprContextCreateFrameBuffer(g_rpr_context, desc, &frame_buffer);
	rprContextSetAOV(g_rpr_context, RPR_AOV_COLOR, frame_buffer);

	// Effectuer le rendu de la scène à l'aide de Radeon ProRender
	rprContextRender(g_rpr_context);
	rprFrameBufferGetInfo(frame_buffer, RPR_FRAMEBUFFER_DATA_FLOAT4, g_fbdata.get(), &size);

	// Lier la texture OpenGL et afficher le rendu dans la fenêtre GLFW
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, g_fbdata.get());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1);
	glTexCoord2f(1, 0); glVertex2f(1, -1);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(0, 1); glVertex2f(-1, 1);
	glEnd();









	// Debug console
	debug_console_->render();

	

	if (close_window == true)
	{
		is_running_ = false;
	}
	

	// End frame
	imgui_context_->post_render();
	radeon_context_->post_render();
	opengl_context_->post_render();

	// update window
	update(BJE_window_);
}



















































