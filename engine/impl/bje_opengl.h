// Description: OpenGL implementation of the engine
#pragma once
#include "bje_context.h"

namespace bje_opengl
{
	class BJE_OpenGL : public context::BJE_RenderContext
	{
	public:
		bool init(int width, int height, config::BJE_window_config* window) override;

		void init_render() override;
		void post_render() override;
		void quit_render() override;
	};
}
