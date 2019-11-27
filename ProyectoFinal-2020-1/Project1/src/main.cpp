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

//include loader model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

bool cameraSel = 0;

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
std::shared_ptr<FirstPersonCamera> camera2(new FirstPersonCamera());

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

Box techo;

Box boxMaterials;
Box box1;
Box box2;

Box cuadro;
Box regalo;
Box regalo2;

Box rosca;
Box guirnalda;
Box nacimiento;

//SOFA

Box sofa;
Box sofa2;
Box sofa3;
Box sofa4;
Box sofa5;

// COMEDOR 

// MESA
Box mesa1;
Box mesa2;

//Sillas

//

// pista

Box pista;
Box helipuerto;


//CASA

Box box3; // suelo


Box pared1;  //Pared iazquierda habitacion
Box pared2; //pared fondo habutacion 
Box paredFondoComedor;
Box paredFondo;
Box paredFondoCocina;
Box Cocina;
Box Superior;
Box pared3; // pared DERECHA habitacion
Box pared4; // pared frontal  habitacion
Box pared5;// pared externa 
Box pared6; // pared DERECHA EXTERNA
Box paredComedor;
Box paredBanio, piso1, piso2, piso3, techo1;
Box papelPicado;
//Para los sofas
Box sofabk, sofac1, sofac2, sofac3, sofac4, sofac5, sofal, sofar;
//Para muebles
Box mueble, escritorio;
//Para mesa y muebles 
Box mesa, asientoSilla, respaldoSilla, mueble1Sup, mueble1Inf, muebleCocina;

//JARDIN CASA

Box sueloJardin;
Box paredJardinExterna;
Box sueloAlberca;


//COCINA

Box paredCocinaExterna;

Model modelEclipse;
Model modelHeli;
Model modelMueble, modelToilet, modelBanio, modelRegadera, modelVela;
Model modelCama, modelLaptop, modelCajon, modelMesaOfrenda, modelLich, modelGravestone, modelZombie,
modelSkull, modelPumpkin, modelPanMuerto, modelBanana;




// Models complex instances
Model modelRock;
Model modelRailRoad;
Model modelAircraft;

Model modelMesa;

//HABITACION

Model modelBuro;

Model autoEclipse;
Model modelHelicoptero;

//JUGUETE CARRO 

Model modelEclipseChasis;
Model modelEclipseFrontalWheel;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;

// juguete dart

Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

//SALA

Model modelTV;
Model modelBuroSala;
Model planta;
Model planta2;
Model planta3;
Model modelTree;
Model horno;

// habitacion

Model modelTorreRegalos;


glm::mat4 view;

GLuint textureID0, textureID1A, textureID2A, textureID3A, textureID4A, textureID5A, textureID6A;
GLuint textureID7A, textureID8A, textureID9A, textureID10A, textureID11A, textureID12A, textureID13A;
GLuint textureID14A;
GLuint textureID15A, textureID16A, textureID17A, textureID18A, textureID19A, textureID20A, textureID21A;
GLuint skyboxTextureID;
GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureIDCocina, textureIDPared, textureIDParedInter,
textureIDEstufa, textureIDEstufaSup, textureIDRefri, textureIDRefriSup, textureIDAzulejos, textureIDPasto, textureIDSofa,
textureIDCarretera, textureIDPuerta, textureIDPuertaBanio, textureIDPisoAzulejos, textureIDTv, textureIDVela,
textureIDMueble, textureIDVentana, textureIDPisoBanio, textureIDTierra, textureIDPapel, textureIDPapelPicado;

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_sandcastle/sandcastle_ft.tga",
"../Textures/mp_sandcastle/sandcastle_bk.tga",
"../Textures/mp_sandcastle/sandcastle_up.tga",
"../Textures/mp_sandcastle/sandcastle_dn.tga",
"../Textures/mp_sandcastle/sandcastle_rt.tga",
"../Textures/mp_sandcastle/sandcastle_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

// Descomentar
float rotDartHead = 0.0, rotDartBody = 0.0, advanceDartBody = 0.0, rotDartLeftArm = 0.0,
rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
// Descomentar
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
// Descomentar
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 40;
int numPasosDartJoints = 0;

int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;



