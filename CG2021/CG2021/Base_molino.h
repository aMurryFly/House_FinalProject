#pragma once
#ifndef BASE_MOLINO_H_
#define BASE_MOLINO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class Base_Molino
{
public:
	Base_Molino(GLfloat base);
	void init();
	void render();
	virtual ~Base_Molino();
private:
	float base;
	GLuint base_molino_VAO[4], base_molino_VBO[4];
};

#endif