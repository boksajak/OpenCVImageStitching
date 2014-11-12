#pragma once
#include "glm.h"

struct Camera {

	glm::mat4 perspectiveMatrix;
	glm::mat4 lookAtMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 normalMatrix;

	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 upVector;
	glm::vec3 lookAtVector;
	glm::vec3 rightVector;

	float cameraAngle;    //< Horizontal field of view angle in degrees
	float nearPlane;      //< near clipping plane distance
	float farPlane;       //< far clipping plane distance

	float planeDistance;  //<Distance of projection plane from camera position
	float aspectRatio;    //< Width to height ratio


	inline void calculatePlaneDistance() {
		planeDistance = 1.0f/tanf(glm::radians(cameraAngle) / 2.0f);
	}

	inline void calculateDirection() {
		direction = glm::normalize(lookAtVector - position);
	}

	inline void calculateLookAtVector() {
		lookAtVector = position + lookAtVector;
	}

	inline void calculateRightVector() {
		rightVector = glm::normalize(glm::cross(direction, upVector));
	}

	inline void calculateUpVector() {
		upVector = glm::normalize(glm::cross(rightVector, direction));
	}

	inline void calculatePerspectiveMatrix() {
		perspectiveMatrix = glm::perspective(cameraAngle, aspectRatio, nearPlane, farPlane);
	}

	inline void calculateViewMatrices() {
		lookAtMatrix = glm::lookAt(glm::vec3(0,0,0), direction, upVector);
		viewMatrix = glm::translate(lookAtMatrix, -position);
		normalMatrix = glm::transpose(glm::inverse(viewMatrix));
	}

};