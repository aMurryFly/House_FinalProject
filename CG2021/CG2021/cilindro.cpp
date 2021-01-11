#include "cilindro.h"


Cilindro::Cilindro(GLfloat radio, GLfloat altura) :
	radio(radio), altura(altura) {
	cilindro_VAO[4] = (-1, -1, -1, -1);
	cilindro_VBO[4] = (-1, -1, -1, -1);
}


Cilindro::~Cilindro()
{
	glDeleteVertexArrays(4, cilindro_VAO);
	glDeleteBuffers(4, cilindro_VBO);
}


void Cilindro::init()
{
	const int nn = PARALELOS * MERIDIANOS * 3;
	GLfloat cylinder_nor[nn]; 
	GLfloat x, y, z, a, b, da, db, r = radio, height = altura;
	int ia, ib, ix, iy;
	//Superficie
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	//Altura del cilindro
	db = (GLfloat)2.0*height / GLfloat(PARALELOS - 1);


	for (ix = 0, b = -height, ib = 0; ib < PARALELOS; ib++, b += db) {
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			x = sin(a);
			y = b;
			z = cos(a);
			cilindro_pos[ix + 0] = x * r;
			cilindro_pos[ix + 1] = y * r;
			cilindro_pos[ix + 2] = z * r;
			cylinder_nor[ix + 0] = x;
			cylinder_nor[ix + 1] = y;
			cylinder_nor[ix + 2] = z;
		}
	}

	//Generación de triangulos
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// first half of QUAD
			cilindro_index[ix] = iy;      ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// first half of QUAD
		cilindro_index[ix] = iy;       ix++;
		cilindro_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		cilindro_index[ix] = iy + MERIDIANOS;    ix++;
		// second half of QUAD
		cilindro_index[ix] = iy + MERIDIANOS;    ix++;
		cilindro_index[ix] = iy - MERIDIANOS + 1;  ix++;
		cilindro_index[ix] = iy + 1;     ix++;
		iy++;
	}
	//Tapas
	
	


	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, cilindro_VAO);
	glGenBuffers(4, cilindro_VBO);
	glBindVertexArray(cilindro_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilindro_pos), cilindro_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_nor), cylinder_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cilindro_index), cilindro_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cilindro::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(cilindro_VAO[0]);
	//glDrawArrays(GL_POINTS, 0, sizeof(cilindro_pos) / sizeof(GLfloat));
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(cilindro_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    
	glBindVertexArray(0);
}