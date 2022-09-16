#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glew.h"

class Shader {
public:
	//идентификатор программы
	GLuint Program;

	//Конструктор считывает и собирвет шейдер
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

		//1.Получаем исходный код шейдеров из FilePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Удостоверимся, что ifstream объекты могут выбрасывать исключения
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {
			//Открываем файлы
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//Считываем данные в потоки
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Закрываем файлы
			vShaderFile.close();
			vShaderFile.close();

			//Преобразоываем потоки в массивы GLchar
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		//2. Сборка шейдеров
		GLuint vertex, fragment;
		GLint success;
		GLchar infLog[512];

		//вершинный шейдер
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		//вывод ошибок
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infLog << std::endl;
		}


		//фрагментный шейдер
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//вывод ошибок
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILD\n" << infLog << std::endl;
		}

		//шейдерная программа
		Program = glCreateProgram();
		glAttachShader(Program, vertex);
		glAttachShader(Program, fragment);
		glLinkProgram(Program);

		//Выводим ошибки
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(Program, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infLog << std::endl;
		}

		//Удаляем шейдеры
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	//Испольщование программы
	void use() { glUseProgram(Program); };
};

#endif