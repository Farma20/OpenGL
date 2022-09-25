#include <iostream>

#include "glew.h"
#include "GLFW/glfw3.h"
#include "SOIL2/SOIL2/SOIL2.h"
#include "ShaderCompileClass.h"
#include "CameraClass.h"

//���������� ����������
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//������� ����� ������
Camera camera;

GLfloat transparency = 0.2f;

//���������� ������������ ��������� �����
glm::vec3 lightPos(2.2f, 3.0f, 1.0f);

//������������ ���������
// �����, ��������� ����� ��������� � ������� ������
GLfloat deltaTime = 0.0f;
// ����� ������ ���������� �����
GLfloat lastFrame = 0.0f;

//������ ������������ ��������� ������
bool keys[1024];

//callBack �������, ������������� ������� ������ 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	//����������� ������� ������
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;


	//����������� ������� �� esc
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		transparency += 0.1f;
		if (transparency >= 1.0f)
			transparency = 1.0f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		transparency -= 0.1f;
		if (transparency <= 0.0f)
			transparency = 0.0f;
	}
}

// �������, ������������� ��������� ����
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.cameraOverview(xpos, ypos);
}

// �������, ������������� ��������� ������ ����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.cameraZoom(xoffset, yoffset);
}

int main() {
	//������������� glfw
	glfwInit();

	//��������� glfw (�������� �������� � �������� ������)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//��������� ��������, ��� �������� ��������� �������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//���������� ����������� �������� ������� ����
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//����� ��������� �������
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//�������� ������� ����
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn OpenGL" ,nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Error. Window not create" << std::endl;
		glfwTerminate();
		return -1;
	}

	//������� ������� �������� � ���������� �� ����
	glfwMakeContextCurrent(window);


	//������ �������� ��������� GLEW ������������ �������� 
	//������� ����������� OpenGL
	glewExperimental = GL_TRUE;

	//�������������� GLEW
	if (glewInit() != GLEW_OK) {
		std::cout << "Error. GLEW not init" << std::endl;
		return -1;
	}



	//������� OpenGL ������ ��������������� ����
	int width, height;

	//������ � ���������� width � height �� �������� ������� 
	glfwGetFramebufferSize(window, &width, &height);

	//������ ��� ��������� �������� ������������ ����� ������
	glViewport(0, 0, width, height);


	//����������� ������������ ������� �� ������, ������� 
	//����� ������������� � glfwPollEvents()
	glfwSetKeyCallback(window, key_callback);

	//������������ ������� ���� �� ������ � ������� ���
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//��������� �������, ������� ����� ���������� ��� ��������� �������
	glfwSetCursorPosCallback(window, mouse_callback);

	//��������� �������, ������� ���������� ��� ��������� ������ ����
	glfwSetScrollCallback(window, scroll_callback);

	//����������� ������ �������
	glEnable(GL_DEPTH_TEST);


	//������� ��������� ���������
	Shader ourShader("..\\Shaders\\vertex.sh", "..\\Shaders\\fragment.sh");

	//��������� ��������� ��� ������� ���������� ����
	Shader colorShader("..\\Shaders\\vertex_color.sh", "..\\Shaders\\fragment_color.sh");

	// ��������� ��������� ��� ��������� �����
	Shader lightShader("..\\Shaders\\vertex_color.sh", "..\\Shaders\\fragment_light.sh");


	//�������� ������� ������ �����������
	GLfloat verticesColor[] = {
		// �������       // ���������� ���������� //������� ��������
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	};


	//�������� ������� �������� EBO ��� IBO
	//��������� ��� ������������ ������� � ��� �������
	//����� �� 4 ������ ���������� 2 ������������
	//�� OpenGL � �������� �������� � ��������������
	GLuint indices[] = {
		0, 1, 2, //������ �����������
		0, 2, 3, //������ �����������
	};


	//���������� �������� � ���� �� ��������������
	GLuint texture1;
	glGenTextures(1, &texture1);

	//����������� ��������
	glBindTexture(GL_TEXTURE_2D, texture1);


	//��������� OpenGL ��� ������ ����������� ��������
	//���� ��� ������� �� ���� �������� s, t, r = x, y, z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//������������� ������� ����� mipmap � �� ������������
	//���������� ��������
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//���������� �������� (�� ���������� mipmap)
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//��������� �������� ����� SOIL2
	int widthTex1, heightTex1;
	unsigned char* image1 = SOIL_load_image("..\\Textures\\container.jpg", &widthTex1, &heightTex1, 0, SOIL_LOAD_RGB);

	//���������� ��������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex1, heightTex1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

	//������������ ������
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);



	//��������� ������ ��������

	GLuint texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthTex2, heightTex2;
	unsigned char* image2 = SOIL_load_image("..\\Textures\\awesomeface.png", &widthTex2, &heightTex2, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex2, heightTex2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);




	//�������� VAO ��� ������������� ������ �� ������
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//�������� VBO - ������ ��� ���������� �������� 
	//��������� ������ � cpu �� gpu
	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	//����������� VAO � ������
	glBindVertexArray(VAO);


	//���������� ��� ������ EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//��������� ������� � EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//���������� ��� ������ ������ VBO. ��� ����� �� ������� ���,
	//����� ����� ����� ������ ���� GL_ARRAY_BUFFER ��������� � VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��������� ��������� ������ � ��� �����
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);

	

	//��������� openGL ��� ��� ���������������� ��������� ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//���������� VAO
	glBindVertexArray(0);
	//���������� EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//���������� VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	//�������� VAO ��� ������� ��������
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//���������� VAO
	glBindVertexArray(0);
	//���������� VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	//������������� ������� ����� � ����
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	


	//����, ������� ����� ���������� ���� �� ��� ���,
	//���� ������������ ��� ��� �� �������
	
	while (!glfwWindowShouldClose(window)) {

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//������� ��� ������������ ������� ������������ � �����
		glfwPollEvents();

		//������������ ������� ������
		/*do_movement();*/
		camera.cameraMovement(keys, deltaTime);

		//������� �������� ������ ������������ ������
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//��������� ������ � ������� VAO � EBO
		ourShader.use();
		//���������� ���������� ����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		
		glBindVertexArray(VAO);

		GLint offsetLocation = glGetUniformLocation(ourShader.Program, "offset");
		glUniform1f(offsetLocation, 0.5);

		glUniform1f(glGetUniformLocation(ourShader.Program, "transparency"), transparency);


		//�������� ������� ������ ��� �������� � �������� ������������
		glm::mat4 model;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view;
		view = camera.getViewMatrix();

		//��������� ������� �������� ��� �������� � ����������� ���������
		glm::mat4 projection;
		projection = glm::perspective(camera.getFOV(), float(width / height), 0.1f, 100.0f);

		//�������� ������ � ��������� ������
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//���� �������������� ��������� ����� � �� ��������
		for (GLint i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle;
			if ((i + 1) % 3 == 0) {
				angle = (GLfloat)glfwGetTime() * 50.0f;
			}
			else {
				angle = 20.0f * i;
			}
			model = glm::rotate(model, angle, glm::vec3(0.1f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glBindVertexArray(0);


		//��������� �������� �����
		/*colorShader.use();*/
		lightShader.use();

		lightPos = glm::vec3(3 + sin((GLfloat)glfwGetTime()), lightPos.y, 3 + cos((GLfloat)glfwGetTime()));

		glm::mat4 model1 = glm::mat4();
		model1 = glm::translate(model1, lightPos);
		model1 = glm::scale(model1, glm::vec3(0.2f));

		GLint modelLoc1 = glGetUniformLocation(lightShader.Program, "model");
		glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));

		GLint viewLoc1 = glGetUniformLocation(lightShader.Program, "view");
		glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc1 = glGetUniformLocation(lightShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc1, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//��������� ����������, ����������� ����
		colorShader.use();

		GLint lightPosLoc = glGetUniformLocation(colorShader.Program, "lightPos");
		GLint objectColorLoc = glGetUniformLocation(colorShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(colorShader.Program, "lightColor");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);


		glm::mat4 model2 = glm::mat4();
		model2 = glm::translate(model2, glm::vec3(1.0f, 1.0f, 1.0f));


		GLint modelLoc2 = glGetUniformLocation(lightShader.Program, "model");
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));

		GLint viewLoc2 = glGetUniformLocation(lightShader.Program, "view");
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc2 = glGetUniformLocation(lightShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));

		GLint viewPosLoc = glGetUniformLocation(lightShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);


		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	
		//�������, ���������� �������� ����� � �������������� ����
		//������� ������������
		glfwSwapBuffers(window);
	}

	//����� ������ �� �������� ����� ��� ����������
	//��������� ���������� �������
	glfwTerminate();

	return 0;

}