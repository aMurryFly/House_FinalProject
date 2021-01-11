#include "Helice.h"
Helice::Helice(GLfloat unidad):
	uni(unidad){
	helice_VAO[4] = (-1, -1, -1, -1);
	helice_VBO[4] = (-1, -1, -1, -1);
}
Helice::~Helice() {
	glDeleteVertexArrays(4, helice_VAO);
	glDeleteBuffers(4, helice_VBO);
}
void Helice::init() {
	GLfloat vert[] = {
		//Base chica
		//Frente
		-0.125f,0.0f,0.1f,
		0.125f,0.0f,0.1f,
		0.125f,0.25f,0.1f,
		-0.125f,0.25f,0.1f,
		//Atras
		-0.125f,0.0f,-0.1f,
		0.125f,0.0f,-0.1f,
		0.125f,0.25f,-0.1f,
		-0.125f,0.25f,-0.1f,
		//Abajo
		-0.125f,0.0f,0.1f,
		0.125f,0.0f,0.1f,
		0.125f,0.0f,-0.1f,
		-0.125f,0.0f,-0.1f,
		//izquierda
		-0.125f,0.0f,0.1f,
		-0.125f,0.0f,-0.1f,
		-0.125f,0.25f,-0.1f,
		-0.125f,0.25f,0.1f,
		//Derecha
		0.125f,0.0f,0.1f,
		0.125f,0.0f,-0.1f,
		0.125f,0.25f,-0.1f,
		0.125f,0.25f,0.1f,
		//arriba
		-0.125f,0.25f,0.1f,
		0.125f,0.25f,0.1f,
		0.125f,0.25f,-0.1f,
		-0.125f,0.25f,-0.1f,

		//Union
		//Frente
		0.1f,0.25f,0.05f,
		-0.1f,0.25f,0.05f,
		-0.1f,0.5f,0.05f,
		0.1f,0.5f,0.05f,
		//Atras
		0.1f,0.25f,-0.05f,
		-0.1f,0.25f,-0.05f,
		-0.1f,0.5f,-0.05f,
		0.1f,0.5f,-0.05f,
		//Izquierda
		-0.1f,0.25f,0.05f,
		-0.1f,0.25f,-0.05f,
		-0.1f,0.5f,-0.05f,
		-0.1f,0.5f,0.05f,
		//Derecha
		0.1f,0.25f,0.05f,
		0.1f,0.25f,-0.05f,
		0.1f,0.5f,-0.05f,
		0.1f,0.5f,0.05f,

		//Grande
		//Frente
		0.5f,0.5f,0.2f,
		-0.5f,0.5f,0.2f,
		-0.5f,1.0f,0.2f,
		0.5f,1.0f,0.2f,
		//Atras
		0.5f,0.5f,-0.2f,
		-0.5f,0.5f,-0.2f,
		-0.5f,1.0f,-0.2f,
		0.5f,1.0f,-0.2f,

		//Abajo
		0.5f,0.5f,0.2f,
		-0.5f,0.5f,0.2f,
		-0.5f,0.5f,-0.2f,
		0.5f,0.5f,-0.2f,
		//Arriba
		0.5f,1.0f,0.2f,
		-0.5f,1.0f,0.2f,
		-0.5f,1.0f,-0.2f,
		0.5f,1.0f,-0.2f,

		//Izquierda
		-0.5f,0.5f,0.2f,
		-0.5f,1.0f,0.2f,
		-0.5f,1.0f,-0.2f,
		-0.5f,0.5f,-0.2f,

		//Izquierda
		0.5f,0.5f,0.2f,
		0.5f,1.0f,0.2f,
		0.5f,1.0f,-0.2f,
		0.5f,0.5f,-0.2f,


	};


	glGenVertexArrays(1, &helice_VAO[0]);
	glBindVertexArray(helice_VAO[0]);

	glGenBuffers(1, &helice_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, helice_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Helice::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(helice_VAO[0]);
	glDrawArrays(GL_QUADS, 0, 64);
	glBindVertexArray(0);
}