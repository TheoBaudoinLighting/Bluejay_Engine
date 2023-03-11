#pragma once
#include "common.h"
#include "RadeonProRender_v2.h"
#include "glm.hpp"


namespace camera
{
	class BJE_Camera
	{
	public:

		BJE_Camera();
		BJE_Camera(const BJE_Camera& other);
		~BJE_Camera();

		void init(rpr_scene scene_, rpr_context rpr_context_);

		void set_lookat(rpr_camera camera_, glm::vec3 eye, glm::vec3 at, glm::vec3 up);
		void offset_orientation(float dx, float dy);
		void set_fov(rpr_camera camera_, float fov);
		void set_scene(rpr_camera camera_, rpr_scene scene_);

		void destroy();

		rpr_camera camera_ = nullptr;

		float fov_;
		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;

		float focalDist;
		float aperture;
		float fov;
		float width_length;
		float height_length;
		float diagonal_length;

		bool isMoving;

		unsigned int width;
		unsigned int height;
		
	private:

		void update_camera();

		glm::vec3 worldUp;
		glm::vec3 pivot;

		float pitch;
		float radius;
		float yaw;

	};

}


