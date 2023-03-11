// Config file for the ui module
#pragma once

#include <future>
#include <string>
#include <vector>

#include "glm.hpp"

namespace ui_render
{
	class BJE_Debug
	{
	public:

		BJE_Debug()
		{
			init_ui();
		}
		~BJE_Debug() = default;

		void init_ui();
		void render();
		void resize(int width, int height);
		void quit_ui();

		bool get_close_window() { return close_window_; }

	private:

		bool close_window_ = false;

		glm::vec2 size_{};
		std::vector<float> frame;
		std::future<void> async_renderer_;
	};
}
