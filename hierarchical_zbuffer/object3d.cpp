#include <cassert>
#include <cmath>

#include "object3d.h"


/*
 * @brief constructor with specified name
 */
Object3D::Object3D(std::string name): _name(name) { }


/*
 * @brief get the name of the object
 * @return name of the object
 */
std::string Object3D::getName() const {
	return _name;
}


/*
 * @brief set the name of the object
 */
void Object3D::setName(std::string name) {
	_name = name;
}


/*
 * @brief get the parent of the object
 * @return the parent of the object or nullptr if no parent
 */
Object3D* Object3D::getParent() const {
	return _parent;
}


/*
 * @brief set the parent of the object
 * @param parent parent of the object, can be nullptr to detach from the parent
 * @todo maintain the world postion/rotation/scale of the object
 */
void Object3D::setParent(Object3D* parent, bool stayInWorld) {
	// check whether the object is already a child of the parent
	if (parent) {
		for (auto child : parent->_children) {
			if (child == this) {
				return;
			}
		}
	}
	
	if (stayInWorld) {
		assert("TODO: maintain the world postion/rotation/scale of the object");
	}

	// remove relation from the current parent
	if (_parent) {
		_parent->_children.remove(this);
	}

	parent->_children.push_back(this);

	_parent = parent;
}


/*
 * @brief get the numbers of child of the object
 * @return the number of the child of the object
 */
uint32_t Object3D::childCount() const {
	return static_cast<uint32_t>(_children.size());
}


/*
 * @brief get child of the object by index
 * @return the pointer the the child object
 */
Object3D* Object3D::getChild(int index) const {
	auto it = _children.begin();
	std::advance(it, index);
	return *it;
}


/*
 * @brief get all children of the object
 * @return the list of 
 */
std::list<Object3D*> Object3D::getChildren() const {
	return _children;
}


/*
 * @brief get the local position of the object
 * @return the world/local position of the object
 */
glm::vec3 Object3D::getLocalPosition() const {
	return _localPosition;
}


/*
 * @brief set the local position of the object
 * @param postion local position corresponding to parent
 */
void Object3D::setLocalPosition(const glm::vec3& position) {
	_localPosition = position;
}


/*
 * @brief get the world position of the object
 * @return the world position of the object
 */
glm::vec3 Object3D::getWorldPosition() const {
	// the result of glm::vec4 will be (x, y, z, 1.0f)
	// glm::vec3 use the x, y, z of the glm::vec4 to consturct self
	return getModelMatrix() * glm::vec4(_localPosition, 1.0f);
}


/*
 * @brief set the world position of the object
 * @param position world position of 3 dimension
 */
void Object3D::setWorldPosition(const glm::vec3& position) {
	_localPosition = glm::vec3(getModelMatrixInverse() * glm::vec4(position, 1.0f));
}


/*
 * @brief translate object with world/local translation
 * @param translation 3d vector of the movement in world/local space
 * @param space world/local space of the translation
 */
void Object3D::translate(const glm::vec3& translation, enum Space space) {
	if (space == Space::Local) {
		_localPosition += translation;
	} else {
		_localPosition += glm::vec3(getModelMatrixInverse() * glm::vec4(translation, 0.0f));
	}
}


/*
 * @brief get the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
glm::quat Object3D::getLocalRotation() const {
	return _localRotation;
}


/*
 * @brief set the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
void Object3D::setLocalRotation(const glm::quat& rotation) {
	_localRotation = rotation;
}


/*
 * @brief get local euler angles of an object by extrinsic rotations
 * @param order rotate order of the euler angle
 * @return glm::vec3(pitch, yaw, roll)
 *         pitch vec3.x rotate angle around x-axis
 *         yaw   vec3.y rotate angle around y-axis
 *         roll  vec3.z rotate angle around z-axis
 * @todo overcome singularity(atan2) and precision problems
 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
 */
glm::vec3 Object3D::getLocalEulerAngles(enum RotateOrder order) const {
	return quaternionToEulerAngles(_localRotation, order);
}


/*
 * @brief set local euler angles of an object by extrinsic rotations
 * @param glm::vec3(pitch, yaw, roll)
 *         pitch vec3.x rotate angle around x-axis
 *         yaw   vec3.y rotate angle around y-axis
 *         roll  vec3.z rotate angle around z-axis
 * @param order rotate order of the euler angle
 * @todo overcome singularity(atan2) and precision problems
 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
 */
void Object3D::setLocalEulerAngles(const glm::vec3& eulerAngles, enum RotateOrder order) {
	glm::quat q = eulerAnglesToQuaternion(eulerAngles, order);
	_localRotation = q * _localRotation;
}


/*
 * @brief rotate by axis and angle in local space
 * @param axis normalized rotate axis
 * @param angle rotate angle in radians
 * @todo add world space rotate support
 */
void Object3D::rotate(const glm::vec3& axis, float angle) {
	_localRotation = glm::angleAxis(angle, axis) * _localRotation;
}


/*
 * @brief rotate by axis and angle in local space
 * @param eulerAngles in glm::vec3(pitch, yaw, roll)
 *						 pitch vec3.x rotate angle around x-axis
 *                       yaw   vec3.y rotate angle around y-axis
 *                       roll  vec3.z rotate angle around z-axis
 * @param order rotate order
 * @todo add world space rotate support
 */
