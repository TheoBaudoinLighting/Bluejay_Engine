#include <iostream>
#include "bje_window.h"
#include "bje_radeon.h"

#include <chrono>
#include <future>
#include <Math/mathutils.h>
#include <Math/matrix.h>


#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include "assimp/postprocess.h"
#include "GLFW/glfw3.h"

namespace bje_radeon
{
	class gui_render_impl
	{
	public:
		struct update
		{
			update()
			{
				clear();
				m_progress = 0.0f;
			}

			volatile int m_hasUpdate;
			volatile int m_done;
			volatile int m_aborted;
			bool render_ready_;
			int m_camUpdated;
			float m_progress;

			void clear()
			{
				m_hasUpdate = m_done = m_aborted = m_camUpdated = 0;
			}
		};

		static void notifyUpdate(float x, void* userData)
		{
			auto demo = static_cast<update*>(userData);
			demo->m_hasUpdate = 1;
			demo->m_progress = x;
		}
	};

	gui_render_impl::update update_class;

	const auto invalid_time = std::chrono::time_point<std::chrono::high_resolution_clock>::max();
	int benchmark_number_of_render_iteration = 0;
	auto benchmark_start = invalid_time;

	struct MOUSE_DRAG_INFO
	{
		MOUSE_DRAG_INFO()
		{
			leftMouseButtonDown = false;
			mousePosAtMouseButtonDown_X = -1;
			mousePosAtMouseButtonDown_Y = -1;
		}

		RadeonProRender::float3 lookat;
		RadeonProRender::float3 up;
		RadeonProRender::float3 pos;
		RadeonProRender::matrix mat;

		int mousePosAtMouseButtonDown_X;
		int mousePosAtMouseButtonDown_Y;

		bool leftMouseButtonDown;
	};

	MOUSE_DRAG_INFO mouse_camera_;

	void error_function(void* userPtr, enum RTCError error, const char* str)
	{
		printf("error %d: %s\n", error, str);
	}



	bool BJE_Radeon::init(int width, int height, config::BJE_window_config* window)
	{
		// Enumerate each step of the initialization process

		width_ = width;
		height_ = height;

		window_ = window;

		// Init Graphics
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glCullFace(GL_NONE);
		glDisable(GL_DEPTH_TEST);

		// Viewport
		glViewport(0, 0, width_, height_);
		// Enable texturing
		glEnable(GL_TEXTURE_2D);


		// Create vertex and index buffer for a quad
		glGenBuffers(1, &vertex_buffer_id_);
		glGenBuffers(1, &index_buffer_id_);


		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);

		float quadVertexData[] =
		{
			-1, -1, 0.5, 0, 0,
			1, -1, 0.5, 1, 0,
			1, 1, 0.5, 1, 1,
			-1, 1, 0.5, 0, 1
		};

