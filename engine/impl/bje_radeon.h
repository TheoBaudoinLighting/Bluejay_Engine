#pragma once


#include "glad/glad.h"
#include "bje_context.h"
#include "RadeonProRender.h"
#include "RadeonProRender_GL.h"
#include "common.h"

namespace bje_radeon
{
	class BJE_Radeon : public context::BJE_RenderContext
	{
	public:
		bool init(config::BJE_window_config* window) override;

		void init_render() override;
		void post_render() override;
		void quit_render() override;

		/*RTCDevice get_device();
		RTCScene get_scene();*/

	private:

		rpr_context rpr_context_ = NULL;
		GLuint              vertex_buffer_id_ = 0;
		GLuint              index_buffer_id_ = 0;
		GLuint              texture_ = 0;
		rpr_framebuffer		frame_buffer_ = NULL;
		rpr_framebuffer     frame_buffer_2_ = NULL;
		rpr_material_system matsys_ = NULL;
		//ShaderManager       g_shader_manager;
		rpr_scene			scene_ = nullptr;
		rpr_camera			camera_ = nullptr;
		std::shared_ptr<float>	fbdata_ = nullptr;
		RPRGarbageCollector gc_;
		bool                ask_exit_ = false; // push X key to exit

	};
}