void Object3D::rotate(const glm::vec3& eulerAngles, const enum RotateOrder order) {
	_localRotation = eulerAnglesToQuaternion(eulerAngles, order) * _localRotation;
}


/*
 * @brief get local scale of the object
 * @return the local scale of the object in glm::vec3
 */
glm::vec3 Object3D::getLocalScale() const {
		return _localScale;
}


/*
 * @brief set local scale of the object
 * @param scale local scale of the object
 */
void Object3D::setLocalScale(const glm::vec3& scale) {
	_localScale = scale;
}


/*
 * @brief get model matrix of the object
 * @return model matrix to transform the object from local to world space
 */
glm::mat4x4 Object3D::getModelMatrix() const {
	glm::mat4x4 m(1.0f);

	for (Object3D* obj = const_cast<Object3D*>(this); obj; obj = obj->_parent) {
		m = glm::scale(m, obj->_localScale);
		m = glm::mat4_cast(obj->_localRotation) * m;
		m = glm::translate(m, obj->_localPosition);
	}
	
	return m;
}


/*
 * @brief get model matrix inverse of the object
 * @return model matrix inverse to transform the object from world to local space
 */
glm::mat4x4 Object3D::getModelMatrixInverse() const {
	return glm::inverse(getModelMatrix());
}


/*
 * @brief transform euler angles to quaternion
 * @param q source quaternion
 * @param order rotate order of euler angle
 * @return eulerAngles in glm::vec3(pitch, yaw, roll)
 *                        pitch vec3.x rotate angle around x-axis
 *                        yaw   vec3.y rotate angle around y-axis
 *                        roll  vec3.z rotate angle around z-axis
 */
glm::vec3 Object3D::quaternionToEulerAngles(const glm::quat& q, enum RotateOrder order) {
	glm::vec3 ret;
	float r11, r12, r21, r31, r32;
	switch (order) {
		case RotateOrder::XYZ:
			r11 = -2.0f * (q.y * q.z - q.w * q.x);
			r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
			r21 = 2.0f * (q.x * q.z + q.w * q.y);
			r31 = -2.0f * (q.x * q.y - q.w * q.z);
			r32 = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
			ret = glm::vec3(atan2(r11, r12), asin(r21), atan2(r31, r32));
			break;
		case RotateOrder::YZX:
			r11 = -2.0f * (q.x * q.z - q.w * q.y);
			r12 = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
			r21 = 2.0f * (q.x * q.y + q.w * q.z);
			r31 = -2.0f * (q.y * q.z - q.w * q.x);
			r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
			ret = glm::vec3(atan2(r31, r32), atan2(r11, r12), asin(r21));
			break;
		case RotateOrder::ZXY:
			r11 = -2.0f * (q.x * q.y - q.w * q.z);
			r12 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
			r21 = 2.0f * (q.y * q.z + q.w * q.x);
			r31 = -2.0f * (q.x * q.z - q.w * q.y);
			r32 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
			ret = glm::vec3(asin(r21), atan2(r31, r32), atan2(r11, r12));
			break;
		case RotateOrder::XZY:
			r11 = 2.0f * (q.y * q.z + q.w * q.x);
			r12 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
			r21 = -2.0f * (q.x * q.y - q.w * q.z);
			r31 = 2.0f * (q.x * q.z + q.w * q.y);
			r32 = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
			ret = glm::vec3(atan2(r11, r12), atan2(r31, r32), asin(r21));
			break;
		case RotateOrder::YXZ:
			r11 = 2.0f * (q.x * q.z + q.w * q.y);
			r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
			r21 = -2.0f * (q.y * q.z - q.w * q.x);
			r31 = 2.0f * (q.x * q.y + q.w * q.z);
			r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
			ret = glm::vec3(asin(r21), atan2(r11, r12), atan2(r31, r32));
			break;
		case RotateOrder::ZYX:
			r11 = 2.0f * (q.x * q.y + q.w * q.z);
			r12 = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
			r21 = -2.0f * (q.x * q.z - q.w * q.y);
			r31 = 2.0f * (q.y * q.z + q.w * q.x);
			r32 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
			ret = glm::vec3(atan2(r31, r32), asin(r21), atan2(r11, r12));
			break;
	}

	return ret;
}


/*
 * @brief get model matrix inverse of the object
 * @param eulerAngles in glm::vec3(pitch, yaw, roll)
 *						 pitch vec3.x rotate angle around x-axis
 *                       yaw   vec3.y rotate angle around y-axis
 *                       roll  vec3.z rotate angle around z-axis
 * @param order rotate order of euler angle
 * @return quaternion
 */
glm::quat Object3D::eulerAnglesToQuaternion(const glm::vec3& eulerAngles, enum RotateOrder order) {
	glm::quat ret;
	glm::quat qx = glm::angleAxis(eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat qy = glm::angleAxis(eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat qz = glm::angleAxis(eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	switch (order) {
		case RotateOrder::XYZ: ret = qz * qy * qx; break;
		case RotateOrder::YZX: ret = qx * qz * qy; break;
		case RotateOrder::ZXY: ret = qy * qx * qz; break;
		case RotateOrder::XZY: ret = qy * qz * qx; break;
		case RotateOrder::YXZ: ret = qz * qx * qy; break;
		case RotateOrder::ZYX: ret = qx * qy * qz; break;
	}

	return ret;
}