double deltaTime;
double currTime, lastTime;


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
	cylinder2.setShader(&shaderMulLighting); //este solo trabaja con texturas, no color

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



	pista.init();
	pista.setShader(&shaderMulLighting);

	helipuerto.init();
	helipuerto.setShader(&shaderMulLighting);

	sueloJardin.init();
	sueloJardin.setShader(&shaderMulLighting);

	sueloAlberca.init();
	sueloAlberca.setShader(&shaderMulLighting);


	pared1.init();
	pared1.setShader(&shaderMulLighting);

	techo1.init();
	techo1.setShader(&shaderMulLighting);


	escritorio.init();
	escritorio.setShader(&shaderMulLighting);


	papelPicado.init();
	papelPicado.setShader(&shaderMulLighting);


	pared2.init();
	pared2.setShader(&shaderMulLighting);

	paredFondoComedor.init();
	paredFondoComedor.setShader(&shaderMulLighting);

	guirnalda.init();
	guirnalda.setShader(&shaderMulLighting);

	nacimiento.init();
	nacimiento.setShader(&shaderMulLighting);

	paredFondo.init();
	paredFondo.setShader(&shaderMulLighting);


	paredFondoCocina.init();
	paredFondoCocina.setShader(&shaderMulLighting);


	Cocina.init();
	Cocina.setShader(&shaderMulLighting);


	Superior.init();
	Superior.setShader(&shaderMulLighting);


	pared3.init();
	pared3.setShader(&shaderMulLighting);

	pared4.init();
	pared4.setShader(&shaderMulLighting);


	pared5.init();
	pared5.setShader(&shaderMulLighting);


	pared6.init();
	pared6.setShader(&shaderMulLighting);


	paredComedor.init();
	paredComedor.setShader(&shaderMulLighting);
	

	cuadro.init();
	cuadro.setShader(&shaderMulLighting);

	regalo.init();
	regalo.setShader(&shaderMulLighting);

	regalo2.init();
	regalo2.setShader(&shaderMulLighting);

	rosca.init();
	rosca.setShader(&shaderMulLighting);

	paredCocinaExterna.init();
	paredCocinaExterna.setShader(&shaderMulLighting);

	paredJardinExterna.init();
	paredJardinExterna.setShader(&shaderMulLighting);

	// sOFA

	sofa.init();
	sofa.setShader(&shaderMulLighting);

	sofa2.init();
	sofa2.setShader(&shaderMulLighting);

	sofa3.init();
	sofa3.setShader(&shaderMulLighting);


	sofa4.init();
	sofa4.setShader(&shaderMulLighting);

	sofa5.init();
	sofa5.setShader(&shaderMulLighting);

	mesa1.init();
	mesa1.setShader(&shaderMulLighting);

	mesa2.init();
	mesa2.setShader(&shaderMulLighting);

	//Halloween

	pared1.init();
	// Settea el shader a utilizar
	pared1.setShader(&shaderMulLighting);

	paredBanio.init();
	// Settea el shader a utilizar
	paredBanio.setShader(&shaderMulLighting);

	piso1.init();
	// Settea el shader a utilizar
	piso1.setShader(&shaderMulLighting);

	piso2.init();
	// Settea el shader a utilizar
	piso2.setShader(&shaderMulLighting);

	piso3.init();
	// Settea el shader a utilizar
	piso3.setShader(&shaderMulLighting);

	//Mueble
	mueble.init();
	// Settea el shader a utilizar
	mueble.setShader(&shaderMulLighting);


	//Mueble
	mueble1Sup.init();
	// Settea el shader a utilizar
	mueble1Sup.setShader(&shaderMulLighting);


	//Mueble
	mueble1Inf.init();
	// Settea el shader a utilizar
	mueble1Inf.setShader(&shaderMulLighting);

	//sofa1
	sofabk.init();
	// Settea el shader a utilizar
	sofabk.setShader(&shaderMulLighting);

	sofac1.init();
	// Settea el shader a utilizar
	sofac1.setShader(&shaderMulLighting);

	sofac2.init();
	// Settea el shader a utilizar
	sofac2.setShader(&shaderMulLighting);

	sofac3.init();
	// Settea el shader a utilizar
	sofac3.setShader(&shaderMulLighting);

	sofac4.init();
	// Settea el shader a utilizar
	sofac4.setShader(&shaderMulLighting);


	sofac5.init();
	// Settea el shader a utilizar
	sofac5.setShader(&shaderMulLighting);

	sofar.init();
	// Settea el shader a utilizar
	sofar.setShader(&shaderMulLighting);

	sofal.init();
	// Settea el shader a utilizar
	sofal.setShader(&shaderMulLighting);


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

	muebleCocina.init();
	muebleCocina.setShader(&shaderMulLighting);



	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/railroad/railroad_track.obj");
	modelRock.setShader(&shaderMulLighting);

	modelCama.loadModel("../models/bed/Full_Size_Bed_with_White_Sheets_Black_V1.obj");
	modelCama.setShader(&shaderMulLighting);

	modelHelicoptero.loadModel("../models/Helicopter/Mi_24.obj");
	modelHelicoptero.setShader(&shaderMulLighting);

	autoEclipse.loadModel("../models/Eclipse/2003eclipse.obj");
	autoEclipse.setShader(&shaderMulLighting);

	modelMesa.loadModel("../models/Wood_Table/Wood_Table.obj");
	modelMesa.setShader(&shaderMulLighting);

	modelBuro.loadModel("../models/buro/Nightstand.obj");
	modelBuro.setShader(&shaderMulLighting);

	modelTorreRegalos.loadModel("../models/E/13495_Stack_of_Gifts_v2_L2.obj");
	modelTorreRegalos.setShader(&shaderMulLighting);


	//JUGUETE CARRO 

	// Eclipse

	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);


	// lego de juguete

		//DARt LEGO

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);



	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	modelTree.loadModel("../models/Tree/Christmas Tree.obj");
	modelTree.setShader(&shaderMulLighting);

	techo.init();
	techo.setShader(&shaderMulLighting);


	//SALA

	//modelTV.loadModel("../models/tv/OBJ/Samsung LED TV.obj");//9
	//modelHelicopter.loadModel("../models/tv/cocacola/mpm_vol.09_p35.obj");//9
	//modelTV.setShader(&shaderMulLighting);//9

	modelBuroSala.loadModel("../models/buro/Nightstand.obj");
	modelBuroSala.setShader(&shaderMulLighting);

	planta.loadModel("../models/Indoor plant 3/Low-Poly Plant_.obj");
	planta.setShader(&shaderMulLighting);

	planta2.loadModel("../models/Indoor plant 3/Low-Poly Plant_.obj");
	planta2.setShader(&shaderMulLighting);

	planta3.loadModel("../models/Indoor plant 3/Low-Poly Plant_.obj");
	planta3.setShader(&shaderMulLighting);

	//HALLOWEEEN**********************************************+

	modelEclipse.loadModel("../models/Eclipse/2003eclipse.obj");
	modelEclipse.setShader(&shaderMulLighting);

	modelHeli.loadModel("../models/Helicopter/Mi_24.obj");
	modelHeli.setShader(&shaderMulLighting);

	modelMueble.loadModel("../models/Wood_Table/Wood_Table.obj");
	modelMueble.setShader(&shaderMulLighting);

	modelToilet.loadModel("../models/Toilet/toilet.obj");
	modelToilet.setShader(&shaderMulLighting);

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




	camera->setPosition(glm::vec3(-5.0, 0.0, 2.0));
	camera2->setPosition(glm::vec3(70, 2.0, -1));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;



	// Definiendo la texturaS a utilizar



	Texture texture1A("../Textures/closet.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1A.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1A.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1A);
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
	texture1A.freeImage(bitmap);


	///////////////N TECHO /////////////////

	// Definiendo la textura a utilizar
	Texture texture0("../Textures/tejas.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture0.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture0.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID0);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID0);
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
	texture0.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2A("../Textures/madera-de-pino.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2A.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2A);
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
	texture2A.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture5A("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture5A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID5A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID5A);
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
	texture5A.freeImage(bitmap);



	// Definiendo la textura a utilizar
	Texture texture3A("../Textures/water2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3A);
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
	texture3A.freeImage(bitmap);

	//textura de pared jardin


	// Definiendo la textura a utilizar
	Texture texture6A("../Textures/piedra.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture6A.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture6A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID6A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID6A);
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
	texture6A.freeImage(bitmap);

	//pared de marmol


	// Definiendo la textura a utilizar
	Texture texture4A("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4A);
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
	texture4A.freeImage(bitmap);

	//Tectura del cuadro CORONA


	// Definiendo la textura a utilizar
	Texture texture15A("../Textures/corona.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture15A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture15A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID15A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID15A);
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
	texture15A.freeImage(bitmap);


	//Terura del regalo

	//Tectura del regalo


	// Definiendo la textura a utilizar
	Texture texture16A("../Textures/envoltura-nav.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture16A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture16A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID16A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID16A);
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
	texture16A.freeImage(bitmap);


	//Tectura del regalo2


	// Definiendo la textura a utilizar
	Texture texture17A("../Textures/cuadro.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture17A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture17A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID17A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID17A);
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
	texture17A.freeImage(bitmap);



	//Tectura del LA ROSCA

	// Definiendo la textura a utilizar
	Texture texture18A("../Textures/rosca.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture18A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture18A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID18A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID18A);
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
	texture18A.freeImage(bitmap);




	//Tectura del LA ROSCA

	// Definiendo la textura a utilizar
	Texture texture19A("../Textures/guirnalda.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture19A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture19A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID19A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID19A);
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
	texture19A.freeImage(bitmap);
	//TEXTURA DE LA VENTANA


		//Tectura del LA ROSCA

	// Definiendo la textura a utilizar
	Texture texture20A("../Textures/ventanal.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture20A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture20A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID20A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID20A);
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
	texture20A.freeImage(bitmap);
	//TEXTURA DE LA VENTANA


		//Tectura del LA ROSCA

	// Definiendo la textura a utilizar
	Texture texture21A("../Textures/nacimiento.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture21A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture21A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID21A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID21A);
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
	texture21A.freeImage(bitmap);
	//TEXTURA DE LA VENTANA




		// Definiendo la textura a utilizar
	Texture texture7A("../Textures/venta.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture7A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture7A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID7A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID7A);
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
	texture7A.freeImage(bitmap);

	//Cocina integral


	//TEXTURA DE LA VENTANA


		// Definiendo la textura a utilizar
	Texture texture8A("../Textures/cocina.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture8A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture8A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID8A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID8A);
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
	texture8A.freeImage(bitmap);

	//mueble arriba

	// Definiendo la textura a utilizar
	Texture texture9A("../Textures/MuebleSup.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture9A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture9A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID9A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID9A);
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
	texture9A.freeImage(bitmap);

	// Refrigerador

	// Definiendo la textura a utilizar
	Texture texture10A("../Textures/refri.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture10A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture10A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID10A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID10A);
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
	texture10A.freeImage(bitmap);

	// sofa textura


   // Definiendo la textura a utilizar
	Texture texture11A("../Textures/lino.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture11A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture11A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID11A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID11A);
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
	texture11A.freeImage(bitmap);

	// =============== MESA ============== 

	// Definiendo la textura a utilizar
	Texture texture12A("../Textures/madera.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture12A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture12A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID12A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID12A);
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
	texture12A.freeImage(bitmap);




	// =============== PISTA ============== 

	// Definiendo la textura a utilizar
	Texture texture13A("../Textures/pista.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture13A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture13A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID13A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID13A);
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
	texture13A.freeImage(bitmap);


	// Helipuerto


	// Definiendo la textura a utilizar
	Texture texture14A("../Textures/helipuerto.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture14A.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture14A.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID14A);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID14A);
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
	texture14A.freeImage(bitmap);


	//HALLOWEEEN*********************************************************************************


	// Definiendo la textura a utilizar
	Texture texture1("../Textures/losetas.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture1.convertToData(bitmap, imageWidth,
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
	}
	else
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
	}
	else
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
	bitmap = texture16.loadImage();
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
		}
		else
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
		case GLFW_KEY_ESCAPE:			//camara caso de navidad
			exitApp = true;
			break;
		case GLFW_KEY_0:				//camara casa de terror
			cameraSel = 1;
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (cameraSel == 0)
			camera->moveFrontCamera(true, deltaTime);
		else if (cameraSel == 1)
			camera2->moveFrontCamera(true, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (cameraSel == 0)
			camera->moveFrontCamera(false, deltaTime);
		else if (cameraSel == 1)
			camera2->moveFrontCamera(false, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (cameraSel == 0)
			camera->moveRightCamera(false, deltaTime);
		else if (cameraSel == 1)
			camera2->moveRightCamera(false, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (cameraSel == 0)
			camera->moveRightCamera(true, deltaTime);
		else if (cameraSel == 1)
			camera2->moveRightCamera(true, deltaTime);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (cameraSel == 0)
			camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
		else if (cameraSel == 1)
			camera2->mouseMoveCamera(offsetX, offsetY, deltaTime);
	}

	offsetX = 0;
	offsetY = 0;

	// Descomentar
	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;


	// Descomentar
	// Guardar key frames
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = true;
		if (myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = false;
		myfile.close();
		if (modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
	}
	if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		saveFrame = true;
		availableSave = false;
	}if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.0001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.0001;

	sentido = true;


	// Descomentar (ESTOS SON LOS MOVIMIENTOS POR LAS TECLAS DEL DARK) 
	//Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotDartBody = 0.02;
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotDartBody = -0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		advanceDartBody = -0.02;
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		advanceDartBody = 0.02;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		cameraSel = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		cameraSel = 0;
	}

	glfwPollEvents();
	return continueApplication;


}

void applicationLoop() {
	bool psi = true;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	int state = 0, state2 = 0, stateLich=0; //se agregan variables para el movimiento 
	int estadoHelicoptero = 0;

	float angle2 = 0.0;
	float ratio2 = 30.0;
	glm::mat4 matrixModelAircraft = glm::mat4(1.0); //se cortan y pegan estas lineas, estaban mas abajo
	matrixModelAircraft = glm::scale(matrixModelAircraft, glm::vec3(0.1, 0.1, 0.1));
	matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(-35, -15.0, 0.0));
	glm::mat4 matrixModelEclipse = glm::mat4(1.0);
	matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(70.0, 0.2, 21.0));
	matrixModelEclipse = glm::rotate(matrixModelEclipse, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 matrixModelLich = glm::mat4(1.0);
	matrixModelLich = glm::translate(matrixModelLich, glm::vec3(70, 1.75, -15));
	matrixModelLich = glm::rotate(matrixModelLich, glm::radians(270.0f), glm::vec3(0.0, 1.0, 0));

	glm::mat4 matrixModelHeli = glm::mat4(1.0);
	matrixModelHeli = glm::translate(matrixModelHeli, glm::vec3(75.0, 0.0, 12.0));
	matrixModelHeli = glm::rotate(matrixModelHeli, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	int stateAir = 0; //se agregan variables para el movimiento 
	float offsetAircraftAdvance = 0.0, offsetAircraftAdvance2 = 0.0;
	float offsetAircraftRot = 0.0;
	float offsetHeliAdvance = 0.0;
	float offsetLich = 0.0;

	//ESPECIFICACIONES DEL AUTO 

	float offsetAutoAdvance = 0.0;
	float offsetAutotRot = 0.0;
	glm::mat4 matrixModelAuto = glm::mat4(1.0);
	matrixModelAuto = glm::translate(matrixModelAuto, glm::vec3(40.0, -1.0, -40.0));
	matrixModelAuto = glm::rotate(matrixModelAuto, 55.0f, glm::vec3(0.0, 1.0, 0.0));

	//Auto de juguete



	glm::mat4 modelMatrixEclipse2 = glm::mat4(1.0f);
	modelMatrixEclipse2 = glm::scale(modelMatrixEclipse2, glm::vec3(0.08, 0.08, 0.08));
	modelMatrixEclipse2 = glm::translate(modelMatrixEclipse2, glm::vec3(-20, -15.0, 0.0));
	int stateAuto = 0, stateEclipse = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;




	//ESPECIFICACIONES DEL HELICOPTERO

	float offsetHelicoptero2AdvanceY = 0.0;
	glm::mat4 matrixModelHelicoptero = glm::mat4(1.0);
	matrixModelHelicoptero = glm::scale(matrixModelHelicoptero, glm::vec3(0.1, 0.1, 0.1));
	matrixModelHelicoptero = glm::translate(matrixModelHelicoptero, glm::vec3(-20.0, 5.0, 0.0));

	glm::mat4 modelMatrixDart = glm::mat4(1.0f);
	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(11.0, -2.0, -1.5));
	modelMatrixDart = glm::scale(modelMatrixDart, glm::vec3(0.5, 0.5, 0.5));

	// Descomentar
	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

		if (cameraSel == 0)
			view = camera->getViewMatrix();
		if (cameraSel == 1)
			view = camera2->getViewMatrix();

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
			glm::value_ptr(glm::mat4((view))));

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
		//Esto es para la luz spotLight
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
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.005);



		// =============   LUCES NAVIDEÑAS  Y VELAS    =============


		// Esto es para la luces pointlights
		// Numero de luces a utilizar de tipo pointlights = 12
		shaderMulLighting.setInt("pointLightCount", 12);


		//Posicion de la pared de fondo de la habitacion =======> 14.2, 0.0, -5.0



		shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-11.0, 1.5, -3.5))));
		//Propiedades de la luz verde
		shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);

		//Propiedades de la luz roja  -5.1, 4.8, -5.0
		shaderMulLighting.setVectorFloat3("pointLights[1].position", glm::value_ptr((glm::vec3(-9.0, 1.5, -3.5))));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[1].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[1].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[1].quadratic", 0.004);

		//Propiedades de la luz azul -5.1, 4.5, -6.5
		shaderMulLighting.setVectorFloat3("pointLights[2].position", glm::value_ptr((glm::vec3(-7.0, 1.5, -3.5))));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.0, 0.01)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[2].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[2].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[2].quadratic", 0.004);


		//Propiedades de la luz amarilla
		shaderMulLighting.setVectorFloat3("pointLights[3].position", glm::value_ptr((glm::vec3(-5.0, 1.3, -3.5))));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[3].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[3].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[3].quadratic", 0.004);
		//Propiedades de la luz morada
		shaderMulLighting.setVectorFloat3("pointLights[4].position", glm::value_ptr((glm::vec3(-3.0, 1.5, -3.5))));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.0, 0.01)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[4].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[4].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[4].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[5].position", glm::value_ptr((glm::vec3(-9.5, 0.0, -3.0))));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[5].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[5].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[5].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[5].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[6].position", glm::value_ptr((glm::vec3(-9.0, -1, -4.0))));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[6].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[6].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[6].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[6].quadratic", 0.004);

		// Posicion de la luz con indice [7]
		shaderMulLighting.setVectorFloat3("pointLights[7].position", glm::value_ptr((glm::vec3(65.0, 4.5, 5.1))));
		//Propiedades de la luz verde
		shaderMulLighting.setVectorFloat3("pointLights[7].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[7].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[7].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[7].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[7].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[7].quadratic", 0.004);

		//Propiedades de la luz roja 
		shaderMulLighting.setVectorFloat3("pointLights[8].position", glm::value_ptr((glm::vec3(64.5, 4.5, 5.1))));
		shaderMulLighting.setVectorFloat3("pointLights[8].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[8].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[8].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[8].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[8].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[8].quadratic", 0.004);

		//Propiedades de la luz azul 
		shaderMulLighting.setVectorFloat3("pointLights[9].position", glm::value_ptr((glm::vec3(64, 4.5, 5.1))));
		shaderMulLighting.setVectorFloat3("pointLights[9].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[9].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.0, 0.01)));
		shaderMulLighting.setVectorFloat3("pointLights[9].light.specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[9].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[9].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[9].quadratic", 0.004);
		//Propiedades de la luz amarilla
		shaderMulLighting.setVectorFloat3("pointLights[10].position", glm::value_ptr((glm::vec3(63.5, 2, -21.5))));
		shaderMulLighting.setVectorFloat3("pointLights[10].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[10].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[10].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[10].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[10].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[10].quadratic", 0.004);
		//Propiedades de la luz amarilla2 (vela)
		shaderMulLighting.setVectorFloat3("pointLights[11].position", glm::value_ptr((glm::vec3(63.5, 2, -18.5))));
		shaderMulLighting.setVectorFloat3("pointLights[11].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[11].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[11].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[11].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[11].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[11].quadratic", 0.004);

		// navideñas 2
			//Esto es para colocar las esferas de las luces (modelo, no las propiedades) 
		sphereLamp.setScale(glm::vec3(0.3, 0.3, 0.2));
		sphereLamp.setPosition(glm::vec3(-11.0, 1.5, -3.5));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.3, 0.3, 0.2));
		sphereLamp.setPosition(glm::vec3(-9, 1.3, -3.5));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.3, 0.3, 0.2));
		sphereLamp.setPosition(glm::vec3(-7.0, 1.5, -3.5));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.3, 0.3, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.0, 1.3, -3.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.3, 0.3, 0.2));
		sphereLamp.setPosition(glm::vec3(-3.0, 1.5, -3.5));
		sphereLamp.setColor(glm::vec4(3.2, 0.0, 0.8, 1.0));
		sphereLamp.render();



		//azul 


		//====ESTAS SON DEL ARBOL DE NAVIDAD 


		//Esto es para colocar las esferas de las luces (modelo, no las propiedades) 
		//verde
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-9.5, 0.0, -3.0));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();
		//ROJO
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-9.0, -1, -4.0));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();
		//AZul
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-9.8, 1.0, -3.5));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();


		//		Techo de la sala

		sphereLamp.setScale(glm::vec3(1.0, 0.3, 1.0));
		sphereLamp.setPosition(glm::vec3(5.0, 1.7, -0.8));
		sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereLamp.render();


		sphereLamp.setScale(glm::vec3(1.0, 0.3, 1.0));
		sphereLamp.setPosition(glm::vec3(-6.0, 1.7, -0.8));
		sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereLamp.render();


		//AMARILLA

		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-9.3, 0.0, -3.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();

		//ROSA

		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(-9.0, 1.0, -3.0));
		sphereLamp.setColor(glm::vec4(3.2, 0.0, 0.8, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(65, 4.5, 5.1));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(64.5, 3.0, 5.1));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.2, 0.1, 0.1));
		sphereLamp.setPosition(glm::vec3(64.0, 3.0, 5.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();


		//CARRO ECLIPSE BASICO
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0, 0.0)));
		autoEclipse.render(matrixModelAuto);
		glActiveTexture(GL_TEXTURE0);

		//helicoptero
		modelHelicoptero.render(matrixModelHelicoptero);
		glActiveTexture(GL_TEXTURE0);


		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
			glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
			glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix
					* glm::vec4(0.0, 0.0, 0.0, 1.0))));





		// AUTO DE JUGETE

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse2);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse2, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);
		glActiveTexture(GL_TEXTURE0);


		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(3.0, -2.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(36.0, 0.0, 10.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);



		//JARDIN

		glm::mat4 sueloMatAlberca = glm::mat4(1.0);
		sueloMatAlberca = glm::translate(sueloMatAlberca, glm::vec3(-10.0, -2.0, 7.5));
		sueloMatAlberca = glm::scale(sueloMatAlberca, glm::vec3(9.7, 0.0, 5.2));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID3A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		sueloAlberca.render(sueloMatAlberca);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		glm::mat4 sueloJardinMatrix = glm::mat4(1.0);
		sueloJardinMatrix = glm::translate(sueloJardinMatrix, glm::vec3(8.0, -2.0, 6.8));
		sueloJardinMatrix = glm::scale(sueloJardinMatrix, glm::vec3(27.0, 0.0, 4.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID5A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		sueloJardin.render(sueloJardinMatrix);

		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);

	//	PRIMER PARED IZQUIERDA HABITACION

		glm::mat4 modelPared1 = glm::mat4(1.0);
		modelPared1 = glm::translate(modelPared1, glm::vec3(-1.5, 0.0, -3.0));
		modelPared1 = glm::scale(modelPared1, glm::vec3(0.0, 4.0, 3.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//	shaderMulLighting.setFloat("offsetX", offX);
		pared1.render(modelPared1);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		// PARED EXTERNA IZQUIERDA (habitacion) 
		glm::mat4 modelPared3 = glm::mat4(1.0);
		modelPared3 = glm::translate(modelPared3, glm::vec3(9.0, 0.0, 0.0));
		modelPared3 = glm::scale(modelPared3, glm::vec3(0.0, 4.0, 10.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//	shaderMulLighting.setFloat("offsetX", offX);
		pared1.render(modelPared3);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		//pared izquerda sala

		//PARED EXTERNA izquierda COMEDOR DE DONDE IRA EL CUADRO
		glm::mat4 modelPared6 = glm::mat4(1.0);
		modelPared6 = glm::translate(modelPared6, glm::vec3(-15.0, 0.0, 2.0));
		modelPared6 = glm::scale(modelPared6, glm::vec3(0.0, 4.0, 14.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		pared6.render(modelPared6);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		//PARED EXTERNA izquierda COMEDOR DE DONDE IRA EL CUADRO
		glm::mat4 modelparedComedor = glm::mat4(1.0);
		modelparedComedor = glm::translate(modelparedComedor, glm::vec3(-25.0, 0.0, 2.0));
		modelparedComedor = glm::scale(modelparedComedor, glm::vec3(0.0, 4.0, 14.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		paredComedor.render(modelparedComedor);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);





		// este es el cuadro 

		glm::mat4 modelCuadro = glm::mat4(1.0);
		modelCuadro = glm::translate(modelCuadro, glm::vec3(-14.5, 0.0, 2.2));
		modelCuadro = glm::scale(modelCuadro, glm::vec3(0.0, 2.0, 4.0));
		// Se activa la textura del agua		
		glBindTexture(GL_TEXTURE_2D, textureID15A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		cuadro.render(modelCuadro);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);

		// este es el regalo 

		glm::mat4 modelRegalo = glm::mat4(1.0);
		modelRegalo = glm::translate(modelRegalo, glm::vec3(-10.5, -1.7, -2.0));
		modelRegalo = glm::scale(modelRegalo, glm::vec3(0.7, 0.7, 0.7));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID16A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		regalo.render(modelRegalo);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);

		glm::mat4 modelRegalo2 = glm::mat4(1.0);
		modelRegalo2 = glm::translate(modelRegalo2, glm::vec3(-8.5, -1.7, -2.0));
		modelRegalo2 = glm::scale(modelRegalo2, glm::vec3(0.7, 0.7, 0.7));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID17A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		regalo2.render(modelRegalo2);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		//==============TECHO==================
		glm::mat4 modelTecho = glm::mat4(1.0);
		modelTecho = glm::translate(modelTecho, glm::vec3(5.0, 2.7, 2.5));
		modelTecho = glm::scale(modelTecho, glm::vec3(40, 0.0, 27.5));
		// Se activa la textu
		glBindTexture(GL_TEXTURE_2D, textureID0);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		techo.render(modelTecho);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);



		//PARED EXTERNA DERECHA cocina
		glm::mat4 modelBox3 = glm::mat4(1.0);
		modelBox3 = glm::translate(modelBox3, glm::vec3(20.6, 0.0, 1.5));
		modelBox3 = glm::scale(modelBox3, glm::vec3(0.0, 5.0, 14.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID7A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		box3.render(modelBox3);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);

		// este es el closet

		glm::mat4 modelPared7 = glm::mat4(1.0);
		modelPared7 = glm::translate(modelPared7, glm::vec3(8.0, 0.0, 0.0));
		modelPared7 = glm::scale(modelPared7, glm::vec3(0.7, 4.0, 3.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID1A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		paredCocinaExterna.render(modelPared7);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);

		//PARED DEL FONDO DE LA CASA  solo es de la habitacion 

		glm::mat4 modelPared2 = glm::mat4(1.0);

		modelPared2 = glm::translate(modelPared2, glm::vec3(3.4, 0.0, -5.0));

		modelPared2 = glm::scale(modelPared2, glm::vec3(11.0, 4.0, 0.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID7A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		pared2.render(modelPared2);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);



		//pared DEL COMEDOR

		glm::mat4 modelFondoComedor = glm::mat4(1.0);

		modelFondoComedor = glm::translate(modelFondoComedor, glm::vec3(-20, 0.0, -5.0));

		modelFondoComedor = glm::scale(modelFondoComedor, glm::vec3(11.0, 4.0, 0.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID7A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		paredFondoComedor.render(modelFondoComedor);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);



		//GUIRNNALLDAAA


		glm::mat4 modelGuirnalda = glm::mat4(1.0);
		modelGuirnalda = glm::translate(modelGuirnalda, glm::vec3(4.0, 1.3, -3.8));
		modelGuirnalda = glm::scale(modelGuirnalda, glm::vec3(10.0, 0.6, 0.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID19A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		guirnalda.render(modelGuirnalda);
		glBindTexture(GL_TEXTURE_2D, 0);		//shaderMulLighting.setFloat("offsetX", 0);


		//NACIMIENTOOO

		glm::mat4 modelNacimiento = glm::mat4(1.0);
		modelNacimiento = glm::translate(modelNacimiento, glm::vec3(20.0, -1.0, 0.0));
		modelNacimiento = glm::scale(modelNacimiento, glm::vec3(0.5, 2.0, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID21A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		nacimiento.render(modelNacimiento);
		glBindTexture(GL_TEXTURE_2D, 0);		//shaderMulLighting.setFloat("offsetX", 0);

		// PARED FONDO SALA 

		glm::mat4 modelParedFondoCocina = glm::mat4(1.0);
		modelParedFondoCocina = glm::translate(modelParedFondoCocina, glm::vec3(14.2, 0.0, -5.0));

		modelParedFondoCocina = glm::scale(modelParedFondoCocina, glm::vec3(13.0, 4.0, 0.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		paredFondoCocina.render(modelParedFondoCocina);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		//cocina bajo


		glm::mat4 modelAbajo = glm::mat4(1.0);
		modelAbajo = glm::translate(modelAbajo, glm::vec3(14.2, -1.0, -4.0));
		modelAbajo = glm::scale(modelAbajo, glm::vec3(10.0, 1.5, 1.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID8A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		Cocina.render(modelAbajo);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		glm::mat4 modelSuperior = glm::mat4(1.0);
		modelSuperior = glm::translate(modelSuperior, glm::vec3(14.2, 1.5, -4.0));
		modelSuperior = glm::scale(modelSuperior, glm::vec3(10.0, 1.5, 1.5));

		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID9A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		Superior.render(modelSuperior);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		// COCINA INTEGRAL

		// PARED FONDO COCINA

		glm::mat4 modelCocina = glm::mat4(1.0);
		modelCocina = glm::translate(modelCocina, glm::vec3(14.2, 0.0, -5.0));

		modelCocina = glm::scale(modelCocina, glm::vec3(10.0, 4.0, 0.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID4A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		Cocina.render(modelCocina);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);


		// FONDO DE LA SALA  


		glm::mat4 modelParedFondo = glm::mat4(1.0);
		modelParedFondo = glm::translate(modelParedFondo, glm::vec3(-8.5, 0.0, -5.0));

		modelParedFondo = glm::scale(modelParedFondo, glm::vec3(13.0, 4.0, 0.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID7A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		paredFondo.render(modelParedFondo);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);



		//PARED FRENTE HABITACION (REFRIGERADOR)

		glm::mat4 modelPared4 = glm::mat4(1.0);
		modelPared4 = glm::translate(modelPared4, glm::vec3(15.0, 0.0, 3.5));
		modelPared4 = glm::scale(modelPared4, glm::vec3(4.0, 4.0, 1.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID10A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);	
		pared4.render(modelPared4);
		glBindTexture(GL_TEXTURE_2D, 0);
		//shaderMulLighting.setFloat("offsetX", 0);





		//pared externa
		glm::mat4 modelPared5 = glm::mat4(1.0);
		modelPared5 = glm::translate(modelPared5, glm::vec3(-5.0, 0.0, 5.0));
		modelPared5 = glm::scale(modelPared5, glm::vec3(20.0, 4.0, 0.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID20A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
		//shaderMulLighting.setFloat("offsetX", offX);
		pared4.render(modelPared5);
		glBindTexture(GL_TEXTURE_2D, 0);
		//	shaderMulLighting.setFloat("offsetX", 0);







			//SALA

			// ------------------------- JARDIN -------------------------- 

			//pared externa JARDIN 
		glm::mat4 modelJardinPared = glm::mat4(1.0);
		modelJardinPared = glm::translate(modelJardinPared, glm::vec3(3.7, 0.0, 9.0));
		modelJardinPared = glm::scale(modelJardinPared, glm::vec3(36.0, 4.0, 0.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID6A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//shaderMulLighting.setFloat("offsetX", offX);
		paredJardinExterna.render(modelJardinPared);
		glBindTexture(GL_TEXTURE_2D, 0);
		//	shaderMulLighting.setFloat("offsetX", 0);


		//---------------- HABITACION --------------------------------------------

		glm::mat4 matrixModelCama0 = glm::mat4(1.0);
		matrixModelCama0 = glm::translate(matrixModelCama0, glm::vec3(2.0, -1.0, -2.0));
		matrixModelCama0 = glm::rotate(matrixModelCama0, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		matrixModelCama0 = glm::scale(matrixModelCama0, glm::vec3(0.02, 0.02, 0.02));
		modelCama.render(matrixModelCama0);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);



		//====  arbol ====

		//Esto es para el modelo Tree
		glm::mat4 matrixModelTree = glm::mat4(1.0);
		matrixModelTree = glm::translate(matrixModelTree, glm::vec3(-10.0, -1.8, -3.5));
		matrixModelTree = glm::scale(matrixModelTree, glm::vec3(0.1, 0.1, 0.1));
		modelTree.render(matrixModelTree);
		//Forze to enable the unit texture to 0 always-------------------------IMPORTANT
		glActiveTexture(GL_TEXTURE0);
		

		//				TORRE DE REGALOS ////

		glm::mat4 matrixModelRegalos = glm::mat4(1.0);
		matrixModelRegalos = glm::translate(matrixModelRegalos, glm::vec3(-13.0, -2.0, -3.5));
		matrixModelRegalos = glm::rotate(matrixModelRegalos, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		matrixModelRegalos = glm::scale(matrixModelRegalos, glm::vec3(0.01, 0.01, 0.01));
		modelTorreRegalos.render(matrixModelRegalos);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelRegalos2 = glm::mat4(1.0);
		matrixModelRegalos2 = glm::translate(matrixModelRegalos2, glm::vec3(-5.0, -2.0, -3.5));
		matrixModelRegalos2 = glm::rotate(matrixModelRegalos2, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		matrixModelRegalos2 = glm::scale(matrixModelRegalos2, glm::vec3(0.01, 0.01, 0.01));
		modelTorreRegalos.render(matrixModelRegalos2);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);


		//=============== mesa cocina ===============


		//Models complex render
		glm::mat4 matrixModelMesa = glm::mat4(1.0);
		matrixModelMesa = glm::translate(matrixModelMesa, glm::vec3(13.0, -2.0, 0.0));
		matrixModelMesa = glm::scale(matrixModelMesa, glm::vec3(3.5, 1.0, 3.5));

		modelMesa.render(matrixModelMesa);





		glm::mat4 matrixModelPlanta = glm::mat4(1.0);
		matrixModelPlanta = glm::translate(matrixModelPlanta, glm::vec3(13.0, -1.5, 0.0));
		//matrixModelPlanta = glm::scale(matrixModelPlanta, glm::vec3(3.5, 1.0, 3.5));

		planta.render(matrixModelPlanta);

		//SEGUNDA PLANTA

		glm::mat4 matrixModelPlanta2 = glm::mat4(1.0);
		matrixModelPlanta2 = glm::translate(matrixModelPlanta2, glm::vec3(-5.0, -2.0, 8.0));
		matrixModelPlanta2 = glm::scale(matrixModelPlanta2, glm::vec3(2.0, 2.0, 2.0));

		planta2.render(matrixModelPlanta2);





		//TERCERA PLANTA

		glm::mat4 matrixModelPlanta3 = glm::mat4(1.0);
		matrixModelPlanta3 = glm::translate(matrixModelPlanta3, glm::vec3(0.0, -2.0, 8.0));
		matrixModelPlanta3 = glm::scale(matrixModelPlanta3, glm::vec3(2.0, 2.0, 2.0));

		planta3.render(matrixModelPlanta3);







		//Forze to enable the unit texture to 0 always-------------------------modelCAMA
		glActiveTexture(GL_TEXTURE0);



		//MODEL BURO

		glm::mat4 matBuro = glm::mat4(1.0);
		matBuro = glm::translate(matBuro, glm::vec3(-1.0, -2.0, -2.5));
		matBuro = glm::scale(matBuro, glm::vec3(1.3, 1.3, 1.3));


		//shaderMulLighting.setFloat("offsetX", 0);
		modelBuro.render(matBuro);
		//FORCE TO ENABLE THE UNIT TEXTURE TO 0 ALWAYS .............. IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		//animacion

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		modelDartLegoBody.render(modelMatrixDart);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDart);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDart);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDart);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDart);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDart);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		glActiveTexture(GL_TEXTURE0); // IMPORTANTE regresar a la textura 0
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		// Para salvar el frame
		if (record && modelSelected == 1) {
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		}
		else if (keyFramesDartJoints.size() > 0) {
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float)maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;

			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}

		if (record && modelSelected == 2) {
			modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(advanceDartBody, 0.0, 0.0));
			modelMatrixDart = glm::rotate(modelMatrixDart, rotDartBody, glm::vec3(0, 1, 0));
			matrixDart.push_back(modelMatrixDart);
			if (saveFrame) {
				appendFrame(myfile, matrixDart);
				saveFrame = false;
			}
		}
		else if (keyFramesDart.size() > 0) {
			// Para reproducir el frame
			interpolationDart = numPasosDart / (float)maxNumPasosDart;
			numPasosDart++;
			if (interpolationDart > 1.0) {
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if (indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;

			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}

		


		//**********************************************primitivas hallowen***************************************************************


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

		piso3.render(glm::scale(carretera, glm::vec3(60.0, 0.5, 5)));
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
		piso3.render(glm::scale(techo, glm::vec3(20, 0.0, 30)));
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






		//********************************************MODELOS Halloween***************************************************************************

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
		matrixModelCama = glm::translate(pisoMed, glm::vec3(-7.25, 0.85, 0));
		matrixModelCama = glm::rotate(matrixModelCama, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0));
		matrixModelCama = glm::rotate(matrixModelCama, glm::radians(270.0f), glm::vec3(1.0, 0.0, 0));
		modelCama.render(glm::scale(matrixModelCama, glm::vec3(0.018, 0.018, 0.018)));
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelCama2 = glm::mat4(1.0);
		matrixModelCama2 = glm::translate(pisoMed, glm::vec3(6.0, 0.85, 2.5));
		matrixModelCama2 = glm::rotate(matrixModelCama2, glm::radians(180.0f), glm::vec3(0, 1.0, 0.0));
		matrixModelCama2 = glm::rotate(matrixModelCama2, glm::radians(270.0f), glm::vec3(1.0, 0.0, 0));
		modelCama.render(glm::scale(matrixModelCama2, glm::vec3(0.018, 0.018, 0.018)));
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


		modelEclipse.render(glm::scale(matrixModelEclipse, glm::vec3(0.5, 0.5, 0.5)));
		glActiveTexture(GL_TEXTURE0);

		modelHeli.render(glm::scale(matrixModelHeli, glm::vec3(1.0, 1.0, 1.0)));
		glActiveTexture(GL_TEXTURE0);



		// -------------------------- SALA ----------------------------------- 

			//TELEVISION 

		glm::mat4 matrixModelTV = glm::mat4(1.0);//9
		matrixModelTV = glm::translate(matrixModelTV, glm::vec3(-5.0, -2.0, -3.0));//9
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTV = glm::scale(matrixModelTV, glm::vec3(0.2, 0.2, 0.2));
		//matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		modelTV.render(matrixModelTV);//9
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);



		// =========================== MESA ==============================0

		glm::mat4 modelMesa = glm::mat4(1.0);
		modelMesa = glm::translate(modelMesa, glm::vec3(-11, -1.0, 2.0));
		modelMesa = glm::scale(modelMesa, glm::vec3(3.0, 0.3, 4.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID12A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//shaderMulLighting.setFloat("offsetX", offX);
		mesa.render(modelMesa);
		glBindTexture(GL_TEXTURE_2D, 0);
		//	shaderMulLighting.setFloat("offsetX", 0);


		//ROSCA
		glm::mat4 modelRosca = glm::mat4(1.0);
		modelRosca = glm::translate(modelRosca, glm::vec3(-10.2, -0.7, 2.0));
		modelRosca = glm::scale(modelRosca, glm::vec3(1.5, 0.0, 2.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID18A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//shaderMulLighting.setFloat("offsetX", offX);
		rosca.render(modelRosca);
		glBindTexture(GL_TEXTURE_2D, 0);
		//	shaderMulLighting.setFloat("offsetX", 0);


		glm::mat4 modelMesaBajo = glm::mat4(1.0);
		modelMesaBajo = glm::translate(modelMesaBajo, glm::vec3(-10.0, -2.0, 1.0));
		modelMesaBajo = glm::scale(modelMesaBajo, glm::vec3(1.0, 1.7, 0.5));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID12A);
		//le cambiamos el shader con multiplesluces NO OLVIDAR
	//shaderMulLighting.setFloat("offsetX", offX);
		mesa2.render(modelMesaBajo);
		glBindTexture(GL_TEXTURE_2D, 0);
		//	shaderMulLighting.setFloat("offsetX", 0);






		//MODEL BURO

		glm::mat4 matBuroSala = glm::mat4(1.0);
		matBuroSala = glm::translate(matBuroSala, glm::vec3(-7.0, -2.0, -3.0));
		matBuroSala = glm::scale(matBuroSala, glm::vec3(1.5, 1.5, 1.5));
		//shaderMulLighting.setFloat("offsetX", 0);
		modelBuroSala.render(matBuroSala);

		//FORCE TO ENABLE THE UNIT TEXTURE TO 0 ALWAYS .............. IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		modelAircraft.render(matrixModelAircraft);
		glActiveTexture(GL_TEXTURE0);

		if (angle2 > 2 * M_PI)
			angle2 = 0.0;
		else
			angle2 += 0.0001;



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

		//MAQUINA DE ESTADOS DEL AUTO 

		switch (stateAuto) {
		case 0:
			std::cout << "Advance:" << std::endl;
			// -0.01 debe ser igual en magnitud 
			matrixModelAuto = glm::translate(matrixModelAuto, glm::vec3(0.0, 0.0, 0.01));
			offsetAutoAdvance += 0.01;
			if (offsetAutoAdvance > 60.0) {

				offsetAutoAdvance = 0.0;
				stateAuto = 1;
			}




			break;
		}

		// Constantes de animaciones
		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.01;


		// Descomentar
		advanceDartBody = 0.0;
		rotDartBody = 0.0;

		/*******************************************
		 * State machines
		 *******************************************/
		 // State machine for eclipse car
		switch (stateEclipse) {
		case 0:
			modelMatrixEclipse2 = glm::translate(modelMatrixEclipse2, glm::vec3(0.0, 0.0, 0.1));
			advanceCount += 0.1;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.02;
			if (rotWheelsY < 0)
				rotWheelsY = 0;
			if (advanceCount > 10.0) {
				advanceCount = 0;
				stateEclipse = 1;
			}
			break;
		case 1:
			modelMatrixEclipse2 = glm::translate(modelMatrixEclipse2, glm::vec3(0.0, 0.0, 0.025));
			modelMatrixEclipse2 = glm::rotate(modelMatrixEclipse2, glm::radians(0.5f), glm::vec3(0, 1, 0));
			rotCount += 0.5f;
			rotWheelsX += 0.05;
			rotWheelsY += 0.02;
			if (rotWheelsY > 0.25)
				rotWheelsY = 0.25;
			if (rotCount >= 90.0) {
				rotCount = 0;
				stateEclipse = 0;
			}
			break;
		}


		//MAquina de estados delaircraft

		switch (stateAir) {
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
		}



		//MAQUINA DE ESTADOS HELICOPTERO

		switch (estadoHelicoptero) {
		case 0:
			std::cout << "Advance Helicoptero:" << std::endl;
			matrixModelHelicoptero = glm::translate(matrixModelHelicoptero, glm::vec3(0.0, -0.01, 0.0));
			offsetHelicoptero2AdvanceY += 0.001;
			if (offsetHelicoptero2AdvanceY > 2.2) {
				offsetHelicoptero2AdvanceY = 0.0;
				estadoHelicoptero = 1;
			}
			break;
		}

		
		// =======================Movimiento del Lich ===========================


		switch (stateLich) {
		case 0:
			std::cout << "FrenteLich:" << std::endl;
			matrixModelLich = glm::translate(matrixModelLich, glm::vec3(0.1, 0.0, 0.0));
			offsetLich += 0.1;
			if (offsetLich > 20.0) {
				offsetLich = 0.0;
				stateLich = 1;
			}
			break;
		case 1:
			std::cout << "AtrasLich:" << std::endl;
			matrixModelLich = glm::translate(matrixModelLich, glm::vec3(-0.1, 0.0, 0.0));
			offsetLich += -0.1;
			if (offsetLich < -20.0) {
				offsetLich = 0.0;
				stateLich = 0;
			}
			break;
		}



		//-------------------------------------MOVIMIENTO DE Auto Halloween ---------------------------------------------------

		switch (state) {
		case 0:
			std::cout << "Frente:" << std::endl;
			matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(0.0, 0.0, 0.1));
			offsetAircraftAdvance2 += 0.1;
			if (offsetAircraftAdvance2 > 10.0) {
				offsetAircraftAdvance2 = 0.0;
				state = 1;
			}
			break;
		case 1:
			std::cout << "Atras:" << std::endl;
			matrixModelEclipse = glm::translate(matrixModelEclipse, glm::vec3(0.0, 0.0, -0.1));
			offsetAircraftAdvance2 += -0.1;
			if (offsetAircraftAdvance2 < -10.0) {
				offsetAircraftAdvance2 = 0.0;
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
			if (offsetHeliAdvance < -12.0) {
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