#pragma once
#ifndef CIRCUNFERENCIA_H_
#define CIRCUNFERENCIA_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 50

class Circunferencia
{
public:
	Circunferencia(GLfloat radio);
	void init();
	void render();
	virtual ~Circunferencia();
private:
	float radio;
	GLuint circunferencia_VAO[4], circunferencia_VBO[4], circunferencia_index[MERIDIANOS * 6];
	GLfloat circunferencia_pos[ MERIDIANOS * 3];
};

#endif 