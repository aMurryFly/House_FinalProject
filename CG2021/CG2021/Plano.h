#pragma once
#ifndef	PLANO_H_
#define PLANO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class Plano
{
public:
	Plano(GLfloat unidad);
	void init();
	void render();
	virtual ~Plano();
private:
	float uni;
	GLuint Plano_VAO[4], Plano_VBO[4];
};

#endif