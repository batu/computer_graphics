#pragma once
#include "includes.h"

class Mesh {
private:
	
	
public:
	
	GLuint num_vertices;
	GLuint num_faces;
	vector<GLfloat> vertices;
	vector<GLuint> faces;
	vector<GLfloat> colors;
	vector<GLfloat> normals;
	
	Mesh(char *filename, float maxCoordMagnitude, bool convertToRHS);
	int Load(char* filename);
	void normalize(float max_mag);
	void computeNormals();
	void lhs_to_rhs();

};