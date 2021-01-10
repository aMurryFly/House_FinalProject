#pragma once
#ifndef	PUERTA_H_
#define PUERTA_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class Puerta
{
public:
	Puerta(GLfloat unidad);
	void init();
	void render();
	virtual ~Puerta();
private:
	float uni;
	GLuint Puerta_VAO[4], Puerta_VBO[4];
};

#endif