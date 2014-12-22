#include "includes.h"
#include "LoadShaders.h"
#include "Mesh.h"
#include "Camera.h"

using namespace std;
#define BUFFER_OFFSET(offset) ( (void *) (offset))

GLuint vao;
GLuint time_unif;
GLuint Mcam_unif;
GLuint Mproj_unif;

GLuint camPos;

Mesh m("dragon.obj", 0.7f, true);

int g_height = 512;
int g_width = 512;

Camera cam;

glm::vec3 camPosition, gaze, up;


void init(void){


	ShaderInfo shader = { GL_VERTEX_SHADER, "vertex_shader.vsh", GL_FRAGMENT_SHADER, "fragment_shader.fsh" };
	GLuint program = LoadShaders(shader);
	glUseProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertices_bytes = m.vertices.size()*sizeof(m.vertices[0]);
	GLuint colors_bytes = m.colors.size()*sizeof(m.colors[0]);
	GLuint normals_bytes = m.normals.size()*sizeof(m.normals[0]);

	GLuint buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertices_bytes + colors_bytes + normals_bytes,
		NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_bytes, &m.vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices_bytes, colors_bytes, &m.colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices_bytes + colors_bytes, normals_bytes, &m.normals[0]);


	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.faces.size()*sizeof(m.faces[0]), &m.faces[0], GL_STATIC_DRAW);


	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices_bytes));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices_bytes + colors_bytes));

	time_unif = glGetUniformLocation(program, "time");
	Mcam_unif = glGetUniformLocation(program, "Mcam");
	Mproj_unif = glGetUniformLocation(program, "Mproj");

	camPos = glGetUniformLocation(program, "camPos");

	GLuint vnewEyePos = glGetAttribLocation(program, "vnewEyePosition");
	glEnableVertexAttribArray(vnewEyePos);
	glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	



	cam.setAttribLocations(Mcam_unif, Mproj_unif);

	cam.position = glm::vec3(0, 2, 0);
	cam.gaze = glm::vec3(0, -1, 0);
	cam.up = glm::vec3(1, 0, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, -1, -100);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // for wireframe
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);



	glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);


	glDrawElements(GL_TRIANGLES, m.faces.size(), GL_UNSIGNED_INT, 0);
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}



float x_axis, y_axis, z_axis;



void special(int key, int x, int y)
{

	float theta = 0.10;
	switch (key)
	{
	case GLUT_KEY_UP:

		x_axis = cos(theta)*cam.gaze.x - sin(theta)*cam.gaze.y;
		y_axis = sin(theta)*cam.gaze.x + cos(theta)*cam.gaze.y;
		cam.gaze.x = x_axis;
		cam.gaze.y = y_axis;
		cam.setCameraMatrix();
		break;

	case GLUT_KEY_DOWN:

		x_axis = cos(6.28318531 - theta)*cam.gaze.x - sin(6.28318531 - theta)*cam.gaze.y;
		y_axis = sin(6.28318531 - theta)*cam.gaze.x + cos(6.28318531 - theta)*cam.gaze.y;
		cam.gaze.x = x_axis;
		cam.gaze.y = y_axis;
		cam.setCameraMatrix();
		break;

	case GLUT_KEY_LEFT:
		y_axis = cos(theta)*cam.gaze.y - sin(theta)*cam.gaze.z;
		z_axis = sin(theta)*cam.gaze.y + cos(theta)*cam.gaze.z;
		cam.gaze.y = y_axis;
		cam.gaze.z = z_axis;
		cam.setCameraMatrix();
		break;

	case GLUT_KEY_RIGHT:
		y_axis = cos(6.28318531 - theta)*cam.gaze.y - sin(6.28318531 - theta)*cam.gaze.z;
		z_axis = sin(6.28318531 - theta)*cam.gaze.y + cos(6.28318531 - theta)*cam.gaze.z;
		cam.gaze.y = y_axis;
		cam.gaze.z = z_axis;
		cam.setCameraMatrix();
		break;
	}
}

void keyboard(unsigned char key, int x, int y){
	if (key == 'a'){
		cam.position += 0.1f*cam.gaze;
		cam.setCameraMatrix();
	}
	else if (key == 'z'){
		cam.position -= 0.1f*cam.gaze;
		cam.setCameraMatrix();
	}
}

int main(int argc, char **argv){


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_height, g_width);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("PROJECT");

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "GLEW Init Error" << endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc (GL_LESS) ;
	glDepthFunc(GL_GREATER); glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	init();

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	int glVersion[2] = { -1, -1 }; // Set some default values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using
	cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << endl;

	glutMainLoop();

	return 0;
}