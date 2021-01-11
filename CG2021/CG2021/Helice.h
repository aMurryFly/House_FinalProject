#pragma once
#ifndef HELICE_H_
#define HELICE_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class Helice
{
public:
	Helice(GLfloat unidad);
	void init();
	void render();
	virtual ~Helice();
private:
	float uni;
	GLuint helice_VAO[4], helice_VBO[4];
};

#endif