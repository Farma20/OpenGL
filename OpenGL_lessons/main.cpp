#include <iostream>

#include "glew.h"
#include "GLFW/glfw3.h"
#include "SOIL2/SOIL2/SOIL2.h"
#include "ShaderCompileClass.h"
#include "CameraClass.h"

//Библиотека математики
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//Создаем класс камеры
Camera camera;

GLfloat transparency = 0.2f;

//глобальное расположение источника света
glm::vec3 lightPos(2.2f, 3.0f, 1.0f);

//коэффициенты изменения
// Время, прошедшее между последним и текущим кадром
GLfloat deltaTime = 0.0f;
// Время вывода последнего кадра
GLfloat lastFrame = 0.0f;

//массив отслеживания состояний клавиш
bool keys[1024];

//callBack функция, отслеживающяя нажатие клавиш 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	//Отслеживаем события кнопок
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;


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

// функция, отслеживающая изменение мыши
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.cameraOverview(xpos, ypos);
}

// Функция, отслеживающая изменение колеса мыши
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.cameraZoom(xoffset, yoffset);
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

	//Отлавливание курсора мыши на экране и скрытие его
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Назначаем функцию, которая будет вызываться при изменении курсора
	glfwSetCursorPosCallback(window, mouse_callback);

	//Назначаем функцию, которая вызывается при изменении колеса мыши
	glfwSetScrollCallback(window, scroll_callback);

	//подключение буфера глубины
	glEnable(GL_DEPTH_TEST);


	//Создаем шейдерную программу
	Shader ourShader("..\\Shaders\\vertex.sh", "..\\Shaders\\fragment.sh");

	//шейдерная программа для обектов отражаюзих свет
	Shader colorShader("..\\Shaders\\vertex_color.sh", "..\\Shaders\\fragment_color.sh");

	// шейдерная программа для источника света
	Shader lightShader("..\\Shaders\\vertex_color.sh", "..\\Shaders\\fragment_light.sh");


	//Создание цветных вершин треугльника
	GLfloat verticesColor[] = {
		// Позиции       // Текстурные координаты //вектора нормалей
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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Отвязываем VAO
	glBindVertexArray(0);
	//Отвязываем EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Отвязываем VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	//Создание VAO для объекта свечения
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Отвязываем VAO
	glBindVertexArray(0);
	//Отвязываем VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	//РАспределение позиций кубов в мире
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
	


	//Цикл, который будет отображать окно до тех пор,
	//пока пользователь его сам не закроет
	
	while (!glfwWindowShouldClose(window)) {

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//Функция для отслеживания событий производимых с окном
		glfwPollEvents();

		//Отслеживание нажатий клавиш
		/*do_movement();*/
		camera.cameraMovement(keys, deltaTime);

		//Функция отчистки экрана определенным цветом
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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


		//Создание матрицы модели для перехода к мировому пространству
		glm::mat4 model;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view;
		view = camera.getViewMatrix();

		//Созданием матрицы проекции для перехода к координатам отсечения
		glm::mat4 projection;
		projection = glm::perspective(camera.getFOV(), float(width / height), 0.1f, 100.0f);

		//передача матриц в вершинный шейдер
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//цикл преобразования множества кубов и их отрисоки
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


		//отрисовка источков света
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

		//отрисовка контейнера, отражающего свет
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
	
		//функция, заменяющая цветовой буфер в отривывающемся окне
		//Двойная буфферизация
		glfwSwapBuffers(window);
	}

	//После выхода из игрового цикла нам необходимо
	//отчистить выделенные ресурсы
	glfwTerminate();

	return 0;

}