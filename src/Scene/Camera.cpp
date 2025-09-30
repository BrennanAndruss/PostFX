#include "Camera.h"

Camera::Camera(const glm::vec3& position, float aspect) : _position(position), _velocity(), 
	_acceleration(15.0f), _pitch(0.0f), _yaw(0.0f), _fov(45.0f), _aspect(aspect), 
	_near(0.01f), _far(100.0f) 
{
	updateCameraVectors();
	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::setPosition(const glm::vec3& position)
{
	_position = position;

	updateCameraVectors();
	updateViewMatrix();
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	_fov = fov;
	_aspect = aspect;
	_near = near;
	_far = far;

	updateProjectionMatrix();
}

void Camera::updatePosition(glm::vec3 direction, float deltaTime)
{
	if (direction.x != 0.0f || direction.y != 0.0f || direction.z != 0.0f)
	{
		// Convert move direction to camera space
		direction = direction.x * _right + direction.y * _up + direction.z * _forward;
		direction = glm::normalize(direction);

		// Accelerate
		_velocity += direction * _acceleration * deltaTime;
	}

	// Apply drag independent of framerate
	_velocity *= powf(0.98f, deltaTime * 60.0f);
	
	// Integrate position
	_position += _velocity * deltaTime;

	updateViewMatrix();
}

void Camera::updateRotation(glm::vec2 direction)
{
	_yaw += direction.x;
	_pitch += direction.y;

	// Constrain pitch to avoid gimbal lock
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	updateCameraVectors();
	updateViewMatrix();
}

void Camera::updateAspect(float aspect)
{
	_aspect = aspect;
	updateProjectionMatrix();
}

void Camera::updateCameraVectors()
{
	// Convert from degrees to radians
	float p = glm::radians(_pitch);
	float y = glm::radians(_yaw);

	// Calculate new orthonormal basis
	glm::vec3 forward;
	forward.x = cos(p) * cos(y);
	forward.y = sin(p);
	forward.z = cos(p) * sin(y);

	_forward = glm::normalize(forward);
	_right = glm::normalize(glm::cross(_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	_up = glm::cross(_right, _forward);
}

void Camera::updateViewMatrix()
{
	_view = glm::lookAt(_position, _position + _forward, _up);
}

void Camera::updateProjectionMatrix()
{
	_projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
}