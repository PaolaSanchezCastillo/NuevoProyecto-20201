#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Cylinder pata(20, 20, 0.5, 0.5), pataSilla(20, 20, 0.5, 0.5);
Cylinder velas(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box1;
Box box2;
Box box3;
Box boxCesped;
Box papelPicado; 

Box pared1, paredBanio, piso1, piso2, piso3, techo1;
//Para los sofas
Box sofabk, sofac1, sofac2, sofac3, sofac4, sofac5, sofal, sofar;
//Para muebles
Box mueble, escritorio;
//Para mesa y muebles 
Box mesa, asientoSilla, respaldoSilla, mueble1Sup, mueble1Inf, muebleCocina;
// Models complex instances
Model modelAircraft;
Model modelEclipse;
Model modelHeli;
Model modelMueble, modelToilet, modelBanio, modelRegadera, modelVela;
Model modelCama, modelLaptop, modelCajon, modelMesaOfrenda, modelLich, modelGravestone, modelZombie,
modelSkull, modelPumpkin, modelPanMuerto, modelBanana;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureIDCocina, textureIDPared, textureIDParedInter,
textureIDEstufa, textureIDEstufaSup, textureIDRefri, textureIDRefriSup, textureIDAzulejos, textureIDPasto, textureIDSofa, 
textureIDCarretera, textureIDPuerta, textureIDPuertaBanio, textureIDPisoAzulejos, textureIDTv, textureIDVela,
textureIDMueble, textureIDVentana, textureIDPisoBanio, textureIDTierra, textureIDPapel, textureIDPapelPicado;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_perdicus/perdicus_ft.tga",
		"../Textures/mp_perdicus/perdicus_bk.tga",
		"../Textures/mp_perdicus/perdicus_up.tga",
		"../Textures/mp_perdicus/perdicus_dn.tga",
		"../Textures/mp_perdicus/perdicus_rt.tga",
		"../Textures/mp_perdicus/perdicus_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
			"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
			"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/iluminacion_texture_res.fs");
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
			"../Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
			"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderMulLighting);

	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderMulLighting);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderMulLighting);

	sphere3.init();
	sphere3.setShader(&shaderMulLighting);

	box3.init();
	box3.setShader(&shaderMulLighting);

	papelPicado.init();
	papelPicado.setShader(&shaderMulLighting);

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	velas.init();
	velas.setShader(&shaderMulLighting);

	pared1.init();
	// Settea el shader a utilizar
	pared1.setShader(&shaderMulLighting);
	pared1.setColor(glm::vec4(0.85, 0.8, 0.4, 1.0));

	paredBanio.init();
	// Settea el shader a utilizar
	paredBanio.setShader(&shaderMulLighting);
	paredBanio.setColor(glm::vec4(0.85, 0.8, 0.4, 1.0));

	piso1.init();
	// Settea el shader a utilizar
	piso1.setShader(&shaderMulLighting);
	piso1.setColor(glm::vec4(0.3, 0.2, 0.25, 1.0));

	piso2.init();
	// Settea el shader a utilizar
	piso2.setShader(&shaderMulLighting);
	piso2.setColor(glm::vec4(0.3, 0.2, 0.25, 1.0));

	piso3.init();
	// Settea el shader a utilizar
	piso3.setShader(&shaderMulLighting);
	piso3.setColor(glm::vec4(0.3, 0.2, 0.25, 1.0));

	//Mueble
	mueble.init();
	// Settea el shader a utilizar
	mueble.setShader(&shaderMulLighting);
	mueble.setColor(glm::vec4(0.5, 0.4, 0.2, 1.0));

	//sofa1
	sofabk.init();
	// Settea el shader a utilizar
	sofabk.setShader(&shaderMulLighting);
	sofabk.setColor(glm::vec4(0.5, 0.4, 0.2, 1.0));

	sofac1.init();
	// Settea el shader a utilizar
	sofac1.setShader(&shaderMulLighting);
	sofac1.setColor(glm::vec4(0.6, 0.5, 0.3, 1.0));

	sofac2.init();
	// Settea el shader a utilizar
	sofac2.setShader(&shaderMulLighting);
	sofac2.setColor(glm::vec4(0.65, 0.55, 0.35, 1.0));

	sofac3.init();
	// Settea el shader a utilizar
	sofac3.setShader(&shaderMulLighting);
	sofac3.setColor(glm::vec4(0.6, 0.5, 0.3, 1.0));

	sofac4.init();
	// Settea el shader a utilizar
	sofac4.setShader(&shaderMulLighting);
	sofac4.setColor(glm::vec4(0.65, 0.55, 0.35, 1.0));

	sofac5.init();
	// Settea el shader a utilizar
	sofac5.setShader(&shaderMulLighting);
	sofac5.setColor(glm::vec4(0.6, 0.5, 0.3, 1.0));

	sofar.init();
	// Settea el shader a utilizar
	sofar.setShader(&shaderMulLighting);
	sofar.setColor(glm::vec4(0.5, 0.4, 0.2, 1.0));
	sofal.init();
	// Settea el shader a utilizar
	sofal.setShader(&shaderMulLighting);
	sofal.setColor(glm::vec4(0.5, 0.4, 0.2, 1.0));

	mesa.init();
	// Settea el shader a utilizar
	mesa.setShader(&shaderColorLighting);
	mesa.setColor(glm::vec4(0.5, 0.4, 0.2, 1.0));

	pata.init();
	pata.setShader(&shaderColorLighting);
	pata.setColor(glm::vec4(0.12, 0.1, 0.15, 1.0));

	asientoSilla.init();
	asientoSilla.setShader(&shaderMulLighting);
	asientoSilla.setColor(glm::vec4(0.6, 0.5, 0.3, 1.0));

	pataSilla.init();
	pataSilla.setShader(&shaderColorLighting);
	pataSilla.setColor(glm::vec4(0.12, 0.1, 0.15, 1.0));

	respaldoSilla.init();
	respaldoSilla.setShader(&shaderMulLighting);
	respaldoSilla.setColor(glm::vec4(0.65, 0.55, 0.35, 1.0));

	muebleCocina.init();
	muebleCocina.setShader(&shaderMulLighting);
	muebleCocina.setColor(glm::vec4(0.85, 0.75, 0.55, 1.0));

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	modelEclipse.loadModel("../models/Eclipse/2003eclipse.obj");
	modelEclipse.setShader(&shaderMulLighting);

	modelHeli.loadModel("../models/Helicopter/Mi_24.obj");
	modelHeli.setShader(&shaderMulLighting);

	modelMueble.loadModel("../models/Wood_Table/Wood_Table.obj");
	modelMueble.setShader(&shaderMulLighting);

	modelToilet.loadModel("../models/Toilet/toilet.obj");
	modelToilet.setShader(&shaderMulLighting);

	modelCama.loadModel("../models/cama/Snooze_OBJ.obj");
	modelCama.setShader(&shaderMulLighting);

	modelLaptop.loadModel("../models/laptop/lolo.obj");
	modelLaptop.setShader(&shaderMulLighting);

	modelMesaOfrenda.loadModel("../models/Nikari_side_table_obj/Nikari side table.obj");
	modelMesaOfrenda.setShader(&shaderMulLighting);

	modelBanana.loadModel("../models/banana/Banana.obj");
	modelBanana.setShader(&shaderMulLighting);

	//modelRegadera.loadModel("../models/.obj");
	//modelRegadera.setShader(&shaderMulLighting);


	modelCajon.loadModel("../models/cajon/cajon.obj");
	modelCajon.setShader(&shaderMulLighting);

	modelVela.loadModel("../models/Vela/PROP-0001.obj");
	modelVela.setShader(&shaderMulLighting);

	modelLich.loadModel("../models/Lich+Character+Model (1)/Lich.obj");
	modelLich.setShader(&shaderMulLighting);

	modelGravestone.loadModel("../models/Low+Poly+Old+Mossy+Gravestone_obj/Low Poly Old Mossy Gravestone.obj");
	modelGravestone.setShader(&shaderMulLighting);

	modelZombie.loadModel("../models/zombies/zombie.obj");
	modelZombie.setShader(&shaderMulLighting);

	modelSkull.loadModel("../models/skull_obj/skull_full.obj");
	modelSkull.setShader(&shaderMulLighting);

	modelPumpkin.loadModel("../models/pumpkin/Halloween_Pumpkin.obj");
	modelPumpkin.setShader(&shaderMulLighting);

	modelPanMuerto.loadModel("../models/pan_muerto/pan_muerto.obj");
	modelPanMuerto.setShader(&shaderMulLighting);


	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/losetas.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	/*// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture5("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture5.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID5);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture5.freeImage(bitmap); */


	// Definiendo la textura a utilizar
	Texture texture3("../Textures/muebleCocina.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDCocina);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDCocina);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaEstufa.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDEstufa);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDEstufa);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture5("../Textures/texturaEstufaSup.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture5.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDEstufaSup);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDEstufaSup);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture5.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture6("../Textures/texturaRefri.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture6.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDRefri);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDRefri);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture6.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture7("../Textures/metal.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture7.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture7.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDRefriSup);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDRefriSup);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture7.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture8("../Textures/azulejos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture8.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture8.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDAzulejos);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDAzulejos);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture8.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture9("../Textures/pasto.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture9.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture9.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPasto);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPasto);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture9.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture10("../Textures/pared.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture10.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture10.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPared);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPared);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture10.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture11("../Textures/carretera.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture11.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture11.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDCarretera);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDCarretera);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture11.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture12("../Textures/paredInt.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture12.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture12.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDParedInter);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDParedInter);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture12.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture13("../Textures/dor.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture13.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture13.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPuerta);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPuerta);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture13.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture14("../Textures/dor_wc.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture14.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture14.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPuertaBanio);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPuertaBanio);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture14.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture15("../Textures/piso_azulejos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture15.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture15.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPisoAzulejos);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPisoAzulejos);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture15.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture16("../Textures/tv.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture16.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture16.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDTv);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDTv);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture16.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture17("../Textures/vela2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture17.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture17.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDVela);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDVela);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture17.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture18("../Textures/muebleCuarto.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture18.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture18.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDMueble);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDMueble);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture18.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture19("../Textures/paredVentana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture19.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture19.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDVentana);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDVentana);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture19.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture20("../Textures/pisoBanio.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture20.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture20.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPisoBanio);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPisoBanio);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture20.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture21("../Textures/texturaTierra.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture21.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture21.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDTierra);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDTierra);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture21.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture22("../Textures/guirnalda-papel.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture22.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture22.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPapel);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPapel);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture22.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture23("../Textures/papelPicado2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture23.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture23.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDPapelPicado);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDPapelPicado);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture23.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture24("../Textures/texturaSofa.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture24.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture24.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureIDSofa);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureIDSofa);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture24.freeImage(bitmap);

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.001;

	sentido = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 30.0;
	//glm::mat4 matrixModelAircraft = glm::mat4(1.0); //se cortan y pegan estas lineas, estaban mas abajo
	//matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(8.0, 2.0, -10.0));
	glm::mat4 matrixModelEclipse = glm::mat4(1.0);
	matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(70.0, 0.0, 21.0));
	matrixModelEclipse = glm::rotate(matrixModelEclipse, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 matrixModelHeli = glm::mat4(1.0);
	matrixModelHeli = glm::translate(matrixModelHeli, glm::vec3(75.0, 0.0, 12.0));
	matrixModelHeli = glm::rotate(matrixModelHeli, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

	

	int state = 0, state2 = 0; //se agregan variables para el movimiento 
	float offsetAircraftAdvance = 0.0;
	float offsetAircraftRot = 0.0;
	float offsetHeliAdvance = 0.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con material
		shaderMaterialLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
				 glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		// Propiedades de la luz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));
		// Esto es para la luz spotlight
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getFront()));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.1);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.001);

		// Esto es para la luces pointlights
		// Numero de luces a utilizar de tipo pointlights = 7, IMPORTANTE CAMBIAR SI SE USA UNA CANTIDAD DIFERENTE DE LUCES
		shaderMulLighting.setInt("pointLightCount", 5);
		// Posicion de la luz con indice [0]
		shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-5.0, 4.5, 5.1))));
		//Propiedades de la luz verde
		shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);

		//Propiedades de la luz roja 
		shaderMulLighting.setVectorFloat3("pointLights[1].position", glm::value_ptr((glm::vec3(-5.5, 4.5, 5.1))));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[1].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[1].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[1].quadratic", 0.004);

		//Propiedades de la luz azul 
		shaderMulLighting.setVectorFloat3("pointLights[2].position", glm::value_ptr((glm::vec3(-6, 4.5, 5.1))));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.0, 0.01)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[2].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[2].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[2].quadratic", 0.004);
		//Propiedades de la luz amarilla
		shaderMulLighting.setVectorFloat3("pointLights[3].position", glm::value_ptr((glm::vec3(-7.5, 2, -21.5))));	
		shaderMulLighting.setVectorFloat3("pointLights[3].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[3].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[3].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[3].quadratic", 0.004);
		//Propiedades de la luz amarilla2 (vela)
		shaderMulLighting.setVectorFloat3("pointLights[4].position", glm::value_ptr((glm::vec3(-7.5, 2, -18.5))));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[4].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[4].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[4].quadratic", 0.004);
		//Siguiente vela
		/*shaderMulLighting.setVectorFloat3("pointLights[5].position", glm::value_ptr((glm::vec3(-7.5, 2, -20.5))));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[5].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[5].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[5].quadratic", 0.004);
		//Siguiente vela
		shaderMulLighting.setVectorFloat3("pointLights[6].position", glm::value_ptr((glm::vec3(-6.0, 2, -20.5))));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[6].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[6].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[6].quadratic", 0.004);*/

		//Esto es para colocar las esferas de las luces (modelo, no las propiedades) 
		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-5.0, 4.5, 5.1));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-5.5, 4.5, 5.1));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-6.0, 4.5, 5.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();

		/*//Amarilla
		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-7.5, 2, -21.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();
		//Vela2
		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-7.5, 2, -18.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();
		
		//Vela3
		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-7.5, 2, -20.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();
		//Vela4
		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-6.0, 2, -20.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();*/

		glm::mat4 piso = glm::mat4(1.0f);
		piso = glm::translate(piso, glm::vec3(70, 0, 0));
		//piso = glm::rotate(piso, glm::radians(90.0f), glm::vec3(1.0, 0, 0.0));
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		piso1.render(glm::scale(piso, glm::vec3(20.0, 0.1, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pisoMed = glm::mat4(1.0f);
		pisoMed = glm::translate(piso, glm::vec3(0, 0, -10));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		piso1.render(glm::scale(pisoMed, glm::vec3(20.0, 0.1, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pisoTra = glm::mat4(1.0f);
		pisoTra = glm::translate(pisoMed, glm::vec3(0, 0, -10));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		piso1.render(glm::scale(pisoTra, glm::vec3(20.0, 0.1, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pasto = glm::mat4(1.0f);
		pasto = glm::translate(piso, glm::vec3(0, 0, 10));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDPasto);
		piso2.render(glm::scale(pasto, glm::vec3(20.0, 0.1, 10)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 patioTra = glm::mat4(1.0f);
		patioTra = glm::translate(pisoTra, glm::vec3(0, 0, -10));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDTierra);
		piso2.render(glm::scale(patioTra, glm::vec3(20.0, 0.1, 10)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 carretera = glm::mat4(1.0f);
		carretera = glm::translate(pasto, glm::vec3(0, 0, 10));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDCarretera);
		piso3.render(glm::scale(carretera, glm::vec3(25.0, 0.1, 5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTra1 = glm::translate(pisoTra, glm::vec3(-5, 2.55, -4.95));
		glBindTexture(GL_TEXTURE_2D, textureIDVentana);
		pared1.render(glm::scale(paredTra1, glm::vec3(10.0, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTra2 = glm::translate(pisoTra, glm::vec3(5, 2.55, -4.95));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTra2, glm::vec3(10.0, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraLado = glm::translate(pisoTra, glm::vec3(-9.95, 2.55, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTraLado, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 guirnalda = glm::translate(paredTraLado, glm::vec3(0.55, 1.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDPapel);
		papelPicado.render(glm::scale(guirnalda, glm::vec3(0.001, 1.5, 5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredIzq = glm::mat4(1.0f);
		paredIzq = glm::translate(piso, glm::vec3(-9.95, 2.55, 0));
		//paredIzq = glm::rotate(paredIzq, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID pared
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredIzq, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredDer = glm::mat4(1.0f);
		paredDer = glm::translate(piso, glm::vec3(9.95, 2.55, 0));
		//paredDer = glm::rotate(paredDer, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredDer, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredPasiIzq = glm::mat4(1.0f);
		paredPasiIzq = glm::translate(piso, glm::vec3(-1.25, 2.55, -10));
		//paredIzq = glm::rotate(paredIzq, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID pared
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredPasiIzq, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredPasiDer = glm::mat4(1.0f);
		paredPasiDer = glm::translate(piso, glm::vec3(1.25, 2.55, -10));
		//paredIzq = glm::rotate(paredIzq, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID pared
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredPasiDer, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertaPasiDer = glm::mat4(1.0f);
		puertaPasiDer = glm::translate(piso, glm::vec3(1.25, 2.05, -10));
		//paredIzq = glm::rotate(paredIzq, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID pared
		glBindTexture(GL_TEXTURE_2D, textureIDPuerta);
		pared1.render(glm::scale(puertaPasiDer, glm::vec3(0.11, 4, 2)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertaPasiIzq = glm::mat4(1.0f);
		puertaPasiIzq = glm::translate(piso, glm::vec3(-1.25, 2.05, -10));
		//paredIzq = glm::rotate(paredIzq, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID pared
		glBindTexture(GL_TEXTURE_2D, textureIDPuerta);
		pared1.render(glm::scale(puertaPasiIzq, glm::vec3(0.11, 4, 2)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraCuartoIzq = glm::mat4(1.0f);
		paredTraCuartoIzq = glm::translate(piso, glm::vec3(-5.6, 2.55, -15));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTraCuartoIzq, glm::vec3(8.75, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraCuartoDer = glm::mat4(1.0f);
		paredTraCuartoDer = glm::translate(piso, glm::vec3(5.6, 2.55, -15));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTraCuartoDer, glm::vec3(8.75, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredVentCuartoDer = glm::mat4(1.0f);
		paredVentCuartoDer = glm::translate(pisoMed, glm::vec3(9.95, 2.55, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDVentana);
		pared1.render(glm::scale(paredVentCuartoDer, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredVentCuartoIzq = glm::mat4(1.0f);
		paredVentCuartoIzq = glm::translate(pisoMed, glm::vec3(-9.95, 2.55, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDVentana);
		pared1.render(glm::scale(paredVentCuartoIzq, glm::vec3(0.1, 5, 10)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 tvCuartoDer = glm::mat4(1.0f);
		tvCuartoDer = glm::translate(paredTraCuartoDer, glm::vec3(0, 0.5, 0.105));
		glBindTexture(GL_TEXTURE_2D, textureIDTv);
		pared1.render(glm::scale(tvCuartoDer, glm::vec3(3.0, 2.0, 0.05)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		/*glm::mat4 escritorioCuartoDer = glm::mat4(1.0f);
		muebleCuartoDer = glm::translate(pisoMed, glm::vec3(9.5, 2.0, 2.55));
		glBindTexture(GL_TEXTURE_2D, textureIDMueble);
		mueble.render(glm::scale(muebleCuartoDer, glm::vec3(0.8, 4, 4.5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);*/

		glm::mat4 muebleCuartoDer = glm::mat4(1.0f);
		muebleCuartoDer = glm::translate(pisoMed, glm::vec3(9.5, 2.0, 2.55));
		glBindTexture(GL_TEXTURE_2D, textureIDMueble);
		mueble.render(glm::scale(muebleCuartoDer, glm::vec3(0.8, 4, 4.5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredPuertaBanio2 = glm::translate(pisoTra, glm::vec3(5.6, 2.55, -1.25));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredPuertaBanio2, glm::vec3(8.75, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertaBanio2 = glm::translate(paredPuertaBanio2, glm::vec3(0.0, -0.5, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureIDPuertaBanio);
		pared1.render(glm::scale(puertaBanio2, glm::vec3(1.85, 4, 0.11)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredBanio2 = glm::translate(pisoTra, glm::vec3(9.95, 2.55, -2.5));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredBanio2, glm::vec3(0.1, 5, 5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredBanio2Ventana = glm::translate(pisoTra, glm::vec3(9.95, 2.55, 2.5));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredBanio2Ventana, glm::vec3(0.1, 5, 5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredPasiBanio2 = glm::translate(pisoTra, glm::vec3(1.25, 2.55, 2.5));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredPasiBanio2, glm::vec3(0.1, 5, 7.5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pisoBanio2Tx = glm::translate(pisoTra, glm::vec3(5.625, 0.054, 2.0));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDPisoBanio);
		paredBanio.render(glm::scale(pisoBanio2Tx, glm::vec3(8.75, 0.001, 6.5)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraComedor = glm::mat4(1.0f);
		paredTraComedor = glm::translate(piso, glm::vec3(5.6, 2.55, -5));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTraComedor, glm::vec3(8.75, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraSala = glm::mat4(1.0f);
		paredTraSala = glm::translate(piso, glm::vec3(-5.6, 2.55, -5));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredTraSala, glm::vec3(8.75, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrenteComedor = glm::mat4(1.0f);
		paredFrenteComedor = glm::translate(piso, glm::vec3(6.25, 2.55, 0));
		//paredDer = glm::rotate(paredDer, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredFrenteComedor, glm::vec3(7.5, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrenteSala = glm::mat4(1.0f);
		paredFrenteSala = glm::translate(piso, glm::vec3(-6.25, 2.55, 1.2));
		//paredDer = glm::rotate(paredDer, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDParedInter);
		pared1.render(glm::scale(paredFrenteSala, glm::vec3(7.5, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrenteCocina = glm::mat4(1.0f);
		paredFrenteCocina = glm::translate(piso, glm::vec3(6.25, 2.55, 4.95));
		//paredDer = glm::rotate(paredDer, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredFrenteCocina, glm::vec3(7.5, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredPuertaBanio = glm::translate(piso, glm::vec3(-2.5, 2.55, 3.05));
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredPuertaBanio, glm::vec3(0.1, 5, 3.85)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrenteBanio = glm::mat4(1.0f);
		paredFrenteBanio = glm::translate(piso, glm::vec3(-6.25, 2.55, 4.95));
		//paredDer = glm::rotate(paredDer, glm::radians(90.0f), glm::vec3(0, 1, 0));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDPared);
		pared1.render(glm::scale(paredFrenteBanio, glm::vec3(7.5, 5, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertaBanio = glm::translate(piso, glm::vec3(-2.5, 2.05, 3.05));
		glBindTexture(GL_TEXTURE_2D, textureIDPuertaBanio);
		pared1.render(glm::scale(puertaBanio, glm::vec3(0.11, 4, 1.85)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredIzqBanioTx = glm::mat4(1.0f);
		paredIzqBanioTx = glm::translate(paredIzq, glm::vec3(0.06, 0, 3.05));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDAzulejos);
		paredBanio.render(glm::scale(paredIzqBanioTx, glm::vec3(0.001, 5, 3.85)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrenteBanioTx = glm::mat4(1.0f);
		paredFrenteBanioTx = glm::translate(paredFrenteBanio, glm::vec3(0, 0, -0.06));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDAzulejos);
		paredBanio.render(glm::scale(paredFrenteBanioTx, glm::vec3(7.5, 5, 0.001)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredTraBanioTx = glm::mat4(1.0f);
		paredTraBanioTx = glm::translate(paredFrenteSala, glm::vec3(0, 0, 0.06));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDAzulejos);
		paredBanio.render(glm::scale(paredTraBanioTx, glm::vec3(7.5, 5, 0.001)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pisoBanioTx = glm::mat4(1.0f);
		pisoBanioTx = glm::translate(piso, glm::vec3(-6.25, 0.052, 3.05));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDPisoAzulejos);
		paredBanio.render(glm::scale(pisoBanioTx, glm::vec3(7.5, 0.001, 3.75)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 techo = glm::mat4(1.0f);
		techo = glm::translate(piso, glm::vec3(0, 5, -10));
		// Usamos la textura ID 
		glBindTexture(GL_TEXTURE_2D, textureIDParedInter);
		piso3.render(glm::scale(techo, glm::vec3(20, 0.1, 30)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		//*********************VEla******************+++++

		/*glm::mat4 vela = glm::translate(piso, glm::vec3(0, 1,10));
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureIDVela);
		velas.render(glm::scale(vela, glm::vec3(0.25, 0.75, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);*/	

		// *******************************SOFA***************************************************

		glm::mat4 sofaTras = glm::translate(piso, glm::vec3(-5, 1.05, -4.7));
		//sofaTras = glm::rotate(sofaTras, rot0, glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofabk.render(glm::scale(sofaTras, glm::vec3(3.0, 2.0, 0.05)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaIzq = glm::translate(sofaTras, glm::vec3(-1.5, -0.3, 0.5));
		//sofaIzq = glm::rotate(sofaIzq, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		// Usamos la textura IDSofa
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofal.render(glm::scale(sofaIzq, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaDer = glm::translate(sofaTras, glm::vec3(1.5, -0.3, 0.5));
		//sofaDer = glm::rotate(sofaDer, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofar.render(glm::scale(sofaDer, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaCojin1 = glm::translate(sofaTras, glm::vec3(0.0, -0.7, 0.625));
		//sofaCojin1 = glm::rotate(sofaCojin1, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac1.render(glm::scale(sofaCojin1, glm::vec3(2.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaCojin2 = glm::translate(sofaTras, glm::vec3(0.0, -0.3, 0.625));
		//sofaCojin2 = glm::rotate(sofaCojin2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac2.render(glm::scale(sofaCojin2, glm::vec3(2.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaCojin3 = glm::translate(sofaTras, glm::vec3(-0.95, 0.4, 0.1));
		//sofaCojin3 = glm::rotate(sofaCojin3, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac3.render(glm::scale(sofaCojin3, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaCojin4 = glm::translate(sofaTras, glm::vec3(0, 0.4, 0.1));
		//sofaCojin4 = glm::rotate(sofaCojin4, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac4.render(glm::scale(sofaCojin4, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofaCojin5 = glm::translate(sofaTras, glm::vec3(0.95, 0.4, 0.1));
		//sofaCojin5 = glm::rotate(sofaCojin5, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac5.render(glm::scale(sofaCojin5, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		//Segundo sofa--------------------------------------------------------

		glm::mat4 sofa2Tras = glm::translate(piso, glm::vec3(-5, 1.05, 1));
		sofa2Tras = glm::rotate(sofa2Tras, glm::radians(180.0f), glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofabk.render(glm::scale(sofa2Tras, glm::vec3(2.0, 2.0, 0.05)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Izq = glm::translate(sofa2Tras, glm::vec3(-1, -0.3, 0.5));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofal.render(glm::scale(sofa2Izq, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Der = glm::translate(sofa2Tras, glm::vec3(1, -0.3, 0.5));
		//sofaDer = glm::rotate(sofaDer, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofar.render(glm::scale(sofa2Der, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Cojin1 = glm::translate(sofa2Tras, glm::vec3(0.0, -0.7, 0.625));
		//sofaCojin1 = glm::rotate(sofaCojin1, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac1.render(glm::scale(sofa2Cojin1, glm::vec3(1.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Cojin2 = glm::translate(sofa2Tras, glm::vec3(0.0, -0.3, 0.625));
		//sofaCojin2 = glm::rotate(sofaCojin2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac2.render(glm::scale(sofa2Cojin2, glm::vec3(1.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Cojin3 = glm::translate(sofa2Tras, glm::vec3(-0.45, 0.4, 0.15));
		//sofaCojin3 = glm::rotate(sofaCojin3, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac3.render(glm::scale(sofa2Cojin3, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa2Cojin4 = glm::translate(sofa2Tras, glm::vec3(0.45, 0.4, 0.15));
		//sofaCojin4 = glm::rotate(sofaCojin4, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac4.render(glm::scale(sofa2Cojin4, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		//----------------Tercer sofa-------------------------------------------

		glm::mat4 sofa3Tras = glm::translate(piso, glm::vec3(-1.2, 1.05, -1.5));
		sofa3Tras = glm::rotate(sofa3Tras, glm::radians(270.0f), glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofabk.render(glm::scale(sofa3Tras, glm::vec3(1, 2, 0.05)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa3Izq = glm::translate(sofa3Tras, glm::vec3(-0.5, -0.3, 0.5));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofal.render(glm::scale(sofa3Izq, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura             
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa3Der = glm::translate(sofa3Tras, glm::vec3(0.5, -0.3, 0.5));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofar.render(glm::scale(sofa3Der, glm::vec3(0.05, 1.4, 1.0)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa3Cojin1 = glm::translate(sofa3Tras, glm::vec3(0.0, -0.3, 0.625));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac1.render(glm::scale(sofa3Cojin1, glm::vec3(0.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa3Cojin2 = glm::translate(sofa3Tras, glm::vec3(0.0, -0.7, 0.625));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac2.render(glm::scale(sofa3Cojin2, glm::vec3(0.9, 0.4, 1.2)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 sofa3Cojin3 = glm::translate(sofa3Tras, glm::vec3(0, 0.4, 0.125));
		//sofaCojin4 = glm::rotate(sofaCojin4, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		sofac4.render(glm::scale(sofa3Cojin3, glm::vec3(0.9, 1, 0.25)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		//--------------------------------------MESA-------------------------------------------------

		glm::mat4 mesa1 = glm::translate(piso, glm::vec3(5, 1.5, -2.5));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		mesa.render(glm::scale(mesa1, glm::vec3(3, 0.08, 1.5)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1 = glm::translate(mesa1, glm::vec3(-1.45, -0.75, -0.7));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pata.render(glm::scale(pata1, glm::vec3(0.15, 1.42, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2 = glm::translate(mesa1, glm::vec3(-1.45, -0.75, 0.7));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pata.render(glm::scale(pata2, glm::vec3(0.15, 1.42, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3 = glm::translate(mesa1, glm::vec3(1.45, -0.75, -0.7));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pata.render(glm::scale(pata3, glm::vec3(0.15, 1.42, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4 = glm::translate(mesa1, glm::vec3(1.45, -0.75, 0.7));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pata.render(glm::scale(pata4, glm::vec3(0.15, 1.42, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		//-----------------------------------SILLAS---------------------------------------------

		glm::mat4 asientoSilla1 = glm::translate(mesa1, glm::vec3(0.75, -0.6, -1.5));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		asientoSilla.render(glm::scale(asientoSilla1, glm::vec3(1, 0.1, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1Silla1 = glm::translate(asientoSilla1, glm::vec3(0.5, -0.45, 0.5));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pataSilla.render(glm::scale(pata1Silla1, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2Silla1 = glm::translate(asientoSilla1, glm::vec3(-0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata2Silla1, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3Silla1 = glm::translate(asientoSilla1, glm::vec3(0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata3Silla1, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4Silla1 = glm::translate(asientoSilla1, glm::vec3(-0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata4Silla1, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 respaldoSilla1 = glm::translate(asientoSilla1, glm::vec3(0.0, 0.675, -0.45));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		respaldoSilla.render(glm::scale(respaldoSilla1, glm::vec3(1, 1.25, 0.075)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 asientoSilla2 = glm::translate(mesa1, glm::vec3(-0.75, -0.6, -1.5));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		asientoSilla.render(glm::scale(asientoSilla2, glm::vec3(1, 0.1, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1Silla2 = glm::translate(asientoSilla2, glm::vec3(0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata1Silla2, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2Silla2 = glm::translate(asientoSilla2, glm::vec3(-0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata2Silla2, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3Silla2 = glm::translate(asientoSilla2, glm::vec3(0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata3Silla2, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4Silla2 = glm::translate(asientoSilla2, glm::vec3(-0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata4Silla2, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 respaldoSilla2 = glm::translate(asientoSilla2, glm::vec3(0.0, 0.675, -0.45));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		respaldoSilla.render(glm::scale(respaldoSilla2, glm::vec3(1, 1.25, 0.075)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		glm::mat4 asientoSilla3 = glm::translate(mesa1, glm::vec3(0.75, -0.6, 1.5));
		asientoSilla3 = glm::rotate(asientoSilla3, glm::radians(180.0f), glm::vec3(0, 1.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		asientoSilla.render(glm::scale(asientoSilla3, glm::vec3(1, 0.1, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1Silla3 = glm::translate(asientoSilla3, glm::vec3(0.5, -0.45, 0.5));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pataSilla.render(glm::scale(pata1Silla3, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2Silla3 = glm::translate(asientoSilla3, glm::vec3(-0.5, -0.45, 0.5));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		pataSilla.render(glm::scale(pata2Silla3, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3Silla3 = glm::translate(asientoSilla3, glm::vec3(0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata3Silla3, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4Silla3 = glm::translate(asientoSilla3, glm::vec3(-0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata4Silla3, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 respaldoSilla3 = glm::translate(asientoSilla3, glm::vec3(0.0, 0.675, -0.45));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		respaldoSilla.render(glm::scale(respaldoSilla3, glm::vec3(1, 1.25, 0.075)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		glm::mat4 asientoSilla4 = glm::translate(mesa1, glm::vec3(-0.75, -0.6, 1.5));
		asientoSilla4 = glm::rotate(asientoSilla4, glm::radians(180.0f), glm::vec3(0, 1.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		asientoSilla.render(glm::scale(asientoSilla4, glm::vec3(1, 0.1, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1Silla4 = glm::translate(asientoSilla4, glm::vec3(0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata1Silla4, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2Silla4 = glm::translate(asientoSilla4, glm::vec3(-0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata2Silla4, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3Silla4 = glm::translate(asientoSilla4, glm::vec3(0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata3Silla4, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4Silla4 = glm::translate(asientoSilla4, glm::vec3(-0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata4Silla4, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 respaldoSilla4 = glm::translate(asientoSilla4, glm::vec3(0.0, 0.675, -0.45));
		// Usamos la textura ID 1
		//glBindTexture(GL_TEXTURE_2D, textureID);
		respaldoSilla.render(glm::scale(respaldoSilla4, glm::vec3(1, 1.25, 0.075)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		glm::mat4 asientoSilla5 = glm::translate(mesa1, glm::vec3(2.2, -0.6, 0));
		asientoSilla5 = glm::rotate(asientoSilla5, glm::radians(270.0f), glm::vec3(0, 1.0, 0.0));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		asientoSilla.render(glm::scale(asientoSilla5, glm::vec3(1, 0.1, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata1Silla5 = glm::translate(asientoSilla5, glm::vec3(0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata1Silla5, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata2Silla5 = glm::translate(asientoSilla5, glm::vec3(-0.5, -0.45, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata2Silla5, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata3Silla5 = glm::translate(asientoSilla5, glm::vec3(0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata3Silla5, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pata4Silla5 = glm::translate(asientoSilla5, glm::vec3(-0.5, -0.45, -0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID);
		pataSilla.render(glm::scale(pata4Silla5, glm::vec3(0.15, 0.85, 0.15)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 respaldoSilla5 = glm::translate(asientoSilla5, glm::vec3(0.0, 0.675, -0.45));
		glBindTexture(GL_TEXTURE_2D, textureIDSofa);
		respaldoSilla.render(glm::scale(respaldoSilla5, glm::vec3(1, 1.25, 0.075)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);


		//------------------------------------------Cocina-------------------------------------------

		glm::mat4 muebleCocina1 = glm::translate(paredFrenteComedor, glm::vec3(-2, -1.5, 0.6));
		muebleCocina1 = glm::rotate(muebleCocina1, glm::radians(180.0f), glm::vec3(0, 1.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureIDCocina);
		muebleCocina.render(glm::scale(muebleCocina1, glm::vec3(3, 2, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 estufa = glm::translate(paredFrenteComedor, glm::vec3(0.5, -1.5, 0.6));
		glBindTexture(GL_TEXTURE_2D, textureIDEstufa);
		muebleCocina.render(glm::scale(estufa, glm::vec3(2, 2, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 estufaSup = glm::translate(estufa, glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDEstufaSup);
		muebleCocina.render(glm::scale(estufaSup, glm::vec3(2, 0.01, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 muebleCocina2 = glm::translate(paredFrenteComedor, glm::vec3(2.5, -1.5, 0.6));
		glBindTexture(GL_TEXTURE_2D, textureIDCocina);
		muebleCocina.render(glm::scale(muebleCocina2, glm::vec3(2, 2, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 muebleCocina3 = glm::translate(paredFrenteComedor, glm::vec3(-1, -1.5, 4.4));
		glBindTexture(GL_TEXTURE_2D, textureIDCocina);
		muebleCocina.render(glm::scale(muebleCocina3, glm::vec3(3, 2, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 refrigerador = glm::translate(paredFrenteComedor, glm::vec3(1.5, -0.5, 4.4));
		glBindTexture(GL_TEXTURE_2D, textureIDRefri);
		muebleCocina.render(glm::scale(refrigerador, glm::vec3(2, 4, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 refrigeradorSup = glm::translate(refrigerador, glm::vec3(0, 2, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDRefriSup);
		muebleCocina.render(glm::scale(refrigeradorSup, glm::vec3(2, 0.01, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 refrigeradorDer = glm::translate(refrigerador, glm::vec3(1, 0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDRefriSup);
		muebleCocina.render(glm::scale(refrigeradorDer, glm::vec3(0.01, 4, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 refrigeradorIzq = glm::translate(refrigerador, glm::vec3(-1, 0, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDRefriSup);
		muebleCocina.render(glm::scale(refrigeradorIzq, glm::vec3(0.01, 4, 1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 matrixModelMueble = glm::mat4(1.0);
		matrixModelMueble = glm::translate(piso, glm::vec3(-8.0, 0.2, -4.0));
		modelMueble.render(glm::scale(matrixModelMueble, glm::vec3(2, 2, 2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToilet = glm::mat4(1.0);
		matrixModelToilet = glm::translate(piso, glm::vec3(-8.0, 0.2, 3.5));
		modelToilet.render(glm::scale(matrixModelToilet, glm::vec3(0.5, 0.5, 0.5)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToilet2 = glm::mat4(1.0);
		matrixModelToilet2 = glm::translate(pisoTra, glm::vec3(8.5, 0.2, 3.5));
		matrixModelToilet2 = glm::rotate(matrixModelToilet2, glm::radians(180.0f), glm::vec3(0, 1.0, 0));
		modelToilet.render(glm::scale(matrixModelToilet2, glm::vec3(0.5, 0.5, 0.5)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelCama = glm::mat4(1.0);
		matrixModelCama = glm::translate(pisoMed, glm::vec3(-5.25, 0.05, 2.5));
		matrixModelCama = glm::rotate(matrixModelCama, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		matrixModelCama = glm::rotate(matrixModelCama, glm::radians(270.0f), glm::vec3(1.0, 0.0, 0));
		modelCama.render(glm::scale(matrixModelCama, glm::vec3(0.0018, 0.0018, 0.0018)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelCama2 = glm::mat4(1.0);
		matrixModelCama2 = glm::translate(pisoMed, glm::vec3(8.0, 0.05, 0.25));
		matrixModelCama2 = glm::rotate(matrixModelCama2, glm::radians(180.0f), glm::vec3(0, 1.0, 0.0));
		matrixModelCama2 = glm::rotate(matrixModelCama2, glm::radians(270.0f), glm::vec3(1.0, 0.0, 0));
		modelCama.render(glm::scale(matrixModelCama2, glm::vec3(0.0018, 0.0018, 0.0018)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelLaptop = glm::mat4(1.0);
		matrixModelLaptop = glm::translate(pisoMed, glm::vec3(8.5, 0.2, -2));
		//matrixModelLaptop = glm::rotate(matrixModelLaptop, glm::radians(180.0f), glm::vec3(0, 1.0, 0));
		modelLaptop.render(glm::scale(matrixModelLaptop, glm::vec3(0.25, 0.25, 0.25)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelRegadera = glm::mat4(1.0);
		matrixModelRegadera = glm::translate(pisoTra, glm::vec3(8.5, 0.2, 0));
		modelRegadera.render(glm::scale(matrixModelRegadera, glm::vec3(0.5, 0.5, 0.5)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);*/

		glm::mat4 matrixModelCajon = glm::mat4(1.0);
		matrixModelCajon = glm::translate(pisoMed, glm::vec3(-8.0, 0.2, 3.5));
		modelCajon.render(glm::scale(matrixModelCajon, glm::vec3(0.002, 0.002, 0.002)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelMesaOfrenda = glm::mat4(1.0);
		matrixModelMesaOfrenda = glm::translate(pisoTra, glm::vec3(-7.5, 0.0, 0.0));
		modelMesaOfrenda.render(glm::scale(matrixModelMesaOfrenda, glm::vec3(0.08, 0.08, 0.08)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 papelOfrenda = glm::translate(matrixModelMesaOfrenda, glm::vec3(0.0, 1.6, 0));
		glBindTexture(GL_TEXTURE_2D, textureIDPapelPicado);
		papelPicado.render(glm::scale(papelOfrenda, glm::vec3(5, 0.001, 4)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		/*glm::mat4 matrixModelVela = glm::mat4(1.0);
		matrixModelVela = glm::translate(matrixModelMesaOfrenda, glm::vec3(0, 1.6, 0));
		modelVela.render(glm::scale(matrixModelVela, glm::vec3(0.05, 0.05, 0.05)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);*/

		glm::mat4 matrixModelVela2 = glm::mat4(1.0);
		matrixModelVela2 = glm::translate(matrixModelMesaOfrenda, glm::vec3(0, 1.6, 1.5));
		modelVela.render(glm::scale(matrixModelVela2, glm::vec3(0.05, 0.05, 0.05)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelVela3 = glm::mat4(1.0);
		matrixModelVela3 = glm::translate(matrixModelMesaOfrenda, glm::vec3(0, 1.6, -1.5));
		modelVela.render(glm::scale(matrixModelVela3, glm::vec3(0.05, 0.05, 0.05)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelVela4 = glm::mat4(1.0);
		matrixModelVela4 = glm::translate(matrixModelMesaOfrenda, glm::vec3(1, 1.6, 0));
		modelVela.render(glm::scale(matrixModelVela4, glm::vec3(0.05, 0.05, 0.05)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);*/


		glm::mat4 matrixModelZombies = glm::mat4(1.0);
		matrixModelZombies = glm::translate(patioTra, glm::vec3(0, 0.0, -2.5));
		matrixModelZombies = glm::rotate(matrixModelZombies, glm::radians(270.0f), glm::vec3(0.0, 1.0, 0));
		modelZombie.render(glm::scale(matrixModelZombies, glm::vec3(0.2, 0.2, 0.2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelZombies2 = glm::mat4(1.0);
		matrixModelZombies2 = glm::translate(patioTra, glm::vec3(-2, 0.0, 1.5));
		matrixModelZombies2 = glm::rotate(matrixModelZombies2, glm::radians(270.0f), glm::vec3(0.0, 1.0, 0));
		modelZombie.render(glm::scale(matrixModelZombies2, glm::vec3(0.2, 0.2, 0.2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelZombies3 = glm::mat4(1.0);
		matrixModelZombies3 = glm::translate(patioTra, glm::vec3(-5, 0.0, 2.5));
		modelZombie.render(glm::scale(matrixModelZombies3, glm::vec3(0.2, 0.2, 0.2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelZombies4 = glm::mat4(1.0);
		matrixModelZombies4 = glm::translate(pasto, glm::vec3(5, 0.0, 0.5));
		modelZombie.render(glm::scale(matrixModelZombies4, glm::vec3(0.2, 0.2, 0.2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelZombies5 = glm::mat4(1.0);
		matrixModelZombies5 = glm::translate(pasto, glm::vec3(-3, 0.0, 1.5));
		matrixModelZombies5 = glm::rotate(matrixModelZombies5, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0));
		modelZombie.render(glm::scale(matrixModelZombies5, glm::vec3(0.2, 0.2, 0.2)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);


		glm::mat4 matrixModelLich = glm::mat4(1.0);
		matrixModelLich = glm::translate(pisoTra, glm::vec3(0, 1.75, 0));
		matrixModelLich = glm::rotate(matrixModelLich, glm::radians(270.0f), glm::vec3(0.0, 1.0, 0));
		modelLich.render(glm::scale(matrixModelLich, glm::vec3(1, 1, 1)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelLich2 = glm::mat4(1.0);
		matrixModelLich2 = glm::translate(pisoTra, glm::vec3(-6, 1.75, -3));
		modelLich.render(glm::scale(matrixModelLich2, glm::vec3(1, 1, 1)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone = glm::mat4(1.0);
		matrixModelGravestone = glm::translate(pisoTra, glm::vec3(-6, 0, 3));
		matrixModelGravestone = glm::rotate(matrixModelGravestone, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelGravestone.render(glm::scale(matrixModelGravestone, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone2 = glm::mat4(1.0);
		matrixModelGravestone2 = glm::translate(patioTra, glm::vec3(-7, 0, -0.5));
		matrixModelGravestone2 = glm::rotate(matrixModelGravestone2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelGravestone.render(glm::scale(matrixModelGravestone2, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone3 = glm::mat4(1.0);
		matrixModelGravestone3 = glm::translate(patioTra, glm::vec3(-7, 0, -3.5));
		matrixModelGravestone3 = glm::rotate(matrixModelGravestone3, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelGravestone.render(glm::scale(matrixModelGravestone3, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone4 = glm::mat4(1.0);
		matrixModelGravestone4 = glm::translate(patioTra, glm::vec3(-1, 0, -2));
		modelGravestone.render(glm::scale(matrixModelGravestone4, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone5 = glm::mat4(1.0);
		matrixModelGravestone5 = glm::translate(patioTra, glm::vec3(7, 0, -3.5));
		modelGravestone.render(glm::scale(matrixModelGravestone5, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelGravestone6 = glm::mat4(1.0);
		matrixModelGravestone6 = glm::translate(patioTra, glm::vec3(3, 0, -1));
		matrixModelGravestone6 = glm::rotate(matrixModelGravestone6, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelGravestone.render(glm::scale(matrixModelGravestone6, glm::vec3(3, 3, 3)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelPumpkin = glm::mat4(1.0);
		matrixModelPumpkin = glm::translate(matrixModelMesaOfrenda, glm::vec3(1, 1.6, -1));
		matrixModelPumpkin = glm::rotate(matrixModelPumpkin, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelPumpkin.render(glm::scale(matrixModelPumpkin, glm::vec3(1.4, 1.4, 1.4)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelPumpkin2 = glm::mat4(1.0);
		matrixModelPumpkin2 = glm::translate(matrixModelMesaOfrenda, glm::vec3(1, 1.6, 1));
		matrixModelPumpkin2 = glm::rotate(matrixModelPumpkin2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelPumpkin.render(glm::scale(matrixModelPumpkin2, glm::vec3(1.4, 1.4, 1.4)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelPumpkin3 = glm::mat4(1.0);
		matrixModelPumpkin3 = glm::translate(pasto, glm::vec3(3, 0, -4));
		matrixModelPumpkin3 = glm::rotate(matrixModelPumpkin3, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelPumpkin.render(glm::scale(matrixModelPumpkin3, glm::vec3(1.4, 1.4, 1.4)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelPumpkin4 = glm::mat4(1.0);
		matrixModelPumpkin4 = glm::translate(pasto, glm::vec3(-3, 0, -4));
		matrixModelPumpkin4 = glm::rotate(matrixModelPumpkin4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		modelPumpkin.render(glm::scale(matrixModelPumpkin4, glm::vec3(1.4, 1.4, 1.4)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);


		glm::mat4 matrixModelSkull = glm::mat4(1.0);
		matrixModelSkull = glm::translate(matrixModelMesaOfrenda, glm::vec3(1, 1.5, 0));
		matrixModelSkull = glm::rotate(matrixModelSkull, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0));
		modelSkull.render(glm::scale(matrixModelSkull, glm::vec3(0.15, 0.15, 0.15)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelSkull2 = glm::mat4(1.0);
		matrixModelSkull2 = glm::translate(patioTra, glm::vec3(-5, 0, -3));
		matrixModelSkull2 = glm::rotate(matrixModelSkull2, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0));
		modelSkull.render(glm::scale(matrixModelSkull2, glm::vec3(0.15, 0.15, 0.15)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelPanMuerto = glm::mat4(1.0);
		matrixModelPanMuerto = glm::translate(matrixModelMesaOfrenda, glm::vec3(0, 1.65, 0));
		//matrixModelPanMuerto = glm::rotate(matrixModelPanMuerto, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0));
		modelPanMuerto.render(glm::scale(matrixModelPanMuerto, glm::vec3(0.5, 0.5, 0.5)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelBanana = glm::mat4(1.0);
		matrixModelBanana = glm::translate(matrixModelMesaOfrenda, glm::vec3(0, 1.65, 1));
		//matrixModelPanMuerto = glm::rotate(matrixModBananaelPanMuerto, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0));
		modelBanana.render(glm::scale(matrixModelBanana, glm::vec3(0.004, 0.004, 0.004)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelBanio = glm::mat4(1.0);
		matrixModelBanio = glm::translate(pisoTra, glm::vec3(5.5, 0.2, 3));
		matrixModelBanio = glm::rotate(matrixModelBanio, glm::radians(180.0f), glm::vec3(0, 1.0, 0));
		modelBanio.render(glm::scale(matrixModelBanio, glm::vec3(0.5, 0.5, 0.5)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);*/
		
		modelEclipse.render(glm::scale(matrixModelEclipse, glm::vec3(0.5, 0.5, 0.5)));
		glActiveTexture(GL_TEXTURE0);
		
		modelHeli.render(glm::scale(matrixModelHeli, glm::vec3(1.0, 1.0, 1.0)));
		glActiveTexture(GL_TEXTURE0);

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		/*switch (state) {
		case 0:
			std::cout << "Advance:" << std::endl;
			// -0.01 debe ser igual en magnitud 
			matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(0.0, 0.0, -0.01));
			offsetAircraftAdvance += -0.01;
			if (offsetAircraftAdvance < -2.0) {
				offsetAircraftAdvance = 0.0;
				state = 1;
			}
		case 1:
			std::cout << "Turn:" << std::endl;
			matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(0, 0, -0.01));
			matrixModelAircraft = glm::rotate(matrixModelAircraft, glm::radians(0.05f), glm::vec3(0.0, 1.0, 0.0));
			offsetAircraftRot += 0.05;
			if (offsetAircraftRot > 90.0) {
				offsetAircraftRot = 0.0;
				state = 0;
			}
			break;
		}*/

		//-------------------------------------MOVIMIENTO DE LICH ---------------------------------------------------

		switch (state) {
		case 0:
			std::cout << "Sube:" << std::endl;
			matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(0.0, 0.0, 0.1));
			offsetAircraftAdvance += 0.1;
			if (offsetAircraftAdvance > 10.0) {
				offsetAircraftAdvance = 0.0;
				state = 1;
			}
			break;
		case 1:
			std::cout << "Baja:" << std::endl;
			matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(0.0, 0.0, -0.1));
			offsetAircraftAdvance += -0.1;
			if (offsetAircraftAdvance < -10.0) {
				offsetAircraftAdvance = 0.0;
				state = 0;
			}
			break;
		}

		//***********************************MOVIMIENTO DEL HELICOPTERO********************************+++++

		switch (state2) {
		case 0:
			std::cout << "Sube:" << std::endl; 
			matrixModelHeli = glm::translate(matrixModelHeli, glm::vec3(0.0, 0.1, 0.0));
			offsetHeliAdvance += 0.1;
			if (offsetHeliAdvance > 12.0) {
				offsetHeliAdvance = 0.0;
				state2 = 1;
			}
			break;
		case 1:
			std::cout << "Baja:" << std::endl;
			matrixModelHeli = glm::translate(matrixModelHeli, glm::vec3(0.0, -0.1, 0.0));
			offsetHeliAdvance += -0.1;
			if (offsetHeliAdvance < -12.0){ 
				offsetHeliAdvance = 0.0;
				state2 = 0;
			}
			break;
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
