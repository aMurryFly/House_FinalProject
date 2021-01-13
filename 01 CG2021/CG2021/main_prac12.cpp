
/*
Proyecto Final Computación Gráfica
Alumnos:
	Murrieta Villegas Alfonso
	Reza Chavarria Sergio Gabriel
	Valdespino Mendieta Joaquin
*/
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 10.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;



#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;


void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}



void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (recorrido1)
		{
			movAuto_z += 3.7f;
			orienta = 0.0f;
			if (movAuto_z > 150.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x -= 3.0f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z -= 3.0f;
			orienta = 180.0f;
			if (movAuto_z < -150.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_x += 2.7f;
			orienta = 90.0f;
			if (movAuto_x > 0.0f)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
	}
}

void display(Shader shader, Shader SkyboxShader, Skybox skybox,Model pastoExt, Model pared, Model ventana, Model bath,
	Model garage, Model jardin, Model lavado, Model maderablanca, Model pared_interior, Model pasto, Model suelo, Model techo, Model palm, Model carro, Model cocina, Model paredv1, Model paredv2,
	 Model street, Model sofaMesa)
{
	shader.use();
	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[0].constant", 0.8f);
	shader.setFloat("pointLight[0].linear", 0.009f);
	shader.setFloat("pointLight[0].quadratic", 0.032f);

	shader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.009f);
	shader.setFloat("pointLight[1].quadratic", 0.032f);

	shader.setFloat("material_shininess", 32.0f);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 tmp = glm::mat4(1.0f);
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 5.0f, 500.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	//// Light
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

	//Enable Shader
	shader.use();

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);


	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.5f, 10000.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	//------------------------------------------------------------------Objetos del exterior-----------------------------------------------------------------------------------------//


	//Palms
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, -2.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, -2.5f, 5.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, -2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, -2.5f, -5.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	//palm right
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -2.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -2.5f, 5.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -2.5f, -5.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	palm.Draw(shader);


	//Carro
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(11.0f, -1.0f, -0.5f));
	model = glm::scale(model, glm::vec3(0.07f));
	shader.setMat4("model", model);
	carro.Draw(shader);


	//PASTO GLOBAL

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.0f, -2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(300.0f));
	shader.setMat4("model", model);
	pastoExt.Draw(shader);


	//PISO 

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, -2.4f, 5.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	garage.Draw(shader);


	//CALLE - acera
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(45.0f, -2.3f, 9.0f));
	model = glm::scale(model, glm::vec3(90.0f, 1.0f, 3.0f));
	shader.setMat4("model", model);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-45.0f, -2.3f, 9.0f));
	model = glm::scale(model, glm::vec3(90.0f, 1.0f, 3.0f));
	shader.setMat4("model", model);
	pared_interior.Draw(shader);

	//El otro lado de la calle
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(45.0f, -2.3f, 22.0f));
	model = glm::scale(model, glm::vec3(90.0f, 1.0f, 3.0f));
	shader.setMat4("model", model);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-45.0f, -2.3f, 22.0f));
	model = glm::scale(model, glm::vec3(90.0f, 1.0f, 3.0f));
	shader.setMat4("model", model);
	pared_interior.Draw(shader);




	//STREET
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(85.0f, -2.4f, 16.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.24f, 0.01f, 0.8f));
	shader.setMat4("model", model);
	street.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-35.0f, -2.4f, 16.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.24f, 0.01f, 0.8f));
	shader.setMat4("model", model);
	street.Draw(shader);


	//------------------------------------------------------------------Cocina - Comedor- Sala ----------------------------------------------------------------------------------------//

	
	//cocina integral
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(18.0f, -2.4f, -24.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.002f, 0.0022f, 0.002f));
	shader.setMat4("model", model);
	cocina.Draw(shader);


	


	//------------------------------------------------------------------Construccion Casa-----------------------------------------------------------------------------------------//

	//-------------------------------------------Pared 1----------------------------------------------------------//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv2.Draw(shader);
	glEnable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.3f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);

	//Ventana
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0f, 0.0f));
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.15f));
	model = glm::scale(model, glm::vec3(5.0f, 3.0f, 0.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);



	//-------------------------------------------Pared 2----------------------------------------------------------//
	model = glm::mat4(1.0f);

	temp = model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(40.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);



	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(40.0f, 1.0f, 5.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);




	//-------------------------------------------Pared 3----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, -40.0f));
	model = glm::scale(model, glm::vec3(12.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(12.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	//-------------------------------------------Pared 4--[TIENE VENTANA]-----------------------------------------------//
	
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, -40.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);
	
	//Ventana

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.15f));
	model = glm::scale(model, glm::vec3(5.0f, 3.0f, 0.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	
	
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv2.Draw(shader);
	glEnable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	temp = model = glm::translate(temp, glm::vec3(0.0f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);

	//ventana

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.15f));
	model = glm::scale(model, glm::vec3(5.0f, 3.0f, 0.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);
	*/



	//-------------------------------------------Pared 5----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(15.0f, 0.0f, -30.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);


	model = glm::translate(temp, glm::vec3(-0.15f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);




	//-------------------------------------------Pared 6----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(15.0f, 0.0f, -15.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.15f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 3.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);




	//-------------------------------------------Pared 7----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(15.0f, 0.0f, -5.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	temp = model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.15f, 0.0f, 5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);



	//Paredes de enmedio

	//-------------------------------------------Pared 8---------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.0f, 0.0f, -5.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.3f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	//-------------------------------------------Pared 9----------------------------------------------------------//
	//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -9.8f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);



	model = glm::translate(temp, glm::vec3(1.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	//Puerta Cuarto Alfonso

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(4.5f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(3.0f, 2.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	//-------------------------------------------Pared 10----------------------------------------------------------//

	//Baño
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-0.2f, 0.0f, -8.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-4.75f, 0.0f, -8.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);




	//-------------------------------------------Pared 11----------------------------------------------------------//
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -6.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-1.0f, 2.0f, -6.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-0.1f, -0.5f, -6.0f));
	model = glm::scale(model, glm::vec3(0.2f, 4.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	

	


	//-------------------------------------------Pared 9B----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.5f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 2.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.5f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	//Ventana

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.10f));
	model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);

	


	//-------------------------------------------Pared 12----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, -19.8f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(-1.25f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);

	model = glm::translate(temp, glm::vec3(2.75f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	maderablanca.Draw(shader);






	
	//-------------------------------------------Pared 13----------------------------------------------------------//





	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 0.0f, -25.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.0f, 1.0f, 5.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	//Puerta Lavado
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 2.0f, -21.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.2f, 0.0f, -23.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 5.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 0.0f, -19.9f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 1.0f, 5.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	//Puerta Baño 2

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.2f, 0.0f, -26.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 5.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 2.0f, -29.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);

	


	


	//-------------------------------------------Pared 13B----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(9.75f, 0.0f, -24.0f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	
	



	//-------------------------------------------Pared 14 y 15----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.3f, 2.0f, -21.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	maderablanca.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.3f, 0.0f, -23.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 5.0f);
	lavado.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(12.3f, 0.0f, -27.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model =glm::scale(model, glm::vec3(6.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 5.0f);
	bath.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.5f, 0.0f, -26.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 5.0f);
	maderablanca.Draw(shader);


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(14.75f, 0.0f, -25.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 5.0f);
	maderablanca.Draw(shader);

	
	
	//-------------------------------------------Pared 16----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, -30.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(-1.25f, 0.0f, 0.2f));
	model = glm::scale(model, glm::vec3(5.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);

	model = glm::translate(temp, glm::vec3(2.75f, 0.0f, 0.2f));
	model = glm::scale(model, glm::vec3(2.5f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	maderablanca.Draw(shader);



	//-------------------------------------------Pared 17----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -20.0f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	

	//-------------------------------------------Pared 18----------------------------------------------------------//


	model = glm::mat4(1.0f);
	temp=model = glm::translate(model, glm::vec3(4.0f, 2.0f, -25.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(4.0f, 0.0f, -20.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(4.0f, 0.0f, -30.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 5.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);






	//-------------------------------------------Pared 19----------------------------------------------------------//

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(2.5f, 0.0f, -32.0f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(3.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(3.5f, 0.0f, -32.0f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(5.5f, 2.0f, -32.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);
	



	

	//-------------------------------------------Pared 20----------------------------------------------------------//


	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 0.0f, -36.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, 2.0f, -31.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(7.0f, -0.5f, -29.9f));

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 1.0f, 4.0f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	//-------------------------------------------Pared 21----------------------------------------------------------//
	

	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, -36.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);
	
	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::translate(temp, glm::vec3(0.1f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 3.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);

	/*----------------------------------------Suelo------------------------------------*/

	//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));


	model = glm::translate(temp, glm::vec3(0.0f, 0.1f, -3.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(2.5f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(0.0f, 0.1f, -8.0f));
	model = glm::scale(model, glm::vec3(5.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-2.5f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(0.0f, 0.1f, -8.0f));
	model = glm::scale(model, glm::vec3(5.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);

	


	//Garaje
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(10.0f, 0.1f, -5.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	garage.Draw(shader);

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.40f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.85f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(9.7f, 0.2, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	garage.Draw(shader);

	

	//Sala Comedor

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(5.0f, 0.1f, -15.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(5.5f, 0.1f, -26.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);

	
	

	//Lavado
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.75f, 0.1f, -22.0f));
	model = glm::scale(model, glm::vec3(5.5f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);

	

	//Jardin

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(13.75f, 0.1f, -25.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pasto.Draw(shader);

	
	//Baño2
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.75f, 0.1f, -27.0f));
	model = glm::scale(model, glm::vec3(5.5f, 1.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);

	
	//Cuarto Joaquin
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(11.0f, 0.1f, -35.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);



	//Caurto Sergio
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(4.0f, 0.1f, -36.0f));
	model = glm::scale(model, glm::vec3(6.0f, 1.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);

	//Descanso
	
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(-0.5f, 0.1f, -24.0f));
	model = glm::scale(model, glm::vec3(9.0f, 1.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	jardin.Draw(shader);
	
	//Piscina

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(-2.0f, 0.1f, -29.0f));
	model = glm::scale(model, glm::vec3(6.0f, 1.0f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	garage.Draw(shader);



	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, -2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(2.5f, 0.1f, -30.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	garage.Draw(shader);

	


	/*----------------------------Techo------------------------------------*/
		//Cuarto Alfonso
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));


	model = glm::translate(temp, glm::vec3(0.0f, -0.1f, -3.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	suelo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(2.5f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(0.0f, -0.1f, -8.0f));
	model = glm::scale(model, glm::vec3(5.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-2.5f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(0.0f, -0.1f, -8.0f));
	model = glm::scale(model, glm::vec3(5.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);




	//Garaje
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(10.0f, -0.1f, -5.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	//Sala Comedor

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(5.0f, -0.1f, -15.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(5.5f,-0.1f, -26.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);




	//Lavado
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.75f, -0.1f, -22.0f));
	model = glm::scale(model, glm::vec3(5.5f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	lavado.Draw(shader);




	//Baño2
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.75f, -0.1f, -27.0f));
	model = glm::scale(model, glm::vec3(5.5f, 1.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	bath.Draw(shader);


	//Cuarto Joaquin
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(11.0f, -0.1f, -35.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);



	//Caurto Sergio
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(4.0f, -0.1f, -36.0f));
	model = glm::scale(model, glm::vec3(6.0f, 1.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	pared_interior.Draw(shader);

	//Descanso

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(-0.5f, -0.1f, -24.0f));
	model = glm::scale(model, glm::vec3(9.0f, 1.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	jardin.Draw(shader);

	//Piscina


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
	model = glm::translate(temp, glm::vec3(2.5f, -0.1f, -30.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	jardin.Draw(shader);


	//Techo Exterior


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(10.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(20.0f, 0.2f, 20.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(8.75f, 0.0f, -24.0f));
	model = glm::scale(model, glm::vec3(17.5f, 0.2f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(13.0f, 0.0f, -35.0f));
	model = glm::scale(model, glm::vec3(14.0f, 0.2f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(11.75f, 0.0f, -29.0f));
	model = glm::scale(model, glm::vec3(11.5f, 0.2f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(0.1f, 0.0f, -34.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);

	

	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(3.0f, 0.0f, -39.85f));
	model = glm::scale(model, glm::vec3(6.0f, 0.2f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.6f, 0.0f));
	model = glm::translate(temp, glm::vec3(19.85f, 0.0f, -25.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.2f, 10.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);


	//Skybox

	SkyboxShader.use();
	skybox.Draw(SkyboxShader, view, projection, camera);
}


int main()
{

	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	//LOAD MODELS

	//HOUSE MODELS - TEXTURES
	Model pastoExt("resources/objects/Texplanes/pasto.fbx");
	Model pared_ext("resources/objects/Texplanes/pared.fbx");
	Model ventana("resources/objects/Ventana/ventana4.fbx");
	Model bath("resources/objects/Texplanes/mosaic.fbx");
	Model garage("resources/objects/Texplanes/garage.fbx");
	Model jardin("resources/objects/Texplanes/jardin.fbx");
	Model lavado("resources/objects/Texplanes/lavado.fbx");
	Model maderablanca("resources/objects/Texplanes/maderablanca.fbx");
	Model pared_interior("resources/objects/Texplanes/pared_interior.fbx");
	Model pasto("resources/objects/Texplanes/pasto.fbx");
	Model suelo("resources/objects/Texplanes/suelo.fbx");
	Model techo("resources/objects/Texplanes/marc.fbx");

	//Ventanas
	Model paredv1("resources/objects/Texplanes/paredintv.fbx");
	Model paredv2("resources/objects/Texplanes/paredvent.fbx");

	//MODELS -EXTER
	Model palm("resources/objects/exterior/palmera/palm.fbx");
	Model carro("resources/objects/exterior/carro/carro.fbx");
	Model street("resources/objects/exterior/calle/calle13.fbx");

	//MODELS COMEDOR - COCINA - SALA
	Model cocina("resources/objects/cocina/cocinaInt/cocina.fbx");
	Model sofaMesa("resources/objects/sala/sofaMesa/sofaError.fbx");//pendiente

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		display(staticShader, skyboxShader ,skybox, pastoExt,pared_ext,ventana,bath,garage,jardin,lavado,maderablanca,pared_interior,pasto,suelo, techo, palm, carro, cocina, paredv1, paredv2, street, sofaMesa);


		deltaTime = SDL_GetTicks() - lastFrame; 
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime*0.005);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime*0.005);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime*0.005);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime*0.005);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
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

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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