// Config file for the window module
#pragma once

#include <RadeonProRender_v2.h>
#include <string>

#include "config/bje_window_config.h"

namespace context
{
	class BJE_RenderContext
	{
	public:
		BJE_RenderContext() : window_(nullptr)
		{
		}

		virtual bool init(int width, int height, config::BJE_window_config* window)
		{
			window_ = window;
			width_ = width;
			height_ = height;
			return true;
		}

		virtual void init_render() = 0;
		virtual void post_render() = 0;
		virtual void quit_render() = 0;

	protected:
		config::BJE_window_config* window_;

		int width_ = 0;
		int height_ = 0;

	};
}
