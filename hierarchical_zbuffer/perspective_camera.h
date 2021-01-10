#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fov, float aspect, float znear = 0.1f, float zfar = 10000.0f)
		: Camera(fov, aspect, znear, zfar) { }

	glm::mat4x4 getProjectionMatrix() override {
		return glm::perspective(glm::radians(_fov), _aspect, _znear, _zfar);
	}
};