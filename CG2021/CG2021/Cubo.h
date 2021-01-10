#pragma once
#ifndef	CUBO_H_
#define CUBO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class Cubo
{
public:
	Cubo(GLfloat unidad);
	void init();
	void render();
	virtual ~Cubo();
private:
	float uni;
	GLuint Cubo_VAO[4], Cubo_VBO[4];
};

#endif