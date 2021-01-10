/*---------------------------------------------------------*/
/*Proyecto Final*/
/*Alumnos:
Murrieta Villegas Alfonso
Reza Chavarria Sergio Gabriel
Valdespino Mendieta Joaquin

*/
#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "Plano.h"
#include "ParedV.h"
#include "Puerta.h"
#include "Cubo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>


void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

//Primitivas
Plano plane(1.0f);
ParedV pared(1.0f);
Puerta puerta(1.0f);
Cubo cube(1.0f);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 5.0f, 10.0f));

GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;


void myData(void);
void display(Shader);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;
//Obnjetos

//Texture
unsigned int	 t_pared, t_piso, t_interior, t_bath, t_lavado,t_jardin, t_techo,t_suelo,t_garage,t_sala,t_pasto;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	//1-Canal Alpha, transparencias (Utilizado para png)

	
	t_pared = generateTextures("resources/Textures/pared.jpg",0);
	t_piso =  generateTextures("resources/Textures/Piso.jpg", 0);
	t_interior = generateTextures("resources/Textures/pared_interior.jpg", 0);
	t_bath = generateTextures("resources/Textures/baño.jpg", 0);
	t_lavado = generateTextures("resources/Textures/lavado.jpg", 0);
	t_jardin = generateTextures("resources/Textures/jardin.jpg", 0);
	t_techo = generateTextures("resources/Textures/techo.jpg", 0);
	t_suelo = generateTextures("resources/Textures/suelo.jpg", 0);
	t_garage = generateTextures("resources/Textures/garage.jpg", 0);
	t_sala = generateTextures("resources/Textures/sala.jpg", 0);
	t_pasto = generateTextures("resources/Textures/pasto.jpg", 0);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void myData()
{
	float vertices[] = {
		// positions          // texture coords con 1 para las coordenadas, más se repite o modifica el pintado de la image

		//Lado Numero 3
		40.0f,  0.0f, 40.0f,   15.0f, 0.0f, // top right
		40.0f,	0.0f, -40.0f,   15.0f, 15.0f, // bottom right
		-40.0f,  0.0f, -40.0f,   0.0f, 15.0f, // bottom left
		-40.0f,  0.0f, 40.0f,   0.0f, 0.0f,  // top left 

		

	};
	unsigned int indices[] = {
		//Indices lado numero 3
		0, 1, 3, // first triangle
		1, 2, 3,  // second triangle
		
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void animate(void)
{

}

void display(Shader shader)
{
	//Enable Shader
	shader.use();

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);

	/*--------------------------------------------------Piso----------------------------------------------------------*/
	//Piso
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_piso);
	glDrawArrays(GL_QUADS, 0, 4);


	//Paredes


	//-------------------------------------------Pared 1----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	pared.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.3f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();


	//-------------------------------------------Pared 2----------------------------------------------------------//
	model = glm::mat4(1.0f);

	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(40.0f, 5.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();


	model = glm::translate(temp, glm::vec3(0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(40.0f, 5.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();


	//-------------------------------------------Pared 3----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 5.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.3f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 5.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();


	//-------------------------------------------Pared 4----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -40.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	pared.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.3f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();


	//-------------------------------------------Pared 5----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(20.0f, 0.0f, -40.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();


	model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();


	//-------------------------------------------Pared 6----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(20.0f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	pared.render();

	model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();


	//-------------------------------------------Pared 7----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(20.0f, 0.0f, -10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();

	temp = model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.3f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();

	//Paredes de enmedio

	//-------------------------------------------Pared 8---------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();

	model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	plane.render();


	//-------------------------------------------Pared 9----------------------------------------------------------//
	//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();



	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 0.0f, -10.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();


	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(-2.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();



	//-------------------------------------------Pared 10----------------------------------------------------------//

	//Baño
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.0f, 0.0f, -10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();


	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();



	//-------------------------------------------Pared 11----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();



	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(3.0f, 0.0f, -6.0f));
	model = glm::scale(model, glm::vec3(2.2f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(2.2f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	puerta.render();

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.4f, 0.0f, -10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();



	//-------------------------------------------Pared 9B----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(10.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(3.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.2f));
	model = glm::scale(model, glm::vec3(3.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	puerta.render();


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(13.5f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(6.3f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.2f));
	model = glm::scale(model, glm::vec3(6.3f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pared);
	pared.render();


	//-------------------------------------------Pared 12----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -20.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();

	model = glm::translate(temp, glm::vec3(5.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();


	//-------------------------------------------Pared 13----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	puerta.render();


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -23.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	temp = model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -27.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	puerta.render();





	//-------------------------------------------Pared 13B----------------------------------------------------------//


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -24.0f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();



	//-------------------------------------------Pared 14----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(17.5f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	puerta.render();

	model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	puerta.render();

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(17.5f, 0.0f, -23.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();

	model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();



	//-------------------------------------------Pared 15----------------------------------------------------------//

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(17.5f, 0.0f, -28.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(17.3f, 0.0f, -24.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(19.6f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();


	//-------------------------------------------Pared 16----------------------------------------------------------//


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -30.0f));

	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	model = glm::translate(temp, glm::vec3(5.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();


	//-------------------------------------------Pared 17----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();

	//-------------------------------------------Pared 18----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(9.0f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -8.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();



	temp = model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -8.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();


	//-------------------------------------------Pared 19----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.0f, 0.0f, -32.0f));

	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::translate(temp, glm::vec3(4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();


	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::translate(temp, glm::vec3(4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();


	//-------------------------------------------Pared 20----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.0f, 0.0f, -30.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	temp = model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	puerta.render();

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	//-------------------------------------------Pared 21----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.0f, 0.0f, -32.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	pared.render();


	/*----------------------------------------Suelo------------------------------------*/

	//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 6.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_suelo);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.1f, -6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_suelo);
	plane.render();

	//Baño
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();


	//Garaje
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(15.0f, 0.1f, -5.0f));
	model = glm::scale(model, glm::vec3(10.0f, 0.2f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_garage);
	cube.render();

	//Sala Comedor

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 10.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_sala);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 0.1f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 12.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_sala);
	plane.render();


	//Lavado

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.1f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();

	//Jardin
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(17.5f, 0.1f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 10.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_pasto);
	plane.render();

	//Baño2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.1f, -24.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f, 6.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	//Cuarto Joaquin

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.1f, -30.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 10.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_suelo);
	plane.render();

	//Caurto Sergio
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.1f, -32.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 8.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_suelo);
	plane.render();

	//Descanso
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(9.0f, 8.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	plane.render();


	//Piscina

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -28.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 2.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_garage);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.1f, -28.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_garage);
	plane.render();


	//Techo parte externa

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 5.1f, -10.0f));
	model = glm::scale(model, glm::vec3(20.0f, 0.2f, 20.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.75f, 5.1f, -24.0f));
	model = glm::scale(model, glm::vec3(17.5f, 0.2f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(11.25f, 5.1f, -29.0f));
	model = glm::scale(model, glm::vec3(12.5f, 0.2f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.5f, 5.1f, -35.0f));
	model = glm::scale(model, glm::vec3(15.0f, 0.2f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.15f, 5.1f, -34.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.2f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.5f, 5.1f, -39.85f));
	model = glm::scale(model, glm::vec3(5.0f, 0.2f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_techo);
	cube.render();


	//--------------------------------Techo Parte interna---------------------------
	//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 4.9f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 6.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 4.9f, -6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	//Baño
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.9f, -6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();


	//Sala Comedor

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.9f, -10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 10.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 4.9f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 12.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();


	//Lavado

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 4.9f, -20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_lavado);
	plane.render();


	//Baño2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 4.9f, -24.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f, 6.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bath);
	plane.render();

	//Cuarto Joaquin

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 4.9f, -30.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 10.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	//Caurto Sergio
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 4.9f, -32.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 8.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_interior);
	plane.render();

	//Descanso
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.5f, 4.9f, -24.0f));
	
	model = glm::scale(model, glm::vec3(9.0f, 0.2f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_jardin);
	cube.render();


	


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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();

	//Mis funciones
	//Datos a utilizar

	LoadTextures();
	myData();
	plane.init();
	pared.init();
	puerta.init();
	cube.init();



	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		//double currentFrame = glfwGetTime(); 
		lastFrame = SDL_GetTicks();
		
		//std::cout <<"frame time = " << deltaTime << "sec"<< std::endl;
        // input
        // -----
		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		my_input(window);
		animate();
		//Mi función de dibujo
		display(projectionShader);
		//lastFrame = currentFrame;
		deltaTime = SDL_GetTicks() - lastFrame;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime*0.5);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime*0.5);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime*0.5);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime*0.5);
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}