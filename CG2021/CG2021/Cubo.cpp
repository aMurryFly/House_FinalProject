#include "Cubo.h"
//Decoración de los bloques principales del nexo
Cubo::Cubo(GLfloat unidad) :
	uni(unidad) {
	Cubo_VAO[4] = (-1, -1, -1, -1);
	Cubo_VBO[4] = (-1, -1, -1, -1);
}
Cubo::~Cubo() {
	glDeleteVertexArrays(4, Cubo_VAO);
	glDeleteBuffers(4, Cubo_VBO);
}
void Cubo::init() {
	float vert[] = {
		/*
		1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // top left
		*/
		//DADO
		0.5f,  0.5f, 0.5f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,   1.0f,0.0f,
		-0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,   0.0f, 1.0f,

		0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
		0.5f,-0.5f,-0.5f, 1.0f,0.0f,
		0.5f,-0.5f,0.5f, 0.0f, 0.0f,
		0.5f, 0.5f,0.5f, 0.0f, 1.0f,

		-0.5f,0.5f,-0.5f, 1.0f, 1.0f,
		-0.5f,-0.5f,-0.5f, 1.0f,0.0f,
		0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
		0.5f,0.5f,-0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f,0.5f, 1.0f, 1.0f,
		-0.5f,-0.5f,0.5f, 1.0f,0.0f,
		-0.5f,-0.5f,-0.5f,	0.0f, 0.0f,
		-0.5f, 0.5f,-0.5f, 0.0f, 1.0f,

		0.5f,0.5f,-0.5f, 5.0f, 5.0f,
		0.5f,0.5f,0.5f,     5.0f,0.0f,
		-0.5f,0.5f,0.5f, 0.0f, 0.0f,
		-0.5f,0.5f,-0.5f, 0.0f, 5.0f,

		0.5f,-0.5f,0.5f, 1.0f, 1.0f,
		0.5f,-0.5f,-0.5f, 1.0f,0.0f,
		-0.5f,-0.5f,-0.5f,	0.0f, 0.0f,
		-0.5f,-0.5f,0.5f, 0.0f, 1.0f,
	};

	glGenVertexArrays(1, &Cubo_VAO[0]);
	glBindVertexArray(Cubo_VAO[0]);

	glGenBuffers(1, &Cubo_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, Cubo_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void Cubo::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(Cubo_VAO[0]);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);
}