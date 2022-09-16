#include <iostream>

#include "glew.h"
#include "GLFW/glfw3.h"
#include "SOIL2/SOIL2/SOIL2.h"
#include "ShaderCompileClass.h"

//Библиотека математики
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


GLfloat transparency = 0.2f;

//callBack функция, отслеживающяя нажатие клавиш 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//Отлавливаем нажатие на esc
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
	//Инициализация glfw
	glfwInit();

	//Настройки glfw (выбираем мажорную и минорную версию)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Установка профайла, для которого создается контент
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Выключение возможности изменеия размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Режим отрисовки линиями
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Создание объекта окна
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn OpenGL" ,nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Error. Window not create" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Создаем текущий контекст с указателем на окно
	glfwMakeContextCurrent(window);


	//Данное значение позволяет GLEW использовать новейшие 
	//техники управлением OpenGL
	glewExperimental = GL_TRUE;

	//Инициализируем GLEW
	if (glewInit() != GLEW_OK) {
		std::cout << "Error. GLEW not init" << std::endl;
		return -1;
	}



	//Сообщим OpenGL размер отрисовываемого окна
	int width, height;

	//Отсюда в переменные width и height мы получаем размеры 
	glfwGetFramebufferSize(window, &width, &height);

	//первые два аргумента являются координатами точки начала
	glViewport(0, 0, width, height);


	//регистрация отслеживания нажатия на кнопку, которое 
	//будет отлавливаться в glfwPollEvents()
	glfwSetKeyCallback(window, key_callback);


	//Создаем шейдерную программу
	Shader ourShader("..\\Shaders\\vertex.sh", "..\\Shaders\\fragment.sh");


	//Создание цветных вершин треугльника
	GLfloat verticesColor[] = {
		// Позиции         // Цвета			// Текстурные координаты
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // Верхний правый
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,    // Нижний правый
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,    // Нижний левый
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // Верхний левый
	};


	//Создание массива индексов EBO или IBO
	//Позволяет нам отрисовывать вершины в том порядке
	//чтобы из 4 вершин получилось 2 треугольника
	//тк OpenGL в основном работает с треугольниками
	GLuint indices[] = {
		0, 1, 2, //первый треугольник
		0, 2, 3, //второй треугольник
	};


	//Генерируем текстуру и даем ей индентификатор
	GLuint texture1;
	glGenTextures(1, &texture1);

	//привязываем текстуру
	glBindTexture(GL_TEXTURE_2D, texture1);


	//указываем OpenGL как именно накладывать текстуры
	//если они выходят за свой диапазон s, t, r = x, y, z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//Устанавливаем способы смены mipmap и их интерполяцию
	//Уменьшение текстуры
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//Увеличение текстуры (не использует mipmap)
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//загружаем текстуру через SOIL2
	int widthTex1, heightTex1;
	unsigned char* image1 = SOIL_load_image("..\\Textures\\container.jpg", &widthTex1, &heightTex1, 0, SOIL_LOAD_RGB);

	//генерируем текстуру
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex1, heightTex1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

	//высвобождаем память
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);



	//Настройка второй текстуры

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




	//создание VAO для интерпретации данных из буфера
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//Создание VBO - буфера для мгновенной передачи 
	//вершинных данных с cpu на gpu
	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	//Привязываем VAO к буферу
	glBindVertexArray(VAO);


	//Определяем тип буфера EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//Скопируем индексы в EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//Определяем тип нашего буфера VBO. Тем самым мы сделаем так,
	//Чтобы любой вызов буфера типа GL_ARRAY_BUFFER обращался к VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Скопируем вершинные данные в наш буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);

	

	//Указываем openGL как ему интерпретировать вершинные данные
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Отвязываем VAO
	glBindVertexArray(0);
	//Отвязываем EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Отвязываем VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << nrAttributes << std::endl;*/

	

	//Цикл, который будет отображать окно до тех пор,
	//пока пользователь его сам не закроет
	
	while (!glfwWindowShouldClose(window)) {
		//Функция для отслеживания событий производимых с окном
		glfwPollEvents();

		//Функция отчистки экрана определенным цветом
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//Отрисовка фигуры с помощью VAO и EBO
		ourShader.use();
		//активируем текстурный блок
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

		//Создание матрицы трансформации 
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		//передаем матрицу в шейдер
		GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//Создание матрицы трансформации 
		glm::mat4 trans1;
		trans1 = glm::translate(trans1, glm::vec3(0.5f, 0.5f, 0.0f));
		trans1 = glm::scale(trans1, glm::vec3(sin((GLfloat)glfwGetTime()), sin((GLfloat)glfwGetTime()), 0.0f));

		//передаем матрицу в шейдер
		GLint transformLoc1 = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(trans1));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	
		//функция, заменяющая цветовой буфер в отривывающемся окне
		//Двойная буфферизация
		glfwSwapBuffers(window);
	}

	//После выхода из игрового цикла нам необходимо
	//отчистить выделенные ресурсы
	glfwTerminate();

	return 0;

}