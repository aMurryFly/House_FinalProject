#include "Base_molino.h"

Base_Molino::Base_Molino(GLfloat base) :
	base(base) {
	base_molino_VAO[4] = (-1, -1, -1, -1);
	base_molino_VBO[4] = (-1, -1, -1, -1);
}
Base_Molino::~Base_Molino() {
	glDeleteVertexArrays(4, base_molino_VAO);
	glDeleteBuffers(4, base_molino_VBO);
}

void Base_Molino::init() {
	GLfloat vert[] = {
		//Triangulo
		//Frontal
		0.5f,0.25f,0.5f,
		0.0f,0.5f,0.5f,
		-0.5f,0.25f,0.5f,

		//Trasera
		0.5f,0.25f,-0.5f,
		0.0f,0.5f,-0.5f,
		-0.5f,0.25f,-0.5f,

		//Cara Fontal
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.25f,0.5f,
		-0.5f,0.25f,0.5f,
		//Cara trasera
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.25f,-0.5f,
		-0.5f,0.25f,-0.5f,

		//Base
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,

		//Izquierda
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.25f,0.5f,
		-0.5f,0.25f,-0.5f,

		-0.5f,0.25f,0.5f,
		-0.5f,0.25f,-0.5f,
		0.0f,0.5f,-0.5f,
		0.0f,0.5f,0.5f,

		//Derecha
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.25f,0.5f,
		0.5f,0.25f,-0.5f,

		0.5f,0.25f,0.5f,
		0.5f,0.25f,-0.5f,
		0.0f,0.5f,-0.5f,
		0.0f,0.5f,0.5f,

	};


	glGenVertexArrays(1, &base_molino_VAO[0]);
	glBindVertexArray(base_molino_VAO[0]);

	glGenBuffers(1, &base_molino_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, base_molino_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Base_Molino::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(base_molino_VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArrays(GL_QUADS, 6, 28);
	glBindVertexArray(0);
}