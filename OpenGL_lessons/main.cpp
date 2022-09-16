#include <iostream>

#include "glew.h"
#include "GLFW/glfw3.h"
#include "SOIL2/SOIL2/SOIL2.h"
#include "ShaderCompileClass.h"

//���������� ����������
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


GLfloat transparency = 0.2f;

//callBack �������, ������������� ������� ������ 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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


	//������� ��������� ���������
	Shader ourShader("..\\Shaders\\vertex.sh", "..\\Shaders\\fragment.sh");


	//�������� ������� ������ �����������
	GLfloat verticesColor[] = {
		// �������         // �����			// ���������� ����������
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // ������� ������
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,    // ������ ������
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,    // ������ �����
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // ������� �����
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//���������� VAO
	glBindVertexArray(0);
	//���������� EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//���������� VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << nrAttributes << std::endl;*/

	

	//����, ������� ����� ���������� ���� �� ��� ���,
	//���� ������������ ��� ��� �� �������
	
	while (!glfwWindowShouldClose(window)) {
		//������� ��� ������������ ������� ������������ � �����
		glfwPollEvents();

		//������� �������� ������ ������������ ������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


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

		//�������� ������� ������������� 
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		//�������� ������� � ������
		GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//�������� ������� ������������� 
		glm::mat4 trans1;
		trans1 = glm::translate(trans1, glm::vec3(0.5f, 0.5f, 0.0f));
		trans1 = glm::scale(trans1, glm::vec3(sin((GLfloat)glfwGetTime()), sin((GLfloat)glfwGetTime()), 0.0f));

		//�������� ������� � ������
		GLint transformLoc1 = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(trans1));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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