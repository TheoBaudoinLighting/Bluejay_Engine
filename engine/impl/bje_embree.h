#pragma once
#include "embree4/rtcore.h"
#include "bje_context.h"


namespace bje_embree
{
	class BJE_Embree : public context::BJE_RenderContext
	{
	public:
		bool init(config::BJE_window_config* window) override;

		void init_render() override;
		void post_render() override;
		void quit_render() override;

	private:

		RTCDevice embree_device_ = nullptr;
		RTCScene embree_scene_ = nullptr;
	};
}
