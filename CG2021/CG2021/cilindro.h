#pragma once
#ifndef CILINDRO_H_
#define CILINDRO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 50
#define PARALELOS 20
class Cilindro
{
public:
	Cilindro(GLfloat radio,GLfloat altura);
	void init();
	void render();
	virtual ~Cilindro();
private:
	float radio;
	float altura;
	GLuint cilindro_VAO[4], cilindro_VBO[4], cilindro_index[MERIDIANOS * (PARALELOS - 1) * 6];
	GLfloat cilindro_pos[PARALELOS * MERIDIANOS * 3];
};

#endif // !CILINDRO_H_