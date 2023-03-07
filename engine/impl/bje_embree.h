#pragma once

#if defined(EMBREE_SYSCL_SUPPORT)
#include <sycl/sycl.hpp>
#endif
#include "embree4/rtcore.h"

#if defined(EMBREE_SYCL_SUPPORT)
#include "../common/sycl/util.h"
#endif

// use embree namespace
RTC_NAMESPACE_USE

#include <xmmintrin.h>

// define embree SSE denormals zero mode
#if !defined(_MM_SET_DENORMALS_ZERO_MODE)
#define _MM_DENORMALS_ZERO_ON   (0x0040)
#define _MM_DENORMALS_ZERO_OFF  (0x0000)
#define _MM_DENORMALS_ZERO_MASK (0x0040)
#define _MM_SET_DENORMALS_ZERO_MODE(x) (_mm_setcsr((_mm_getcsr() & ~_MM_DENORMALS_ZERO_MASK) | (x)))
#endif


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
