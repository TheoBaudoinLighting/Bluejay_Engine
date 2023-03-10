#pragma once


#include <thread>

#include "glad/glad.h"
#include "bje_context.h"
#include "../window/bje_window.h"
#include "RadeonProRender.h"
#include "RadeonProRender_GL.h"
#include "common.h"
#include "imgui.h"
#include "camera/bje_camera.h"
#include "GLFW/glfw3.h"
#include "mesh/bje_importer.h"
#include "shader/bje_shader.h"


class BJE_window;

namespace bje_radeon
{
	class BJE_Radeon : public context::BJE_RenderContext
	{
	public:
		bool init(int width, int height, config::BJE_window_config* window) override;

		void init_render() override;
		bool init_graphic();
		void post_render() override;
		void quit_render() override;

		void render_phase_01();
		void render_phase_02();

		void on_mouse_move_event(GLFWwindow* window);
		void on_mouse_button_event(int button, int action, int x, int y);
		void on_key_event(unsigned char key, int xmouse, int ymouse);

		bool get_context();
		bool get_scene();

		//GLuint get_vertex_buffer() { return vertex_buffer_id_; }
		//GLuint get_index_buffer() { return index_buffer_id_; }
		//GLuint get_texture() { return texture_; }
		rpr_framebuffer get_frame_buffer() { return frame_buffer_; }
		rpr_framebuffer get_frame_buffer_2() { return frame_buffer_2_; }
		//rpr_material_system get_material() { return matsys_; }
		//rpr_camera get_camera() { return camera_; }
		//RPRGarbageCollector get_garbage_collector() { return garbage_collector_; }
		//std::shared_ptr<float> get_fbdata() { return fbdata_; }
		//bool get_ask_exit() { return ask_exit_; }

	private:
		// GLFW window
		int width_ = 0;
		int height_ = 0;
		config::BJE_window_config* window_ = nullptr;

		// Iteration
		const int batch_size_ = 15;
		bool frame_ready_ = false;

		// Import other class
		camera::BJE_Camera camera_;
		import::BJE_Importer importer_;


		ImVec2 size_ = ImVec2(0, 0);

		rpr_context rpr_context_ = nullptr;
		GLuint vertex_buffer_id_ = 0;
		GLuint index_buffer_id_ = 0;
		GLuint texture_ = 0;
		rpr_framebuffer frame_buffer_ = nullptr;
		rpr_framebuffer frame_buffer_2_ = nullptr;
		rpr_material_system matsys_;
		BJE_Shader shader_manager_;
		rpr_scene scene_ = nullptr;
		//rpr_camera camera_ = nullptr;
		std::shared_ptr<float> fbdata_ = nullptr;
		RPRGarbageCollector garbage_collector_;
		bool ask_exit_ = false; // push X key to exit
	};
}
