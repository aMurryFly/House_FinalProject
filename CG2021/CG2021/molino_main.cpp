/*---------------------------------------------------------*/
/* ----------------  Molino --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*-------Alumno: Reza Chavarria Sergio Gabriel-------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

#include "cono.h"
#include "cilindro.h"
#include "Circunferencia.h"
#include "Base_molino.h"
#include "Helice.h"



void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

//Primitivas
Cilindro cylinder(0.5f, 1.0f);
Cono cone(0.5f, 1.0f);
Base_Molino base(1.0f);
Circunferencia circ(0.5f);
Helice hel(1.0f);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(Shader);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotY = 0.0f;
float giroHelices = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		
			2.375f, 0.0f, 2.375f,			1.0f, 0.0f, 0.0f,
		2.375f, 0.0f, -2.375f,		1.0f, 0.0f, 0.0f,
		-2.375f, 0.0f, -2.375f,		1.0f, 0.0f, 0.0f,
		-2.375f, 0.0f,2.375f,		1.0f, 0.0f, 0.0f,
		/*
		4.75f, 0.0f, -4.75f,			1.0f, 0.0f, 0.0f,
		4.75f, 5.0f, -4.75f,		1.0f, 0.0f, 0.0f,
		-4.75f, 5.0f, -4.75f,			1.0f, 0.0f, 0.0f,
		-4.75f, 0.0f, -4.75f,		1.0f, 0.0f, 0.0f,
	
		2.375f, 5.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		2.375f, 6.0f,  0.0f,		1.0f, 0.0f, 0.0f,
		-2.375f, 6.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-2.375f, 5.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		*/
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}




void display(Shader shader)
{

	shader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT,5.0f,1000.0f);
	projection = glm::translate(projection, glm::vec3(0.0f, -5.0f, -10.0f));
	
	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 tmp2 = glm::mat4(1.0f);
	
	
	//Piso
	
	shader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	//Cilindro molino
	//Traslación (0,2.5,0)
	//Escala(4.75,5,4.75)
	
	tmp = model = glm::translate(model, glm::vec3(0.0f,2.5f,0.0f));
	model = glm::scale(model, glm::vec3(4.75f, 5.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.8353f, 0.7294f, 0.545f));
	cylinder.render();

	//Cono Techo
	//Traslación y=4.0 (El centro del cono está en la punta)
	//Escala (4.75,1.5,4.75)
	
	tmp=model = glm::translate(tmp, glm::vec3(0.0f, 2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(4.75f, 1.5f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.1686f, 0.1372f, 0.09411f));
	cone.render();
	
	
	//Parte de engranajes de molino
	tmp=model = glm::translate(tmp, glm::vec3(0.0f, 0.5f,1.375f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	base.render();

	//Cilindro
	
	//Punto de giro
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.5f));
	model = glm::rotate(model, glm::radians(giroHelices), glm::vec3(0.0f, 0.0f, 1.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.9f, 0.5f, 0.0f));
	cylinder.render();

	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.9f, 0.5f, 0.0f));
	circ.render();

	//Helices
	tmp2 = model=glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.5f));

	
	model = glm::translate(tmp2, glm::vec3(-0.125f, 0.125f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	hel.render();
	
	model = glm::translate(tmp2, glm::vec3(-0.125f, -0.125f, 0.0f));
	model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	hel.render();

	model = glm::translate(tmp2, glm::vec3(0.125f, -0.125f, 0.0f));
	model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	hel.render();

	model = glm::translate(tmp2, glm::vec3(0.125f, 0.125f, 0.0f));
	model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	hel.render();

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Molino", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();

	hel.init();
	cone.init();
	cylinder.init();
	circ.init();
	base.init();
	

	glEnable(GL_DEPTH_TEST);
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display(projectionShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	//~Esfera();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.18f;
	
	//Giro Helices
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		giroHelices += 0.20f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		giroHelices -= 0.20f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}