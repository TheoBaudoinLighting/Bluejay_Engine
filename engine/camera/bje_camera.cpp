#include "bje_camera.h"



camera::BJE_Camera::BJE_Camera()
{
	
}

camera::BJE_Camera::BJE_Camera(const BJE_Camera& other)
{
	// Create a perfect copy of the camera object
	*this = other;
}

void camera::BJE_Camera::init(rpr_scene scene_, rpr_context rpr_context_)
{
	auto eye_ = glm::vec3(0.0f, 5.0f, 20.0f);
	auto at_ = glm::vec3(0.0f, 1.0f, 0.0f);
	auto up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	float fov_ = 75.0f;

	CHECK(rprContextCreateCamera(rpr_context_, &camera_));

	set_lookat(camera_, eye_, at_, up_);
	set_fov(camera_, fov_);
	set_scene(camera_, scene_);


}

void camera::BJE_Camera::set_scene(rpr_camera camera_, rpr_scene scene_)
{
	CHECK(rprSceneSetCamera(scene_, camera_));
}


void camera::BJE_Camera::set_lookat(rpr_camera camera_, glm::vec3 eye, glm::vec3 at, glm::vec3 up)
{
	CHECK(rprCameraLookAt(camera_, eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z));
}

void camera::BJE_Camera::offset_orientation(float dx, float dy)
{
	pitch -= dy;
	yaw -= dx;


	update_camera();
}

void camera::BJE_Camera::set_fov(rpr_camera camera_, float fov)
{
	CHECK(rprCameraSetFocalLength(camera_, fov));
}

camera::BJE_Camera::~BJE_Camera()
{
	
}

void camera::BJE_Camera::update_camera()
{
		// Update the camera's position
	position = glm::vec3(radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch)), radius * sin(glm::radians(pitch)), radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	position += pivot;

	// Update the camera's forward vector
	forward = glm::normalize(pivot - position);

	// Update the camera's right vector
	right = glm::normalize(glm::cross(forward, worldUp));

	// Update the camera's up vector
	up = glm::normalize(glm::cross(right, forward));

	// Update the camera's lookat
	set_lookat(camera_, position, pivot, up);
}

void camera::BJE_Camera::destroy()
{
	CHECK(rprObjectDelete(camera_));
}




