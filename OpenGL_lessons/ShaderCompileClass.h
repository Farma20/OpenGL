#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glew.h"

class Shader {
public:
	//������������� ���������
	GLuint Program;

	//����������� ��������� � �������� ������
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

		//1.�������� �������� ��� �������� �� FilePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//�������������, ��� ifstream ������� ����� ����������� ����������
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {
			//��������� �����
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//��������� ������ � ������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//��������� �����
			vShaderFile.close();
			vShaderFile.close();

			//�������������� ������ � ������� GLchar
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		//2. ������ ��������
		GLuint vertex, fragment;
		GLint success;
		GLchar infLog[512];

		//��������� ������
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		//����� ������
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infLog << std::endl;
		}


		//����������� ������
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//����� ������
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILD\n" << infLog << std::endl;
		}

		//��������� ���������
		Program = glCreateProgram();
		glAttachShader(Program, vertex);
		glAttachShader(Program, fragment);
		glLinkProgram(Program);

		//������� ������
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(Program, 512, NULL, infLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infLog << std::endl;
		}

		//������� �������
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	//������������� ���������
	void use() { glUseProgram(Program); };
};

#endif