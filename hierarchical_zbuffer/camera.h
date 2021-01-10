#pragma once

#include "object3d.h"

/**
 * @brief Base class of the camera
 * @detail In the local coordinate of the camera, it always looks at negative z-axis,
 *         with up vector as y-axis and right vector as x-axis
 */
class Camera: public Object3D {
public:
	Camera(float fov, float aspect, float znear, float zfar)
		: _fov(fov), _aspect(aspect), _znear(znear), _zfar(zfar) {}

	~Camera() = default;

	float getFov() {
		return _fov;
	}

	void setFov(float fov) {
		_fov = fov;
	}

	float getAspectRatio() {
		return _aspect;
	}

	void setAspectRatio(float aspect) {
		_aspect = aspect;
	}

	float getNearClip() {
		return _znear;
	}

	void setNearClip(float znear) {
		_znear = znear;
	}

	float getFarClip() {
		return _zfar;
	}

	void setFarClip(float zfar) {
		_zfar = zfar;
	}

	void setFrustum(float fov, float aspect, float znear, float zfar) {
		_fov = fov;
		_aspect = aspect;
		_znear = znear;
		_zfar = zfar;
	}

	glm::mat4x4 getViewMatrix() {
		glm::mat4x4 view(1.0f);
		view = glm::translate(view, -_localPosition);
		view = glm::mat4_cast(_localRotation) * view;
		return view;
	}

	virtual glm::mat4x4 getProjectionMatrix() = 0;

	virtual void lookAt(glm::vec3 worldPosition, glm::vec3 worldUp) = 0;

protected:
	/* camera attributes: field of view */
	float _fov;
	/* camera attributes: aspect ratio */
	float _aspect;
	/* camera attributes: near plane */
	float _znear = 0.1f;
	/* camera attributes: far plane */
	float _zfar = 10000.0f;
};