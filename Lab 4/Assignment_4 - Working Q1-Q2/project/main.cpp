#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;
#define BUFFER_OFFSET(offset) ( (void *) offset )
#include "LoadShaders.h"

GLuint rot_x1;
GLuint rot_x2;
GLuint rot_x3;

GLuint rot_y1;
GLuint rot_y2;
GLuint rot_y3;

GLuint rot_z1;
GLuint rot_z2;
GLuint rot_z3;

GLfloat row_x1 = 1;
GLfloat row_x2 = 0;
GLfloat row_x3 = 0;

GLfloat row_y1 = 0;
GLfloat row_y2 = 1;
GLfloat row_y3 = 0;

GLfloat row_z1 = 0;
GLfloat row_z2 = 0;
GLfloat row_z3 = 1;


void Rotate(GLfloat& x, GLfloat& y, GLfloat& z, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	//Setting the radious at a random integer. I found out that if the mouse leaves the radious the shape is deleted. But a larger int makes the
	// turn rate too slow. So in order to compansate I will fiddle with the theta value.
	GLfloat r = 10;

	// Grabbing the initial x,y,z values to avoid confusion later.
	GLfloat xinit = x;
	GLfloat yinit = y;
	GLfloat zinit = z;

	// Creating the Z values using the sphere formula and the points x1,y1 x2,y2
	GLfloat z1 = sqrt((r * r) - ((x1*x1) + (y1*y1)));
	GLfloat z2 = sqrt((r * r) - ((x2*x2) + (y2*y2)));

	// Value of PI
	GLfloat PI = 3.141592653589793238463;

	// Calculation of theta in radian
	GLfloat theta = acos(
		(x1*x2 + y1*y2 + z1*z2) /
		(sqrt(((x1*x1) + (y1*y1) + (z1*z1))) * sqrt(((x2*x2) + (y2*y2) + (z2*z2))))
		);

	// It wasn't necesary to convert the values to degrees. It just changed the rotation speed.
	// theta = theta * (180 / PI);

	// in order to increase the speed of the rotation artifically while keeping a large radious to keep the mouse contained within it. There is mouse acceleration.
	theta = theta * 2;

	// Grabbing the cross product components.
	GLfloat u1 = (y1*z2 - z1*y2);
	GLfloat u2 = (z1*x2 - x1*z2);
	GLfloat u3 = (x1*y2 - y1*x2);

	// Getting the cross product vector length for normalization.
	GLfloat ulen = sqrt(u1*u1 + u2*u2 + u3*u3);

	// Normalization of components.
	u1 = (y1*z2 - z1*y2) / ulen;
	u2 = (z1*x2 - x1*z2) / ulen;
	u3 = (x1*y2 - y1*x2) / ulen;

	// Dividing the rotation matrix into variables for ease of work. (m)atrix(row)(column)
	GLfloat m11 = ((1 - cos(theta)) *u1*u1) + (cos(theta));
	GLfloat m12 = ((1 - cos(theta)) *u1*u2) + (sin(theta)*u3);
	GLfloat m13 = ((1 - cos(theta)) *u1*u3) - (sin(theta)*u2);

	GLfloat m21 = ((1 - cos(theta)) *u1*u2) - (sin(theta)*u3);
	GLfloat m22 = ((1 - cos(theta)) *u2*u2) + (cos(theta));
	GLfloat m23 = ((1 - cos(theta)) *u2*u3) + (sin(theta)*u1);

	GLfloat m31 = ((1 - cos(theta)) *u1*u3) + (sin(theta)*u2);
	GLfloat m32 = ((1 - cos(theta)) *u2*u3) - (sin(theta)*u1);
	GLfloat m33 = ((1 - cos(theta)) *u3*u3) + (cos(theta));

	// applying the transformation matrix to the initial values and putting it back to x,y and z.
	//x = (xinit*m11) + (yinit*m12) + (zinit*m13);
	//y = (xinit*m21) + (yinit*m22) + (zinit*m23);
	//z = (xinit*m31) + (yinit*m32) + (zinit*m33);

	row_x1 = m11;
	row_x2 = m12;
	row_x3 = m13;

	row_y1 = m21;
	row_y2 = m22;
	row_y3 = m23;

	row_z1 = m31;
	row_z2 = m32;
	row_z3 = m33;
}

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];