		GLshort quadIndexData[] =
		{
			0, 1, 3,
			3, 1, 2
		};


		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndexData), quadIndexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Create a texture for FR rendering
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);

		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);

		init_graphic();
		init_render();

		return true;
	}

	// get variable 
	bool BJE_Radeon::get_context()
	{
		return rpr_context_;
	}

	bool BJE_Radeon::get_scene()
	{
		return scene_;
	}

	bool BJE_Radeon::init_graphic()
	{


		std::cout << "------------------ Initializing Radeon ------------------" << std::endl;

		// Initialize Radeon

		rpr_int status = RPR_SUCCESS;
		std::cout << "RPR_API_VERSION : " << RPR_API_VERSION << std::endl;


		rpr_int pluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME);
		CHECK_NE(pluginID, -1);

		rpr_int plugins[] = {pluginID};
		size_t numPlugins = sizeof(plugins) / sizeof(plugins[0]);
		std::cout << "Number of plugins : " << numPlugins << std::endl;

		// Create Radeon ProRender context
		status = rprCreateContext(RPR_API_VERSION, plugins, numPlugins, RPR_CREATION_FLAGS_ENABLE_GPU0,
		                          context_properties, nullptr, &rpr_context_);
		CHECK(status);


		CHECK(rprContextSetActivePlugin(rpr_context_, plugins[0]));
		std::cout << "Rpr context successfully set active plugin.." << std::endl;

		CHECK(rprContextCreateMaterialSystem(rpr_context_, 0, &matsys_));

		char deviceName_gpu0[1024];
		deviceName_gpu0[0] = 0;
		CHECK(rprContextGetInfo(rpr_context_, RPR_CONTEXT_GPU0_NAME, sizeof(deviceName_gpu0), deviceName_gpu0, 0));

		std::cout << "Rpr context successfully created.." << std::endl;

		CHECK(rprContextCreateScene(rpr_context_, &scene_));

		CHECK(CreateNatureEnvLight(rpr_context_, scene_, garbage_collector_, 1.0f));


		camera_.init(scene_, rpr_context_); // init camera


		CHECK(rprContextSetScene(rpr_context_, scene_));

		auto mesh = importer_.load_obj("Meshes/teapot.obj", scene_, rpr_context_);

		//std::cout << "Loading mesh\n";
		//rpr_shape mesh = nullptr;
		//{

		//	//mesh = rpr_shape(scene);
		//	mesh = ImportOBJ("Meshes/teapot.obj", scene_, rpr_context_);
		//	garbage_collector_.GCAdd(mesh);

		//	RadeonProRender::matrix m = RadeonProRender::rotation_x(MY_PI); // rotate 180 degrees around X axis
		//	CHECK(rprShapeSetTransform(mesh, RPR_TRUE, &m.m00)); // set transform
		//}

		


		CHECK(CreateAMDFloor(rpr_context_, scene_, matsys_, garbage_collector_, 1.f, 1.f));

		{
			rpr_image uber_image1 = nullptr;
			CHECK(rprContextCreateImageFromFile(rpr_context_, "Textures/lead_rusted_Base_Color.jpg", &uber_image1));
			// Good path
			garbage_collector_.GCAdd(uber_image1);

			rpr_image uber_image2 = nullptr;
			CHECK(rprContextCreateImageFromFile(rpr_context_, "Textures/lead_rusted_Normal.jpg", &uber_image2));
			garbage_collector_.GCAdd(uber_image2);


			// CPU model of the material

			rpr_material_node uberMat2_imgTexture1 = nullptr;
			CHECK(rprMaterialSystemCreateNode(matsys_, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &uberMat2_imgTexture1));
			garbage_collector_.GCAdd(uberMat2_imgTexture1);
			CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture1, RPR_MATERIAL_INPUT_DATA, uber_image1));


			rpr_material_node uberMat2_imgTexture2 = nullptr;
			CHECK(rprMaterialSystemCreateNode(matsys_, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &uberMat2_imgTexture2));
			garbage_collector_.GCAdd(uberMat2_imgTexture2);
			CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture2, RPR_MATERIAL_INPUT_DATA, uber_image2));


			rpr_material_node matNormalMap = nullptr;
			CHECK(rprMaterialSystemCreateNode(matsys_, RPR_MATERIAL_NODE_NORMAL_MAP, &matNormalMap));
			garbage_collector_.GCAdd(matNormalMap);
			CHECK(rprMaterialNodeSetInputFByKey(matNormalMap, RPR_MATERIAL_INPUT_SCALE, 1.0f, 1.0f, 1.0f, 1.0f));
			CHECK(rprMaterialNodeSetInputNByKey(matNormalMap, RPR_MATERIAL_INPUT_COLOR, uberMat2_imgTexture2));


			rpr_material_node uberMat2 = nullptr;
			CHECK(rprMaterialSystemCreateNode(matsys_, RPR_MATERIAL_NODE_UBERV2, &uberMat2));
			garbage_collector_.GCAdd(uberMat2);

			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR, uberMat2_imgTexture1));
			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL, matNormalMap));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT, 1, 1, 1, 1));

			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, uberMat2_imgTexture1
			));
			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL, matNormalMap));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT, 1, 1, 1, 1));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, 0, 0, 0, 0));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY, 0, 0, 0, 0));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION, 0, 0,
				0, 0));
			CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,
				RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR, 1.36, 1.36, 1.36, 1.36
			));

			CHECK(rprShapeSetMaterial(mesh, uberMat2));
		}


		CHECK(rprContextSetParameterByKey1f(rpr_context_, RPR_CONTEXT_DISPLAY_GAMMA, 2.2f)); // gamma correction

		const unsigned int WIN_WIDTH = width_;
		const unsigned int WIN_HEIGHT = height_;

		rpr_framebuffer_desc desc = {WIN_WIDTH, WIN_HEIGHT};

		rpr_framebuffer_format format = {4, RPR_COMPONENT_TYPE_FLOAT32};
		CHECK(rprContextCreateFrameBuffer(rpr_context_, format, &desc, &frame_buffer_));
		CHECK(rprContextCreateFrameBuffer(rpr_context_, format, &desc, &frame_buffer_2_));
		std::cout << "025 Radeon frame buffer successfully created.." << std::endl;


		CHECK(rprContextSetAOV(rpr_context_, RPR_AOV_COLOR, frame_buffer_));

		CHECK(rprContextSetAOV(rpr_context_, RPR_AOV_COLOR, frame_buffer_));
		std::cout << "026 Radeon AOV successfully set.." << std::endl;

		rprContextSetParameterByKeyPtr(rpr_context_, RPR_CONTEXT_RENDER_UPDATE_CALLBACK_FUNC,
		                               (void*)gui_render_impl::notifyUpdate);
		rprContextSetParameterByKeyPtr(rpr_context_, RPR_CONTEXT_RENDER_UPDATE_CALLBACK_DATA, &update_class);


		std::cout << "027 Cache and scene building... " << std::endl;
		CHECK(rprContextSetParameterByKey1u(rpr_context_, RPR_CONTEXT_ITERATIONS, 1));
		std::cout << "028 done\n";

		CHECK(rprContextSetParameterByKey1u(rpr_context_, RPR_CONTEXT_ITERATIONS, batch_size_));


		fbdata_ = std::shared_ptr<float>(new float[width_ * height_ * 4], std::default_delete<float[]>());


		// Building the scene
		std::cout << "------------------ Initializing Radeon done.. ------------------" << std::endl;

		return rpr_context_;
	}

	void BJE_Radeon::post_render()
	{
	}

	void BJE_Radeon::quit_render()
	{
		std::cout << "Release memory.." << std::endl;

		camera_.destroy(); // destroy camera

		CHECK(rprObjectDelete(matsys_));
		matsys_ = nullptr;
		CHECK(rprObjectDelete(frame_buffer_));
		frame_buffer_ = nullptr;
		CHECK(rprObjectDelete(frame_buffer_2_));
		frame_buffer_2_ = nullptr;

		garbage_collector_.GCClean();

		CHECK(rprObjectDelete(scene_));
		scene_ = nullptr;

		// last 
		CheckNoLeak(rpr_context_);
		CHECK(rprObjectDelete(rpr_context_));
		std::cout << "Radeon is out.." << std::endl;
	}

	void render_job(rpr_context ctxt, gui_render_impl::update* update)
	{
		CHECK(rprContextRender(ctxt));
		//rprContextRender(ctxt);
		update->render_ready_ = true;
	}

	void BJE_Radeon::init_render()
	{
	}

	void BJE_Radeon::render_phase_01()
	{
		// Bind vertex & index buffers of a quad
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);

		// Set shaders
		GLuint program = shader_manager_.get_program("out/Bluejay/Debug/simple");
		GLuint texture_loc = glGetUniformLocation(program, "g_Texture");
		GLuint position_attr_id = glGetAttribLocation(program, "inPosition");
		GLuint texcoord_attr_id = glGetAttribLocation(program, "inTexcoord");

		glUseProgram(program);
		glUniform1i(texture_loc, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);

		glVertexAttribPointer(position_attr_id, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
		glVertexAttribPointer(texcoord_attr_id, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(position_attr_id);
		glEnableVertexAttribArray(texcoord_attr_id);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		glDisableVertexAttribArray(position_attr_id);
		glDisableVertexAttribArray(texcoord_attr_id);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	void BJE_Radeon::render_phase_02()
	{
		const auto timeUpdateStarts = std::chrono::high_resolution_clock::now();


		if (benchmark_start == invalid_time)
			benchmark_start = timeUpdateStarts;
		if (benchmark_number_of_render_iteration >= 100)
		{
			double elapsed_time_ms = std::chrono::duration<double, std::milli>(timeUpdateStarts - benchmark_start).
				count();
			double renderPerSecond = static_cast<double>(benchmark_number_of_render_iteration) * 1000.0 /
				elapsed_time_ms;
			std::cout << renderPerSecond << " iterations per second." << std::endl;
			benchmark_number_of_render_iteration = 0;
			benchmark_start = timeUpdateStarts;
		}

		update_class.clear();


		auto bje_thread = std::async(std::launch::async, &render_job, rpr_context_, &update_class);
		//std::thread bje_thread(&render_job, rpr_context_, &update_class);


		CHECK(rprContextResolveFrameBuffer(rpr_context_, frame_buffer_, frame_buffer_2_, false));
		size_t frame_buffer_dataSize = 0;
		CHECK(rprFrameBufferGetInfo(frame_buffer_2_, RPR_FRAMEBUFFER_DATA, 0, NULL, &frame_buffer_dataSize));


		if (frame_buffer_dataSize != width_ * height_ * 4 * sizeof(float))
		{
			CHECK(RPR_ERROR_INTERNAL_ERROR)
		}

		CHECK(rprFrameBufferGetInfo(frame_buffer_2_, RPR_FRAMEBUFFER_DATA, frame_buffer_dataSize, fbdata_.get(), NULL));
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_FLOAT,
		                static_cast<const GLvoid*>(fbdata_.get()));
		glBindTexture(GL_TEXTURE_2D, 0);


		update_class.render_ready_ = false;

		bje_thread.wait();
		//bje_thread.join();

		benchmark_number_of_render_iteration += batch_size_;


		// Inputs update





	}
}
