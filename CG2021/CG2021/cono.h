#pragma once
#ifndef CONO_H_
#define CONO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOSC 50

class Cono
{
public:
	Cono(GLfloat radio, GLfloat altura);
	void init();
	void render();
	virtual ~Cono();
private:
	float radio;
	float altura;
	GLuint cono_VAO[4], cono_VBO[4], cono_index[(MERIDIANOSC + 2) * 6];
	GLfloat cono_pos[ (MERIDIANOSC+2) * 3];

};
#endif // !CONO_H_