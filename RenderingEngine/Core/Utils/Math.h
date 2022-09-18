#pragma once
#include<glm/glm.hpp>
class Math
{
public:
	static glm::mat4 BuildCameraMatrix(glm::vec3 cam_position, glm::vec3 target, glm::vec3 world_up);
	static glm::mat4 BuildPerspectiveProjectionMatrix(float width, float height, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovx(float fov_x, float aspect_ratio, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovy(float fov_y, float aspect_ratio, float near, float far);
};
