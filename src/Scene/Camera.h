#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraUBO
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 position;
};

class Camera
{
public:
	Camera(const glm::vec3& position, float aspect);
	~Camera() = default;

	void setPosition(const glm::vec3& position);
	void setPerspective(float fov, float aspect, float near, float far);

	void updatePosition(glm::vec3 direction, float deltaTime);
	void updateRotation(float xOffset, float yOffset);

	const glm::mat4& getViewMatrix() const { return _view; }
	const glm::mat4& getProjectionMatrix() const { return _projection; }
	const glm::vec3& getPosition() const { return _position; }

private:
	glm::vec3 _position, _velocity;
	float _acceleration;
	float _pitch, _yaw;

	glm::vec3 _forward, _up, _right;
	float _fov, _aspect, _near, _far;

	glm::mat4 _view, _projection;

	void updateCameraVectors();
	void updateViewMatrix();
	void updateProjectionMatrix();
};