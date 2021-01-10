#pragma once

#include <list>
#include <string>

#define GLM_FORCE_RADIANS
#ifdef GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object3D {
public:
	/* 
	 * @brief enum class for coordinate system type
	 */
	enum class Space {
		Local, World
	};

	/*
	 * @brief enum class for rotate order of an euler angle 
	 * @detial ABC can be understood as rotate first by C-axis, then B-axis, finally A-axis
	 */
	enum class RotateOrder {
		XYZ, YZX, ZXY, XZY, YXZ, ZYX
	};

	/*
	 * @brief default constructor
	 */
	Object3D() = default;

	/*
	 * @brief constructor with specified name
	 */
	explicit Object3D(std::string name);
	
	/*
     * @brief default destructor
     */
	~Object3D() = default;

	/*
	 * @brief get the name of the object
	 */
	std::string getName() const;

	/*
	 * @brief set the name of the object
     */
	void setName(std::string name);

	/*
	 * @brief get the parent of the object, if no parent return nullptr
	 */
	Object3D* getParent() const;

	/*
	 * @brief set the new parent of the object
	 * @todo maintain the world postion/rotation/scale of the object
	 */
	void setParent(Object3D* parent, bool stayInWorld = false);

	/*
	 * @brief get the numbers of child of the object
	 */
	uint32_t childCount() const;

	/*
	 * @brief get child of the object by index
	 */
	Object3D* getChild(int index) const;

	/*
	 * @brief get all children of the object
     */
	std::list<Object3D*> getChildren() const;

	/*
	 * @brief get the local position of the object
	 * @return the world/local position of the object
	 */
	glm::vec3 getLocalPosition() const;

	/*
	 * @brief set the local position of the object
	 */
	void setLocalPosition(const glm::vec3& position);

	/*
	 * @brief get the world position of the object
	 */
	glm::vec3 getWorldPosition() const;

	/*
	 * @brief set the world position of the object
	 */
	void setWorldPosition(const glm::vec3& position);

	/*
	 * @brief translate object with world/local translation
	 */
	void translate(const glm::vec3& translation, enum Space space);

	/*
     * @brief get the local rotation of the object in quaternion
     */
	glm::quat getLocalRotation() const;

	/*
	 * @brief set the local rotation of the object in quaternion
	 */
	void setLocalRotation(const glm::quat& rotation);

	/*
	 * @brief get local euler angles of an object by extrinsic rotations
	 * @todo overcome singularity(atan2) and precision problems
	 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
	 */
	glm::vec3 getLocalEulerAngles(enum RotateOrder order) const;

	/*
	 * @brief set local euler angles of an object by extrinsic rotations
	 * @todo overcome singularity and precision problems
	 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
	 */
	void setLocalEulerAngles(const glm::vec3& eulerAngles, enum RotateOrder order);

	/*
	 * @brief rotate by normalized axis and angle in radians in local space
	 * @todo add world space rotate support
	 */
	void rotate(const glm::vec3& axis, float angle);

	/*
	 * @brief rotate by euler angles and specified rotate order in local space
	 * @todo add world space rotate support
	 */
	void rotate(const glm::vec3& eulerAngles, const enum RotateOrder order);


	/*
	 * todo: rotateAround
	 */

	/*
	 * @brief get local scale of the object
	 * @return local scale of the object in 3 dimensions
	 */
	glm::vec3 getLocalScale() const ;

	/*
     * @brief set local scale of the object
     */
	void setLocalScale(const glm::vec3& scale);

	/*
	 * @brief get model matrix of the object
	 */
	glm::mat4x4 getModelMatrix() const;

	/*
	 * @brief get model matrix inverse of the object
	 */
	glm::mat4x4 getModelMatrixInverse() const;

	/*
	 * @brief transform euler angles to quaternion
	 */
	static glm::quat eulerAnglesToQuaternion(const glm::vec3& eulerAngles, enum RotateOrder order);

	/*
	 * @brief transform quaternion to euler angles
	 */
	static glm::vec3 quaternionToEulerAngles(const glm::quat& q, enum RotateOrder order);

protected:
	/* name of the object */
	std::string _name;
	/* local position of the object */
	glm::vec3 _localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	/* local rotation of the object */
	glm::quat _localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	/* local scale of the object */
	glm::vec3 _localScale = glm::vec3(1.0f, 1.0f, 1.0f);
	/* relationship:parent */
	Object3D* _parent = nullptr;
	/* relationship children */
	std::list<Object3D*> _children;
};