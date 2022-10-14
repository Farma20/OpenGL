#pragma once

#include "glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//если true то, первое вхождение камеры
bool firstMouse = true;

//переменные для хранения углов поворота камеры
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

//Последние значения указания курсора (по умолчанию центр эерана 800х600)
GLfloat lastX = 400;
GLfloat lastY = 300;

//Вектора расположения камеры
glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);


class Camera {
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 viewMatrix;
	float fov;

public:
	Camera(glm::vec3 pos = Pos, glm::vec3 front = Front, glm::vec3 up = Up){
		cameraPos = pos;
		cameraFront = front;
		cameraUp = up;
		fov = 45.0f;
		cameraLookAt();
	}

	void cameraMovement(bool *_keys, GLfloat &_deltaTime) {
		GLfloat cameraSpeed = _deltaTime * 5.0f;

		if (_keys[GLFW_KEY_W]) {
			cameraPos += cameraSpeed * cameraFront;
		}

		if (_keys[GLFW_KEY_S]) {
			cameraPos -= cameraSpeed * cameraFront;
		}

		if (_keys[GLFW_KEY_A]) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		if (_keys[GLFW_KEY_D]) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		cameraLookAt();
	}

	void cameraOverview(double xpos, double ypos) {
		//Убираем баг с дергающейся камерой при использовании мыши в начале
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		// Вычисляем на сколько изменились координаты курсора
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//добавляем смещение углам
		yaw += xoffset;
		pitch += yoffset;

		// ограничения углов
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// настраиваем углы поворота камеры
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		cameraFront = glm::normalize(front);
		cameraLookAt();
	}

	void cameraZoom(double xoffset, double yoffset) {
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}

	glm::mat4 getViewMatrix() {
		return viewMatrix;
	}

	glm::vec3 getCameraPos() {
		return cameraPos;
	}

	float getFOV() {
		return fov;
	}
private:
	void cameraLookAt() {
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

};