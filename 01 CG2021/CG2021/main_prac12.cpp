/*
Proyecto Final Computación Gráfica
Alumnos:
	Murrieta Villegas Alfonso
	Reza Chavarria Sergio Gabriel
	Valdespino Mendieta Joaquin
*/
#include <windows.h>
#include <mmsystem.h>

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


//Para music and sound
#pragma comment(lib, "winmm.lib")
//Global variable for sound
bool soundon = true;

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




//Keyframes (Manipulación y dibujo)
float	posCarX = 11.0f,
posCarY = -1.0f,
posCarZ = -0.5f,
rotCarY = 0.0f;

float incX = 0.0f, incY = 0.0f, incZ = 0.0f, incrotY = 0.0f;

//Animations variables 
float giroPanel = 30.0f;
bool panelbool = false;
int estadopanel = 1;

float giroCamaras = 0.0f;
int estadocam = 1;

int  stdCan = 1; 
float movCan = 0.0f;


float giroSilla = 0.0f;
bool sillabool = false;
int sillaEdo = 0;

//ciclo dia noche
float day = 0.0f,
difus = 1.0f,
night = 0.0f;
int stateday = 0;


#define MAX_FRAMES 7
int i_max_steps = 30;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posCarX = 0.0f;
	float posCarY = 0.0f;
	float posCarZ = 0.0f;
	float rotCarY = 0.0f;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir datos
bool play = false;
int playIndex = 0;





void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posCarX = posCarX;
	KeyFrame[FrameIndex].posCarY = posCarY;
	KeyFrame[FrameIndex].posCarZ = posCarZ;

	KeyFrame[FrameIndex].rotCarY = rotCarY;
	FrameIndex++;
}

void resetElements(void)
{
	posCarX = KeyFrame[0].posCarX;
	posCarY = KeyFrame[0].posCarY;
	posCarZ = KeyFrame[0].posCarZ;

	rotCarY= KeyFrame[0].posCarY;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posCarX - KeyFrame[playIndex].posCarX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posCarY - KeyFrame[playIndex].posCarY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posCarZ - KeyFrame[playIndex].posCarZ) / i_max_steps;

	incrotY = (KeyFrame[playIndex + 1].rotCarY - KeyFrame[playIndex].rotCarY) / i_max_steps;
	

}

void sound() {
	if (soundon) {
		//bool played = PlaySound(TEXT("ele.wav"), NULL, SND_FILENAME);
		bool played = PlaySound("more.wav", NULL, SND_LOOP | SND_ASYNC);
		cout << "Ambient:" << played << endl;
		soundon = false;
	}
}

void animate(void)
{
	//-------------------------------------Animación de Carro
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
			posCarX += incX;
			posCarY += incY;
			posCarZ += incZ;

			rotCarY += incrotY;

			i_curr_steps++;
		}
	}


	//*-------------------------------Animación de Paneles solares -------------------------*/
	if (true) {

		switch (estadopanel) {
			case 1:
				giroPanel += 0.5f;
				if (giroPanel > 30) {
					estadopanel = 2;
				}
				break;
			case 2:
				giroPanel -= 0.5f;
				if (giroPanel < -30) {
					estadopanel = 1;
				}
				break;
			default:
				giroPanel = giroPanel;

		}

	}
	/*--------------------------------Animacion automática de Camaras*/
	if (true) {
		switch (estadocam) {
		case 1:
			giroCamaras += 0.5f;
			if (giroCamaras > 6.0f) {
				estadocam = 2;
			}
			break;
		case 2:
			giroCamaras -= 0.5f;
			if (giroCamaras < -6.0f) {
				estadocam = 1;
			}
			break;
		}
	}
	
	//---------------------Animación automática de Cancel-----------------------------

	if (true) {
		switch (stdCan) {
		case 1:
			movCan += 0.01f;
			if (movCan > 0.2f) {
				stdCan = 2;
			}
			break;
		case 2:
			movCan -= 0.01f;
			if (movCan < -0.2f) {
				stdCan = 1;
			}
			break;
		}
	}

	/*------------------------------Sillas girando---------------------------------*/
	if (sillabool) {
		switch (sillaEdo) {
		case 1:
			giroSilla += 3.0f;
			if (giroSilla > 20.0f) {
				sillaEdo = 2;
			}
			break;
		case 2:
			giroSilla -= 3.0f;
			if (giroSilla < -20.0f) {
				sillaEdo = 3;
			}
			break;
		case 3:
			giroSilla += 5.0f;
			if (giroSilla > 90.0f) {
				sillaEdo = 4;
			}
			break;
		case 4:
			giroSilla -= 5.0f;
			if (giroSilla < -90.0f) {
				sillaEdo = 1;
			}
			break;
		case 0:
			giroSilla = 0.0f;
			
		}
	}


	
	//----------------------------------------Animacion dia noche
	if (stateday == 0) {
		day += 0.001;
		if (day >= 0.8)
			stateday = 1;
	}
	if (stateday == 1) {
		day -= 0.002;
		night += 0.001;

		difus -= 0.002;
		if (night >= 0.5)
			stateday = 2;
	}

	if (stateday == 2) {
		if (day <= 0.02)
			day += 0.002;

		night -= 0.001;
		difus += 0.002;

		if (difus >= 1)
			stateday = 0;

	}




}

