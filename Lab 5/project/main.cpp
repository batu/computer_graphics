#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"
#include "texture.h"
#include "LoadShaders.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>


// ASSIGNMENT 3 INCLUDES / Declerations / Initilizations //

#include "includes.h"
#include "Camera.h"


GLuint time_unif;
GLuint Mcam_unif;
GLuint Mproj_unif;

GLuint camPos;

Camera cam;

glm::vec3 camPosition, gaze, up;
//////////////////////////////////////////////////////////


using namespace std;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

GLuint shaderProgramID;
GLuint shaderProgramID2;

Texture textureClouds ,textureGround, textureGroundNormal;



enum VAO_IDs { TopFaces, BottomFace, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];


GLfloat s = 15.00f;
//NumVertices = number of faces * 3;
const GLuint NumVertices = 30;

GLfloat vertices[] = {
	//front -- 123, 134
	s, s, s, -s, s, s, -s, -s, s,
	s, s, s, -s, -s, s, s, -s, s,

	//back -- 658,687
	-s, s, -s, s, s, -s, s, -s, -s,
	-s, s, -s, s, -s, -s, -s, -s, -s,

	//left -- 267,273
	-s, s, s, -s, s, -s, -s, -s, -s,
	-s, s, s, -s, -s, -s, -s, -s, s,

	//rgiht -- 514,548
	s, s, -s, s, s, s, s, -s, s,
	s, s, -s, s, -s, s, s, -s, -s,
	//top -- 562,521
	s, s, -s, -s, s, -s, -s, s, s,
	s, s, -s, -s, s, s, s, s, s,
	//bottom -- 437,478
	//s,-s,s,         -s,-s,s,      -s,-s,-s,
	//s,-s,s,         -s,-s,-s,      s,-s,-s
};

GLfloat texcoords[] = {
	//front -- 123, 134
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	//back -- 658,687
	1.0f, 1 / 3.0f, 3 / 4.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	1.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f, 1.0f, 2 / 3.0f,

	//left -- 267,273
	1 / 4.0f, 1 / 3.0f, 0.0f, 1 / 3.0f, 0.0f, 2 / 3.0f,
	1 / 4.0f, 1 / 3.0f, 0.0f, 2 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	//rgiht -- 514,548
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	//top -- 562,521
	1 / 2.0f, 0.0f, 1 / 4.0f, 0.0f, 1 / 4.0f, 1 / 3.0f,
	1 / 2.0f, 0.0f, 1 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f,
	//bottom -- 437,478
	//1/2.0f, 2/3.0f,        1/4.0f, 2/3.0f,     1/4.0f, 1.0f,
	//1/2.0f, 2/3.0f,        1/4.0f, 1.0f,       1/2.0f, 1.0f
};


GLfloat verticesBot[] = {
	s, -s, s, -s, -s, s, -s, -s, -s,
	s, -s, s, -s, -s, -s, s, -s, -s
};
GLfloat texcoordsBot[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

void loadTextures(void){

	glActiveTexture(GL_TEXTURE0);
	textureClouds.Load("cubemaplayout.png");
	textureClouds.Bind();

	glActiveTexture(GL_TEXTURE2);
	textureGround.Load("cobblestone.jpg");
	textureGround.Bind();

	glActiveTexture(GL_TEXTURE1);			// Make texture1 active
	textureGroundNormal.Load("cobblestone_normal.jpg");	// Load texture from file
	textureGroundNormal.Bind();

}

glm::vec2 p1, p2;

int g_height = 512;
int g_width = 512;
const float epsilon = 0.000001f;

const GLuint NumVerticesBot = 6;

//Any time the windows is resized, this function is called.
//The parameters passed to it are the new dimensions of the window.
void changeViewport(int w, int h){
	glViewport(0,0,w,h);
}

//This function is called each time the window is redrawn.
void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);

	//Draw Topfaces
	glUseProgram(shaderProgramID);
	glBindVertexArray(VAOs[TopFaces]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//Draw Bottomface
	glUseProgram(shaderProgramID2);
	glBindVertexArray(VAOs[BottomFace]);
	glDrawArrays(GL_TRIANGLES, 0, NumVerticesBot);

	glFlush();
	glutSwapBuffers();
}

void init_camera_top(){
	Mcam_unif = glGetUniformLocation(shaderProgramID, "Mcam");
	Mproj_unif = glGetUniformLocation(shaderProgramID, "Mproj");

	camPos = glGetUniformLocation(shaderProgramID, "camPos");

	GLuint vnewEyePos = glGetAttribLocation(shaderProgramID, "vnewEyePosition");
	glEnableVertexAttribArray(vnewEyePos);
	glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif);

	cam.position = glm::vec3(0, 0, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, -1, -100);
}


void init_camera_bottom(){
	Mcam_unif = glGetUniformLocation(shaderProgramID2, "Mcam");
	Mproj_unif = glGetUniformLocation(shaderProgramID2, "Mproj");

	camPos = glGetUniformLocation(shaderProgramID2, "camPos");

	GLuint vnewEyePos = glGetAttribLocation(shaderProgramID2, "vnewEyePosition");
	glEnableVertexAttribArray(vnewEyePos);
	glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif);

	cam.position = glm::vec3(0, 0, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, -1, -100);
}


