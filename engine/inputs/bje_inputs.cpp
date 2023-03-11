

//
//mouse_drag_info mouse_camera_;
//
//bje_radeon::BJE_Radeon* radeon_context_;
//
//void BJE_Inputs::on_mouse_move_event(int x, int y, GLFWwindow* window)
//{
//	int delaX = (x - mouse_camera_.mousePosAtMouseButtonDown_X);
//	int delaY = -(y - mouse_camera_.mousePosAtMouseButtonDown_Y);
//
//	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//	{
//		//is_left_mouse_clicked_ = true;
//
//		RadeonProRender::matrix rotZ = RadeonProRender::rotation(mouse_camera_.up, (float)delaX * 0.001);
//
//		RadeonProRender::float3 look_at_vec = mouse_camera_.lookat - mouse_camera_.pos;
//		look_at_vec.normalize();
//
//		RadeonProRender::float3 left = RadeonProRender::cross(mouse_camera_.up, look_at_vec);
//		left.normalize();
//
//		RadeonProRender::matrix rot_left = RadeonProRender::rotation(left, (float)delaY * 0.001);
//		RadeonProRender::matrix new_mat = rot_left * rotZ * mouse_camera_.mat;
//
//		rprCameraSetTransform(radeon_context_->get_camera(), false, &new_mat.m00);
//	}
//
//	CHECK(rprFrameBufferClear(radeon_context_->get_frame_buffer()));
//}
//
//void BJE_Inputs::on_mouse_button_event(int button, int action, int x, int y, GLFWwindow* window)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		mouse_camera_.leftMouseButtonDown = true;
//
//		rprCameraGetInfo(radeon_context_->get_camera(), RPR_CAMERA_LOOKAT, sizeof(mouse_camera_.lookat), &mouse_camera_.lookat, 0);
//		rprCameraGetInfo(radeon_context_->get_camera(), RPR_CAMERA_UP, sizeof(mouse_camera_.up), &mouse_camera_.up, 0);
//		rprCameraGetInfo(radeon_context_->get_camera(), RPR_CAMERA_POSITION, sizeof(mouse_camera_.pos), &mouse_camera_.pos, 0);
//		rprCameraGetInfo(radeon_context_->get_camera(), RPR_CAMERA_TRANSFORM, sizeof(mouse_camera_.mat), &mouse_camera_.mat, 0);
//
//		mouse_camera_.mousePosAtMouseButtonDown_X = x;
//		mouse_camera_.mousePosAtMouseButtonDown_Y = y;
//
//	}
//
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
//	{
//		is_left_mouse_clicked_ = false;
//		mouse_camera_.leftMouseButtonDown = false;
//	}
//
//	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//	{
//		is_right_mouse_clicked_ = true;
//	}
//
//	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
//	{
//		is_right_mouse_clicked_ = false;
//	}
//
//	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
//	{
//		is_middle_mouse_clicked_ = true;
//	}
//
//	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
//	{
//		is_middle_mouse_clicked_ = false;
//	}
//}
//
//void BJE_Inputs::on_key_event(int key, int action, GLFWwindow* window)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//	}
//
//	if (key == GLFW_KEY_W && action == GLFW_PRESS)
//	{
//		key_up_ = 'z';
//	}
//
//	if (key == GLFW_KEY_S && action == GLFW_PRESS)
//	{
//		key_down_ = 's';
//	}
//
//	if (key == GLFW_KEY_A && action == GLFW_PRESS)
//	{
//		key_left_ = 'q';
//	}
//
//	if (key == GLFW_KEY_D && action == GLFW_PRESS)
//	{
//		key_right_ = 'd';
//	}
//
//	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
//	{
//		key_up_ = ' ';
//	}
//
//	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
//	{
//		key_down_ = ' ';
//	}
//
//	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
//	{
//		key_left_ = ' ';
//	}
//
//	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
//	{
//		key_right_ = ' ';
//	}
//}
