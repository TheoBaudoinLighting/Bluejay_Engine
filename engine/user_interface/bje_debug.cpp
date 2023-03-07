#include "bje_debug.h"

#include "bje_window.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

BJE_window window;

namespace ui_render
{
	void ui_render::BJE_Debug::resize(int width, int height)
	{
		// TODO : make probably something here for resize.. or not :)
	}

	void ui_render::BJE_Debug::init_ui()
	{
		// Setup debug console here

	}

	void ui_render::BJE_Debug::render()
	{
		// Start the Dear ImGui frame
		ImGui::Begin("Bluejay debug");

		// get fps
		int fps = ImGui::GetIO().Framerate;
		bool is_running = window.is_running();

		/*int width = window.get_width();
		int height = window.get_height();*/


		// fps plot line
		ImGui::Text("FPS: %d", fps);

		// get frames
		if (frame.size() > 100)
		{
			for (int i = 1; i < frame.size(); i++)
			{
				frame[i - 1] = frame[i];
			}
			frame[frame.size() - 1] = fps;
		}
		else
		{
			frame.push_back(fps);
		}

		ImGui::PlotLines("FPS", frame.data(), frame.size(), 0, nullptr, 0.0f, 200.0f, ImVec2(0, 80));
		ImGui::Text("Mouse Position: (%f, %f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);

		ImGui::Separator();

		// show is running
		ImGui::Text("Is running: %s", is_running ? "true" : "false");

		


















		// quit button
		if (ImGui::Button("Quit"))
		{
			close_window_ = true;
		}



		ImGui::End();
	}


	

	void ui_render::BJE_Debug::quit_ui()
	{
		// Post Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
