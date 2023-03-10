#pragma once


#include "glad/glad.h"
#include "bje_context.h"
#include "../window/bje_window.h"
#include "RadeonProRender.h"
#include "RadeonProRender_GL.h"
#include "common.h"
#include "GLFW/glfw3.h"
#include "shader/bje_shader.h"


class BJE_window;

namespace bje_radeon
{
	class BJE_Radeon : public context::BJE_RenderContext
	{
	public:
		bool init(int width, int height, config::BJE_window_config* window) override;

		void init_render() override;
		void init_graphic();
		void post_render() override;
		void quit_render() override;

		void render_phase_01();
		void render_phase_02();

		bool get_context();
		bool get_scene();

		/*GLuint get_vertex_buffer() { return vertex_buffer_id_; }
		GLuint get_index_buffer() { return index_buffer_id_; }
		GLuint get_texture() { return texture_; }
		rpr_framebuffer get_frame_buffer() { return frame_buffer_; }
		rpr_framebuffer get_frame_buffer_2() { return frame_buffer_2_; }
		rpr_material_system get_material() { return matsys_; }
		rpr_camera get_camera() { return camera_; }
		RPRGarbageCollector get_garbage_collector() { return garbage_collector_; }
		std::shared_ptr<float> get_fbdata() { return fbdata_; }
		bool get_ask_exit() { return ask_exit_; }*/

	private:

		// GLFW window
		int width_ = 0;
		int height_ = 0;

		// Iteration
		const int batch_size_ = 15;


		rpr_context         rpr_context_ = NULL;
		GLuint              vertex_buffer_id_ = 0;
		GLuint              index_buffer_id_ = 0;
		GLuint              texture_ = 0;
		rpr_framebuffer		frame_buffer_ = NULL;
		rpr_framebuffer     frame_buffer_2_ = NULL;
		rpr_material_system matsys_ = NULL;
		BJE_Shader          shader_manager_;
		rpr_scene			scene_ = nullptr;
		rpr_camera			camera_ = nullptr;
		std::shared_ptr<float>	fbdata_ = nullptr;
		RPRGarbageCollector garbage_collector_;
		bool                ask_exit_ = false; // push X key to exit

	};
}
