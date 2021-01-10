#include "Puerta.h"
//Decoración de los bloques principales del nexo
Puerta::Puerta(GLfloat unidad) :
	uni(unidad) {
	Puerta_VAO[4] = (-1, -1, -1, -1);
	Puerta_VBO[4] = (-1, -1, -1, -1);
}
Puerta::~Puerta() {
	glDeleteVertexArrays(4, Puerta_VAO);
	glDeleteBuffers(4, Puerta_VBO);
}
void Puerta::init() {
	float vert[] = {

		1.0f,  1.0f, 0.0f,   3.0f, 3.0f,
		1.0f, 0.8f,0.0f,	3.0f,0.0f,
		0.0f,0.8f,0.0f,		0.0f,0.0f,
		0.0f,1.0f,0.0f,		0.0f,3.0f,


		0.2f,  0.8f, 0.0f,   1.0f, 1.0f,
		0.2f, 0.0f,0.0f,	1.0f,0.0f,
		0.0f,0.0f,0.0f,		0.0f,0.0f,
		0.0f,0.8f,0.0f,		0.0f,1.0f,

		1.0f,  0.8f, 0.0f,   1.0f, 1.0f,
		1.0f, 0.0f,0.0f,	1.0f,0.0f,
		0.8f,0.0f,0.0f,		0.0f,0.0f,
		0.8f,0.8f,0.0f,		0.0f,1.0f,

	};

	glGenVertexArrays(1, &Puerta_VAO[0]);
	glBindVertexArray(Puerta_VAO[0]);

	glGenBuffers(1, &Puerta_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, Puerta_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void Puerta::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(Puerta_VAO[0]);
	glDrawArrays(GL_QUADS, 0, 12);
	glBindVertexArray(0);
}