#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"
#include "texture.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

using namespace std;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, texcoordsID;
GLuint num_indices;

GLuint sharpnessID;
GLfloat sharpness = 1200;

Texture texture;

glm::mat4 Mmodel;
GLuint Mmodel_id, mainSharpness_id;

glm::vec2 p1, p2;
bool tracking = false;

int g_height = 800;
int g_width = 800;
const float epsilon = 0.000001f;


 void rotateMatrix(){
	float R = 2.0f;
	float t = R*R- p1.x*p1.x - p1.y * p1.y;
	float z1 = (t>0)?sqrt(t):0;
	t = R*R - p2.x*p2.x - p2.y * p2.y;
	float z2 = (t>0)?sqrt(t):0;

	glm::vec3 a(p1, z1);
	glm::vec3 b(p2, z2);
	glm::vec3 u = glm::cross(glm::normalize(a) , glm::normalize(b) );
	float l = glm::length(u);
	
	if (l >= epsilon) {
		l = (l < 1.0f) ? l : 1.0f;
		Mmodel = glm::rotate(glm::mat4(1.0), 8 * glm::asin(l), u) * Mmodel;
	}
 }

 void keyboard(unsigned char key, int x, int y){

	 if (key == '+'){
		sharpness += 50; 
		printf("Printing sharpness: %f\n", sharpness);
		glUniform1f(sharpnessID, sharpness);
		glutPostRedisplay();
	 }

	 if (key == '-'){
		 if (sharpness - 50 > 0){
			 sharpness -= 25;
			 printf("Printing sharpness: %f\n", sharpness);
			 glUniform1f(sharpnessID, sharpness);
			 glutPostRedisplay();
		 }
	 }
 }


//Any time the windows is resized, this function is called.
//The parameters passed to it are the new dimensions of the window.
void changeViewport(int w, int h){
	glViewport(0,0,w,h);
}

//This function is called each time the window is redrawn.
void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES,0,3);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT, (void *)0);
	glutSwapBuffers();
}

void init(){

	GLfloat s = 0.5f;
	
	unsigned int V[8][3];

	for(unsigned int i = 0; i< 8; ++i){
		for(unsigned int j = 0; j < 3; ++j) {
			V[i][j] = (i>>j) & 1u;
		}
	}

	GLfloat vertices[14][3];
	int v_index[] = {0,4,5,1,0,2,6,7,3,2,0,1,2,3};
	for(unsigned int i = 0; i < 14; ++i){
		for(unsigned j=0; j< 3; ++j){
			vertices[i][j] = s*(2*V[v_index[i]][j] - 1.0f);
		}
	}

	GLfloat texcoords[14][2];
	for(unsigned int i =0; i< 10; ++i){
		texcoords[i][0] = (i%5)/4.0f;
		texcoords[i][1] = (i/5 + 1)/3.0f;
	}
	texcoords[10][0] = texcoords[12][0] = 1.0f/4;
	texcoords[11][0] = texcoords[13][0] = 1.0f/2;
	texcoords[10][1] = texcoords[11][1] = 0.0f;
	texcoords[12][1] = texcoords[13][1] = 1.0f;

	GLushort indices[12][3] = { {0,6,5}, {0,1,6}, {1,7,6}, {1,2,7},
								{2,8,7}, {2,3,8}, {3,9,8}, {3,4,9},
								{10,2,1}, {10,11,2}, {6,13,12}, {6,7,13} };

	num_indices = sizeof(indices)/sizeof(indices[0]);

	Shader sh("myshader");
	GLuint shaderProgramID = sh.Bind();

	Mmodel = glm::mat4(1.0); // both matrices set to identity in the beginning
	Mmodel_id = glGetUniformLocation(shaderProgramID, "Mmodel");
	glUniformMatrix4fv(Mmodel_id, 1, GL_FALSE, glm::value_ptr(Mmodel));	


	GLuint texcoordsID = glGetAttribLocation(shaderProgramID, "s_vTexcoord");
	glEnableVertexAttribArray(texcoordsID);
	glVertexAttribPointer(texcoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	///////////// ASSIGNMNET 6 /////////////////////
	////////////////////////////////////////////////
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv//

	sharpnessID = glGetUniformLocation(shaderProgramID, "shaderSharpness");
	glUniform1f(sharpnessID, sharpness);

	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
	////////////////////////////////////////////////
	///////////// ASSIGNMNET 6 /////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	//Create a VAO and make it current
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// Create a VBO and make it current
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texcoords), NULL, GL_STATIC_DRAW);
	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//Load the texcoords data right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texcoords), texcoords);

	
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	glEnableVertexAttribArray(positionID);
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);

	texcoordsID = glGetAttribLocation(shaderProgramID, "s_vTexcoord");
	glEnableVertexAttribArray(texcoordsID);
	glVertexAttribPointer(texcoordsID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//Texture texture("cubemaplayout.png");
	//texture.Load("cubemaplayout.png");
	texture.Load("Crate.png");
	texture.Bind();

}

void mouse(int button, int state, int xx, int yy){
	// normalize xx and yy to x and y
	GLfloat x = 2*(GLfloat)xx/g_width - 1;
	GLfloat y = 1 - 2*(GLfloat) yy/g_width;
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			p1 = glm::vec2(x,y);
			tracking = true;
		}
		else if(state == GLUT_UP){
			tracking = false;
		}
	}	
}

void mouseMotion(int xx, int yy){
	if(tracking){
		GLfloat x = 2*(GLfloat)xx/g_width - 1;
		GLfloat y = 1 - 2*(GLfloat) yy/g_width;
		p2 = glm::vec2(x,y);
		rotateMatrix();
		p1 = glm::vec2(x,y);
		glUniformMatrix4fv(Mmodel_id, 1, GL_FALSE, glm::value_ptr(Mmodel));
		glutPostRedisplay();
	}
}


int main(int argc, char **argv){
	
	glutInit(&argc,argv);			 //Initialize GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//Set up some memory buffers for our display
	glutInitWindowSize(g_height,g_width);					//Set the window size
	glutCreateWindow("Assignment 6");						//Set the window title
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

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
		
	init();
		
	glutMainLoop();
	
	return 0;
} 