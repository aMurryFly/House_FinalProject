#pragma once
#pragma once
#ifndef	PAREDV_H_
#define PAREDV_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

class ParedV
{
public:
	ParedV(GLfloat unidad);
	void init();
	void render();
	virtual ~ParedV();
private:
	float uni;
	GLuint ParedV_VAO[4], ParedV_VBO[4];
};

#endif