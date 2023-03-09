

#include <iostream>
#include "bje_window.h"
#include "bje_radeon.h"

#include <Math/mathutils.h>
#include <Math/matrix.h>

#include "GLFW/glfw3.h"

namespace bje_radeon
{
	void errorFunction(void* userPtr, enum RTCError error, const char* str)
	{
		printf("error %d: %s\n", error, str);
	}

	

	bool BJE_Radeon::init(config::BJE_window_config* window)
	{
		// Initialize Radeon
		rpr_int status = RPR_SUCCESS;

		rpr_int pluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME);
		CHECK_NE(pluginID, -1);

		rpr_int plugins[] = { pluginID };
		size_t numPlugins = sizeof(plugins) / sizeof(plugins[0]);

		// Create context
		status = rprCreateContext(RPR_API_VERSION, plugins, numPlugins, RPR_CREATION_FLAGS_ENABLE_GPU0, nullptr, nullptr, &rpr_context_);
		CHECK(status);

		// Set active plugin
		CHECK(rprContextSetActivePlugin(rpr_context_, plugins[0]));

		// Set context material
		CHECK(rprContextCreateMaterialSystem(rpr_context_, 0, &matsys_));

		char deviceName_gpu0[1024]; deviceName_gpu0[0] = 0;
		CHECK(rprContextGetInfo(rpr_context_, RPR_CONTEXT_GPU0_NAME, sizeof(deviceName_gpu0), deviceName_gpu0, 0));

		// Output the name of the GPU
		std::cout << "GPU0 name : " << std::string(deviceName_gpu0) << std::endl;

		window->width_;
		window->height_;

		std::cout << "Rpr context successfully created.." << std::endl;

		// TODO : make that properly in function

		// Create scene
		std::cout << "Creating scene.." << std::endl;

		CHECK(rprContextCreateScene(rpr_context_, &scene_));

		// Env light
		CHECK(CreateNatureEnvLight(rpr_context_, scene_, garbage_collector_, 0.9f));

		// Create camera
		{
			CHECK(rprContextCreateCamera(rpr_context_, &camera_));

			// Set camera position
			CHECK(rprCameraLookAt(camera_, 0.f, 5.f, 20.f, 0, 1, 0, 0, 1, 0));

			// set camera field of view
			CHECK(rprCameraSetFocalLength(camera_, 75.f));

			// Set camera for the scene
			CHECK(rprSceneSetCamera(scene_, camera_));
		}
		// Set scene to render for the context
		CHECK(rprContextSetScene(rpr_context_, scene_));

		rpr_shape mesh = nullptr;
		{
			mesh = ImportOBJ("Meshes/teapot.obj", scene_, rpr_context_);
			garbage_collector_.GCAdd(mesh);

			RadeonProRender::matrix m = RadeonProRender::rotation_x(MY_PI);
			CHECK(rprShapeSetTransform(mesh, RPR_TRUE, &m.m00));
		}

		// create the floor 
		CHECK(CreateAMDFloor(rpr_context_, scene_, matsys_, garbage_collector_, 1.f, 1.f));

		// Create a material for the mesh
		{
			rpr_image uber_image1 = nullptr;
			CHECK(rprContextCreateImageFromFile(rpr_context_, "Textures/lead_rusted_Base_Color.jpg", &uber_image1)); // Good path
			garbage_collector_.GCAdd(uber_image1);

			rpr_image uber_image2 = nullptr;
			CHECK(rprContextCreateImageFromFile(rpr_context_, "Textures/lead_rusted_Normal.jpg", &uber_image2));
			garbage_collector_.GCAdd(uber_image2);

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


			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR , uberMat2_imgTexture1));
			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL, matNormalMap));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT, 1, 1, 1, 1));

			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, uberMat2_imgTexture2));
			CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL, matNormalMap));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,1, 1 , 1, 1));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, 0, 0, 0, 0));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY, 0, 0, 0, 0));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION, 0, 0, 0, 0));
			CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE, RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
			CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR, 1.36, 1.36, 1.36, 1.36));

			CHECK(rprShapeSetMaterial(mesh, uberMat2));
		}

		CHECK(rprContextSetParameterByKey1f(rpr_context_, RPR_CONTEXT_DISPLAY_GAMMA, 2.2f)); // gamma correction

		const unsigned int WIN_WIDTH = width_;
		const unsigned int WIN_HEIGHT = height_;

		rpr_framebuffer_desc desc = { WIN_WIDTH, WIN_HEIGHT };

		rpr_framebuffer_format format = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
		CHECK(rprContextCreateFrameBuffer(rpr_context_, format, &desc, &frame_buffer_));
		CHECK(rprContextCreateFrameBuffer(rpr_context_, format, &desc, &frame_buffer_2_));

		// Set framebuffer for the context
		CHECK(rprContextSetAOV(rpr_context_, RPR_AOV_COLOR, frame_buffer_));

		// framebuffer for the context
		CHECK(rprContextSetAOV(rpr_context_, RPR_AOV_COLOR, frame_buffer_));

		// Define update callback
		//CHECK(rprContextSetParameterByKeyPtr(rpr_context_, RPR_CONTEXT_RENDER_UPDATE_CALLBACK_FUNC, (void*)gui_render_impl::notifyUpdate));
		// TODO : check if this is needed

		// Building the scene
		std::cout << "Scene building done.." << std::endl;
		
		return rpr_context_;
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

	void BJE_Radeon::init_render()
	{

		glGenBuffers(1, &vertex_buffer_id_);
		glGenBuffers(1, &index_buffer_id_);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);

		float quadVertexData[] =
		{
			-1, -1, 0.5, 0, 0,
			1, -1, 0.5, 1, 0,
			1,  1, 0.5, 1, 1,
			-1,  1, 0.5, 0, 1
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

		// Create texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);

		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BJE_Radeon::post_render()
	{


	}

	void BJE_Radeon::quit_render()
	{
		std::cout << "Release memory.." << std::endl;

		CHECK(rprObjectDelete(camera_)); camera_ = nullptr;
		CHECK(rprObjectDelete(matsys_)); matsys_ = nullptr;
		CHECK(rprObjectDelete(frame_buffer_)); frame_buffer_ = nullptr;
		CHECK(rprObjectDelete(frame_buffer_2_)); frame_buffer_2_ = nullptr;

		garbage_collector_.GCClean();

		CHECK(rprObjectDelete(scene_)); scene_ = nullptr;

		// last 
		CheckNoLeak(rpr_context_);
		CHECK(rprObjectDelete(rpr_context_));
		std::cout << "Embree is out.." << std::endl;
	}

	void BJE_Radeon::render()
	{
		// Render the scene with Radeon ProRender

		// Bind vertex & index buffers of a quad
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);

		// Set shaders
		GLuint program = shader_manager_.get_program("out/Bluejay/Debug");
		glUseProgram(program);

		// Set texture with the image rendered by FR
		GLuint texture_loc = glGetUniformLocation(program, "g_Texture");
		CHECK_GE(texture_loc, 0);

		glUniform1i(texture_loc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_FLOAT, static_cast<const GLvoid*>(g_fbdata));         

		GLuint position_attr_id = glGetAttribLocation(program, "inPosition");
		GLuint texcoord_attr_id = glGetAttribLocation(program, "inTexcoord");

		glVertexAttribPointer(position_attr_id, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glVertexAttribPointer(texcoord_attr_id, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(position_attr_id);
		glEnableVertexAttribArray(texcoord_attr_id);

		// Draw quad with the texture on top of it
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(position_attr_id);
		glDisableVertexAttribArray(texcoord_attr_id);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
		
	}

}
