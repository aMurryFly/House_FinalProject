#include "Circunferencia.h"

Circunferencia::Circunferencia(GLfloat radio):
	radio(radio) {
	circunferencia_VAO[4] = (-1, -1, -1, -1);
	circunferencia_VBO[4] = (-1, -1, -1, -1);
}


Circunferencia::~Circunferencia()
{
	glDeleteVertexArrays(4, circunferencia_VAO);
	glDeleteBuffers(4, circunferencia_VBO);
}


void Circunferencia::init()
{
	const int nn = MERIDIANOS * 3;
	GLfloat circunferencia_nor[nn];
	GLfloat x, y, z, a, da, r = radio;
	int ia, ix, iy;
	//Superficie
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	


	for (ix = 0, a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
	{
		// unit cylinder

		x = sin(a);
		y = 0;
		z = cos(a);
		circunferencia_pos[ix + 0] = x * r;
		circunferencia_pos[ix + 1] = y * r;
		circunferencia_pos[ix + 2] = z * r;
		circunferencia_nor[ix + 0] = x;
		circunferencia_nor[ix + 1] = y;
		circunferencia_nor[ix + 2] = z;

	}

	//Generación de triangulos
	ix = 0;
	iy = 0;
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// first half of QUAD
			circunferencia_index[ix] = iy;      ix++;
			circunferencia_index[ix] = iy + 1;    ix++;
			circunferencia_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			circunferencia_index[ix] = iy + MERIDIANOS;   ix++;
			circunferencia_index[ix] = iy + 1;    ix++;
			circunferencia_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, circunferencia_VAO);
	glGenBuffers(4, circunferencia_VBO);
	glBindVertexArray(circunferencia_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, circunferencia_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circunferencia_pos), circunferencia_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, circunferencia_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circunferencia_nor), circunferencia_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circunferencia_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circunferencia_index), circunferencia_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Circunferencia::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(circunferencia_VAO[0]);
	//glDrawArrays(GL_POINTS, 0, sizeof(cilindro_pos) / sizeof(GLfloat));
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(circunferencia_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}