#include "Plano.h"
//Decoración de los bloques principales del nexo
Plano::Plano(GLfloat unidad) :
	uni(unidad) {
	Plano_VAO[4] = (-1, -1, -1, -1);
	Plano_VBO[4] = (-1, -1, -1, -1);
}
Plano::~Plano() {
	glDeleteVertexArrays(4, Plano_VAO);
	glDeleteBuffers(4, Plano_VBO);
}
void Plano::init() {
	float vert[] = {
		/*
		1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // top left
		*/
		
		1.0f,  1.0f, 0.0f,   10.0f, 10.0f,
		1.0f, 0.0f, 0.0f,   10.0f,0.0f,
		0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,   0.0f, 10.0f,

	};

	glGenVertexArrays(1, &Plano_VAO[0]);
	glBindVertexArray(Plano_VAO[0]);

	glGenBuffers(1, &Plano_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, Plano_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void Plano::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(Plano_VAO[0]);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
}