void display(Shader shader, Shader animShader, Shader SkyboxShader, Skybox skybox, Model pastoExt, Model pared, Model ventana, Model bath,
	Model garage, Model jardin, Model lavado, Model maderablanca, Model pared_interior, Model pasto, Model suelo, Model techo, Model palm, Model carro, Model cocina, Model paredv1, Model paredv2,
	Model street, Model lavadora, Model cameraObj, Model ttv, Model sofa, Model mesaComer, Model panel, Model camaJ, Model camaS, Model camaA,Model wc, Model banera, Model lavamanos, Model closet, Model puerta, Model piscina, Model cancel1, Model cancel2,Model lampara,
	Model pant, Model esc1, Model silla)

{
	shader.use();
	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(day, 0.2f, night));
	shader.setVec3("dirLight.diffuse", glm::vec3(difus, difus, difus));
	shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

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

	//PERSONAJES
	//Remember to activate the shader with the animation
	/*
	animShader.use();
	animShader.setMat4("projection", projection);
	animShader.setMat4("view", view);

	animShader.setVec3("material.specular", glm::vec3(0.5f));
	animShader.setFloat("material.shininess", 32.0f);
	animShader.setVec3("light.ambient", ambientColor);
	animShader.setVec3("light.diffuse", diffuseColor);
	animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	animShader.setVec3("light.direction", lightDirection);
	animShader.setVec3("viewPos", camera.Position);


	ModelAnim yasuo("resources/objects/personajes/yasuo/yasuo4.fbx");
	yasuo.initShaders(animShader.ID);


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, -0.4f, -38.0f));
	model = glm::scale(model, glm::vec3(0.005f));
	animShader.setMat4("model", model);
	yasuo.Draw(animShader);
	*/






	//------------------------------------------------------------------Objetos del exterior-----------------------------------------------------------------------------------------//

	shader.use();

	//CANCELES - POOL DOORS
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.85f, -0.4f, -25.0f));
	model = glm::scale(model, glm::vec3(0.3f, 3.8f, 8.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	cancel1.Draw(shader);
	glEnable(GL_BLEND);


	//es el que se mueve
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.85f, -0.4f, -22.0f));
	model = glm::scale(model, glm::vec3(0.1f, 3.8f, 10.0f));
	//para el movimiento
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, movCan));
	shader.setMat4("model", model);
	cancel2.Draw(shader);
	glEnable(GL_BLEND);


	//Piscina
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.9f, -2.5f, -33.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.01f, 0.02f));
	shader.setMat4("model", model);
	piscina.Draw(shader);

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
	model = glm::translate(model, glm::vec3(posCarX, posCarY, posCarZ));
	model = glm::rotate(model, glm::radians(rotCarY), glm::vec3(0.0f, 0.0f, 1.0f));
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

	//LAMPARAS

	//cuarto murry
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.4f, -4.0f));
	model = glm::scale(model, glm::vec3(0.03f));
	shader.setMat4("model", model);
	lampara.Draw(shader);


	// Sala - conina
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.4f, -14.0f));
	model = glm::scale(model, glm::vec3(0.03f));
	shader.setMat4("model", model);
	lampara.Draw(shader);


	//cocina
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 1.4f, -14.0f));
	model = glm::scale(model, glm::vec3(0.03f));
	shader.setMat4("model", model);
	lampara.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 1.4f, -14.5f));
	model = glm::scale(model, glm::vec3(0.03f));
	shader.setMat4("model", model);
	lampara.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 1.4f, -15.0f));
	model = glm::scale(model, glm::vec3(0.03f));
	shader.setMat4("model", model);
	lampara.Draw(shader);




	//cocina integral
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(18.0f, -2.4f, -24.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.002f, 0.0022f, 0.002f));
	shader.setMat4("model", model);
	cocina.Draw(shader);

	//tv-mueble
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.5f, -2.3f, -14.5f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.35f));
	shader.setMat4("model", model);
	ttv.Draw(shader);


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.5f, -2.3f, -15.0f));
	model = glm::scale(model, glm::vec3(2.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	mesaComer.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.5f, -2.3f, -14.0f));
	model = glm::scale(model, glm::vec3(0.02f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	sofa.Draw(shader);
	
	/*--------------------------------Escritorio----------------------------------------*/
	
	//Escritorio Alfonso
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.5f, 0.0f, -3.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.1f, 0.0f));
	model = glm::scale(model, glm::vec3(0.002f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	pant.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, -3.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.004f,0.005f,0.003f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	esc1.Draw(shader);

	//Silla
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -3.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.004f, 0.003f, 0.004f));
	model = glm::rotate(model, glm::radians(giroSilla), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);
	
	
	
	//Escritorio Joaquin
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.5f, 0.0f, -32.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.1f, 0.0f));
	model = glm::scale(model, glm::vec3(0.002f,0.002f,0.002f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	pant.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.0f, -32.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.004f, 0.005f, 0.003f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	esc1.Draw(shader);

	//Silla
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.5f, 0.0f, -32.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.004f, 0.003f, 0.004f));
	model = glm::rotate(model, glm::radians(giroSilla), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);


	//Escritorio Sergio
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, -33.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0015f,0.002f,0.002f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	pant.Draw(shader);
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, -34.0f));
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0025f, 0.005f, 0.003f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	esc1.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, -35.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);

	/*-----------------------------------------------------------------BAÑOS--------------------------------------------------------------------------------------------*/
	//Baño 1
	
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-6.0f, -2.5f, -6.0f));
	model = glm::translate(temp, glm::vec3(-1.0f, 0.0f, -4.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.1f, 2.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	shader.setMat4("model", model);
	wc.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
	model = glm::translate(model, glm::vec3(-3.75f, 0.0f, -8.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.025f, 0.0175f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	banera.Draw(shader);

	//Lavamanos
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::translate(temp, glm::vec3(-0.1f, -0.5f, -7.0f));
	model = glm::scale(model, glm::vec3(0.025f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	lavamanos.Draw(shader);


	//Baño 2
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-6.0f, -2.5f, -6.0f));
	model = glm::translate(temp, glm::vec3(15.5f, 0.0f, -25.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.1f, 2.0f));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	wc.Draw(shader);


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
	model = glm::translate(model, glm::vec3(11.0f, 0.0f, -26.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.025f, 0.0175f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	banera.Draw(shader);

	//Lavamanos
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::translate(temp, glm::vec3(9.25f, -1.0f, -24.25f));
	model = glm::scale(model, glm::vec3(0.025f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	lavamanos.Draw(shader);

	

	//------------------------------------------------------------------PSICINA - EXT ----------------------------------------------------------------------------------------//

	//Lavadora
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.2f, -2.4f, -23.0f));
//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.003f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));

	shader.setMat4("model", model);
	lavadora.Draw(shader);

	//Camara - sala
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(14.85f, 0.8f, -10.0f));
	//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.007f));
	model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = model = glm::rotate(model, glm::radians(giroCamaras), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	cameraObj.Draw(shader);


	//Camara - piscina
	model = glm::mat4(1.0f);
	
	model = glm::translate(model, glm::vec3(-4.8f, 1.0f, -39.8f));
	//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(0.007f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = model = glm::rotate(model, glm::radians(giroCamaras), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	cameraObj.Draw(shader);


	//Paneles con base
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.7f, 0.0f));
	model = glm::translate(temp, glm::vec3(5.0f, 0.0f, -15.0f));
	model = glm::scale(model, glm::vec3(5.0f, 0.3f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);

	//Panel 1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.6f, -15.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, glm::radians(giroPanel), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	panel.Draw(shader);



	//Panel con base
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 2.7f, 0.0f));
	model = glm::translate(temp, glm::vec3(15.0f, 0.0f, -15.0f));
	model = glm::scale(model, glm::vec3(5.0f, 0.3f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	techo.Draw(shader);

	//Panel 2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 2.6f, -15.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, glm::radians(giroPanel), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	panel.Draw(shader);

	/*-------------------------------------------Objetos Cuartos----------------------------------------------*/

	
	//Camas
	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
	model = glm::translate(temp, glm::vec3(17.5f, -2.5f, -36.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.5f, 2.5f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	camaJ.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
	model = glm::translate(temp,glm::vec3(9.0f,-2.5f,-37.5f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f,2.5f,2.0f));
	shader.setMat4("model", model);
	camaS.Draw(shader);


	model = glm::mat4(1.0f);
	temp = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
	model = glm::translate(temp, glm::vec3(2.5f, -2.5f, -3.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.5f, 2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	camaA.Draw(shader);

	////////////////////CLOSET/////////////////////////
	
	//Closet - cuerto murry
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.6f, -2.4f, -8.05f));
	model = glm::scale(model, glm::vec3(0.002f, 0.0015f, 0.001f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	closet.Draw(shader);


	//Closet - cuerto sergio
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.5f, -2.4f, -36.9f));
	model = glm::scale(model, glm::vec3(0.0015f, 0.0015f, 0.002f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	closet.Draw(shader);


	//Closet - cuerto joaco
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.5f, -2.4f, -36.9f));
	model = glm::scale(model, glm::vec3(0.0015f, 0.0015f, 0.002f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	closet.Draw(shader);


	////////////////////PUERTAS/////////////////////////
	//Puerta principal 
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.0f, -2.4f, -10.0f));
	model = glm::scale(model, glm::vec3(0.025f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	puerta.Draw(shader);
		
	//puerta cuarto murry
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, -2.4f, -10.0f));
	model = glm::scale(model, glm::vec3(0.025f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	puerta.Draw(shader);

	//puerta baño cuarto murry
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.1f, -2.4f, -6.0f));
	model = glm::scale(model, glm::vec3(0.025f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	puerta.Draw(shader);


	//PUERTAS PEGADAS

	//puerta baño
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.0f, -2.4f, -28.9f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.03f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	puerta.Draw(shader);

	//puerta cuarto joaco
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.0f, -2.4f, -31.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.03f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shader.setMat4("model", model);
	puerta.Draw(shader);
	
	//Cuarto sergio 
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.7f, -2.4f, -32.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	puerta.Draw(shader);

	//CUARTO LAVADO
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.2f, -2.4f, -21.2f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.03f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	puerta.Draw(shader);
	

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


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, -40.0f));
	model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));
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

	//Ventana
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.15f));
	model = glm::scale(model, glm::vec3(5.0f, 3.0f, 0.8f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);

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



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(11.5f, 0.0f, -9.8f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv2.Draw(shader);
	glEnable(GL_BLEND);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);


	//Ventana

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.10f));
	model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.6f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);



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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -20.0f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);




	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(9.0f, 5.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);


	//Ventana
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.0f, 0.0f, -0.15f));
	model = glm::scale(model, glm::vec3(4.5f, 3.0f, 0.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);

	

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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::mat4(1.0f);
	temp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, -36.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);


	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.2f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 1.0f, 5.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	paredv1.Draw(shader);
	glEnable(GL_BLEND);



	//Ventana
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = glm::translate(temp, glm::vec3(0.1f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 3.0f, 5.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	ventana.Draw(shader);
	glEnable(GL_BLEND);


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

	//Escritorio
	Model pant("resources/objects/Setup/pantalla/pantallas4.fbx");
	Model esc1("resources/objects/Setup/escritorio/escritorio2.obj");
	Model silla("resources/objects/Setup/silla/silla5.obj");
	
	//Ventanas
	Model paredv1("resources/objects/Texplanes/paredintv.fbx");
	Model paredv2("resources/objects/Texplanes/paredvent.fbx");

	//MODELS -EXTER
	Model palm("resources/objects/exterior/palmera/palm.fbx");
	Model carro("resources/objects/exterior/carro/carro.fbx");
	Model street("resources/objects/exterior/calle/calle13.fbx");
	Model panel("resources/objects/exterior/panel/panel.obj");

	//MODELS COMEDOR - COCINA - SALA
	Model cocina("resources/objects/cocina/cocinaInt/cocina.fbx");
	//Model sofaMesa("resources/objects/sala/sofaMesa/sofaError.fbx");//pendiente

	Model lavadora("resources/objects/piscinaLavanderia/lavadora/wash_mashine2.fbx");
	Model cameraObj("resources/objects/piscinaLavanderia/camera/cameratext.fbx");
	Model ttv("resources/objects/sala/tele/ttv.fbx");

	//Mesa-barra y sala
	Model sofa("resources/objects/sala/salita/sala4.obj");
	Model mesaComer("resources/objects/cocina/comedor/comedor.fbx");
	Model lampara("resources/objects/cocina/lamp/lamp.fbx");


	//Camas
	
	Model camaJ("resources/objects/rooms/camas/bed1.fbx");
	Model camaS("resources/objects/rooms/camas/bed2.fbx");
	Model camaA("resources/objects/rooms/camas/bed3.fbx");
	
	
	//Model camaJ("");
	//Model camaS("");
	//Model camaA("");
	

	//Baños
	Model wc("resources/objects/bathroom/wc/wc.fbx");
	Model banera("resources/objects/bathroom/banera/banera6.obj");
	Model lavamanos("resources/objects/bathroom/lavamanos/lavamanos2.fbx");


	Model puerta("resources/objects/rooms/puerta/puerta6.fbx");
	Model closet("resources/objects/rooms/closet/closet.fbx");
	Model piscina("resources/objects/piscinaLavanderia/piscina/psicina1.obj");
	Model cancel1("resources/objects/piscinaLavanderia/cancel/cancel1pt1.fbx");
	Model cancel2("resources/objects/piscinaLavanderia/cancel/cancel2pt2.fbx");




	//Inicialización de KeyFrames
	/*
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posCarX = 0;
		KeyFrame[i].posCarY = 0;
		KeyFrame[i].posCarZ = 0;
		KeyFrame[i].rotCarY = 0;
		
	}*/
	/*----------------------------------------------------------------------------------*/
	/*-------------------------ANIMACION CARRO--------------------------------------------*/


	KeyFrame[0].posCarX = 11.0f;
	KeyFrame[0].posCarY = -1.0f;
	KeyFrame[0].posCarZ = -0.5f;
	KeyFrame[0].rotCarY = -90.0f;
	
	KeyFrame[1].posCarX = 11.0f;
	KeyFrame[1].posCarY = -6.0f;
	KeyFrame[1].posCarZ = -0.5f;
	KeyFrame[1].rotCarY = -90.0f;

	KeyFrame[2].posCarX = 12.5f;
	KeyFrame[2].posCarY = -12.0f;
	KeyFrame[2].posCarZ = -0.5f;
	KeyFrame[2].rotCarY = -45.0f;

	KeyFrame[3].posCarX = 14.0f;
	KeyFrame[3].posCarY = -16.0f;
	KeyFrame[3].posCarZ = -0.5f;
	KeyFrame[3].rotCarY = 0.0f;

	KeyFrame[4].posCarX = 17.0f;
	KeyFrame[4].posCarY = -16.0f;
	KeyFrame[4].posCarZ = -0.5f;
	KeyFrame[4].rotCarY = 0.0f;

	KeyFrame[5].posCarX = -60.0f;
	KeyFrame[5].posCarY = -15.0f;
	KeyFrame[5].posCarZ = -0.5f;
	KeyFrame[5].rotCarY = 0.0f;

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


		display(staticShader, animShader, skyboxShader ,skybox, pastoExt,pared_ext,ventana,bath,garage,jardin,lavado,maderablanca,pared_interior,pasto,suelo, techo, palm, 
			carro, cocina, paredv1, paredv2, street, lavadora, cameraObj, ttv, sofa, mesaComer,panel,camaJ,camaS,camaA,wc,banera,lavamanos, closet, 
			puerta, piscina, cancel1, cancel2,lampara, pant, esc1, silla);



		deltaTime = SDL_GetTicks() - lastFrame; 
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//sound();
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

	//For music start
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		sound();

	//Animación de Sillas.
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (sillabool == false) {
			sillabool = true;
			sillaEdo = 1;
		}
		else {
			sillabool = false;
			sillaEdo = 0;
		}

	}

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