void initTop(){


	//ShaderInfo shader_0 = { GL_VERTEX_SHADER, "myshader.vs", GL_FRAGMENT_SHADER, "myshader.fs" };
	//shaderProgramID = LoadShaders(shader_0);

	Shader sh("myshader");
	shaderProgramID = sh.Bind();

	glUseProgram(shaderProgramID);

	init_camera_top();

	glGenVertexArrays(1, &VAOs[TopFaces]);
	glBindVertexArray(VAOs[TopFaces]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texcoords), NULL, GL_STATIC_DRAW);
	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//Load the texcoords data right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texcoords), texcoords);
	
	GLuint positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	glEnableVertexAttribArray(positionID);
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint texcoordsID = glGetAttribLocation(shaderProgramID, "s_vTexcoord");
	glEnableVertexAttribArray(texcoordsID);
	glVertexAttribPointer(texcoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	glUseProgram(shaderProgramID);
	glUniform1i(glGetUniformLocation(shaderProgramID, "diffuseMap"), 0); // set the variable diffuseMap to 0 so that it uses texture0

}


void initBot(){

	Shader sh1("myshader1");
	shaderProgramID2 = sh1.Bind();

	glUseProgram(shaderProgramID2);

	init_camera_bottom();

	glGenVertexArrays(1, &VAOs[BottomFace]);
	glBindVertexArray(VAOs[BottomFace]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBot) + sizeof(texcoordsBot), NULL, GL_STATIC_DRAW);
	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesBot), verticesBot);
	//Load the texcoords data right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticesBot), sizeof(texcoordsBot), texcoordsBot);

	GLuint positionID = glGetAttribLocation(shaderProgramID2, "s_vPosition");
	glEnableVertexAttribArray(positionID);
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint texcoordsID = glGetAttribLocation(shaderProgramID2, "s_vTexcoord");
	glEnableVertexAttribArray(texcoordsID);
	glVertexAttribPointer(texcoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticesBot)));

	glUseProgram(shaderProgramID2);
	glUniform1i(glGetUniformLocation(shaderProgramID2,"diffuseMap"), 2); // set the variable diffuseMap to 0 so that it uses texture0
	glUniform1i(glGetUniformLocation(shaderProgramID2, "normalMap"), 1); // set the variable normalMap to 1 so that it uses texture1

}


float x_axis, y_axis, z_axis;



void special(int key, int x, int y)
{

	float theta = 0.10;


	glm::vec3 old_cam_gaze = cam.gaze;
	glm::vec3 old_cam_up = cam.up;
	glm::vec3 old_cam_w = glm::cross(-cam.gaze, cam.up);

	switch (key)
	{
	case GLUT_KEY_UP:

		cam.gaze = cos(theta)*old_cam_gaze + sin(theta)*old_cam_up;
		cam.up = cos(theta)*old_cam_up - sin(theta)*old_cam_gaze;
		cam.setCameraMatrix();
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:

		cam.gaze = cos(theta)*old_cam_gaze - sin(theta)*old_cam_up;
		cam.up = cos(theta)*old_cam_up + sin(theta)*old_cam_gaze;
		cam.setCameraMatrix();
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		cam.gaze = cos(theta)*old_cam_gaze + sin(theta)*old_cam_w;
		cam.setCameraMatrix();
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:

		cam.gaze = cos(theta)*old_cam_gaze - sin(theta)*old_cam_w;
		cam.setCameraMatrix();
		glutPostRedisplay();
		break;
	}
}

void keyboard(unsigned char key, int x, int y){
	float next_x = cam.position.x + 0.1f*cam.gaze.x;
	float next_y = cam.position.y + 0.1f*cam.gaze.y;
	float next_z = cam.position.z + 0.1f*cam.gaze.z;


	if (key == 'a'){
		if (next_x > -13.0f  && next_x < 13.0f &&
			next_y > -13.0f  && next_y < 13.0f &&
			next_z > -13.0f  && next_z < 13.0f
			){
		cam.position += 0.1f*cam.gaze;
		cam.setCameraMatrix();
		glutPostRedisplay();
		}
	}
	else if (key == 'z'){
		if (next_x > -13.0f  && next_x < 13.0f &&
			next_y > -13.0f  && next_y < 13.0f &&
			next_z > -13.0f  && next_z < 13.0f
			){
			cam.position -= 0.1f*cam.gaze;
			cam.setCameraMatrix();
			glutPostRedisplay();
		}
	}
}


int main(int argc, char **argv){
	
	//printf("Printing Mcam_unif: %f",Mcam_unif);
	glutInit(&argc,argv);					 //Initialize GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//Set up some memory buffers for our display
	glutInitWindowSize(g_height,g_width);					//Set the window size
	glutCreateWindow("CUBE");						//Set the window title
	glutReshapeFunc(changeViewport);

	GLenum err = glewInit();		 //Very important! This initializes the entry points 
	if(GLEW_OK!=err){				 //in the OpenGL driver so we can call all the functions 
		cerr<<"GLEW error";			 //in the API.
		return 1;
	}

	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



	loadTextures();

	initTop();
	initBot();


	// RENDER FUNCTION IS CHANGED TO FIT DISPLAY
	glutDisplayFunc(render);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
} 