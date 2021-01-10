#pragma once

#include "input.h"
#include "perspective_camera.h"

class FpsCamera: public PerspectiveCamera {
public:
	FpsCamera(float fov, float aspect, float znear = 0.1f, float zfar = 10000.0f)
		: PerspectiveCamera(fov, aspect, znear, zfar) {}


	void update(const KeyboardInput& keyboardInput, const MouseInput& mouseInput, float deltaTime) {
		glm::vec3 movement;
		if (keyboardInput.keyPressed[GLFW_KEY_W]) {
			movement += _speed * deltaTime * glm::vec3(0.0f, 0.0f, -1.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_A]) {
			movement += _speed * deltaTime * glm::vec3(-1.0f, 0.0f, 0.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_S]) {
			movement += _speed * deltaTime * glm::vec3(0.0f, 0.0f, 1.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_D]) {
			movement += _speed * deltaTime * glm::vec3(1.0f, 0.0f, 0.0f);
		}

		translate(movement, Object3D::Space::Local);

		double xoffset = mouseInput.move.xCurrent - mouseInput.move.xOld;
		// OpenGL flip y-axis 
		double yoffset = -(mouseInput.move.yCurrent - mouseInput.move.yOld);

		glm::vec3 eulerAngles = getLocalEulerAngles(Object3D::RotateOrder::ZXY);
		eulerAngles.x += yoffset * _sensitivity;
		eulerAngles.y += xoffset * _speed; 
		rotate(eulerAngles, Object3D::RotateOrder::ZXY);
	}

	void lookAt(glm::vec3 worldPosition, glm::vec3 worldUp) override {
		assert("TODO: need to perform look at");
	}

private:
	float _speed = 2.5f;
	float _sensitivity = 0.1f;
};