int g_height = 512;
int g_width = 512;

GLfloat s = 0.2;
const GLuint NumVertices = 36;
GLfloat vertices[NumVertices][3] = {
		{ -s, -s, s }, { s, -s, s }, { s, s, s }, { -s, -s, s }, { s, s, s }, { -s, s, s },
		{ -s, -s, -s }, { s, -s, -s }, { s, s, -s }, { -s, -s, -s }, { s, s, -s }, { -s, s, -s },
		{ s, s, s }, { -s, s, s }, { -s, s, -s }, { s, s, s }, { -s, s, -s }, { s, s, -s },
		{ s, -s, s }, { s, s, s }, { s, s, -s }, { s, -s, s }, { s, s, -s }, { s, -s, -s },
		{ s, -s, s }, { -s, -s, s }, { -s, -s, -s }, { s, -s, s }, { -s, -s, -s }, { s, -s, -s },
		{ -s, -s, s }, { -s, s, s }, { -s, s, -s }, { -s, -s, s }, { -s, s, -s }, { -s, -s, -s }
};

GLfloat vertices2[NumVertices][3];


GLfloat colors[NumVertices][3] = {
		{ 1, 1, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 0, 0, 1 }, { 0, 1, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 },
};


GLfloat p1_x, p1_y, p2_x, p2_y;
bool tracking = false;


void init(void){

	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	ShaderInfo shader = { GL_VERTEX_SHADER, "vertex_shader.vsh", GL_FRAGMENT_SHADER, "fragment_shader.fsh" };
	GLuint program = LoadShaders(shader);

	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	rot_x1 = glGetUniformLocation(program, "rot_x1");
	rot_x2 = glGetUniformLocation(program, "rot_x2");
	rot_x3 = glGetUniformLocation(program, "rot_x3");

	rot_y1 = glGetUniformLocation(program, "rot_y1");
	rot_y2 = glGetUniformLocation(program, "rot_y2");
	rot_y3 = glGetUniformLocation(program, "rot_y3");

	rot_z1 = glGetUniformLocation(program, "rot_z1");
	rot_z2 = glGetUniformLocation(program, "rot_z2");
	rot_z3 = glGetUniformLocation(program, "rot_z3");

}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAOs[Triangles]);

	glUniform1f(rot_x1, row_x1);
	glUniform1f(rot_x2, row_x2);
	glUniform1f(rot_x3, row_x3);

	glUniform1f(rot_y1, row_y1);
	glUniform1f(rot_y2, row_y2);
	glUniform1f(rot_y3, row_y3);

	glUniform1f(rot_z1, row_z1);
	glUniform1f(rot_z2, row_z2);
	glUniform1f(rot_z3, row_z3);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
	glutSwapBuffers();
}


void RotateAll(){
	int i, j;

	for (i = 0; i<NumVertices; ++i){
		for (j = 0; j< 3; ++j) { vertices2[i][j] = vertices[i][j]; }
		Rotate(vertices2[i][0], vertices2[i][1], vertices2[i][2], p1_x, p1_y, p2_x, p2_y);
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2);
	//glutPostRedisplay();
}


void mouse(int button, int state, int xx, int yy){
	int i, j;
	// normalize xx and yy to x and y
	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			p1_x = x; p1_y = y;
			tracking = true;
		}
		else if (state == GLUT_UP){
			tracking = false;
			for (i = 0; i<NumVertices; ++i){
				for (j = 0; j< 3; ++j) {
					vertices[i][j] = vertices2[i][j];
				}
			}
		}
	}
}

void mouseMotion(int xx, int yy){

	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;
	p2_x = x; p2_y = y;
	RotateAll();
	glutPostRedisplay();
}



int main(int argc, char **argv){

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_height, g_width);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Aytemiz Assignment 4");

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "GLEW Init Error" << endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	init();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	int glVersion[2] = { -1, -1 }; // Set some default values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using
	cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << endl;

	glutMainLoop();

	return 0;
}