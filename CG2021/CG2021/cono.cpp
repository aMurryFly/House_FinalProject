#include "cono.h"
Cono::Cono(GLfloat radio, GLfloat altura) :
	radio(radio), altura(altura) {
	cono_VAO[4] = (-1, -1, -1, -1);
	cono_VBO[4] = (-1, -1, -1, -1);

}
Cono::~Cono()
{
	glDeleteVertexArrays(4, cono_VAO);
	glDeleteBuffers(4, cono_VBO);
}
void Cono::init()
{
	const int nn = (2+ MERIDIANOSC) * 3;
	GLfloat cone_nor[nn]; // normal
	// generate the sphere data
	GLfloat x, y, z, a, da, r = radio, height = altura;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOSC);


	// [Generate sphere point data]

	//punta

	x = 0;
	y = height;
	z = 0;
	cono_pos[0] = 0;
	cono_pos[1] = y;
	cono_pos[2] = 0;
	cone_nor[0] = x;
	cone_nor[1] = y;
	cone_nor[2] = z;

	//Base
	for (ix = 3, a = 0.0, ia = 0; ia < MERIDIANOSC+1; ia++, a += da, ix += 3)
	{

		x = sin(a);
		y = 0;
		z = cos(a);
		cono_pos[ix + 0] = x * r;
		cono_pos[ix + 1] = y * r;
		cono_pos[ix + 2] = z * r;
		cone_nor[ix + 0] = x;
		cone_nor[ix + 1] = y;
		cone_nor[ix + 2] = z;

	}

	// [Generate GL_TRIANGLE indices]
	ix = 0;
	iy = 0;
	for (ia = 1; ia < MERIDIANOSC; ia++, iy++)
	{
		// first half of QUAD
		cono_index[ix] = iy;      ix++;
		cono_index[ix] = iy + 1;    ix++;
		cono_index[ix] = iy + MERIDIANOSC;   ix++;
		// second half of QUAD
		cono_index[ix] = iy + MERIDIANOSC;   ix++;
		cono_index[ix] = iy + 1;    ix++;
		cono_index[ix] = iy + MERIDIANOSC + 1; ix++;
	}
	// first half of QUAD
	cono_index[ix] = iy;       ix++;
	cono_index[ix] = iy + 1 - MERIDIANOSC;  ix++;
	cono_index[ix] = iy + MERIDIANOSC;    ix++;
	// second half of QUAD
	cono_index[ix] = iy + MERIDIANOSC;    ix++;
	cono_index[ix] = iy - MERIDIANOSC + 1;  ix++;
	cono_index[ix] = iy + 1;     ix++;
	iy++;

	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, cono_VAO);
	glGenBuffers(4, cono_VBO);
	glBindVertexArray(cono_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cono_pos), cono_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cone_nor), cone_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cono_index), cono_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cono::render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(cono_VAO[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, (sizeof(cono_pos) / sizeof(GLfloat)));
	//glDrawElements(GL_TRIANGLES, sizeof(cono_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}
